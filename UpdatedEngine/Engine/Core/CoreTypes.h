#pragma once
#include "3DMath.h"

namespace ENGINE
{
	struct LineVertex
	{
		// Default constructor
		LineVertex()
			: pos(float3(0.0f, 0.0f, 0.0f)),
			color(float4(1.0f, 1.0f, 1.0f, 1.0f)),
			normal(float3(0.0f, 1.0f, 0.0f))
		{}

		// Constructor with position only
		LineVertex(float x, float y, float z)
			: pos(x, y, z),
			color({ 1.0f, 0.3f, 0.06f, 1.0f }),
			normal(float3(0.0f, 1.0f, 0.0f))
		{}

		// Constructor with position and color
		LineVertex(const float3& _pos, const float4& _color)
			: pos(_pos),
			color(_color),
			normal(float3(0.0f, 1.0f, 0.0f))
		{}

		// Constructor with position, color, and normal
		LineVertex(const float3& _pos, const float4& _color, const float3& _normal)
			: pos(_pos), color(_color), normal(_normal) {}

		float3 pos;   // Position of the vertex
		float4 color; // Color of the vertex
		float3 normal; // Normal vector for lighting calculations

		// TODO: INPUT_Layout
	};

	struct Vertex
	{
		float3 pos;
		float3 normal;
		float2 st; // UV
		float3 binormal;
		float3 tangent;
	};
}
