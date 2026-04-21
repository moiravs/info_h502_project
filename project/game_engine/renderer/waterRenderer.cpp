#include "waterRenderer.h"

#include "../mainCamera.h"

WaterRenderer::WaterRenderer(const Shader& shader, const WaterFrameBuffer& fbos) :
transparent(false), _shader(shader), _fbos(fbos){}

void WaterRenderer::registerObject(const std::shared_ptr<Object> object)
{
    this->Renderer::registerObject(object);

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // define VBO and VAO as active buffer and active vertex array
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, static_cast<long>(sizeof(Vertex) * object->getNumVertices()), object->getVertices().data(),
                 GL_STATIC_DRAW);

    auto att_pos = glGetAttribLocation(_shader.getID(), "position");
    glEnableVertexAttribArray(att_pos);
    glVertexAttribPointer(att_pos, 3, GL_FLOAT, false, 8 * sizeof(float), nullptr);

    auto att_col = glGetAttribLocation(_shader.getID(), "normal");
    glEnableVertexAttribArray(att_col);
    glVertexAttribPointer(att_col, 3, GL_FLOAT, false, 8 * sizeof(float),
                          reinterpret_cast<void*>(5 * sizeof(float)));

    auto att_tex = glGetAttribLocation(_shader.getID(), "tex_coord");
    glEnableVertexAttribArray(att_tex);
    glVertexAttribPointer(att_tex, 2, GL_FLOAT, false, 8 * sizeof(float),
                          reinterpret_cast<void*>(3 * sizeof(float)));

    // desactive the buffer
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    this->_shader.setInteger("reflectionTexture", 0);
    this->_shader.setInteger("refractionTexture", 1);
    this->_shader.setInteger("dudvMap", 2);
}

void WaterRenderer::draw() const
{
    for (const auto& object: this->_objects)
    {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glBindVertexArray(this->VAO);
        glDrawArrays(GL_TRIANGLES, 0, static_cast<int>(object->getNumVertices()));
    }
}

void WaterRenderer::render()
{
    this->_shader.use();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _fbos.getReflectionTexture());

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, _fbos.getRefractionTexture());

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, dudvMap.getTexture());

    this->_shader.updatePos(MainCamera::get());
    this->draw();
}
