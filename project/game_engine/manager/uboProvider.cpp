
#include "uboProvider.h"

#include "uboManager.h"

UboProvider::UboProvider(const std::string& bindingName, const long bufferSize): bufferSize(bufferSize)
{
    glGenBuffers(1, &ubo);

    glBindBuffer(GL_UNIFORM_BUFFER, ubo);
    glBufferData(GL_UNIFORM_BUFFER, bufferSize, nullptr, GL_STATIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, UboManager::get().getBinding(bindingName), ubo);
}

UboProvider::~UboProvider()
{
    glDeleteBuffers(1, &ubo);
}