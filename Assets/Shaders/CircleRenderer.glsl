// Basic Texture Shader

#type vertex
#version 330 core

layout(location = 0) in vec2 a_Vertices;
layout(location = 1) in vec2 a_Position;
layout(location = 2) in float a_Radius;
layout(location = 3) in float a_OutlineSize;
layout(location = 4) in vec4 a_FillColor;
layout(location = 5) in vec4 a_OutlineColor;

uniform mat4 u_ViewProjection;

out vec2 v_Position;
out vec4 v_FillColor;
out vec4 v_OutlineColor;
out float v_OutlineSize;
out float v_Radius;


void main()
{
	v_Position = a_Position;
	v_FillColor = a_FillColor;
	v_OutlineColor = a_OutlineColor;
	v_OutlineSize = a_OutlineSize;
	v_Radius = a_Radius;

	gl_Position = u_ViewProjection * vec4(a_Vertices, 0.0, 1.0);
}

#type fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_Position;
in vec4 v_FillColor;
in vec4 v_OutlineColor;
in float v_OutlineSize;
in float v_Radius;

void main()
{
	vec2 pixel = gl_FragCoord.xy / gl_FragCoord.w;
	float distanceSquared = (v_Position.x - pixel.x) * (v_Position.x - pixel.x) + (v_Position.y - pixel.y) * (v_Position.y - pixel.y);
	float radiusSquared = v_Radius * v_Radius;
	float outlineSquared = (v_Radius + v_OutlineSize) * (v_Radius + v_OutlineSize);

	if(distanceSquared <= radiusSquared)
	{
		color = v_FillColor;
	}
	else if(distanceSquared <= outlineSquared)
	{
	    color = v_OutlineColor;
	}
	else
	{
		discard;
	}
}
