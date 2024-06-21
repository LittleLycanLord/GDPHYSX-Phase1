#version 330 core
#extension GL_EXT_debug_printf : enable

out vec4 FragmentColor;

in vec3 normalCoordinate;
in vec2 textureCoordinate;
in vec3 fragmentPosition;
in mat3 TBN;

uniform sampler2D modelTexture;
uniform vec3 modelTint;
uniform sampler2D modelNormalTexture;

uniform vec3 lightPosition;
uniform vec3 lightColor;
// Take in Brightness
uniform float lightBrightness;

uniform float ambientStrength;
uniform vec3 ambientColor;

uniform vec3 cameraPosition;
uniform float specularStrength;
uniform float specularPhong;

uniform int directionalLightCount;
struct DirectionalLight {    
    vec3 direction;
    vec3 color;
    float ambientStrength;
    vec3 ambientColor;
    float specularStrength;
    float specularPhong;
    float brightness;
};  
uniform DirectionalLight directionalLights[10];

uniform int pointLightCount;
struct PointLight {    
    vec3 position;
    vec3 color;
    float ambientStrength;
    vec3 ambientColor;
    float specularStrength;
    float specularPhong;
    float brightness;
};  
uniform PointLight pointLights[10];

uniform int spotLightCount;
struct SpotLight {    
    vec3 position;
    vec3 direction;
    float coneSize;
    vec3 color;
    float ambientStrength;
    vec3 ambientColor;
    float specularStrength;
    float specularPhong;
    float brightness;
};  
uniform SpotLight spotLights[10];

vec4 directionalLighting(int i, vec3 normal, vec3 viewDirection) {
    float diffusion = max(dot(normal, directionalLights[i].direction), 0.0);
    vec3 diffusedLight = diffusion * directionalLights[i].color;

    vec3 ambientLight = directionalLights[i].ambientStrength * directionalLights[i].ambientColor;
    
    vec3 reflectDirection = reflect(-directionalLights[i].direction, normal);

    float specular = pow(max(dot(reflectDirection, viewDirection), 0.1), directionalLights[i].specularPhong);
    vec3 specularColor = specular * directionalLights[i].specularStrength * directionalLights[i].color;

    return vec4(specularColor + diffusedLight + ambientLight, 1.0) * directionalLights[i].brightness;
}


vec4 pointLighting(int i, vec3 normal, vec3 viewDirection) {
// Add Attenuation while accounting for Brightness
    float distance    = length(pointLights[i].position - fragmentPosition);
    float attenuation = 1.0 / (distance * distance) * pointLights[i].brightness;    

    vec3 lightDirection = normalize(pointLights[i].position - fragmentPosition);

    float diffusion = max(dot(normal, lightDirection), 0.0);
    vec3 diffusedLight = diffusion * pointLights[i].color;

    vec3 ambientLight = pointLights[i].ambientStrength * pointLights[i].ambientColor;
    vec3 reflectDirection = reflect(-lightDirection, normal);

    float specular = pow(max(dot(reflectDirection, viewDirection), 0.1), pointLights[i].specularPhong);
    vec3 specularColor = specular * pointLights[i].specularStrength * pointLights[i].color;

    // Apply the Attenuation
    diffusedLight *= attenuation;
    ambientLight *= attenuation;
    specularColor *= attenuation;

    //     - Use Lighting -                     - Use Texture -
    return vec4(specularColor + diffusedLight + ambientLight, 1.0) * pointLights[i].brightness;
}

vec4 spotLighting(int i, vec3 normal, vec3 viewDirection) {
	vec3 lightDirection = normalize(spotLights[i].direction);
    float theta = dot(lightDirection, normalize(-lightDirection));
    float spotLightFeathering = 17.5f;
    float epsilon = spotLights[i].coneSize - spotLightFeathering;
    float feathering = clamp((theta - spotLightFeathering) / epsilon, 0.0f, 1.0f);
    //? Reference: https://learnopengl.com/Lighting/Light-casters

    float distance    = length(spotLights[i].position - fragmentPosition);
    float attenuation = 1.0 / (distance * distance) * spotLights[i].brightness;    

    float diffusion = max(dot(normal, lightDirection), 0.0);
    vec3 diffusedLight = diffusion * spotLights[i].color;

    vec3 ambientLight = spotLights[i].ambientStrength * spotLights[i].ambientColor;
    vec3 reflectDirection = reflect(-lightDirection, normal);

    float specular = pow(max(dot(reflectDirection, viewDirection), 0.1), spotLights[i].specularPhong);
    vec3 specularColor = specular * spotLights[i].specularStrength * spotLights[i].color;

    // Apply the Attenuation
    diffusedLight *= attenuation;
    ambientLight *= attenuation;
    specularColor *= attenuation;

    // Apply the Feathering
    diffusedLight *= feathering;
    specularColor *= feathering;

    //     - Use Lighting -                     - Use Texture -
    return vec4(specularColor + diffusedLight + ambientLight, 1.0) * spotLights[i].brightness;
}

void main() {
    //* - - - - - BLENDING - - - - -
    vec4 pixelColor = texture(modelTexture, textureCoordinate);

    if(pixelColor.a < 0.001) {
        discard;
    }
    //* - - - - - END OF BLENDING - - - - -
    
    // vec3 normal = normalize(normalCoordinate);
    vec3 normal = texture(modelNormalTexture, textureCoordinate).rgb;
    normal = normalize(normal * 2.0f - 1.0f);
    normal =  normalize(TBN * normal);
    vec3 viewDirection = normalize(cameraPosition - fragmentPosition);

    //* - - - - - LIGHTING - - - - -
    for(int i = 0; i < directionalLightCount; i++) {
        FragmentColor += directionalLighting(i, normal, viewDirection);
    }
    for(int i = 0; i < pointLightCount; i++) {
        FragmentColor += pointLighting(i, normal, viewDirection);
    }
    for(int i = 0; i < spotLightCount; i++) {
        FragmentColor += spotLighting(i, normal, viewDirection);
    }
    //* - - - - - END OF LIGHTING - - - - -
   

    //                       - Use Lighting -                     - Use Texture -
    // FragmentColor = (FragmentColor * texture(modelTexture, textureCoordinate));
    FragmentColor = vec4(clamp((FragmentColor * texture(modelTexture, textureCoordinate)).x + modelTint.x, 0.0f, 1.0f), 
                         clamp((FragmentColor * texture(modelTexture, textureCoordinate)).y + modelTint.y, 0.0f, 1.0f), 
                         clamp((FragmentColor * texture(modelTexture, textureCoordinate)).z + modelTint.z, 0.0f, 1.0f), 1.0f);
}