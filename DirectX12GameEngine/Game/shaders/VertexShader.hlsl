struct vs_in
{
	float3 pos : POSITION;
	float4 color : COLOR;

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
	output.color = input.color;

	return output;
}