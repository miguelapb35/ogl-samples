#include "test.hpp"

int const MAJOR = 3;
int const MINOR = 2;

glm::ivec2 const test::DEFAULT_WINDOW_SIZE(640, 480);
std::size_t const test::DEFAULT_MAX_FRAME(1000);

test::test(int argc, char* argv[], profile Profile, std::size_t FrameMax, glm::ivec2 const & WindowSize) :
	Window(nullptr),
	Version(version(3, 2)),
	Profile(Profile),
	FrameNum(0),
	FrameMax(FrameMax),
	TimeConvergent(0.0),
	TimeMin(0.0),
	TimeMax(0.0)
{
	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, this->Profile == ES ? GLFW_OPENGL_ES_API : GLFW_OPENGL_API);

	if(Version >= version(MAJOR, MINOR) || (this->Profile == ES))
	{
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, MAJOR);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, MINOR);
#		if defined(__APPLE__)
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
			glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#		else
			if(this->Profile != ES)
			{
#				if defined(__APPLE__)
					glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#				else
					glfwWindowHint(GLFW_OPENGL_PROFILE, this->Profile == CORE ? GLFW_OPENGL_CORE_PROFILE : GLFW_OPENGL_COMPAT_PROFILE);
					glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, this->Profile == CORE ? GL_TRUE : GL_FALSE);
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

#	if _DEBUG
	if(this->isExtensionSupported("GL_ARB_debug_output"))
	{
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS_ARB);
		glDebugMessageControlARB(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE);
		glDebugMessageCallbackARB(&test::debugOutput, NULL);
	}
#	endif
}

test::~test()
{
	glfwDestroyWindow(this->Window);
	this->Window = 0;

	glfwTerminate();
}

int test::operator()()
{
	while(true)
	{
		this->render();

		glfwPollEvents();
		if(glfwWindowShouldClose(this->Window))
			break;

		this->swap();

		if(FrameNum < FrameMax)
			this->stop();
	}

	return 0;
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
	CSV.log(String, this->TimeConvergent, this->TimeMin, this->TimeMax);
}

void test::updateTime(double InstantTime)
{
	if(this->TimeConvergent == 0)
		this->TimeConvergent = InstantTime;
	else
		this->TimeConvergent = glm::mix(this->TimeConvergent, InstantTime, 0.01);
	fprintf(stdout, "\rConverging Time: %2.4f ms, Instant Time: %2.4f ms    ", this->TimeConvergent, InstantTime);
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

void test::cursorPositionCallback(GLFWwindow* Window, double x, double y)
{
	test * Test = reinterpret_cast<test*>(glfwGetWindowUserPointer(Window));
	assert(Test);

	Test->MouseCurrent = glm::ivec2(x, y);
	Test->TranlationCurrent = Test->MouseButtonFlags & test::MOUSE_BUTTON_LEFT ? Test->TranlationOrigin + (Test->MouseCurrent - Test->MouseOrigin) / 10.f : Test->TranlationOrigin;
	Test->RotationCurrent = Test->MouseButtonFlags & test::MOUSE_BUTTON_RIGHT ? Test->RotationOrigin + (Test->MouseCurrent - Test->MouseOrigin) :Test->RotationOrigin;
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
					Test->RotationOrigin += Test->MouseCurrent - Test->MouseOrigin;
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

	Test->KeyPressed[Key] = Action != KEY_RELEASE;

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
	//FILE* f;
	//f = fopen("debug_output.txt","a");
	//if(f)
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
			//fclose(f);
	}
}
