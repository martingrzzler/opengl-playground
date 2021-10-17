#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <assert.h>
#include <cstdlib>
#include <ctime>
#include <exception>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>
#include <memory>
#include <vector>

#include "Camera.h"
#include "DirectionalLight.h"
#include "Material.h"
#include "Mesh.h"
#include "Model.h"
#include "PointLight.h"
#include "ShaderProgram.h"
#include "SpotLight.h"
#include "Texture.h"
#include "Window.h"
#include "utils.h"

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
	std::vector<std::unique_ptr<ShaderProgram>> shaders;

	float delta = 0.0f;
	float lastFrame = 0.0f;
	int res = window.init();
	if (res != 0)
		throw std::runtime_error("Initilization of Window failed");
	std::srand((int)time(0));

	create_shaders(shaders);
	auto tiles = std::make_shared<Texture>("../../assets", "tiles.png", TextureMaterial::DIFFUSE);

	std::vector<Vertex> vertices = {
			Vertex{glm::vec3(-100.0f, 0.0f, -100.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 100.0f)},
			Vertex{glm::vec3(100.0f, 0.0f, -100.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(100.0f, 100.0f)},
			Vertex{glm::vec3(-100.0f, 0.0f, 100.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f)},
			Vertex{glm::vec3(100.0f, 0.0f, 100.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(100.0f, 0.0f)},
	};
	std::vector<unsigned int> indices = {
			0, 3, 1,
			3, 2, 1,
			0, 2, 1,
			0, 2, 3};

	auto ground = std::make_unique<Mesh>(vertices, indices, std::vector({tiles}));

	GLuint uniform_model = 0,
				 uniform_projection = 0;
	GLuint uniform_view = 0,
				 uniform_eye_position = 0;

	DirectionalLight directional_light = DirectionalLight::builder()
																					 .direction(0.0f, 1.0f, 0.0f)
																					 .color(1.0f, 1.0f, 1.0f)
																					 .diffuse_intensity(0.7f)
																					 .ambient_intensity(0.1f);
	PointLight point_lights[MAX_POINT_LIGHTS];
	unsigned int point_light_count = 3;
	point_lights[0] = PointLight::builder()
												.position(-4.0f, 0.0f, 0.0f)
												.color(0.0f, 1.0f, 0.0f)
												.ambient_intensity(0.1f)
												.diffuse_intensity(1.0f)
												.attenuation(0.3f, 0.2f, 0.1f);
	point_lights[1] = PointLight::builder()
												.position(4.0f, 0.0f, 0.0f)
												.color(1.0f, 0.0f, 0.0f)
												.ambient_intensity(0.1f)
												.diffuse_intensity(1.0f)
												.attenuation(0.3f, 0.2f, 0.1f);
	point_lights[2] = PointLight::builder()
												.position(0.0f, 0.0f, -7.0f)
												.color(0.0f, 0.0f, 1.0f)
												.ambient_intensity(0.1f)
												.diffuse_intensity(1.0f)
												.attenuation(0.3f, 0.2f, 0.1f);

	SpotLight spot_lights[MAX_SPOT_LIGHTS];
	unsigned int spot_light_count = 2;
	spot_lights[0] = SpotLight::builder()
											 .position(-8.0f, 2.0f, 0.0f)
											 .color(1.0f, 1.0f, 1.0f)
											 .ambient_intensity(0.1f)
											 .diffuse_intensity(1.0f)
											 .attenuation(0.2f, 0.1f, 0.05f)
											 .edge(20.0f)
											 .direction(0.0f, -1.0f, 0.0f);
	spot_lights[1] = SpotLight::builder()
											 .position(8.0f, 2.0f, 0.0f)
											 .color(1.0f, 1.0f, 1.0f)
											 .ambient_intensity(0.1f)
											 .diffuse_intensity(1.0f)
											 .attenuation(1.0f, 0.0f, 0.0f)
											 .edge(20.0f)
											 .direction(100.0f, 0.0f, 0.0f);

	Material shiny_mat = Material(1.0f, 128);
	Material rougher_mat = Material(0.3f, 4);

	Model backpack = Model("../../assets/models/eye/eyeball.obj");
	Model plane = Model("../../assets/models/airplane/11805_airplane_v2_L2.obj");
	// Model mountain = Model("../../assets/models/mountain/mount.blend1.obj");
	// Model landscape = Model("../../assets/models/landscape/landscape.blend");

	uniform_model = shaders[0]->model_location();
	uniform_projection = shaders[0]->projection_location();
	uniform_view = shaders[0]->view_location();
	uniform_eye_position = shaders[0]->eye_position_location();

	float scale = 1.0f;
	bool grow = false;
	glm::mat4 projection(1.0f);
	projection = glm::perspective(glm::radians(45.0f), (GLfloat)window.buffer_width() / window.buffer_height(), 0.1f, 200.0f);

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
		shaders[0]->use_directional_light(directional_light);
		shaders[0]->use_point_lights(point_lights, point_light_count);
		shaders[0]->use_spot_lights(spot_lights, spot_light_count);
		glUniformMatrix4fv(uniform_projection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniform_view, 1, GL_FALSE, glm::value_ptr(camera.view_matrix()));
		glm::vec3 cam_pos = camera.position();
		glUniform3f(uniform_eye_position, cam_pos.x, cam_pos.y, cam_pos.z);

		glm::mat4 model(1.0f);
		model = glm::translate(model, glm::vec3(-3.0f, 5.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f));
		glUniformMatrix4fv(uniform_model, 1, GL_FALSE, glm::value_ptr(model));
		shaders[0]->use_material(shiny_mat);
		backpack.draw(shaders[0]);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		glUniformMatrix4fv(uniform_model, 1, GL_FALSE, glm::value_ptr(model));
		shaders[0]->use_material(shiny_mat);
		ground->draw(shaders[0]);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(8.0f, 4.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.01f));
		glUniformMatrix4fv(uniform_model, 1, GL_FALSE, glm::value_ptr(model));
		shaders[0]->use_material(shiny_mat);
		plane.draw(shaders[0]);

		// model = glm::mat4(1.0f);
		// model = glm::translate(model, glm::vec3(0.0f, 0.0f, -20.0f));
		// glUniformMatrix4fv(uniform_model, 1, GL_FALSE, glm::value_ptr(model));
		// shaders[0]->use_material(rougher_mat);
		// mountain.draw(shaders[0]);

		// model = glm::mat4(1.0f);
		// model = glm::translate(model, glm::vec3(0.0f, 0.0f, 20.0f));
		// glUniformMatrix4fv(uniform_model, 1, GL_FALSE, glm::value_ptr(model));
		// shaders[0]->use_material(rougher_mat);
		// landscape.draw(shaders[0]);

		shaders[0]->unuse();

		window.swap_buffers();
	}

	glfwTerminate();
}
