//--------------------------------------------------------------------------------------
// 文件：渲染对象的效果文件
// 功能：渲染效果
//--------------------------------------------------------------------------------------

//texture g_ColorTexture;             // 纹理

float    g_fTime;                   // 时间
float4x4 g_matWorld;                // 世界矩阵
float4x4 g_matView;                 // 观察矩阵
float4x4 g_matProject;              // 投影矩阵
vector color;

//sampler MeshTextureSampler = sampler_state
//  {
//    //Texture = <g_ColorTexture>;
//    MipFilter = LINEAR;
//    MinFilter = LINEAR;
//    MagFilter = LINEAR;
//  };

//--------------------------------------------------------------------------------------
// Vertex shader output structure
//--------------------------------------------------------------------------------------
struct VS_INPUT
  {
    float4 Position   : POSITION;  
//    float3 Tex        : TEXCOORD0;
  };


struct VS_OUTPUT
  {
    float4 Position   : POSITION;   
//    float3 Tex        : TEXCOORD0;
	vector color	  : COLOR;
  };


VS_OUTPUT RenderSceneVS( VS_INPUT In )
  {
    VS_OUTPUT Out = ( VS_OUTPUT )0;
    float4x4 matWorldView = mul( g_matWorld ,g_matView );
    float4x4 matWorldViewProject = mul( matWorldView ,g_matProject );
    Out.Position = mul( In.Position , matWorldViewProject );
	Out.color = color;
    //Out.Tex = In.Tex;
    //
    return Out;
  }

float4 RenderScenePS( VS_OUTPUT In ) : COLOR
  {
    //return tex2D( MeshTextureSampler , In.Tex );
  }
//------------------------------------------------------------------------------
// 实现技术方式
//------------------------------------------------------------------------------
technique DefaultTech
  {
    pass P0
      {
        VertexShader = compile vs_2_0 RenderSceneVS( );
        //PixelShader  = compile ps_2_0 RenderScenePS( );
      }
  }

