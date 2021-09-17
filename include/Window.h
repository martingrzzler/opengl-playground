#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Window
{
public:
	Window();
	Window(GLint width, GLint height);
	int init();
	GLint buffer_width();
	GLint buffer_height();
	bool should_close();
	void swap_buffers();
	GLfloat change_in_x();
	GLfloat change_in_y();
	bool *key_states();
	~Window();

private:
	GLFWwindow *_window;
	GLint _width, _height;
	GLint _buffer_width, _buffer_height;
	GLfloat _last_x, _last_y, _change_in_x, _change_in_y;
	bool _keys[1024];
	bool _mouse_first_moved;

	static void on_mouse(GLFWwindow *window, double x, double y);
	static void on_key_pressed(GLFWwindow *window, int key, int code, int action, int mode);
};