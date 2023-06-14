#version 330

// very simple, emissive only shader

out vec4 finalColor;
uniform vec4 color;
uniform float cycleTime = 4.5f;



void main()
{
	//finalColor = color * (1.0f + sin(cycleTime)) * 0.5f;
	//finalColor.r = color  * float((1.0f + sin(cycleTime + 0.5f)*0.5f);
	//finalColor.g = color  * float((1.0f + sin(cycleTime + 1.0f)*0.5f);
	//finalColor.b = color  * float((1.0f + sin(cycleTime + 1.5f)*0.5f);
	//finalColor.a = 1.0f;

	vec3 color = 0.5 + 0.5*sin(cycleTime/2 + vec3(0,2,4));
	//Rate of colour cycle change

	finalColor = vec4(color,1.0);


}