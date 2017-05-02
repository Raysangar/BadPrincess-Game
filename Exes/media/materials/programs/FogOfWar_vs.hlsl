struct VS_OUTPUT {
   float4 pos: POSITION;
   float2 texCoord0: TEXCOORD0;
   float4 worldPosition: TEXCOORD1;
};

float4x4 worldViewProj;
float4x4 world;
float4 unit_0;
float4 unit_1;
float4 unit_2;
float4 unit_3;
float4 unit_4;
float4 unit_5;
float4 unit_6;
float4 unit_7;
float4 unit_8;
float4 unit_9;
float4 unit_10;
float4 unit_11;
float4 unit_12;
float4 unit_13;
float4 unit_14;
float4 unit_15;
float4 chest_0;
float4 chest_1;
float4 chest_2;
float4 well_0;
float4 well_1;
float4 well_2;
float4 well_3;
float4 well_4;


VS_OUTPUT main(
	float4 objectPosition: POSITION,
	float2 uv : TEXCOORD0)
{
	VS_OUTPUT Out;
    Out.pos = mul(worldViewProj, objectPosition);
    Out.texCoord0 = uv;
	Out.worldPosition = mul(world, objectPosition);
	
	return Out;
}

		
sampler Tex0: register(s0);

float checkDistance(float distance, float rangeVision)
{
	if (distance < rangeVision-0.1*rangeVision)
	{
		return 1;
	}
	if (distance < rangeVision)
	{
		return ((rangeVision - distance)/(0.1*rangeVision));
	}
	return 0;
}

float setMultiplier(float newMultiplier, float oldMultiplier)
{
	if (newMultiplier > oldMultiplier)
	{
		return newMultiplier;
	}
	return oldMultiplier;
}

float4 main_fp(float4 texCoord0: TEXCOORD0, float4 position: TEXCOORD1): COLOR0
{
	float4 textureColour = tex2D(Tex0, texCoord0.xy);
	float multiplier = 0;
	float3 worldPosition = position.xyz;
	
	float4 units[24] = {unit_0,unit_1,unit_2, unit_3,unit_4,unit_5,unit_6,unit_7, unit_8,unit_9,
						unit_10,unit_11,unit_12, unit_13,unit_14,unit_15,chest_0,chest_1, chest_2,well_0
						,well_1,well_2,well_3,well_4};
	
	for (int i = 0; i < 24; ++i)
	{
		multiplier = setMultiplier(checkDistance(distance(worldPosition, units[i].xyz), units[i].w), multiplier);
	}
	
    float colour = multiplier*0.4 + 0.6;
    return (textureColour*float4(colour,colour,colour,1));
}