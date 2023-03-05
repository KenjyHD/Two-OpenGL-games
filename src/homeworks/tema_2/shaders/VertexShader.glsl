#version 330

// Input
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_coord;
layout(location = 3) in vec3 v_color;


// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform vec4 carPos;
uniform int CameraType;
uniform float Curvature;

// Output
out vec3 frag_position;
out vec3 frag_normal;
out vec2 frag_coord;
out vec3 frag_color;

void main()
{	
	frag_coord = v_coord;
	frag_normal = v_normal;
    frag_color = v_color;
	frag_position = v_position;

	gl_Position = Projection * View * Model * vec4(v_position, 1.0);
	vec4 curve = carPos - gl_Position;
	if (CameraType == 0){
		gl_Position[1] = gl_Position[1] - (curve[0] * curve[0] + curve[1] * curve[1] + curve[2] * curve[2] + curve[3] * curve[3]) * Curvature;
	}
	
}
