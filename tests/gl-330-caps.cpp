#include "test.hpp"

class gl_330_caps : public framework
{
public:
	gl_330_caps(int argc, char* argv[]) :
		framework(argc, argv, "gl-330-caps", framework::CORE, 3, 3)
	{}

private:
	bool checkCaps()
	{
		caps Caps(caps::CORE);

		return true;
	}

	bool begin()
	{
		return checkCaps();
	}

	bool end()
	{
		return true;
	}

	bool render()
	{
		glm::uvec2 WindowSize = this->getWindowSize();

		glViewport(0, 0, WindowSize.x, WindowSize.y);
		glClearBufferfv(GL_COLOR, 0, &glm::vec4(1.0f, 0.5f, 0.0f, 1.0f)[0]);

		return true;
	}
};

int main(int argc, char* argv[])
{
	int Error(0);

	gl_330_caps Test(argc, argv);
	Error += Test();

	return Error;
}
