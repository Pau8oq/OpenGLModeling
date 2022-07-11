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

	Texture texture1("assets/Textures/wood.jpg", "texture1");
	//Texture texture2("assets/Textures/wall.jpg", "texture2");
	
	Cube cube(texture1);
	cube.init();

	Lamp lamp(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.2f, 3.0f), glm::vec3(0.1f));
	lamp.init();

	//shader.active();
	//shader.setInt("texture1", 0);
	//shader.setInt("texture2", 1);

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
		shader.set3Float("lightColor", lamp.lightColor);
		shader.set3Float("objectColor", glm::vec3(1.0f, 0.5f, 0.31f));
		shader.set3Float("lightPos", lamp.pos);
		shader.set3Float("viewPos", camera.getPos());
		

		cube.render(shader);


		lamp_shader.active();
		lamp_shader.setMat4("model", model);
		lamp_shader.setMat4("view", view);
		lamp_shader.setMat4("projection", proj);

		lamp.render(lamp_shader);
		
		
		screen.newFrame();
	}

	cube.cleanup();
	lamp.cleanup();
	shader.clear();

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