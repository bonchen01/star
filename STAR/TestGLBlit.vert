varying vec2 v_texCoord;

void main()
{

     gl_Position     = ftransform();
	v_texCoord = gl_Position.xy;
}