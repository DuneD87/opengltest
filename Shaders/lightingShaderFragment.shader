#version 410 core
out vec4 FragColor;
#define maxText 8
#define maxBiomes 6
struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
    int hasTexture;
    vec3 mAmbient;
    vec3 mDiffuse;
    vec3 mSpecular;
};

struct DirLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
struct TextHeight {
    int index;//4
    float hStart;//4
    float hEnd;//4
};
struct Biome {
    float latStart;//4  0
    float latEnd;//4    4
    int nTextBiome;//4  8
    TextHeight[maxText] textIndex;//12*16=192   200
};
uniform DirLight dirLight;


struct PointLight {
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
#define NR_POINT_LIGHTS 8

uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform int nPointLights;
struct SpotLight {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

};
uniform SpotLight spotLight;
in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;
in vec4 FragPosLightSpace;
in vec3 LocalPos;
in vec3 worldPos;
in mat3 tbn;
in vec3 _viewPos;
in vec3 TangentLightPos;
in vec3 TangentViewPos;
in vec3 TangentFragPos;

uniform sampler2D texture_diffuse[maxText];
uniform sampler2D texture_normal[maxText];
uniform int nTextures;
uniform int nBiomes;
uniform Biome biomes[maxBiomes];
uniform float blendFactor;
uniform float depthBlend;
uniform int isTerrain;
uniform float hPoint;
uniform float lPoint;
uniform vec3 pPosition;
uniform float pRadius;
uniform Material material;
uniform sampler2D shadowMap;

const float PI = 3.14159265359;

vec3 saturate(vec3 a)
{
    return clamp(a,0.0,1.0);
}

float ShadowCalculation(vec4 fragPosLightSpace, vec3 lightDir)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r;
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // calculate bias (based on depth map resolution and slope)
    vec3 normal = normalize(Normal);

    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
    // check whether current frag pos is in shadow
    // float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;
    // PCF
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
            shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;
        }
    }
    shadow /= 9.0;

    // keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
    if(projCoords.z > 1.0)
        shadow = 0.0;

    return shadow;
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // attenuation
    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance +
  			     light.quadratic * (distance * distance));
    // combine results
    vec3 ambient  = light.ambient  * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}
vec3 CalcSpotLight(SpotLight light, vec3 norm, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(spotLight.position - TangentFragPos);
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon   = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    if(theta > light.outerCutOff)
    {
        // ambient
        vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;

        // diffuse
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoords).rgb;
        // specular
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
        vec3 specular = light.specular * spec * texture(material.specular, TexCoords).rgb;
        diffuse  *= intensity;
        specular *= intensity;
        vec3 result = ambient + diffuse + specular;
        return result;
    } else
    {
        return vec3(light.ambient * vec3(texture(material.diffuse, TexCoords)));
    }
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir, vec4 FragPosLightSpace, vec3 lightPos, vec4 text)
{
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // combine results
    vec3 mDiffuse;
    vec3 mSpecular;
    if (material.hasTexture == 1)
    {
        if (isTerrain == 0)
        {
            mDiffuse = vec3(texture(material.diffuse,TexCoords));
            mSpecular = vec3(texture(material.specular,TexCoords));
        } else
        {
            mDiffuse = vec3(text);
            //mSpecular = vec3(text);
        }

    } else
    {
        mDiffuse = material.mDiffuse;
        mSpecular = material.mSpecular;
    }
    vec3 ambient  = light.ambient  * mDiffuse;
    vec3 diffuse  = light.diffuse  * diff * mDiffuse;
    vec3 specular = light.specular * spec * mSpecular;

    float shadow = ShadowCalculation(FragPosLightSpace,lightDir);
    return (ambient + (1-shadow)*(diffuse + specular));
}

vec3 _ACESFilmicToneMapping(vec3 x) {
    float a = 2.51;
    float b = 0.03;
    float c = 2.43;
    float d = 0.59;
    float e = 0.14;
    return saturate((x*(a*x+b))/(x*(c*x+d)+e));
}
vec3 blend(vec4 texture1, float a1, vec4 texture2, float a2)
{
    float ma = max(texture1.a + a1, texture2.a + a2) - depthBlend;

    float b1 = max(texture1.a + a1 - ma, 0);
    float b2 = max(texture2.a + a2 - ma, 0);

    return (texture1.rgb * b1 + texture2.rgb * b2) / (b1 + b2);
}
vec4 hash4( vec2 p )
{
    return fract(sin(vec4( 1.0+dot(p,vec2(37.0,17.0)),
                    2.0+dot(p,vec2(11.0,47.0)),
                    3.0+dot(p,vec2(41.0,29.0)),
                    4.0+dot(p,vec2(23.0,31.0))))*103.0);
}
vec4 textureNoTile( sampler2D samp, in vec2 uv )
{
    ivec2 iuv = ivec2( floor( uv ) );
    vec2 fuv = fract( uv );

    // generate per-tile transform
    vec4 ofa = hash4( iuv + ivec2(0,0) );
    vec4 ofb = hash4( iuv + ivec2(1,0) );
    vec4 ofc = hash4( iuv + ivec2(0,1) );
    vec4 ofd = hash4( iuv + ivec2(1,1) );

    vec2 ddx = dFdx( uv );
    vec2 ddy = dFdy( uv );

    // transform per-tile uvs
    ofa.zw = sign( ofa.zw-0.5 );
    ofb.zw = sign( ofb.zw-0.5 );
    ofc.zw = sign( ofc.zw-0.5 );
    ofd.zw = sign( ofd.zw-0.5 );

    // uv's, and derivatives (for correct mipmapping)
    vec2 uva = uv*ofa.zw + ofa.xy, ddxa = ddx*ofa.zw, ddya = ddy*ofa.zw;
    vec2 uvb = uv*ofb.zw + ofb.xy, ddxb = ddx*ofb.zw, ddyb = ddy*ofb.zw;
    vec2 uvc = uv*ofc.zw + ofc.xy, ddxc = ddx*ofc.zw, ddyc = ddy*ofc.zw;
    vec2 uvd = uv*ofd.zw + ofd.xy, ddxd = ddx*ofd.zw, ddyd = ddy*ofd.zw;

    // fetch and blend
    vec2 b = smoothstep( 0.25,0.75, fuv );

    return mix( mix( textureGrad( samp, uva, ddxa, ddya ),
    textureGrad( samp, uvb, ddxb, ddyb ), b.x ),
    mix( textureGrad( samp, uvc, ddxc, ddyc ),
    textureGrad( samp, uvd, ddxd, ddyd ), b.x), b.y );
}


vec3 CalcTerrainLight(DirLight light, vec3 normal, vec3 viewDir, vec4 FragPosLightSpace, vec3 lightPos,vec4 text)
{

    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // combine results
    vec3 mDiffuse;
    vec3 mSpecular;
    if (material.hasTexture == 1)
    {
        mDiffuse = vec3(text);

    } else
    {
        mDiffuse = material.mDiffuse;
        mSpecular = material.mSpecular;
    }
    vec3 ambient  = light.ambient  * mDiffuse;
    vec3 diffuse  = light.diffuse  * diff * mDiffuse;
    vec3 specular = light.specular * spec;

    float shadow = ShadowCalculation(FragPosLightSpace,lightDir);
    return (ambient + (1-shadow)*(diffuse));
}
/*
    0 - sandy rocks ( beach ? )
    1 - sandy ground ( kind of wet )
    2 - rough wet cobble
    3 - grass
    4 - dirt
    5 - bumpy rock
    6 - rock snow ice
    7 - snow
*/

vec4 createTerrainTextHeight(vec4 loadedTextures[maxText],Biome biome, float height)
{
    vec4 terrainColor = vec4(0.0, 0.0, 0.0, 1.0);
    int nText = biome.nTextBiome;
    for (int i = 0; i < nText; i++)
    {
        float a = smoothstep(biome.textIndex[i].hStart,biome.textIndex[i].hEnd,height);
        if (height >= biome.textIndex[i].hStart && height <  biome.textIndex[i].hEnd)
        {
            if (i == 0)
                terrainColor += vec4(blend(loadedTextures[biome.textIndex[0].index],1-a,loadedTextures[biome.textIndex[0].index],a),0.0);
            else
                terrainColor += vec4(blend(loadedTextures[biome.textIndex[i - 1].index],1-a,loadedTextures[biome.textIndex[i].index],a),0.0);
        }

    }
    return terrainColor;
}

vec4 biomeInterpolation(vec4 loadedTextures[maxText], int index, float lat, float height)
{
    vec4 result = vec4(0.0);
    if (lat <= biomes[index].latStart && lat > biomes[index].latEnd)
    {
        float a = smoothstep(biomes[index].latStart, biomes[index].latEnd,lat);
        if (index == 0)
            result += createTerrainTextHeight(loadedTextures,biomes[0], height);
        else
            result += vec4(blend(createTerrainTextHeight(loadedTextures,biomes[index - 1], height), 1-a, createTerrainTextHeight(loadedTextures,biomes[index], height), a), 0.0);
    }
    return result;
}

void main()
{
    float dist = length(_viewPos - FragPos);
    vec3 norm = Normal;
    vec3 viewDir = normalize(_viewPos - FragPos);
    vec4 text = vec4(0,0,0,1);
    if (isTerrain == 1)
    {
        vec4 loadedTextures[maxText];
        for (int i = 0; i < nTextures; i++)
        {
            loadedTextures[i] = textureNoTile(texture_diffuse[i],TexCoords);
        }

        vec3 dir = pPosition-FragPos;
        float absHeigth = (abs(lPoint) + abs(hPoint));
        float height = abs((length(dir) - pRadius) - lPoint) / absHeigth;

        vec3 pos = (dir)/pRadius;
        float lat = atan(pos.y,sqrt(pos.x*pos.x+pos.z*pos.z)) + PI/2.0;//convert to positive 0..3.14
        lat = lat / PI;

        for (int i = 0; i < nBiomes; i++)
        {
            text += biomeInterpolation(loadedTextures,i,lat,height);
        }

    }
    vec3 result = vec3(0.0);
    //for(int i = 0; i < nPointLights; i++)
        //result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);
    const float C = 10.0;
    const float far = 10000000.0;
    const float offset = 1.0;
    gl_FragDepth = (log(C * worldPos.z + offset) / log(C * far + offset));
    result += CalcDirLight(dirLight, norm, viewDir, FragPosLightSpace,TangentLightPos,text);
    result += CalcSpotLight(spotLight, norm, TangentFragPos, viewDir);
    FragColor = vec4(_ACESFilmicToneMapping(result), 1.0);
}