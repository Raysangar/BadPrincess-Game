sampler Tex0: register(s0);

float4 main_fp(float2 texCoord0: TEXCOORD0): COLOR0
{
	float4 result = float4(0,0,0,0);
	for( float x = -0.001;x < 0.002; x = x + 0.001 )
	{
		for( float y = -0.001;y < 0.002; y = y + 0.001 )
		{
			result += tex2D(Tex0, float2(texCoord0.x + x, texCoord0.y + y));
		}
	}
	return result/9;
}
