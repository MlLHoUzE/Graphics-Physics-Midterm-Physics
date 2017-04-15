#version 400

// FRAGMENT shader

in vec3 diffuseColour;
in vec3 ambientColour;
in vec4 vertWorldXYZ;
in vec4 vertNorm;


uniform vec3 myLightPosition;		// In the world
uniform vec3 myLightDiffuse;		// 1,1,1 (white colour light)
uniform vec3 myLightAmbient;

// For spec:
uniform vec3 eyeLocation;

out vec4 out_Colour;

void main()
{
	vec3 lightv = myLightPosition - vertWorldXYZ.xyz;
	
	float distanceFromLight = length(lightv); 
	float lightLinAtten = 1.0001f;
	float lightQuadAtten = 0.0f;
	
	float linAtten = (1.0f/distanceFromLight) * lightLinAtten;  
	
//	float quadAtten = (1.0f/(distanceFromLight*distanceFromLight))
//	                    * lightQuadAtten;
	
	lightv = normalize(lightv);
	
 	vec3 diffuse = max(0.0, dot(lightv, vertNorm.xyz)) 
                   * diffuseColour
				   * myLightDiffuse;
				   	
	// Calculate the linear attenuation of the diffuse
	diffuse.xyz *= linAtten;	
	
	vec3 ambient =  ambientColour * myLightAmbient;
							
	// With newer (we're using version 4 of GLSL) some of the 
	// 	built in variables are depricated, like gl_FragColor
	//gl_FragColor = vec4(diffuse, 1.0f);	
	
	// Bump up the brightness (projector is too dark)
	//diffuse.xyz *= vec3(1.5f);
	
	
	//return clamp( ambient + diffuse + specular, 0.0, 1.0);
	
	vec3 out_Colour_Clamp = clamp(ambient + diffuse, 0.0, 1.0);
	out_Colour = vec4(out_Colour_Clamp, 1.0f);
	
	
	//out_Colour *= 0.001f;	// make it (essentially) zero.	
	//out_Colour += vec4(vertNorm);
}