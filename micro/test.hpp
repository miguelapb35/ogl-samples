#ifndef TEST_INCLUDED
#define TEST_INCLUDED

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "glf/common.hpp"
#include "glf/vertex.hpp"
#include "glf/compiler.hpp"
#include "glf/sementics.hpp"
#include "glf/error.hpp"
#include "csv.hpp"

#include <glu/glu.hpp>

#include <memory>
#include <array>

class test
{
public:
	enum profile
	{
		CORE = GLFW_OPENGL_CORE_PROFILE,
		COMPATIBILITY = GLFW_OPENGL_COMPAT_PROFILE,
		ES = GLFW_OPENGL_ES_PROFILE
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

	test(int argc, char* argv[], profile Profile, std::size_t FrameMax, glm::ivec2 const & WindowSize);
	virtual ~test();

	virtual void render() = 0;

	int operator()();
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
	profile Profile;
	int Version;
	GLuint QueryName;
	std::size_t FrameNum;
	std::size_t FrameMax;
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
