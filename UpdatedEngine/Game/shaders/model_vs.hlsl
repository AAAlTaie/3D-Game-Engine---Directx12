cbuffer matrices : register(b0)
{
    matrix proj;
    matrix view;

};

cbuffer matrices : register(b1)
{
    matrix model;
};

struct vs_in
{
    float3 pos : POSITION;
    float3 nomal : NORMAL;
    float2 st : ST;
    float3 binormal : BINORMAL;
    float3 tangent : TANGENT;

};

struct vs_out
{
    float4 pos : SV_Position;
    float4 color : COLOR;
};

vs_out vs_main(vs_in input)
{
    vs_out output;
    output.pos = float4(input.pos, 1);
    
    //MVP
    
    output.pos = mul(output.pos, model);
    output.pos = mul(output.pos, view);
    output.pos = mul(output.pos, proj);
    
    output.color = float4(input.nomal, 1);
    return output;
   
}