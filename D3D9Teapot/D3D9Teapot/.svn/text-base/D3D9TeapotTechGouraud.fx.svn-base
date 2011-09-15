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
float4 g_Diffuse    = float4(1, 0, 0, 1);    // Material's diffuse color
float4 g_Specular   = float4(0.65, 0.65, 0.25, 1);
float  g_kDiffuse   = 1.0f;
float  g_PhongExp   = 128.0f;


//************ Textures *************/
texture g_DiffuseTex : DIFFUSE;
texture g_NormalTex  : NORMAL;

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
    float4 vPos      : POSITION;	// Xfmed Wvp vertex position
    float4 vColor    : COLOR;
};


//************ Shaders *************/

VS_OUTPUT VS_woNorm( VS_IN In )
{
    VS_OUTPUT Out = (VS_OUTPUT)0;
    

    float3 normalW = mul(float4(In.vNormal, 0.0f), g_mWorld).xyz;
    normalW = normalize(normalW);

    float3 posW = mul(float4(In.vPos, 1.0f), g_mWorld).xyz;
    float3 vLightDir = g_vLightPos[0] - posW;
    vLightDir = normalize(vLightDir);
    float3 toEye = normalize(g_vEyePos - posW);

    float3 r = reflect(-vLightDir, normalW);

    float t = pow(max(dot(r, toEye), 0.0f), 32);

    float s = max(dot(vLightDir, normalW), 0.0f);

    float3 spec = t * (g_Specular*g_vLightColor[0]).rgb;
    float3 diffuse = s * (g_Diffuse*g_vLightColor[0]).rgb;
    float3 ambient = g_Ambient * float4(0.4, 0.4, 0.4, 1.0);

    Out.vColor.rgb =  ambient + diffuse + spec;
    Out.vColor.a = g_Diffuse.a;

    // Transform the position from object space to homogeneous projection space
    Out.vPos      = mul( float4(In.vPos, 1.0f), g_mWorldViewProjection );

    return Out;
}


float4 PS_woNorm( VS_OUTPUT In, uniform bool bTexture ) : COLOR
{
  return In.vColor;

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








