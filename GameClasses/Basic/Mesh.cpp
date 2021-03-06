//
// Created by drive on 2/7/21.
//

#include "Mesh.h"

#include <utility>

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures) {
    this->vertices = std::move(vertices);
    this->indices = std::move(indices);
    this->textures = std::move(textures);
    this->position = glm::vec3(0);
    this->scale = glm::vec3(1);
    this->axis = glm::vec3(1);
    this->angle = 0;
    // now that we have all the required data, set the vertex buffers and its attribute pointers.
    setupMesh();
}

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, const char *path, std::string directory) {
    this->vertices = std::move(vertices);
    this->indices = std::move(indices);
    this->position = glm::vec3(0);
    this->scale = glm::vec3(1);
    this->axis = glm::vec3(1);
    this->angle = 0;
    Texture baseText;
    baseText.path = "../Textures/" + std::string(path);
    baseText.type = "texture_diffuse";
    baseText.id = TextureFromFile( path,"../Textures/",false,true);
    textures.push_back(baseText);

    // now that we have all the required data, set the vertex buffers and its attribute pointers.
    setupMesh();
}
Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, const char **path, uint nTextures, uint nNoise) {
    this->vertices = vertices;
    this->indices = indices;
    this->position = glm::vec3(0);
    this->scale = glm::vec3(1);
    this->axis = glm::vec3(1);
    this->angle = 0;
    //Diffuse
    for (auto i = 0; i < nTextures; i++)
    {
        std::cout<<std::string(path[i])<<std::endl;
        Texture text;
        text.path = "../Textures/" + std::string(path[i]);
        text.type = "texture_diffuse";
        text.id = TextureFromFile( path[i],"../Textures/",false,true);
        textures.push_back(text);
    }
    std::cout<<std::endl;
    //Normal
    for (auto i = nTextures; i < nTextures * 2; i++)
    {
        //std::cout<<std::string(path[i])<<std::endl;
        Texture text;
        text.path = "../Textures/" + std::string(path[i]);
        text.type = "texture_normal";
        text.id = TextureFromFile( path[i],"../Textures/",false,true);
        textures.push_back(text);
    }
    //Noise (for interpolation)
    for (int i = nTextures * 2; i < (nTextures*2) + nNoise; i++)
    {
        //std::cout<<std::string(path[i])<<std::endl;
        Texture text;
        text.path = "../Textures/" + std::string(path[i]);
        text.type = "texture_height";
        text.id = TextureFromFile(path[i],"../Textures/",false,true);
        textures.push_back(text);
    }

    // now that we have all the required data, set the vertex buffers and its attribute pointers.
    setupMesh();
}

void Mesh::bindTextures(Shader &shader,unsigned int depthMap) {
    // bind appropriate textures
    unsigned int diffuseNr  = 0;
    unsigned int specularNr = 0;
    unsigned int normalNr   = 0;
    unsigned int heightNr   = 0;

    unsigned int i = 0;

    for(i; i < textures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit before binding
        // retrieve texture number (the N in diffuse_textureN)
        std::string number = "1";
        std::string name = textures[i].type;
        if(name == "texture_diffuse")
            number = std::to_string(diffuseNr++);
        else if(name == "texture_specular")
            number = std::to_string(specularNr++); // transfer unsigned int to stream
        else if(name == "texture_normal")
            number = std::to_string(normalNr++); // transfer unsigned int to stream
        else if(name == "texture_height")
            number = std::to_string(heightNr++); // transfer unsigned int to stream

        // now set the sampler to the correct texture unit
        if (name != "material")
        {
            shader.setInt("material.hasTexture",1);
            //std::cout<<(name + number).c_str()<<std::endl;
            glUniform1i(glGetUniformLocation(shader.ID, (name +"["+= number+="]").c_str()), i);
            glActiveTexture(GL_TEXTURE0+i);
            // and finally bind the texture
            glBindTexture(GL_TEXTURE_2D, textures[i].id);
        } else
        {

            shader.setInt("material.hasTexture",0);
            shader.setVec3("material.mAmbient",textures[i].ka);
            shader.setVec3("material.mDiffuse",textures[i].kd);
            shader.setVec3("material.mSpecular",textures[i].ks);
        }

    }
    // draw mesh
    if (depthMap != -1)
    {

        glActiveTexture(GL_TEXTURE0+i);
        glBindTexture(GL_TEXTURE_2D, depthMap);
        shader.setInt("shadowMap",i);
    }
}


void Mesh::Draw(Shader &shader, bool outlined,unsigned int depthMap,bool wireframe, bool patches) {
    if (outlined)
    {
        glStencilFunc(GL_ALWAYS, 1, 0xFF);
        glStencilMask(0xFF);
    }
    else
    {
        glStencilMask(0x00);
    }
    bindTextures(shader,depthMap);
    glBindVertexArray(VAO);

    glm::mat4 meshModel = glm::mat4(1.0f);
    meshModel = glm::translate(meshModel,position);
    meshModel = glm::rotate(meshModel,angle,axis);
    meshModel = glm::scale(meshModel,scale);
    shader.setMat4("model", meshModel);

    if (patches)
    {

        glDrawElements(GL_PATCHES, indices.size(), GL_UNSIGNED_INT, 0);
    }

    else
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    // always good practice to set everything back to defaults once configured.
    glActiveTexture(GL_TEXTURE0);

}

void Mesh::setupMesh() {
    // create buffers/arrays
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    // load data into vertex buffers
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // A great thing about structs is that their memory layout is sequential for all its items.
    // The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
    // again translates to 3/2 floats which translates to a byte array.
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_DYNAMIC_DRAW);

    // set the vertex attribute pointers
    // vertex Positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
    // vertex texture coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
    // vertex tangent
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
    // vertex bitangent
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));

    glBindVertexArray(0);
}

void Mesh::outlineMesh(Shader &outlineShader, glm::vec3 scale) {
    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    glStencilMask(0x00);
    glDisable(GL_DEPTH_TEST);
    outlineShader.use();
    glBindVertexArray(VAO);
    glm::mat4 meshModel = glm::mat4(1.0f);
    meshModel = glm::translate(meshModel,position);
    meshModel = glm::rotate(meshModel,angle,axis);
    meshModel = glm::scale(meshModel,scale);
    outlineShader.setMat4("model", meshModel);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glStencilMask(0xFF);
    glStencilFunc(GL_ALWAYS, 0, 0xFF);
    glEnable(GL_DEPTH_TEST);
}

Mesh::Mesh() {

}

void Mesh::setModel(glm::mat4 model) {
    this->model = model;
}

Mesh::~Mesh() {
    for (int i = 0; i < textures.size();i++)
        glDeleteTextures(1,&textures[i].id);
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

Mesh &Mesh::operator=(const Mesh &copy) noexcept{
    if (this == &copy) return *this;
    this->textures = copy.textures;
    this->indices = copy.indices;
    this->vertices = copy.vertices;
    this->VAO = copy.VAO;
    this->position = copy.position;
    this->axis = copy.axis;
    this->angle = copy.angle;
    this->EBO = copy.EBO;
    this->VBO = copy.VBO;
    this->scale = copy.scale;
    this->model = copy.model;
    return *this;
}





