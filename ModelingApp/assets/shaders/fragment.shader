#version 330 core
out vec4 FragColor;

struct Material {
	sampler2D diffuse;
	sampler2D specular;
	sampler2D emission;
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
	//FragColor = texture(texture1, TexCoord);
	// ambient
	vec3 ambient = pointLight.ambient * texture(material.diffuse, TexCoord).rgb;
	
	// diffuse
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(pointLight.position - FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = pointLight.diffuse * diff * texture(material.diffuse, TexCoord).rgb;

	// specular
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflection = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflection), 0.0), material.shininess * 128);
	vec3 specular = pointLight.specular * spec * texture(material.specular, TexCoord).rgb;

	vec3 emission = vec3(0.0);
	if (texture(material.specular, TexCoord).r == 0.0)   /*rough check for blackbox inside spec texture */
	{
		/*apply emission texture some cool effect (might be diff or spec) */
		emission = texture(material.emission, TexCoord).rgb  * spec;
	}

	vec3 result = ambient + diffuse + specular + emission;

	FragColor = vec4(result, 1.0);

	//FragColor = texture(texture1 * ambient, TexCoord);
}