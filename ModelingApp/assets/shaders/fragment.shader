#version 330 core
out vec4 FragColor;

struct Material {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

struct PointLight {
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

in vec3 FragPos;
in vec2 TexCoord;
in vec3 Normal;

uniform sampler2D texture1;
uniform sampler2D texture2;


uniform vec3 viewPos;

uniform Material material;
uniform PointLight pointLight;


void main()
{
	//FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.5);
	// ambient
	vec3 ambient = pointLight.ambient * material.ambient;
	
	// diffuse
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(pointLight.position - FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = pointLight.diffuse * (diff * material.diffuse);

	// specular
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflection = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflection), 0.0), material.shininess * 128);
	vec3 specular = pointLight.specular * (spec * material.specular);

	vec3 result = ambient + diffuse + specular;

	FragColor = vec4(result, 1.0);

	//FragColor = texture(texture1 * ambient, TexCoord);
}