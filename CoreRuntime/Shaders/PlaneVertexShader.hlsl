cbuffer PerModelBuffer : register(b0)
{
    matrix modelViewProjection;
}

cbuffer PerFrameBuffer : register(b1)
{
    float time;
    float deltaTime;
    float bendAmount1;
    bool useWave;
}

struct PSInput
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD;
};

PSInput main(float3 pos : POSITION, float2 uv : TEXCOORD)
{
    PSInput output;
    float bendAmount = 2.0;
    
    if (useWave)
        pos.y += sin(pos.z + time) * 0.5;
    
    output.pos = mul(float4(pos, 1.0), modelViewProjection);
    output.uv = uv;
    
    return output;
}