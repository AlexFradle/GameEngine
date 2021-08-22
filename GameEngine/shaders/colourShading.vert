#version 130

in vec2 vertexPos;
in vec4 vertexCol;
in vec2 vertexUV;

out vec2 fragmentPos;
out vec4 fragmentCol;
out vec2 fragmentUV;

uniform mat4 P;

void main() {
	gl_Position.xy = (P * vec4(vertexPos, 0.0, 1.0)).xy;
	gl_Position.z = 0.0;
	gl_Position.w = 1.0;

	fragmentCol = vertexCol;
	fragmentPos = vertexPos;
	fragmentUV = vec2(vertexUV.x, 1.0 - vertexUV.y);
}
