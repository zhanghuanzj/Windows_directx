//-----------------------------------------------------------------------------
// File: ShadowMap.fx
//
// Desc: Effect file for high dynamic range cube mapping sample.
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//-----------------------------------------------------------------------------


#define SMAP_SIZE 3000


#define SHADOW_EPSILON 0.000015f


float4x4 WorldMatrix;
float4x4 ProjMatrix;
float4x4 ViewMatrix;
float4x4 ViewToLightProjMatrix;

float4   Material;
texture  ColorTexture;
texture  ShadowTexture;

float3   LightDir;  // Light direction in view space
float4   LightDiffuse;// Light diffuse color
float4   LightAmbient;// Use an ambient light of 0.3



sampler2D SceneSampler =
sampler_state
{
	Texture = <ColorTexture>;
	MinFilter = Point;
	MagFilter = Linear;
	MipFilter = Linear;
};

sampler2D ShadowSampler =
sampler_state
{
	Texture = <ShadowTexture>;
	MinFilter = Point;
	MagFilter = Point;
	MipFilter = Point;
	AddressU = Clamp;
	AddressV = Clamp;
};



//-----------------------------------------------------------------------------
// Vertex Shader: VertScene
// Desc: Process vertex for scene
//-----------------------------------------------------------------------------
void VertScene(float4 iPos : POSITION,
	float3 iNormal : NORMAL,
	float2 iTex : TEXCOORD0,
	out float4 oPos : POSITION,
	out float2 Tex : TEXCOORD0,
	out float4 vPos : TEXCOORD1,
	out float3 vNormal : TEXCOORD2,
	out float4 vPosLight : TEXCOORD3)
{
	float4x4 matWorldView = mul(WorldMatrix, ViewMatrix);
	vPos = mul(iPos, matWorldView);
	oPos = mul(vPos, ProjMatrix);
	vNormal = mul(iNormal, (float3x3)matWorldView);
	Tex = iTex;
	vPosLight = mul(vPos, ViewToLightProjMatrix);
}




//-----------------------------------------------------------------------------
// Pixel Shader: PixScene
// Desc: Process pixel (do per-pixel lighting) for enabled scene
//-----------------------------------------------------------------------------
float4 PixScene(float2 Tex : TEXCOORD0,
	float4 vPos : TEXCOORD1,
	float3 vNormal : TEXCOORD2, 
	float4 vPosLight : TEXCOORD3) : COLOR
{
	float4 Diffuse;
	float2 ShadowTexC = 0.5 * vPosLight.xy / vPosLight.w + float2(0.5, 0.5);
	ShadowTexC.y = 1.0f - ShadowTexC.y;

	float2 texelpos = SMAP_SIZE * ShadowTexC;
	float2 lerps = frac(texelpos);
	//这里使用的是2x2 percentage closest filtering,因此是采的邻近的四个点，判断它们是否在阴影中，
	float sourcevals[4];
	sourcevals[0] = (tex2D(ShadowSampler, ShadowTexC) + SHADOW_EPSILON < vPosLight.z / vPosLight.w) ? 0.0f : 1.0f;
	sourcevals[1] = (tex2D(ShadowSampler, ShadowTexC + float2(1.0 / SMAP_SIZE, 0)) + SHADOW_EPSILON < vPosLight.z / vPosLight.w) ? 0.0f : 1.0f;
	sourcevals[2] = (tex2D(ShadowSampler, ShadowTexC + float2(0, 1.0 / SMAP_SIZE)) + SHADOW_EPSILON < vPosLight.z / vPosLight.w) ? 0.0f : 1.0f;
	sourcevals[3] = (tex2D(ShadowSampler, ShadowTexC + float2(1.0 / SMAP_SIZE, 1.0 / SMAP_SIZE)) + SHADOW_EPSILON < vPosLight.z / vPosLight.w) ? 0.0f : 1.0f;

	// 用lerps 
	float LightAmount = lerp(lerp(sourcevals[0], sourcevals[1], lerps.x),
		lerp(sourcevals[2], sourcevals[3], lerps.x),
		lerps.y);

	//float LightAmount = (tex2D(ShadowSampler, ShadowTexC) + SHADOW_EPSILON< vPosLight.z / vPosLight.w) ? 0.0f : 1.0f;
	// Light
	Diffuse = (saturate(dot(-LightDir, normalize(vNormal)))*LightAmount *LightDiffuse + LightAmbient)* Material;
	return tex2D(SceneSampler, Tex) * Diffuse;
}

//-----------------------------------------------------------------------------
// Vertex Shader: VertShadow
// Desc: Process vertex for the shadow map
//-----------------------------------------------------------------------------
void VertShadow(float4 Pos : POSITION,
	float3 Normal : NORMAL,
	out float4 oPos : POSITION,
	out float2 Depth : TEXCOORD0)
{
	//
	// Compute the projected coordinates
	//
	float4x4 matWorldView = mul(WorldMatrix, ViewMatrix);
	oPos = mul(Pos, matWorldView);
	oPos = mul(oPos, ProjMatrix);

	//
	// Store z and w in our spare texcoord
	//
	Depth.xy = oPos.zw;
}




//-----------------------------------------------------------------------------
// Pixel Shader: PixShadow
// Desc: Process pixel for the shadow map
//-----------------------------------------------------------------------------
void PixShadow(float2 Depth : TEXCOORD0,
	out float4 Color : COLOR)
{
	//
	// Depth is z / w
	//
	Color = Depth.x / Depth.y;
}

//-----------------------------------------------------------------------------
// Technique: RenderScene
// Desc: Renders scene objects
//-----------------------------------------------------------------------------
technique RenderScene
{
	pass p0
	{
		VertexShader = compile vs_2_0 VertScene();
		PixelShader = compile ps_2_0 PixScene();
	}
}

technique RenderShadow
{
	pass p0
	{
		VertexShader = compile vs_2_0 VertShadow();
		PixelShader = compile ps_2_0 PixShadow();
	}
}










