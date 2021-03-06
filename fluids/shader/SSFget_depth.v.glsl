# version 330 core

layout(location = 0) in vec3 aPos;

uniform mat4 proj;
uniform mat4 view;
uniform int s_h;
uniform float p_t;
uniform float p_n;
uniform float r;

out vec4 viewPos;
out vec4 projPos;

void main() {
	viewPos = view * vec4(aPos, 1.0);
	float dist = length(vec3(viewPos));

	/* gl_PointSize in pixels
	 * NOT RADIUS BUT DIAMETER  
	 */
	gl_Position = proj * viewPos;
	projPos = proj * viewPos;
	gl_PointSize = r*p_n*s_h / (-viewPos.z * p_t);
}