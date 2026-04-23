
#include "uboManager.h"

#include "../../utils/utils.h"

UboManager::UboManager()
{
    for (GLuint i = 0; i < UBO_BINDING_NAMES.size(); i++)
    {
        this->bindings.emplace(UBO_BINDING_NAMES.at(i), i + 1);
    }
}

void UboManager::registerShader(const Shader* shader)
{
    for (const auto & [name, binding] : this->bindings)
    {
        const GLuint blockIndex = glGetUniformBlockIndex(shader->getID(), name.c_str());
        if (blockIndex != GL_INVALID_INDEX)
        {
            glUniformBlockBinding(shader->getID(), blockIndex, binding);
        }
    }
}

GLuint UboManager::getBinding(const std::string& name) const
{
    return this->bindings.at(name);
}

UboManager& UboManager::get()
{
    static UboManager instance;
    return instance;
}
