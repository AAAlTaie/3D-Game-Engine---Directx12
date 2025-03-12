struct ps_in
{
    float4 pos : SV_Position;
    float4 color : COLOR;
};

float4 ps_main(ps_in input) : SV_TARGET
{
    return input.color;
   // return float4(1, 1, 0, 1);
}