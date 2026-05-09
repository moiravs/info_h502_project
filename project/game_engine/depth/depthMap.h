
#ifndef Shadow_H
#define Shadow_H

#include <memory>
#include "../shader.h"


class DepthCam;
class DepthMap
{
    std::shared_ptr<DepthCam> _depthCam;
    std::shared_ptr<Shader> depthShader;
    GLuint depthMapFBO;
    GLuint depthMapTexture;
public:
    explicit DepthMap(const std::shared_ptr<DepthCam> &depthCam);

    void loadModel(const glm::mat4 &model) const;

    void prepare() const;
    void cleanup() const;

    GLuint getTexture() const;
};

#endif