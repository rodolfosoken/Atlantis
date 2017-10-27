# version 400

layout ( location = 0) in vec4 vPosition ;

uniform mat4 model;

out vec4 v2fcolor ;

void main (void)
{
    gl_Position = model * vPosition;
    v2fcolor = vec4(0.3,0.3,0.7,1) ;//azul
}
