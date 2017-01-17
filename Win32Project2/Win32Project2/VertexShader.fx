texture ColorTexture;

float    Time;
float4   LightDiffuse = float4(1.0f, 1.0f, 1.0f, 1.0f);  // Light diffuse color
float4   LightAmbient = float4(0.3f, 0.3f, 0.3f, 1.0f);  // Use an ambient light of 0.3
float3 	 LightDirection = float3(1.0f, -1.0f, 0.0f);
float4   Material;
float4x4 WorldMatrix;
float4x4 ViewMatrix;
float4x4 ProjMatrix;
vector   color;

sampler MeshTextureSampler = sampler_state
{
	Texture = <ColorTexture>;
	MipFilter = Linear;
	MinFilter = Linear;
	MagFilter = Linear;
};

//--------------------------------------------------------------------------------------
// Vertex shader output structure
//--------------------------------------------------------------------------------------
struct VS_INPUT
{
	float4 Position   	: POSITION;
	float3 Normal		: NORMAL;
	float2 UV			: TEXCOORD0;
};


struct VS_OUTPUT
{
	float4 Position 	: POSITION;
	float3 Normal		: NORMAL;
	float2 UV			: TEXCOORD0;
	vector Color	    : COLOR;
};


VS_OUTPUT RenderSceneVS(VS_INPUT In)
{
	VS_OUTPUT Out = (VS_OUTPUT)0;
	float4x4 matWorldView = mul(WorldMatrix, ViewMatrix);
	float4x4 matWorldViewProject = mul(matWorldView, ProjMatrix);

	Out.Position = mul(In.Position, matWorldViewProject);
	Out.Normal = mul(In.Normal, (float3x3)matWorldView);
	Out.UV = In.UV;
	return Out;
}

float4 RenderScenePS(VS_OUTPUT In) : COLOR
{
	//float4 Diffuse;
	float3 vLight = normalize(LightDirection); //light vector
	float4 Diffuse = (saturate(dot(-vLight,normalize(In.Normal)))*(1 - LightAmbient) + LightAmbient)*Material;

	return tex2D(MeshTextureSampler , In.UV)*Diffuse;ff
}

technique DefaultTech
{
	pass P0
	{
		VertexShader = compile vs_2_0 RenderSceneVS();
		PixelShader = compile ps_2_0 RenderScenePS();
	}
}

