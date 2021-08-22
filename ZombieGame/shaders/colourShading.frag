#version 130

in vec4 fragmentCol;
in vec2 fragmentPos;
in vec2 fragmentUV;

out vec4 colour;

//uniform float time;
uniform sampler2D mySampler;

void main() {
	/*colour = vec4(fragmentCol.r * (sin(fragmentPos.x*10.0 + time) + 1.0) * 0.5,
				  fragmentCol.g * (sin(fragmentPos.y*10.0 + time) + 1.0) * 0.5,
				  fragmentCol.b * (sin(fragmentPos.x*10.0 + time) + 1.0) * 0.5,
				  fragmentCol.a);*/

	vec4 textureCol = texture(mySampler, fragmentUV);
	colour = textureCol * fragmentCol;
}
/*
uniform float time;
float t = time * 0.91;

void main()
{
	vec2 resolution = vec2(1024, 768);
    vec2 r = resolution,
    o = gl_FragCoord.xy - resolution/2.;
    o = vec2(length(o) / r.y - ((1.+sin(t))/8.+.1), atan(o.y,o.x));    
    vec4 s = 0.07*cos(vec4(4.5,3,1.5,0) + t + o.y),
    e = s.yzwx, 
    f = max(o.x-s,e-o.x);
    gl_FragColor = dot(clamp(f*r.y,0.,1.), 90.*(s-e)) * (s-.1) + f;

}
*/