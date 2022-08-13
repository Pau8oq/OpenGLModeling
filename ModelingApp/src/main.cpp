#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../Graphics/shader.h"
#include "../Graphics/texture.h"
#include "../Graphics/mesh.h"
#include "../Graphics/model.h"
#include "../Graphics/Models/cube.h"
#include "../Graphics/Models/lamp.h"

#include "../IO/screen.h"
#include "../IO/keyboard.h"
#include "../IO/mouse.h"
#include "../IO/camera.h"


void process_input(double dt);
void rotateObjYAxis(Cube& obj);
void rotateObjXAxis(Cube& obj);

double deltaTime = 0.0f;
double lastFrame = 0.0f;

Screen screen;
Camera camera(glm::vec3(0.0f, 0.0f, -3.0f), CameraType::ORBIT);

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif 

	if (!screen.init())
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		glfwTerminate();
		return -1;
	}

	screen.setParameters();

	Shader shader("assets/shaders/vertex.shader", "assets/shaders/fragment.shader");
	Shader lamp_shader("assets/shaders/lamp-vertex.shader", "assets/shaders/lamp-fragment.shader");

	/*Texture texture1("assets/Textures/wood.jpg", "texture1");
	Texture texture2("assets/Textures/wall.jpg", "texture2");*/
	Texture deffMap("assets/Textures/diffuseMap.png", "material.diffuse");
	Texture specMap("assets/Textures/specularMap.png", "material.specular");
	//Texture emission("assets/Textures/matrix.jpg", "material.emission");

	
	Cube cube(Material::gold, deffMap, specMap);
	cube.init();

	DirLight dirLight({ glm::vec3(-0.2f, -1.0f, -0.3f), glm::vec3(0.2f), glm::vec3(0.5f), glm::vec3(1.0f) });
	PointLight pointLight({ glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.2f), glm::vec3(0.5f), glm::vec3(1.0f), 1.0f,  0.09f, 0.032f });
	SpotLight spotLight({
		camera.getPos(),
		camera.getFront(),
		glm::cos(glm::radians(7.5f)),
		glm::cos(glm::radians(13.5f)),
		glm::vec3(0.2f),
		glm::vec3(0.5f),
		glm::vec3(1.0f),
		1.0f,  0.09f, 0.032f });

	Lamp lamp1(glm::vec3(1.0f), pointLight, glm::vec3(0.0f, 0.0f, 2.0f), glm::vec3(0.1f));
	lamp1.init();
	Lamp lamp2(glm::vec3(1.0f), pointLight, glm::vec3(0.0f, 0.0f, -2.0f), glm::vec3(0.1f));
	lamp2.init();

	Lamp lamps[2];
	lamps[0] = lamp1;
	lamps[1] = lamp2;


	while (!screen.shouldClose())
	{
		double currentTime = glfwGetTime();
		deltaTime = currentTime - lastFrame;
		lastFrame = currentTime;

		screen.update();

		process_input(deltaTime);

		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = camera.getViewMatrix();
		glm::mat4 proj = glm::perspective(camera.getFov(), float(screen.SCR_WIDTH / screen.SCR_HEIGTH), 0.1f, 100.f);

		shader.active();
		shader.setMat4("model", model);
		shader.setMat4("view", view);
		shader.setMat4("projection", proj);
		shader.setInt("noPointLights", 2);


		//should add scene class like three js has
		for (int i = 0; i < 2; i++)
			lamps[i].pointLight.render(shader, i);
		
		dirLight.render(shader);
		//pointLight.render(shader);
		spotLight.position = camera.getPos();
		spotLight.direction = camera.getFront();
		spotLight.render(shader);

		shader.set3Float("viewPos", camera.getPos());
		
	
		cube.render(shader);


		lamp_shader.active();
		lamp_shader.setMat4("model", model);
		lamp_shader.setMat4("view", view);
		lamp_shader.setMat4("projection", proj);


		for (int i = 0; i < 2; i++)
			lamps[i].render(lamp_shader);
		

		//rotateObjYAxis(lamps[0]);
		//rotateObjXAxis(lamps[1]);

		screen.newFrame();
	}

	cube.cleanup();
	for (int i = 0; i < 2; i++)
		lamps[i].cleanup();
	shader.clear();
	lamp_shader.clear();

	glfwTerminate();
	return 0;
}

void process_input(double dt)
{
	if (Keyboard::key(GLFW_KEY_ESCAPE))
	{
		screen.setShouldClose(true);
	}

	double dx = Mouse::getMouseDx();
	double dy = Mouse::getMouseDy();
	double scrollDy = Mouse::getScrollDy();

	if (Mouse::button(GLFW_MOUSE_BUTTON_LEFT) && (dx != 0 || dy != 0))
	{
		camera.updateCameraDirection(dx, dy);
	}

	if (scrollDy != 0)
		camera.updateCameraZoom(scrollDy);
	

	if (Keyboard::key(GLFW_KEY_W))
	{
		camera.updateCameraPos(CameraDirection::FORWARD, dt);
	}
	if (Keyboard::key(GLFW_KEY_S))
	{
		camera.updateCameraPos(CameraDirection::BACKWARD, dt);
	}
	if (Keyboard::key(GLFW_KEY_A))
	{
		camera.updateCameraPos(CameraDirection::LEFT, dt);
	}
	if (Keyboard::key(GLFW_KEY_D))
	{
		camera.updateCameraPos(CameraDirection::RIGHT, dt);
	}

}

void rotateObjYAxis(Cube& obj)
{
	int speed = 100;
	float x = glm::sin(glm::radians(glfwGetTime() * speed));
	float z = glm::cos(glm::radians(glfwGetTime() * speed));
	obj.pos = glm::vec3(x, 0.0f, z);


	//TODO: need to check  way of casting and ref and pointers
	Lamp* l = dynamic_cast<Lamp*>(&obj);
	
	if (l != 0)
	{
		l->pointLight.position = obj.pos;
	}

	obj.pos = glm::vec3(x, 0.0f, z);
}

void rotateObjXAxis(Cube& obj)
{
	int speed = 100;
	float y = glm::sin(glm::radians(glfwGetTime() * speed));
	float z = glm::cos(glm::radians(glfwGetTime() * speed));
	obj.pos = glm::vec3(0.0f, y, z);

	Lamp* l = dynamic_cast<Lamp*>(&obj);

	if (l != 0)
	{
		l->pointLight.position = obj.pos;
	}
}