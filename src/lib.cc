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
#include "Light.h"
#include "Material.h"
#include "Mesh.h"
#include "ShaderProgram.h"
#include "Texture.h"
#include "Window.h"
#include "utils.h"

void create_meshes(std::vector<std::unique_ptr<Mesh>> &meshes)
{
	GLfloat vertices[] = {
			//x      y       z    s     t    normal
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, // left 0
			0.0f, 0.0f, 1.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f,		// in 1
			1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,	// right 2
			0.0f, 1.0f, 0.0f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f};	// up 3

	unsigned int indices[] = {
			0, 3, 1,
			3, 2, 1,
			0, 2, 1,
			0, 2, 3};
	utils::calc_avg_normals(indices, 12, vertices, 32, 8, 5);
	for (int i = 0; i < 20; i++)
	{
		auto triangle = std::make_unique<Mesh>();
		triangle->create(vertices, indices, 32, 12);
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
	Window window = Window(1366, 768);
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
	GLuint uniform_view = 0, uniform_ambient_intensity = 0, uniform_ambient_color = 0,
				 uniform_diffuse_intensity = 0, uniform_direction = 0, uniform_eye_position = 0,
				 uniform_specular_intensity = 0, uniform_shininess = 0;

	Light ambient_light = Light(1.0f, 1.0f, 1.0f, 0.1f,
															2.0f, -1.0f, 1.0f, 1.0f);

	Texture tiles = Texture("../../assets/tiles.png");
	Texture rust = Texture("../../assets/rust.jpg");
	Texture bunt = Texture("../../assets/bunt.jpg");
	Texture metal = Texture("../../assets/metal.jpg");
	Texture *textures[] = {&tiles, &rust, &bunt, &metal};
	tiles.load();
	rust.load();
	bunt.load();
	metal.load();

	Material shiny_mat = Material(1.0f, 128);
	Material rougher_mat = Material(0.3f, 4);
	Material *materials[] = {&shiny_mat, &rougher_mat};

	uniform_model = shaders[0]->model_location();
	uniform_projection = shaders[0]->projection_location();
	uniform_view = shaders[0]->view_location();
	uniform_ambient_intensity = shaders[0]->ambient_intensity_location();
	uniform_ambient_color = shaders[0]->ambient_color_location();
	uniform_diffuse_intensity = shaders[0]->diffuse_intensity_location();
	uniform_direction = shaders[0]->direction_location();
	uniform_specular_intensity = shaders[0]->specular_intensity_location();
	uniform_shininess = shaders[0]->shininess_location();
	uniform_eye_position = shaders[0]->eye_position_location();

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
		ambient_light.use(uniform_ambient_intensity, uniform_ambient_color, uniform_diffuse_intensity, uniform_direction);
		glUniformMatrix4fv(uniform_projection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniform_view, 1, GL_FALSE, glm::value_ptr(camera.view_matrix()));
		glm::vec3 cam_pos = camera.position();
		glUniform3f(uniform_eye_position, cam_pos.x, cam_pos.y, cam_pos.z);

		for (int i = 0; i < meshes.size(); i++)
		{
			glm::mat4 model(1.0f);
			model = glm::translate(model, positions[i]);
			// model = glm::rotate(model, static_cast<float>(glfwGetTime() * i / 2), glm::vec3(0.0f, 1.0f, 0.0f));
			// model = glm::scale(model, glm::vec3(0.6f));
			glUniformMatrix4fv(uniform_model, 1, GL_FALSE, glm::value_ptr(model));
			textures[i % 4]->use();
			materials[i % 2]->use(uniform_specular_intensity, uniform_shininess);
			meshes[i]->render();
		}

		shaders[0]->unuse();

		window.swap_buffers();
	}

	glfwTerminate();
}
