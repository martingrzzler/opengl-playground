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
#include "DirectionalLight.h"
#include "Material.h"
#include "Mesh.h"
#include "PointLight.h"
#include "ShaderProgram.h"
#include "Texture.h"
#include "Window.h"
#include "utils.h"

void create_meshes(std::vector<std::unique_ptr<Mesh>> &meshes)
{
	GLfloat vertices[] = {
			//x      y       z    s     t    normal
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, // left 0
			0.0f, 0.0f, -2.0f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f,	// in 1
			1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,	// right 2
			0.0f, 1.0f, 0.0f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f};	// up 3

	unsigned int ground_indices[] = {
			0, 2, 1,
			1, 2, 3};
	GLfloat ground_vertices[] = {
			-10.0f, 0.0f, -10.0f, 0.0f, 10.0f, 0.0f, 0.0f, 0.0f, // top left 0
			10.0f, 0.0f, -10.0f, 10.0f, 10.0f, 0.0f, 0.0f, 0.0f, // top right 1
			-10.0f, 0.0f, 10.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,	 // bottom left 2
			10.0f, 0.0f, 10.0f, 10.0f, 0.0f, 0.0f, 0.0f, 0.0f};	 // bottom right 3

	unsigned int indices[] = {
			0, 3, 1,
			3, 2, 1,
			0, 2, 1,
			0, 2, 3};
	utils::calc_avg_normals(indices, 12, vertices, 32, 8, 5);

	auto triangle = std::make_unique<Mesh>();
	triangle->create(vertices, indices, 32, 12);
	meshes.push_back(std::move(triangle));

	auto ground = std::make_unique<Mesh>();
	utils::calc_avg_normals(ground_indices, 6, ground_vertices, 32, 8, 5);
	ground->create(ground_vertices, ground_indices, 32, 6);
	meshes.push_back(std::move(ground));
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
	GLuint uniform_view = 0,
				 uniform_eye_position = 0,
				 uniform_specular_intensity = 0, uniform_shininess = 0;

	DirectionalLight directional_light = DirectionalLight::builder()
																					 .direction(0.0f, 1.0f, 0.0f)
																					 .color(1.0f, 1.0f, 1.0f)
																					 .diffuse_intensity(1.0f)
																					 .ambient_intensity(0.0f);
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
	uniform_specular_intensity = shaders[0]->specular_intensity_location();
	uniform_shininess = shaders[0]->shininess_location();
	uniform_eye_position = shaders[0]->eye_position_location();

	float scale = 1.0f;
	bool grow = false;
	glm::mat4 projection(1.0f);
	projection = glm::perspective(glm::radians(45.0f), (GLfloat)window.buffer_width() / window.buffer_height(), 0.1f, 100.0f);

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
		glUniformMatrix4fv(uniform_projection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniform_view, 1, GL_FALSE, glm::value_ptr(camera.view_matrix()));
		glm::vec3 cam_pos = camera.position();
		glUniform3f(uniform_eye_position, cam_pos.x, cam_pos.y, cam_pos.z);

		glm::mat4 model(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 1.0f, 0.0f));
		// model = glm::rotate(model, static_cast<float>(glfwGetTime() * i / 2), glm::vec3(0.0f, 1.0f, 0.0f));
		// model = glm::scale(model, glm::vec3(0.6f));
		glUniformMatrix4fv(uniform_model, 1, GL_FALSE, glm::value_ptr(model));
		metal.use();
		shiny_mat.use(uniform_specular_intensity, uniform_shininess);
		meshes[0]->render();

		// gound / end of list
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		glUniformMatrix4fv(uniform_model, 1, GL_FALSE, glm::value_ptr(model));
		tiles.use();
		// rougher_mat.use(uniform_specular_intensity, uniform_shininess);
		meshes[1]->render();

		shaders[0]->unuse();

		window.swap_buffers();
	}

	glfwTerminate();
}
