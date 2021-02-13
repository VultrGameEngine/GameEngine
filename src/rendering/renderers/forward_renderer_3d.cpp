#include <rendering/renderers/forward_renderer_3d.h>

namespace Brick3D::Renderer3D
{
void ForwardRenderer::Submit(const MaterialComponent &material,
                             const glm::mat4 &transform, const Mesh &mesh)
{
    material.BindShaders();
    material.SetModelUniforms(transform);
    material.BindTextures();
    mesh.Draw();
}

} // namespace Brick3D::Renderer3D
