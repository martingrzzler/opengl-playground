#include "Window.h"
#include "GL/glew.h"
#include <iostream>

Window::Window()
{
	_width = 800;
	_height = 600;
	_change_in_x = 0.0f;
	_change_in_y = 0.0f;
	_last_x = _width / 2.0f;
	_last_y = _height / 2.0f;
	memset(_keys, 0, 1024);
}

Window::Window(GLint width, GLint height)
{
	_height = height;
	_width = width;
	_last_x = _width / 2.0f;
	_last_y = _height / 2.0f;
	_change_in_x = 0.0f;
	_change_in_y = 0.0f;
	memset(_keys, 0, 1024);
}

int Window::init()
{
	if (!glfwInit())
	{
		std::cerr << "ERRROR: Failed to initialize GLFW " << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	_window = glfwCreateWindow(_width, _height, "opengl-udemy", NULL, NULL);

	if (!_window)
	{
		std::cout << "Failed to create Window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwGetFramebufferSize(_window, &_buffer_width, &_buffer_height);
	glfwMakeContextCurrent(_window);

	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		std::cout << "ERROR: GLEW initialization failed" << std::endl;
		glfwDestroyWindow(_window);
		glfwTerminate();
		return -1;
	}
	std::cout << "Open GL version: " << glGetString(GL_VERSION) << std::endl;
	glViewport(0, 0, _buffer_width, _buffer_height);
	glEnable(GL_DEPTH_TEST);
	glfwSetWindowUserPointer(_window, this);

	glfwSetKeyCallback(_window, on_key_pressed);
	glfwSetCursorPosCallback(_window, on_mouse);
	glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSwapInterval(1);

	return 0;
}

GLint Window::buffer_width()
{
	return _buffer_width;
}

GLint Window::buffer_height()
{
	return _buffer_height;
}

bool Window::should_close()
{
	return glfwWindowShouldClose(_window);
}

void Window::swap_buffers()
{
	glfwSwapBuffers(_window);
}

Window::~Window()
{
}

void Window::on_key_pressed(GLFWwindow *window, int key, int code, int action, int mode)
{
	auto self = static_cast<Window *>(glfwGetWindowUserPointer(window));
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key < 0 || key >= 1024)
		return;

	if (action == GLFW_PRESS)
	{
		self->_keys[key] = true;
	}
	else if (action == GLFW_RELEASE)
	{
		self->_keys[key] = false;
	}
}

bool *Window::key_states()
{
	return _keys;
}

GLfloat Window::change_in_x()
{
	auto tmp = _change_in_x;
	_change_in_x = 0;
	return tmp;
}
GLfloat Window::change_in_y()
{
	auto tmp = _change_in_y;
	_change_in_y = 0;
	return tmp;
}

void Window::on_mouse(GLFWwindow *window, double x, double y)
{
	auto self = static_cast<Window *>(glfwGetWindowUserPointer(window));

	if (self->_mouse_first_moved)
	{
		self->_last_x = static_cast<GLfloat>(x);
		self->_last_y = static_cast<GLfloat>(y);
		self->_mouse_first_moved = false;
	}

	self->_change_in_x = static_cast<GLfloat>(x) - self->_last_x;
	self->_change_in_y = self->_last_y - static_cast<GLfloat>(y);
	self->_last_x = static_cast<GLfloat>(x);
	self->_last_y = static_cast<GLfloat>(y);
}
