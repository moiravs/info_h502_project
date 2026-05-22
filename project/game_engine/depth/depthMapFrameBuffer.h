
#ifndef Shadow_H
#define Shadow_H

#include <memory>
#include "../framebuffer/frameBuffer.h"
#include "../shader/shader.h"


class DepthCam;
class DepthMapFrameBuffer: public FrameBuffer
{
    std::shared_ptr<DepthCam> _depthCam;
    std::shared_ptr<Shader> _shader;
    std::shared_ptr<Shader> _instShader;
    GLuint _tex {};
public:
    explicit DepthMapFrameBuffer(int width, int height, const std::shared_ptr<DepthCam>& depthCam);

    void bindTextures() override;
    void createTextures() override;
    void begin() override;
    void end() override;

    GLuint getTexture() const;
    std::shared_ptr<Shader> getShader() const;
    std::shared_ptr<Shader> getInstShader() const;
};

#endif