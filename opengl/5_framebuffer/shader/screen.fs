#version 460 core
out vec4 FragColor;
  
in vec2 TexCoords;

uniform sampler2D screenTexture;

const float coeff = 300.0;
const float dx[] = {-1.0 / coeff, 0.0 / coeff, 1.0 / coeff};
const float dy[] = {1.0 / coeff, 0.0 / coeff, -1.0 / coeff};

const float sharpen[] = {
    -1.0, -1.0, -1.0,
    -1.0,  9.0, -1.0,
    -1.0, -1.0, -1.0
};

const float blur[] = {
    1.0 / 16, 2.0 / 16, 1.0 / 16,
    2.0 / 16, 4.0 / 16, 2.0 / 16,
    1.0 / 16, 2.0 / 16, 1.0 / 16  
};

const float edgeDetection[] = {
    1.0,  1.0, 1.0,
    1.0, -8.0, 1.0,
    1.0,  1.0, 1.0
};

void main() { 
    // original
    // FragColor = texture(screenTexture, TexCoords);

    // inversion
    // FragColor = vec4(vec3(1.0 - texture(screenTexture, TexCoords)), 1.0);

    // grayscale
    // FragColor = texture(screenTexture, TexCoords);
    // float avg = (FragColor.r + FragColor.g + FragColor.b) / 3.0;
    // FragColor = vec4(vec3(avg), 1.0);


    // kernel
    FragColor = vec4(0.0);
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            FragColor += edgeDetection[i * 3 + j] * texture(screenTexture, TexCoords + vec2(dx[j], dy[i]));
        }
    }


}