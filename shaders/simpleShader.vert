#version 450

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;
layout(location = 2) in vec3 normal;
layout(location = 3) in vec2 texCoord;

layout(location = 0) out vec3 fragColor;

layout(push_constant) uniform Push
{
    mat4 transform;  // projection * view * model
    mat4 modelMatrix;
}
push;

const vec3 lightDir = normalize(vec3(1.0, -2.0, -1.0));
const float ambient = 0.1;

void main()
{
    gl_Position = push.transform * vec4(position, 1.0);

    mat3 normalMatrix = transpose(inverse(mat3(push.modelMatrix)));
    vec3 normal = normalize(normalMatrix * normal);

    float intensity = max(dot(normal, lightDir), 0.0);

    fragColor = intensity * color + ambient * color;
}