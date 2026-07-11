struct VOut
{
    float4 pos : SV_POSITION;
    float2 tex : TEXCOORD;
};

Texture2D objTexture : register(t0);
SamplerState objSampler : register(s0);

float4 main(VOut input) : SV_TARGET
{
    return objTexture.Sample(objSampler, input.tex);
}