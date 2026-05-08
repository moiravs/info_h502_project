#ifndef ObjectRenderer_H
#define ObjectRenderer_H

#include "meshRenderer.h"
#include "../texture.h"

class ObjectRenderer : public MeshRenderer
{
    bool _transparent{};

    static std::shared_ptr<Texture> grassTex;
    static std::shared_ptr<Texture> rockTex;
    static std::shared_ptr<Texture> snowTex;

public:
    explicit ObjectRenderer(const std::shared_ptr<Shader>& shader);
    explicit ObjectRenderer(const std::string &shaderName);
    void updateUniforms() const override;
};
#endif
