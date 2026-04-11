#ifndef SkyboxRenderer_H
#define SkyboxRenderer_H

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "object.h"
#include "texture.h"
#include "renderer.h"
#include "shader.h"
#include "skybox.h"

class SkyboxRenderer : public Renderer
{
public:
    unsigned int skyboxVAO, skyboxVBO;
    bool _transparent;
    std::vector<glm::mat4> _models;
    Shader _shader;
    Skybox *_skybox;
    glm::mat4 model = glm::mat4(1.0);
    unsigned int cubemapTexture;

    SkyboxRenderer(Shader shader, Skybox *skybox) : _shader(shader), _skybox(skybox)
    {
        cubemapTexture = skybox->getTextureId();

        glGenVertexArrays(1, &skyboxVAO);
        glGenBuffers(1, &skyboxVBO);
        glBindVertexArray(skyboxVAO);
        glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
        glBufferData(GL_ARRAY_BUFFER, skybox->getVertices().size() * sizeof(float), skybox->getVertices().data(), GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    }

    void render() override
    {

        glDepthFunc(GL_LEQUAL); // change depth function so depth test passes when values are equal to depth buffer's content
        _shader.use();
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10000.0f);
        view = glm::mat4(glm::mat3(camera.GetViewMatrix())); // remove translation from the view matrix
        _shader.setMatrix4("view", view);
        _shader.setMatrix4("projection", projection);
        // skybox cube
        glBindVertexArray(skyboxVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        glDepthFunc(GL_LESS); // set depth function back to default
    }
};
#endif
