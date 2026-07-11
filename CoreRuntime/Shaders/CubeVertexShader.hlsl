cbuffer PerModelBuffer : register(b0)
{
    matrix ModelViewProjection;
}

cbuffer PerFrameBuffer : register(b1)
{
    float Time;
    float DeltaTime;
    float bendAmount;
    bool UseWave;
}

struct VOut
{
    float4 pos : SV_POSITION;
    float2 tex : TEXCOORD;
};

VOut main(float3 pos : POSITION, float2 tex : TEXCOORD)
{
    VOut output;
    output.pos = mul(float4(pos, 1.0f), ModelViewProjection);
    output.tex = tex;

    return output;
}