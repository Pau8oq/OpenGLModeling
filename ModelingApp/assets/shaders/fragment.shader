#version 330 core

#define MAX_POINT_LIGHTS 4  

out vec4 FragColor;

struct Material {
	sampler2D diffuse;
	sampler2D specular;
	sampler2D emission;
	float shininess;
};

uniform sampler2D diffuse0;
uniform sampler2D specular0;

struct PointLight {
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};
uniform PointLight pointLights[MAX_POINT_LIGHTS];
uniform int noPointLights;

struct DirLight {
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
uniform DirLight dirLight;

struct SpotLight {
	vec3  position;
	vec3  direction;
	float cutOff;
	float outerCutOff;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};
uniform SpotLight spotLight;

vec3 CalcDirLight(DirLight dirLight, vec3 normal, vec3 viewDir, vec3 diffMap, vec3 specMap);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 diffMap, vec3 specMap);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 diffMap, vec3 specMap);

in vec3 FragPos;
in vec2 TexCoord;
in vec3 Normal;

uniform sampler2D texture1;
uniform sampler2D texture2;


uniform vec3 viewPos;
uniform Material material;




void main()
{
	//FragColor = texture(texture1, TexCoord);
	
	vec3 norm = normalize(Normal);
	vec3 viewDir = normalize(viewPos - FragPos);

	vec3 diffMap;
	vec3 specMap;

	diffMap = vec3(texture(diffuse0, TexCoord));
	specMap = vec3(texture(specular0, TexCoord));

	vec3 result = CalcDirLight(dirLight, norm, viewDir, diffMap, specMap);

	for (int i = 0; i < noPointLights; i++)
		result += CalcPointLight(pointLights[i], norm, FragPos, viewDir, diffMap, specMap);
	
	result += CalcSpotLight(spotLight, norm, FragPos, viewDir, diffMap, specMap);

	//test z-buffer
	/*result.z = gl_FragCoord.z;
	result.x = gl_FragCoord.z;
	result.y = gl_FragCoord.z;*/
	FragColor = vec4(result, 1.0);

}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir, vec3 diffMap, vec3 specMap)
{
	vec3 lightDir = normalize(-light.direction);
	//diffuse shading
	float diff = max(dot(normal, lightDir), 0.0);
	//specular shading
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess * 128);
	//combine result
	vec3 ambient = light.ambient * diffMap;
	vec3 diffuse = light.diffuse * diff * diffMap;
	vec3 specular = light.specular * spec * specMap;
	return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 diffMap, vec3 specMap)
{
	vec3 lightDir = normalize(light.position - fragPos);
	//diffuse shading
	float diff = max(dot(normal, lightDir), 0.0);
	//specular shading
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess * 128);
	// attenuation
	float distance = length(light.position - FragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance +
		light.quadratic * (distance * distance));
	// combine results
	vec3 ambient = light.ambient * diffMap;
	vec3 diffuse = light.diffuse * diff * diffMap;
	vec3 specular = light.specular * spec * specMap;
	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;
	return (ambient + diffuse + specular);
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 diffMap, vec3 specMap)
{
	vec3 lightDir = normalize(light.position - fragPos);

	float theta = dot(lightDir, normalize(-light.direction));
	float epsilon = (light.cutOff - light.outerCutOff);
	float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

	if (theta > light.outerCutOff)
	{
		vec3 lightDir = normalize(light.position - fragPos);
		//diffuse shading
		float diff = max(dot(normal, lightDir), 0.0);
		//specular shading
		vec3 reflectDir = reflect(-lightDir, normal);
		float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess * 128);

		// combine results
		vec3 ambient = light.ambient * diffMap;
		vec3 diffuse = light.diffuse * diff * diffMap;
		vec3 specular = light.specular * spec * specMap;

		diffuse *= intensity;
		specular *= intensity;


		return (ambient + diffuse + specular);
	}
	else
		return light.ambient * texture(material.diffuse, TexCoord).rgb;

}