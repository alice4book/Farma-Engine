#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec2 TexCoord;
out vec3 Normal; 
out vec3 FragPos;
out vec4 fragPosLight;
out vec4 ourColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 lightProjection;
uniform vec4 aColor;

void main()
{
    Normal = mat3(transpose(inverse(model))) * aNormal;  
    FragPos = vec3(model * vec4(aPos, 1.0f));
    TexCoord = aTexCoords;   
	ourColor = aColor;	
	fragPosLight = lightProjection * vec4(FragPos, 1.0f);
    gl_Position = projection * view * model * vec4(aPos, 1.0f);
}
