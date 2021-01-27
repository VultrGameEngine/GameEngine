#include <rendering/models/shader.h>

namespace Brick3D {
Shader::Shader() {}

Shader::Shader(unsigned int p_id, RenderType p_type) : id(p_id), type(p_type) {}

Shader::~Shader() { glDeleteProgram(this->id); }

void Shader::Bind() { glUseProgram(this->id); }

void Shader::Unbind() { glUseProgram(0); }

} // namespace Brick3D
