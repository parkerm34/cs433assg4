#version 410 core

struct LightProperties {
    bool isEnabled;
    bool isLocal;
    bool isSpot;
    vec3 ambient;
    vec3 color;
    vec3 position;
    vec3 halfVector;
    vec3 coneDirection;
    float spotCosCutoff;
    float spotExponent;
    float constantAttenuation;
    float linearAttenuation;
    float quadraticAttenuation;
};

const int MAXLIGHTS = 4;
uniform LightProperties Lights[MAXLIGHTS];

in vec3 ambient;
in vec3 diffuse;
in vec3 specular;
in float shininess;
in vec3 Normal;
in vec4 Position;

out vec4 fColor;

void
main()
{
	float tempx = ambient.x + diffuse.x;
	float tempy = ambient.y + diffuse.y;
	float tempz = ambient.z + diffuse.z;
	if(tempx > 1.0)
		tempx = 1.0;
	if(tempy > 1.0)
		tempy = 1.0;
	if(tempz > 1.0)
		tempz = 1.0;
	fColor = vec4(diffuse, 0.0);
/*
	vec3 scatteredLight = vec3(0.0);
	vec3 reflectedLight = vec3(0.0);
	vec3 eyeDirection;
	vec3 lightDirection;
	vec3 halfVector;
	vec3 myNormal;
	float attenuation = 1.0f;
	float diffuseCoeff;
	float specularCoeff;

	for(int light = 0; light < MAXLIGHTS; ++light)
	{
		if(! Lights[light].isEnabled)
			continue;

		attenuation = 1.0;
		eyeDirection = normalize(-vec3(Position));

		if(Lights[light].isLocal)
		{
			lightDirection = Lights[light].position - vec3(Position);
			float lightDistance = length(lightDirection);
			lightDirection = normalize(lightDirection);

			attenuation = 1.0 / (Lights[light].constantAttenuation + 
				Lights[light].linearAttenuation * lightDistance +
				Lights[light].quadraticAttenuation * lightDistance * lightDistance);

			if(Lights[light].isSpot)
			{
				vec3 myConeDirection = normalize(Lights[light].coneDirection);
				float spotCos = dot(lightDirection,-myConeDirection);
				
				if(spotCos < Lights[light].spotCosCutoff)
					attenuation = 0.0;
				else
					attenuation *= pow(spotCos, Lights[light].spotExponent);
				halfVector = normalize(lightDirection + eyeDirection);

			}
		}
		else
		{
			lightDirection = normalize(Lights[light].position);
			halfVector = normalize(lightDirection + eyeDirection);
		}

		myNormal = normalize(Normal);
		diffuseCoeff = max(0.0, dot(myNormal, lightDirection));
		specularCoeff = max(0.0, dot(myNormal, halfVector));

		if(diffuseCoeff == 0.0)
			specularCoeff = 0.0;
		else
			specularCoeff = pow(specularCoeff, shininess);

		scatteredLight += Lights[light].ambient * ambient * attenuation +
			Lights[light].color * (diffuseCoeff * diffuse) * attenuation;
		reflectedLight += Lights[light].color * (specularCoeff * specular) * attenuation;
	}

	vec3 rgb = min(scatteredLight + reflectedLight, vec3(1.0));
	fColor = vec4(rgb, 0.0f);
*/}
