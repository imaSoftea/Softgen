#version 410 core
 layout (location = 0) in vec3 aPos;
 layout (location = 2) in vec2 aTexCoord;

 uniform mat4 model;
 uniform mat4 view;
 uniform mat4 projection;
 uniform mat4 mesh;

 out vec2 TexCoord;

 void main()
 {
     gl_Position = projection * view * model * mesh * vec4(aPos, 1.0);
     TexCoord = aTexCoord;
 }