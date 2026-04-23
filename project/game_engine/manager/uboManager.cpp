
#include "uboManager.h"

UboManager UboManager::instance = UboManager();

UboManager::UboManager()
{
    for (GLuint i = 0; i < UBO_BINDING_NAMES.size(); i++)
    {
        this->bindings.emplace(UBO_BINDING_NAMES.at(i), i);
    }
}

void UboManager::registerShader(const Shader* shader)
{
    for (const auto & [name, binding] : instance.bindings)
    {
        const GLuint blockIndex = glGetUniformBlockIndex(shader->getID(), name.c_str());
        if (blockIndex != GL_INVALID_INDEX)
        {
            glUniformBlockBinding(shader->getID(), blockIndex, binding);
        }
    }
}

GLuint UboManager::getBinding(const std::string& name)
{
    return instance.bindings.at(name);
}
