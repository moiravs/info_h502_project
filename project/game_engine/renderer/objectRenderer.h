#ifndef ObjectRenderer_H
#define ObjectRenderer_H

#include "meshRenderer.h"
#include "../texture.h"

class ObjectRenderer : public MeshRenderer
{
    bool _transparent{};

    Texture grassTex = Texture(PATH_TO_SRC "/../assets/textures/terrain/grass.jpg");
    Texture rockTex = Texture(PATH_TO_SRC "/../assets/textures/terrain/rock.jpg");
    Texture snowTex = Texture(PATH_TO_SRC "/../assets/textures/terrain/snow.jpg");

public:
    explicit ObjectRenderer(const std::string &shaderName);
    void updateUniforms() const override;
};
#endif
