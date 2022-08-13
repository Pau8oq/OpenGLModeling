#include "light.h";

void PointLight::render(Shader shader)
{
	PointLight::render(shader, 0);
}

void PointLight::render(Shader shader, int idx)
{
	std::string name = "pointLights[" + std::to_string(idx) + "]";

	shader.setFloat(name + ".constant", constant);
	shader.setFloat(name + ".linear", linear);
	shader.setFloat(name + ".quadratic", quadratic);

	shader.set3Float(name + ".position", position);
	shader.set3Float(name + ".ambient", ambient);
	shader.set3Float(name + ".diffuse", diffuse);
	shader.set3Float(name + ".specular", specular);
}

void DirLight::render(Shader shader)
{
	std::string name = "dirLight";

	shader.set3Float(name + ".direction", direction);
	shader.set3Float(name + ".ambient", ambient);
	shader.set3Float(name + ".diffuse", diffuse);
	shader.set3Float(name + ".specular", specular);
}

void SpotLight::render(Shader shader)
{
	std::string name = "spotLight";

	shader.set3Float(name + ".position", position);
	shader.set3Float(name + ".direction", direction);
	shader.setFloat(name + ".cutOff", cutOff);
	shader.setFloat(name + ".outerCutOff", outerCutOff);

	shader.set3Float(name + ".ambient", ambient);
	shader.set3Float(name + ".diffuse", diffuse);
	shader.set3Float(name + ".specular", specular);

	shader.setFloat(name + ".constant", constant);
	shader.setFloat(name + ".linear", linear);
	shader.setFloat(name + ".quadratic", quadratic);
}