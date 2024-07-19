//#version 460 core
//
//out vec4 FragColor;
//
//void main()
//{
//    FragColor = vec4(1.0, 0.0, 0.0, 1.0);
//}
#version 460 core

in vec4 fColor;
out vec4 FragColor;

void main()
{
    FragColor = fColor;
}