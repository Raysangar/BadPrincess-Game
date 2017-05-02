sampler Tex0: register(s0);

float4 main_fp(float2 texCoord0: TEXCOORD0): COLOR0
{
	float4 textureColour = tex2D(Tex0, texCoord0);
    float intensity = textureColour.x * 0.2126 + 0.7152 * textureColour.y + 0.0722 * textureColour.z;
	return float4(intensity, intensity, intensity, textureColour.w);
}
