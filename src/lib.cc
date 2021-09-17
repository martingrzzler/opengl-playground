#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <assert.h>
#include <cstdlib>
#include <ctime>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>
#include <memory>
#include <vector>

#include "Camera.h"
#include "Mesh.h"
#include "ShaderProgram.h"
#include "Texture.h"
#include "Window.h"

void create_meshes(std::vector<std::unique_ptr<Mesh>> &meshes)
{
	GLfloat vertices[] = {
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f, // left 0
			0.0f, 0.0f, 1.5f, 0.5f, 0.0f,		// in 1
			1.0f, -1.0f, 0.0f, 1.0f, 1.0f,	// right 2
			0.0f, 1.0f, 0.0f, 0.5f, 1.0f};	// up 3

	unsigned int indices[] = {
			0, 3, 1,
			3, 2, 1,
			0, 2, 1,
			0, 2, 3};
	for (int i = 0; i < 20; i++)
	{
		auto triangle = std::make_unique<Mesh>();
		triangle->create(vertices, indices, 20, 12);
		meshes.push_back(std::move(triangle));
	}
}

void create_shaders(std::vector<std::unique_ptr<ShaderProgram>> &shaders)
{
	auto basic_shader = std::make_unique<ShaderProgram>();
	basic_shader->create_from_file("../../src/shaders/vertex.glsl", "../../src/shaders/fragment.glsl");
	shaders.push_back(std::move(basic_shader));
}

void run()
{
	Window window = Window();
	Camera camera = Camera(
			glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3(0.0f, 1.0f, 0.0f),
			-90.0f,
			0.0f,
			10.0f,
			0.1f);
	std::vector<std::unique_ptr<Mesh>> meshes;
	std::vector<std::unique_ptr<ShaderProgram>> shaders;

	float delta = 0.0f;
	float lastFrame = 0.0f;
	int res = window.init();
	if (res != 0)
		return;
	std::srand((int)time(0));

	create_meshes(meshes);
	create_shaders(shaders);

	GLuint uniform_model = 0, uniform_projection = 0;
	GLuint uniform_view = 0;

	Texture tiles = Texture("../../assets/tiles.png");
	Texture rust = Texture("../../assets/rust.jpg");
	Texture bunt = Texture("../../assets/bunt.jpg");
	Texture *textures[] = {&tiles, &rust, &bunt};
	tiles.load();
	rust.load();
	bunt.load();

	uniform_model = shaders[0]->model_location();
	uniform_projection = shaders[0]->projection_location();
	uniform_view = shaders[0]->view_location();

	float scale = 1.0f;
	bool grow = false;
	glm::mat4 projection(1.0f);
	projection = glm::perspective(glm::radians(45.0f), (GLfloat)window.buffer_width() / window.buffer_height(), 0.1f, 100.0f);

	glm::vec3 positions[100];
	for (int i = 0; i < meshes.size(); i++)
	{
		positions[i] = glm::vec3(-std::rand() % 5 + 2, -std::rand() % 5 + 2, -std::rand() % 10 + 2);
	}

	while (!window.should_close())
	{
		float currentTime = (float)glfwGetTime();
		delta = currentTime - lastFrame;
		lastFrame = currentTime;

		glfwPollEvents();

		camera.key_control(window.key_states(), delta);
		camera.mouse_control(window.change_in_x(), window.change_in_y());

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		shaders[0]->use();
		glUniformMatrix4fv(uniform_projection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniform_view, 1, GL_FALSE, glm::value_ptr(camera.view_matrix()));

		for (int i = 0; i < meshes.size(); i++)
		{
			glm::mat4 model(1.0f);
			model = glm::translate(model, positions[i]);
			model = glm::rotate(model, static_cast<float>(glfwGetTime() * i / 2), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(0.6f));
			glUniformMatrix4fv(uniform_model, 1, GL_FALSE, glm::value_ptr(model));
			textures[i % 3]->use();
			meshes[i]->render();
		}

		shaders[0]->unuse();

		window.swap_buffers();
	}

	glfwTerminate();
}
