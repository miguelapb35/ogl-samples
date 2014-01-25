#ifndef TEST_INCLUDED
#define TEST_INCLUDED

#include "csv.hpp"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

/*
#include "glf/common.hpp"
#include "glf/vertex.hpp"
#include "glf/compiler.hpp"
#include "glf/sementics.hpp"
#include "glf/error.hpp"
*/
#include <glf/glf.hpp>
#include <glu/glu.hpp>

#include <memory>
#include <array>

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

class test
{
public:
	enum profile
	{
		CORE = GLFW_OPENGL_CORE_PROFILE,
		COMPATIBILITY = GLFW_OPENGL_COMPAT_PROFILE,
		ES = GLFW_OPENGL_ES_PROFILE
	};

	int operator()();

protected:
	enum template_test
	{
		TEMPLATE_TEST_IGNORE,
		TEMPLATE_TEST_EXECUTE
	};

	enum mouse_button
	{
		MOUSE_BUTTON_NONE = 0,
		MOUSE_BUTTON_LEFT = (1 << 0),
		MOUSE_BUTTON_RIGHT = (1 << 1),
		MOUSE_BUTTON_MIDDLE = (1 << 2)
	};

	enum key_action
	{
		KEY_PRESS = GLFW_PRESS,
		KEY_RELEASE = GLFW_RELEASE,
		KEY_REPEAT = GLFW_REPEAT
	};

	test(int argc, char* argv[], char const * Title, profile Profile, int Major, int Minor, glm::ivec2 const & WindowSize = glm::ivec2(640, 480), std::size_t FrameCount = 2);
	virtual ~test();

	virtual bool begin() = 0;
	virtual bool end() = 0;
	virtual bool render() = 0;

	void swap();
	void stop();
	void log(csv & CSV, char const * String);

	bool isExtensionSupported(char const * String);
	glm::ivec2 getWindowSize() const;
	bool isKeyPressed(int Key) const;

protected:
	void beginTimer();
	void endTimer();

protected:
	static glm::ivec2 const DEFAULT_WINDOW_SIZE;
	static std::size_t const DEFAULT_MAX_FRAME;

	GLFWwindow* Window;
	template_test const TemplateTest;
	std::string const Title;
	profile const Profile;
	int const Major;
	int const Minor;
	GLuint QueryName;
	std::size_t const FrameCount;
	glm::vec2 MouseOrigin;
	glm::vec2 MouseCurrent;
	glm::vec2 TranlationOrigin;
	glm::vec2 TranlationCurrent;
	glm::vec2 RotationOrigin;
	glm::vec2 RotationCurrent;
	int MouseButtonFlags;
	std::array<bool, 512> KeyPressed;

private:
	double TimeSum, TimeMin, TimeMax;

private:
	int version(int Major, int Minor) {return Major * 100 + Minor * 10;}

	static void cursorPositionCallback(GLFWwindow* Window, double x, double y);
	static void mouseButtonCallback(GLFWwindow* Window, int Button, int Action, int mods);
	static void keyCallback(GLFWwindow* Window, int Key, int Scancode, int Action, int Mods);
	static void APIENTRY debugOutput(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, GLvoid* userParam);
};

#endif//TEST_INCLUDED
