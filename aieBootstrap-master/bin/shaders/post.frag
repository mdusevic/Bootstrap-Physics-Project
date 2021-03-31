// A simple post processing fragment shader
#version 410

in vec2 vTexCoord;

uniform bool SwitchEffect;
uniform sampler2D ColorTarget;

out vec4 FragColor;

// Just output the colour unchanged
vec4 Default(vec2 texCoord)
{
    return texture(ColorTarget, texCoord);
}

// Simple box blur
vec4 BoxBlur(vec2 texCoord)
{
    vec2 texel = 1.0f / textureSize(ColorTarget, 0);

    // 9-tap box kernel
    vec4 color = texture(ColorTarget, texCoord);
    color += texture(ColorTarget, texCoord + texel * vec2(-1, 1));
    color += texture(ColorTarget, texCoord + texel * vec2(-1, 0));
    color += texture(ColorTarget, texCoord + texel * vec2(-1, -1));
    color += texture(ColorTarget, texCoord + texel * vec2(0, 1));
    color += texture(ColorTarget, texCoord + texel * vec2(0, -1));
    color += texture(ColorTarget, texCoord + texel * vec2(1, 1));
    color += texture(ColorTarget, texCoord + texel * vec2(1, 0));
    color += texture(ColorTarget, texCoord + texel * vec2(1, -1));

    return color / 9;
}

// Simple distort
vec4 Distort(vec2 texCoord)
{
    vec2 mid = vec2(0.8f);
    
    float distanceFromCentre = distance(texCoord, mid);
    vec2 normalisedCoord = normalize(texCoord - mid);
    float bias = distanceFromCentre + sin(distanceFromCentre * 15) * 0.07f;

    vec2 newCoord = mid + bias * normalisedCoord;
    return texture(ColorTarget, newCoord);
}

void main()
{
    // Calculate texel size
    vec2 texSize = textureSize(ColorTarget, 0);
    vec2 texelSize = 1.0f / texSize;

    // Adjust texture coordinates
    vec2 scale = (texSize - texelSize) / texSize;
    vec2 texCoord = vTexCoord / scale + texelSize * 0.5f; 

    // Sample post effect
    if (SwitchEffect == true)
    {
        FragColor = BoxBlur(texCoord);
    }
    else if (SwitchEffect == false)
    {
        FragColor = Distort(texCoord);
    }
}