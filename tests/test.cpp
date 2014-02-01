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

#include "test.hpp"
#include "png.hpp"

namespace 
{
	inline std::string vendor()
	{
		std::string String(reinterpret_cast<char const *>(glGetString(GL_VENDOR)));

		if(String.find("NVIDIA") != std::string::npos)
			return "nvidia/";
		else if(String.find("ATI") != std::string::npos || String.find("AMD") != std::string::npos)
			return "amd/";
		else if(String.find("Intel") != std::string::npos)
			return "intel/";
		else
			return "unknown/";
	}

	inline bool checkFramebuffer(GLFWwindow* pWindow, char const * Title)
	{
		//glGetIntegerv(GL_IMPLEMENTATION_COLOR_READ_FORMAT, &Params); // For ES 2 this is actually necessary

		GLint WindowSizeX(0);
		GLint WindowSizeY(0);
		glfwGetFramebufferSize(pWindow, &WindowSizeX, &WindowSizeY);

		gli::texture2D Texture(1, gli::RGB8_UNORM, gli::texture2D::dimensions_type(WindowSizeX, WindowSizeY));

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glReadPixels(0, 0, WindowSizeX, WindowSizeY, GL_RGB, GL_UNSIGNED_BYTE, Texture.data());

		if(!glf::checkError("checkFramebuffer"))
			return false;

		glf::save_png(Texture, (glf::DATA_DIRECTORY + "./results/" + vendor() + Title + ".png").c_str());

		gli::texture2D Template(glf::load_png((glf::DATA_DIRECTORY + "templates/" + vendor() + Title + ".png").c_str()));
		if(Template.empty())
			return false;

		return Template == Texture;
	}
}

test::test
(
	int argc, char* argv[], char const * Title,
	profile Profile, int Major, int Minor,
	glm::vec2 const & Orientation,
	template_test TemplateTest
) :
	test(argc, argv, Title, Profile, Major, Minor, glm::ivec2(640, 480), Orientation, glm::vec2(0, 4), 2, TemplateTest)
{}

test::test
(
	int argc, char* argv[], char const * Title,
	profile Profile, int Major, int Minor,
	glm::ivec2 const & WindowSize, glm::vec2 const & Orientation, glm::vec2 const & Position,
	std::size_t FrameCount, template_test TemplateTest
) :
	Window(nullptr),
	TemplateTest(TemplateTest),
	Title(Title),
	Profile(Profile),
	Major(Major),
	Minor(Minor),
	QueryName(0),
	FrameCount(FrameCount),
	TimeSum(0.0),
	TimeMin(std::numeric_limits<double>::max()),
	TimeMax(0.0),
	MouseOrigin(WindowSize >> 1),
	MouseCurrent(WindowSize >> 1),
	TranlationOrigin(Position),
	TranlationCurrent(Position),
	RotationOrigin(Orientation), 
	RotationCurrent(Orientation),
	MouseButtonFlags(0)
{
	memset(&KeyPressed[0], 0, sizeof(KeyPressed));

	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, Profile == ES ? GLFW_OPENGL_ES_API : GLFW_OPENGL_API);

	if(version(this->Major, this->Minor) >= version(3, 2) || (Profile == ES))
	{
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, this->Major);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, this->Minor);
#		if defined(__APPLE__)
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
			glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#		else
			if(Profile != ES)
			{
#				if defined(__APPLE__)
					glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#				else
					glfwWindowHint(GLFW_OPENGL_PROFILE, Profile == CORE ? GLFW_OPENGL_CORE_PROFILE : GLFW_OPENGL_COMPAT_PROFILE);
					glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, Profile == CORE ? GL_TRUE : GL_FALSE);
#				endif
			}	
#			if !defined(_DEBUG) || defined(__APPLE__)
				glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_FALSE);
#			else
				glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
#			endif
#		endif
	}

	this->Window = glfwCreateWindow(WindowSize.x, WindowSize.y, argv[0], NULL,NULL);
	glfwSetWindowUserPointer(this->Window, this);
	assert(this->Window != nullptr);

	glfwSetMouseButtonCallback(this->Window, test::mouseButtonCallback);
	glfwSetCursorPosCallback(this->Window, test::cursorPositionCallback);
	glfwSetKeyCallback(this->Window, test::keyCallback);
	glfwMakeContextCurrent(this->Window);

	glewExperimental = GL_TRUE;
	glewInit();
	glGetError();

#	ifdef GL_ARB_debug_output
		if(this->Profile != ES && version(this->Major, this->Minor) >= version(3, 0))
		if(this->isExtensionSupported("GL_ARB_debug_output"))
		{
			glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS_ARB);
			glDebugMessageControlARB(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE);
			glDebugMessageCallbackARB(&test::debugOutput, NULL);
		}
#	endif

	glGenQueries(1, &this->QueryName);
}

test::~test()
{
	glDeleteQueries(1, &this->QueryName);

	glfwDestroyWindow(this->Window);
	this->Window = 0;

	glfwTerminate();
}

int test::operator()()
{
	int Result = EXIT_SUCCESS;

	if(version(this->Major, this->Minor) >= version(3, 0))
		Result = glf::checkGLVersion(this->Major, this->Minor) ? EXIT_SUCCESS : EXIT_FAILURE;

	if(Result == EXIT_SUCCESS)
		Result = this->begin() ? EXIT_SUCCESS : EXIT_FAILURE;

	std::size_t FrameNum = 0;

	while(true && Result == EXIT_SUCCESS)
	{
		Result = this->render() ? EXIT_SUCCESS : EXIT_FAILURE;
		Result = Result && glf::checkError("render");

		glfwPollEvents();
		if(glfwWindowShouldClose(this->Window) || (FrameNum >= this->FrameCount))
		{
			//if(this->TemplateTest == TEMPLATE_TEST_EXECUTE && (this->Profile == CORE || (this->Profile == ES && version(this->Major, this->Minor) >= version(3, 0))))
			if(this->TemplateTest == TEMPLATE_TEST_EXECUTE && this->Profile == CORE)
				if(!checkFramebuffer(this->Window, this->Title.c_str()))
					Result = EXIT_FAILURE;
			break;
		}

		this->swap();

#		ifdef AUTOMATED_TESTS
			if(FrameCount > 0)
				++FrameNum;
#		endif//AUTOMATED_TESTS
	}

	if(Result == EXIT_SUCCESS)
		Result = this->end() ? EXIT_SUCCESS : EXIT_FAILURE;

	return Result;
}

void test::swap()
{
	glfwSwapBuffers(this->Window);
}

void test::stop()
{
	glfwSetWindowShouldClose(this->Window, GL_TRUE);
}

void test::log(csv & CSV, char const * String)
{
	CSV.log(String, this->TimeSum / this->FrameCount, this->TimeMin, this->TimeMax);
}

bool test::isExtensionSupported(char const * String)
{
	GLint ExtensionCount(0);
	glGetIntegerv(GL_NUM_EXTENSIONS, &ExtensionCount);
	for(GLint i = 0; i < ExtensionCount; ++i)
		if(std::string((char const*)glGetStringi(GL_EXTENSIONS, i)) == std::string(String))
			return true;
	//printf("Failed to find Extension: \"%s\"\n",String);
	return false;
}

glm::ivec2 test::getWindowSize() const
{
	glm::ivec2 WindowSize(0);
	glfwGetFramebufferSize(this->Window, &WindowSize.x, &WindowSize.y);
	return WindowSize;
}

bool test::isKeyPressed(int Key) const
{
	return this->KeyPressed[Key];
}

glm::mat4 test::view() const
{
	glm::mat4 ViewTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -this->TranlationCurrent.y));
	glm::mat4 ViewRotateX = glm::rotate(ViewTranslate, this->RotationCurrent.y, glm::vec3(1.f, 0.f, 0.f));
	glm::mat4 View = glm::rotate(ViewRotateX, this->RotationCurrent.x, glm::vec3(0.f, 1.f, 0.f));
	return View;
}

void test::beginTimer()
{
	glBeginQuery(GL_TIME_ELAPSED, this->QueryName);
}

void test::endTimer()
{
	glEndQuery(GL_TIME_ELAPSED);

	GLuint QueryTime(0);
	glGetQueryObjectuiv(this->QueryName, GL_QUERY_RESULT, &QueryTime);

	double const InstantTime(static_cast<double>(QueryTime) / 1000.0);

	this->TimeSum += InstantTime;
	this->TimeMax = glm::max(this->TimeMax, InstantTime);
	this->TimeMin = glm::min(this->TimeMin, InstantTime);

	fprintf(stdout, "\rTime: %2.4f ms    ", InstantTime / 1000.0);
}

void test::cursorPositionCallback(GLFWwindow* Window, double x, double y)
{
	test * Test = reinterpret_cast<test*>(glfwGetWindowUserPointer(Window));
	assert(Test);

	Test->MouseCurrent = glm::ivec2(x, y);
	Test->TranlationCurrent = Test->MouseButtonFlags & test::MOUSE_BUTTON_LEFT ? Test->TranlationOrigin + (Test->MouseCurrent - Test->MouseOrigin) / 10.f : Test->TranlationOrigin;
	Test->RotationCurrent = Test->MouseButtonFlags & test::MOUSE_BUTTON_RIGHT ? Test->RotationOrigin + glm::radians(Test->MouseCurrent - Test->MouseOrigin) : Test->RotationOrigin;
}

void test::mouseButtonCallback(GLFWwindow* Window, int Button, int Action, int mods)
{
	test * Test = reinterpret_cast<test*>(glfwGetWindowUserPointer(Window));
	assert(Test);

	switch(Action)
	{
		case GLFW_PRESS:
		{
			Test->MouseOrigin = Test->MouseCurrent;
			switch(Button)
			{
				case GLFW_MOUSE_BUTTON_LEFT:
				{
					Test->MouseButtonFlags |= test::MOUSE_BUTTON_LEFT;
					Test->TranlationOrigin = Test->TranlationCurrent;
				}
				break;
				case GLFW_MOUSE_BUTTON_MIDDLE:
				{
					Test->MouseButtonFlags |= test::MOUSE_BUTTON_MIDDLE;
				}
				break;
				case GLFW_MOUSE_BUTTON_RIGHT:
				{
					Test->MouseButtonFlags |= test::MOUSE_BUTTON_RIGHT;
					Test->RotationOrigin = Test->RotationCurrent;
				}
				break;
			}
		}
		break;
		case GLFW_RELEASE:
		{
			switch(Button)
			{
				case GLFW_MOUSE_BUTTON_LEFT:
				{
					Test->TranlationOrigin += (Test->MouseCurrent - Test->MouseOrigin) / 10.f;
					Test->MouseButtonFlags &= ~test::MOUSE_BUTTON_LEFT;
				}
				break;
				case GLFW_MOUSE_BUTTON_MIDDLE:
				{
					Test->MouseButtonFlags &= ~test::MOUSE_BUTTON_MIDDLE;
				}
				break;
				case GLFW_MOUSE_BUTTON_RIGHT:
				{
					Test->RotationOrigin += glm::radians(Test->MouseCurrent - Test->MouseOrigin);
					Test->MouseButtonFlags &= ~test::MOUSE_BUTTON_RIGHT;
				}
				break;
			}
		}
		break;
	}
}

void test::keyCallback(GLFWwindow* Window, int Key, int Scancode, int Action, int Mods)
{
	test * Test = reinterpret_cast<test*>(glfwGetWindowUserPointer(Window));
	assert(Test);

	Test->KeyPressed[Key] = Action == KEY_PRESS;

	if(Test->isKeyPressed(GLFW_KEY_ESCAPE))
		Test->stop();
}

void APIENTRY test::debugOutput
(
	GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	GLsizei length,
	const GLchar* message,
	GLvoid* userParam
)
{
	char debSource[32], debType[32], debSev[32];
	bool Error(false);

	if(source == GL_DEBUG_SOURCE_API_ARB)
		strcpy(debSource, "OpenGL");
	else if(source == GL_DEBUG_SOURCE_WINDOW_SYSTEM_ARB)
		strcpy(debSource, "Windows");
	else if(source == GL_DEBUG_SOURCE_SHADER_COMPILER_ARB)
		strcpy(debSource, "Shader Compiler");
	else if(source == GL_DEBUG_SOURCE_THIRD_PARTY_ARB)
		strcpy(debSource, "Third Party");
	else if(source == GL_DEBUG_SOURCE_APPLICATION_ARB)
		strcpy(debSource, "Application");
	else if (source == GL_DEBUG_SOURCE_OTHER_ARB)
		strcpy(debSource, "Other");
	else
		assert(0);
 
	if(type == GL_DEBUG_TYPE_ERROR)
		strcpy(debType, "error");
	else if(type == GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR)
		strcpy(debType, "deprecated behavior");
	else if(type == GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR)
		strcpy(debType, "undefined behavior");
	else if(type == GL_DEBUG_TYPE_PORTABILITY)
		strcpy(debType, "portability");
	else if(type == GL_DEBUG_TYPE_PERFORMANCE)
		strcpy(debType, "performance");
	else if(type == GL_DEBUG_TYPE_OTHER)
		strcpy(debType, "message");
	else if(type == GL_DEBUG_TYPE_MARKER)
		strcpy(debType, "marker");
	else if(type == GL_DEBUG_TYPE_PUSH_GROUP)
		strcpy(debType, "push group");
	else if(type == GL_DEBUG_TYPE_POP_GROUP)
		strcpy(debType, "pop group");
	else
		assert(0);
 
	if(severity == GL_DEBUG_SEVERITY_HIGH_ARB)
	{
		strcpy(debSev, "high");
		Error = true;
	}
	else if(severity == GL_DEBUG_SEVERITY_MEDIUM_ARB)
		strcpy(debSev, "medium");
	else if(severity == GL_DEBUG_SEVERITY_LOW_ARB)
		strcpy(debSev, "low");
	else if (severity == GL_DEBUG_SEVERITY_NOTIFICATION)
		strcpy(debSev, "notification");
	else
		assert(0);

	fprintf(stderr,"%s: %s(%s) %d: %s\n", debSource, debType, debSev, id, message);
	assert(!Error);
}
