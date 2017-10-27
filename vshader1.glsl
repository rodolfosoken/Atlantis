# version 410
//(in) put variables processed in parallel
layout ( location = 0) in vec4 vPosition ;
layout ( location = 1) in vec4 vColors ;
//(out ) put variable interpolated at fragment shader raster
uniform mat4 model;
out vec4 v2fcolor ;
void main (void)
{
    // gl_Position - > out builtin variable
    gl_Position = model * vPosition ;
    v2fcolor = vColors ;
}
