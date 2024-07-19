//#version 460 core
//
//layout (location = 0) in vec3 aPos;
//
//layout (location = 0) uniform mat4 uTransform;
//uniform mat4 uProjection;
//uniform mat4 uView;
//
//void main() 
//{
//    gl_Position = uProjection * uView * (uTransform * vec4(aPos, 1.0));
//}

#version 460 core


layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;
out vec4 fColor;

void main() 
{
    gl_Position = vec4(aPos, 1.0);
    fColor = aColor;
}