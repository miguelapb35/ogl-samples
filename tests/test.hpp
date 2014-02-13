///////////////////////////////////////////////////////////////////////////////////
/// OpenGL Samples Pack (ogl-samples.g-truc.net)
///
/// Copyright (c) 2004 - 2014 G-Truc Creation (www.g-truc.net)
/// Permission is hereby granted, free of charge, to any person obtaining a copy
/// of this software and associated documentation files (the "Software"), to deal
/// in the Software without restriction, including without limitation the rights
/// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
/// copies of the Software, and to permit persons to whom the Software is
/// furnished to do so, subject to the following conditions:
/// 
/// The above copyright notice and this permission notice shall be included in
/// all copies or substantial portions of the Software.
/// 
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
/// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
/// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
/// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
/// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
/// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
/// THE SOFTWARE.
///////////////////////////////////////////////////////////////////////////////////

#ifndef TEST_INCLUDED
#define TEST_INCLUDED

#include "csv.hpp"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

#include <glf/compiler.hpp>
#include <glf/sementics.hpp>
#include <glf/vertex.hpp>

#include <memory>
#include <array>

#define BUFFER_OFFSET(i) ((char *)NULL + (i))
#define GLF_BUFFER_OFFSET(i) ((char *)NULL + (i))

struct vertexattrib
{
	vertexattrib() :
		Enabled(GL_FALSE),
		Binding(0),
		Size(4),
		Stride(0),
		Type(GL_FLOAT),
		Normalized(GL_FALSE),
		Integer(GL_FALSE),
		Long(GL_FALSE),
		Divisor(0),
		Pointer(NULL)
	{}

	vertexattrib
	(
		GLint Enabled,
		GLint Binding,
		GLint Size,
		GLint Stride,
		GLint Type,
		GLint Normalized,
		GLint Integer,
		GLint Long,
		GLint Divisor,
		GLvoid* Pointer
	) :
		Enabled(Enabled),
		Binding(Binding),
		Size(Size),
		Stride(Stride),
		Type(Type),
		Normalized(Normalized),
		Integer(Integer),
		Long(Long),
		Divisor(Divisor),
		Pointer(Pointer)
	{}

	GLint Enabled;
	GLint Binding;
	GLint Size;
	GLint Stride;
	GLint Type;
	GLint Normalized;
	GLint Integer;
	GLint Long;
	GLint Divisor;
	GLvoid* Pointer;
};

inline bool operator== (vertexattrib const & A, vertexattrib const & B)
{
	return A.Enabled == B.Enabled && 
		A.Size == B.Size && 
		A.Stride == B.Stride && 
		A.Type == B.Type && 
		A.Normalized == B.Normalized && 
		A.Integer == B.Integer && 
		A.Long == B.Long;
}

inline bool operator!= (vertexattrib const & A, vertexattrib const & B)
{
	return !(A == B);
}

std::string getDataDirectory();

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
	struct DrawArraysIndirectCommand
	{
		GLuint count;
		GLuint primCount;
		GLuint first;
		GLuint baseInstance;
	};

	struct DrawElementsIndirectCommand
	{
		DrawElementsIndirectCommand() :
			primitiveCount(0),
			instanceCount(0),
			firstIndex(0),
			baseVertex(0),
			baseInstance(0)
		{}

		DrawElementsIndirectCommand
		(
			GLuint primitiveCount,
			GLuint instanceCount,
			GLuint firstIndex,
			GLint  baseVertex,
			GLuint baseInstance
		) :
			primitiveCount(primitiveCount),
			instanceCount(instanceCount),
			firstIndex(firstIndex),
			baseVertex(baseVertex),
			baseInstance(baseInstance)
		{}

		GLuint primitiveCount;
		GLuint instanceCount;
		GLuint firstIndex;
		GLint  baseVertex;
		GLuint baseInstance;
	};

	enum success
	{
		RUN_ONLY,
		GENERATE_ERROR,
		MATCH_TEMPLATE
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

	test(
		int argc, char* argv[], char const * Title,
		profile Profile, int Major, int Minor,
		glm::ivec2 const & WindowSize = glm::ivec2(640, 480),
		glm::vec2 const & Orientation = glm::vec2(0, 0),
		glm::vec2 const & Position = glm::vec2(0, 4),
		std::size_t FrameCount = 2,
		success Success = MATCH_TEMPLATE);
	test(
		int argc, char* argv[], char const * Title,
		profile Profile, int Major, int Minor,
		glm::vec2 const & Orientation,
		success Success = MATCH_TEMPLATE);
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
	glm::mat4 view() const;
	float cameraDistance() const {return this->TranlationCurrent.y;}

protected:
	void beginTimer();
	void endTimer();

	std::string loadFile(std::string const & Filename) const;
	void logImplementationDependentLimit(GLenum Value, std::string const & String) const;
	bool validate(GLuint VertexArrayName, std::vector<vertexattrib> const & Expected) const;
	bool checkError(const char* Title) const;
	bool checkFramebuffer(GLuint FramebufferName) const;
	bool checkExtension(char const * ExtensionName) const;

private:
	GLFWwindow* Window;
	success const Success;
	std::string const Title;
	profile const Profile;
	int const Major;
	int const Minor;
	GLuint TimerQueryName;
	std::size_t const FrameCount;
	glm::vec2 MouseOrigin;
	glm::vec2 MouseCurrent;
	glm::vec2 TranlationOrigin;
	glm::vec2 TranlationCurrent;
	glm::vec2 RotationOrigin;
	glm::vec2 RotationCurrent;
	int MouseButtonFlags;
	std::array<bool, 512> KeyPressed;
	bool Error;

private:
	double TimeSum, TimeMin, TimeMax;

private:
	int version(int Major, int Minor) const{return Major * 100 + Minor * 10;}
	bool checkGLVersion(GLint MajorVersionRequire, GLint MinorVersionRequire) const;

	static void cursorPositionCallback(GLFWwindow* Window, double x, double y);
	static void mouseButtonCallback(GLFWwindow* Window, int Button, int Action, int mods);
	static void keyCallback(GLFWwindow* Window, int Key, int Scancode, int Action, int Mods);

public:
	static void APIENTRY debugOutput(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, GLvoid* userParam);
};

#endif//TEST_INCLUDED
