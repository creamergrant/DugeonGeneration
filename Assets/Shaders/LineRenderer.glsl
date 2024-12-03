// Polyline Shader

#type vertex
#version 330 core

layout(location = 0) in vec2 a_Vertices;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in float a_LineWidth;

uniform mat4 u_ViewProjection;

out vec4 v_Color;
out float v_LineWidth;

void main()
{
	v_Color = a_Color;
	v_LineWidth = a_LineWidth;
	gl_Position = u_ViewProjection * vec4(a_Vertices, 0.0, 1.0);
}


#type geometry
#version 330 core

layout( lines_adjacency ) in;
layout( triangle_strip, max_vertices = 7 ) out;

in vec4 v_Color[];
in float v_LineWidth[];

out vec4 f_Color;

uniform vec2 u_Viewport;

vec2 toScreenSpace( vec4 vertex )
{
	return vec2( vertex.xy / vertex.w ) * u_Viewport;
}

void main()
{
    f_Color = v_Color[0];

	//Get the four vertices passed to the shader:
	vec2 p0 = toScreenSpace( gl_in[0].gl_Position );	// start of previous segment
	vec2 p1 = toScreenSpace( gl_in[1].gl_Position );	// end of previous segment, start of current segment
	vec2 p2 = toScreenSpace( gl_in[2].gl_Position );	// end of current segment, start of next segment
	vec2 p3 = toScreenSpace( gl_in[3].gl_Position );	// end of next segment

	//Perform culling
	vec2 area = u_Viewport * 1.2;
	if( p1.x < -area.x || p1.x > area.x ) return;
	if( p1.y < -area.y || p1.y > area.y ) return;
	if( p2.x < -area.x || p2.x > area.x ) return;
	if( p2.y < -area.y || p2.y > area.y ) return;

	//Determine the direction of each of the 3 segments (previous, current, next)
	vec2 v0 = normalize( p1 - p0 );
	vec2 v1 = normalize( p2 - p1 );
	vec2 v2 = normalize( p3 - p2 );

	//Determine the normal of each of the 3 segments (previous, current, next)
	vec2 n0 = vec2( -v0.y, v0.x );
	vec2 n1 = vec2( -v1.y, v1.x );
	vec2 n2 = vec2( -v2.y, v2.x );

	//Determine miter lines by averaging the normals of the 2 segments
	vec2 miter_a = normalize( n0 + n1 );	// miter at start of current segment
	vec2 miter_b = normalize( n1 + n2 );	// miter at end of current segment

	//Determine the length of the miter by projecting it onto normal and then inverse it
	float length_a = v_LineWidth[0] / dot( miter_a, n1 );
	float length_b = v_LineWidth[0] / dot( miter_b, n1 );

	//Generate the triangle strip
	gl_Position = vec4( ( p1 + length_a * miter_a ) / u_Viewport, 0.0, 1.0 );
	EmitVertex();

	gl_Position = vec4( ( p1 - length_a * miter_a ) / u_Viewport, 0.0, 1.0 );
	EmitVertex();

	gl_Position = vec4( ( p2 + length_b * miter_b ) / u_Viewport, 0.0, 1.0 );
	EmitVertex();

	gl_Position = vec4( ( p2 - length_b * miter_b ) / u_Viewport, 0.0, 1.0 );
	EmitVertex();

	EndPrimitive();
}


#type fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec4 f_Color;

void main()
{
    color = f_Color;
}
