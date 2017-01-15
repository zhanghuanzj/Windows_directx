//-----------------------------------------------------------------------------
// File: ShadowMap.fx
//
// Desc: Effect file for high dynamic range cube mapping sample.
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//-----------------------------------------------------------------------------


#define SMAP_SIZE 512


#define SHADOW_EPSILON 0.00005f


float4x4 WorldMatrix;
float4x4 ProjMatrix;
float4x4 ViewMatrix;

float4   Material;
texture  ColorTexture;

float3   LightDir = float3(1.0f, -1.0f, 0.0f);  // Light direction in view space
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
}




//-----------------------------------------------------------------------------
// Pixel Shader: PixScene
// Desc: Process pixel (do per-pixel lighting) for enabled scene
//-----------------------------------------------------------------------------
float4 PixScene(float2 Tex : TEXCOORD0,
	float4 vPos : TEXCOORD1,
	float3 vNormal : TEXCOORD2) : COLOR
{
	float4 Diffuse;
	float3 vLight = normalize(LightDir);

	// Light
	Diffuse = (saturate(dot(-vLight, normalize(vNormal)))  *LightDiffuse + LightAmbient)
		* Material;
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










