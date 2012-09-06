#version 150 

//these are the interpolated values out of the rasterizer, so you can't know
//their specific values without knowing the vertices that contributed to them
in vec3 fs_normal;
in vec3 fs_light_vector;
in vec4 fs_color;

out vec4 out_Color;

void main() {
    //base colors for materials
    vec4 diffuseColor = fs_color;
    
    //calculate diffuse term and clamp to the range [0, 1]
    float diffuseTerm = clamp(dot(normalize(fs_normal), normalize(fs_light_vector)), 0.0, 1.0);
    
    out_Color = 2 * diffuseColor * (diffuseTerm*0.8 + 0.2);
	if (out_Color.x < 0) {
		out_Color.x = 0;
	}
	if (out_Color.y < 0) {
		out_Color.y = 0;
	}
	if (out_Color.z < 0) {
		out_Color.z = 0;
	}
	if (out_Color.x > 1) {
		out_Color.x = 1;
	}
	if (out_Color.y > 1) {
		out_Color.y = 1;
	}
	if (out_Color.z > 1) {
		out_Color.z = 1;
	}
	//out_Color = diffuseColor;
}