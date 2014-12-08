#version 330 core

in vec2 uv; // uv coordinate for frag position

uniform mat4 mvp; // Modelview Projection matrix. This maps the vertices in model (object) space to film coordinates (world->film)
uniform vec4 color;
uniform sampler2D textureSampler;

out vec4 fragColor; //output color

void main(){
    // Figure out something reasonable to do with texColor and color...
    vec4 texColor = texture2D(textureSampler, uv);
    fragColor = color*texColor;
}
