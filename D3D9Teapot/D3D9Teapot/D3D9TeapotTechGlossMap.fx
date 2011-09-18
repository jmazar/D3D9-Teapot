//--------------------------------------------------------------------------------------
// Global variables
//--------------------------------------------------------------------------------------

///////////// UN-TWEAKABLES /////////////

float4x4 g_mWorld;                   // World matrix for object
float4x4 g_mView;					 // View matrix for object
float4x4 g_mWorldView;               // World View matrix for object
float4x4 g_mWorldViewProjection;     // World * View * Projection matrix
float3   g_vEyePos;					 // = float3(5,0,5);
//float    g_fTime;                    // App's time in seconds

///////////// TWEAKABLES /////////////

#define NUM_LIGHTS 1
float3  g_vLightPos[NUM_LIGHTS]   = {float3(0, 10, 0)};

float4  g_vLightColor[NUM_LIGHTS] = {float4(1.0, 1.0, 1.0, 1.0)};

float4 g_Ambient    = float4(0.3, 0.3, 0.3, 1);     // Material's ambient color
float4 g_Diffuse    = float4(1, 1, 1, 1);    // Material's diffuse color
float4 g_Specular   = float4(0.65, 0.65, 0.25, 1);
float  g_kDiffuse   = 1.0f;
float  g_PhongExp   = 128.0f;


//************ Textures *************/
texture g_DiffuseTex : DIFFUSE;
texture g_NormalTex  : NORMAL;
texture g_GlossTex   : DIFFUSE;

sampler DiffuseTextureSampler = 
sampler_state
{
  Texture = <g_DiffuseTex>;
  MipFilter = LINEAR;
  MinFilter = LINEAR;
  MagFilter = LINEAR;
};

sampler NormalTextureSampler = 
sampler_state
{
   Texture = <g_NormalTex>;
   MipFilter = LINEAR;
   MinFilter = LINEAR;
   MagFilter = LINEAR;
};

sampler GlossTextureSampler = 
sampler_state
{
  Texture = <g_GlossTex>;
  MipFilter = LINEAR;
  MinFilter = LINEAR;
  MagFilter = LINEAR;
};


//--------------------------------------------------------------------------------------
// Vertex shader input structure
//--------------------------------------------------------------------------------------
struct VS_IN
{
  float3 vPos        : POSITION;	// vertex Pos from App
  float3 vNormal     : NORMAL;	// vertex Normal from App
  float2 vTexCoord   : TEXCOORD0;	// vertex Texture Coords from App
  float3 vTangent    : TEXCOORD1;	// vertex Tangent
  float3 vBinormal   : TEXCOORD2;	// vertex Binormal
};

//--------------------------------------------------------------------------------------
// Vertex shader output structure
//--------------------------------------------------------------------------------------
struct VS_OUTPUT
{
  float4 vPosH    : POSITION0;
  float3 vNormal   : TEXCOORD0;
  float3 vPosW      : TEXCOORD1;	// Xfmed Wvp vertex position
  float2 vTexCoord	: TEXCOORD2;
  float3 vTangent	: TEXCOORD3;
  float3 vBinormal	: TEXCOORD4;
};


//************ Shaders *************/

VS_OUTPUT VS_woNorm( VS_IN In )
{
  VS_OUTPUT Out = (VS_OUTPUT)0;
  

  Out.vNormal = In.vNormal;
    Out.vTangent = In.vTangent;
    Out.vBinormal = In.vBinormal;

  Out.vPosW = mul(float4(In.vPos, 1.0f), g_mWorld).xyz;

  Out.vTexCoord = In.vTexCoord;
  
  // Transform the position from object space to homogeneous projection space
  Out.vPosH      = mul( float4(In.vPos, 1.0f), g_mWorldViewProjection );

  return Out;
}


float4 PS_woNorm( VS_OUTPUT In, uniform bool bTexture ) : COLOR
{

  float3x3 TBN;
  TBN[0] = normalize(In.vTangent);
  TBN[1] = normalize(In.vBinormal);
  TBN[2] = normalize(In.vNormal);
  
  float3 vNormalT = tex2D(NormalTextureSampler, In.vTexCoord);
  vNormalT = 2.0f * vNormalT - 1.0f;
  vNormalT = normalize(vNormalT);
  float3 vNormalL = mul(vNormalT, TBN);
  
  float3 vNormalW = mul(float4(vNormalL, 0.0f), g_mWorld).xyz;
  vNormalW = normalize(vNormalW);


  float3 toEye = normalize(g_vEyePos - In.vPosW);

  float3 vLightDir = g_vLightPos[0] - In.vPosW;
  vLightDir = normalize(vLightDir);

  float3 r = reflect(-vLightDir, vNormalW);

  float t = pow(max(dot(r, toEye), 0.0f), g_PhongExp);

  float s = max(dot(vLightDir, vNormalW), 0.0f);

  float3 spec = t * (g_Specular*g_vLightColor[0]).rgb;
  float3 diffuse = s * (g_Diffuse*g_vLightColor[0]).rgb;
  float3 ambient = g_Ambient * float4(0.4, 0.4, 0.4, 1.0);

  float3 texColor = tex2D(DiffuseTextureSampler, In.vTexCoord).rgb;
  diffuse = diffuse * texColor;
  
  float3 glossColor = tex2D(GlossTextureSampler, In.vTexCoord).rgb;
  spec = spec * glossColor;

  float4 vColor;
  vColor.rgb =  ambient + diffuse + spec;
  vColor.a = g_Diffuse.a;

  return vColor;

}

//--------------------------------------------------------------------------------------
// Renders scene 
//--------------------------------------------------------------------------------------
technique RenderScene_GeometrynLightsOnly
{
  pass P0
  {   
    CullMode = NONE;
    VertexShader = compile vs_3_0 VS_woNorm();
    PixelShader  = compile ps_3_0 PS_woNorm(false); 
  }
}








