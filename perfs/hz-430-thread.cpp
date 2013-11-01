//**********************************
// OpenGL Multi Draw Indirect
// 25/07/2012 - 15/11/2012
//**********************************
// Christophe Riccio
// ogl-samples@g-truc.net
//**********************************
// G-Truc Creation
// www.g-truc.net
//**********************************

#include <glf/glf.hpp>
#include <thread>
#include <atomic>
#include <array>
#include <cstring>

namespace
{
	char const * SAMPLE_NAME("OpenGL Multi draw indirect");
	char const * VERT_SHADER_SOURCE("hz-430/multi-draw-indirect.vert");
	char const * FRAG_SHADER_SOURCE("hz-430/multi-draw-indirect.frag");
	char const * TEXTURE_DIFFUSE("kueken1-bgr8.dds");
	int const SAMPLE_SIZE_WIDTH(640);
	int const SAMPLE_SIZE_HEIGHT(480);
	int const SAMPLE_MAJOR_VERSION(4);
	int const SAMPLE_MINOR_VERSION(2);

	glf::window Window(glm::ivec2(SAMPLE_SIZE_WIDTH, SAMPLE_SIZE_HEIGHT));

	GLsizei const VertexCount(6);
	GLsizeiptr const VertexSize = VertexCount * sizeof(glm::vec2);
	glm::vec2 const VertexData[VertexCount] =
	{
		glm::vec2(-1.0f,-1.0f),
		glm::vec2( 1.0f,-1.0f),
		glm::vec2( 1.0f, 1.0f),
		glm::vec2( 1.0f, 1.0f),
		glm::vec2(-1.0f, 1.0f),
		glm::vec2(-1.0f,-1.0f)
	};

	GLsizei const DrawCount(100000);

	namespace buffer
	{
		enum type
		{
			VERTEX,
			TRANSFORM,
			INDIRECT,
			MAX
		};
	}//namespace buffer

	struct DrawArraysIndirectCommand
	{
		GLuint count;
		GLuint primCount;
		GLuint first;
		GLuint baseInstance;
	};

	GLuint VertexArrayName(0);
	GLuint PipelineName(0);
	GLuint ProgramName(0);
	GLuint BufferName[buffer::MAX];
	GLuint QueryName(0);

}//namespace

namespace utl
{
	template <typename key, std::size_t N>
	class buffer
	{
	public:
		buffer() :
			Size(0)
		{}

		class const_iterator
		{
			friend buffer;

		private:
			const_iterator
			(
				std::uint8_t const * const Pointer,
				std::ptrdiff_t Offset
			) :
				Pointer(Pointer),
				Offset(Offset)
			{}

		public:
			const_iterator& operator++ ()
			{
				Offset += *reinterpret_cast<std::ptrdiff_t const * const>(this->Pointer);
				return *this;
			}

			inline bool operator== (const_iterator const & it) const
			{
				return this->Offset == it.Offset;
			}

			inline bool operator!= (const_iterator const & it) const
			{
				return this->Offset != it.Offset;
			}

		private:
			std::uint8_t const * const Pointer;
			std::ptrdiff_t Offset;
		};

		typedef std::size_t size_type;

		const_iterator begin() const
		{
			return const_iterator(&this->Data[0], 0);
		}

		const_iterator end() const
		{
			return const_iterator(&this->Data[0], this->Size);
		}

		template <typename chunk>
		bool push(key const & Key, chunk const & Chunk)
		{
			ptrdiff_t WriteSize = sizeof(ptrdiff_t) + sizeof(key) + sizeof(Chunk);
			if(this->Size + WriteSize > this->capacity())
				return false;

			struct helper
			{
				helper(key Key, std::ptrdiff_t WriteSize, chunk const & Chunk) :
					Key(Key),
					WriteSize(WriteSize),
					Chunk(Chunk)
				{}

				std::ptrdiff_t const WriteSize;
				key const Key;
				chunk const Chunk;
			};

			helper Helper(Key, WriteSize, Chunk);

			memcpy(&this->Data[this->Size], &Helper, sizeof(Helper));
			this->Size += WriteSize;

			return true;
		}

		key const & name(const_iterator const & it) const
		{
			return reinterpret_cast<key const &>(this->Data[it.Offset + sizeof(ptrdiff_t)]);
		}

		template <typename command>
		command const & data(const_iterator const & it) const
		{
			return reinterpret_cast<command const &>(this->Data[it.Offset + sizeof(key) + sizeof(ptrdiff_t)]);
		}

		void clear()
		{
			memset(&Data[0], 0, this->capacity());
		}

		size_type size() const
		{
			return this->Size;
		}

		size_type capacity() const
		{
			return this->Data.size();
		}

	private:
		std::array<std::uint8_t, N> Data;
		size_type Size;
	};

	template <typename data>
	class ring
	{
	public:
		class node
		{
		public:


		private:
			node* Next;
			data Data;
		};

	public:
		ring() :
			Head(new node),
			Tail(new node)
		{
			assert(Head && Tail);

			this->Head->Next = Tail;
			this->Tail->Next = Head;
		}

		~ring()
		{
			node* Node = this->Head;
			while(&Node != &this->Head)
			{
				node* Temp = Node;
				Node = Node->Next;
				delete Temp;
			}
		}

		void push_head(node* Node);
		node* pop_head();
		void push_tail(node* Node);
		node* pop_tail();

	private:
		std::atomic<node*> Head;
		std::atomic<node*> Tail;
	};
}//namespapce utl

namespace cmd
{
	enum name
	{
		CLEAR = 76
	};

	namespace buffer
	{
		struct name
		{

		};

		struct create
		{

		};

		struct destroy
		{

		};

		struct map
		{

		};

		struct unmap
		{

		};

		struct copy
		{

		};
	}//namespace

	struct clear
	{
		enum drawbuffer
		{
			DRAWBUFFER0,
			DRAWBUFFER1,
			DRAWBUFFER2,
			DRAWBUFFER3,
			DRAWBUFFER4,
			DRAWBUFFER5,
			DRAWBUFFER6,
			DRAWBUFFER7
		};

		enum mode
		{
			COLOR = (1 << 0),
			DEPTH = (1 << 1),
			STENCIL = (1 << 2),
			COLOR_DEPTH = COLOR | DEPTH,
			DEPTH_STENCIL = DEPTH | STENCIL,
			COLOR_STENCIL = COLOR | STENCIL,
			COLOR_DEPTH_STENCIL = COLOR | DEPTH | STENCIL
		};

		clear(drawbuffer const & Drawbuffer, mode const & Mode, glm::vec4 const & Color, float Depth) :
			Drawbuffer(static_cast<GLint>(Drawbuffer)),
			Mode(Mode),
			Color(Color),
			Depth(Depth)
		{}

		GLint const Drawbuffer;
		mode const Mode;
		glm::vec4 const Color;
		float const Depth;
	};

	struct context
	{

	};

	struct device
	{

	};

	class queue
	{
	public:
		const_iterator begin() const;
		const_iterator end() const;

		void clear(drawbuffer const & Drawbuffer, mode const & Mode, glm::vec4 const & Color, float Depth);

	private:
		std::list<utl::buffer<cmd::name, 65536>> Storage;
	};
}//cmd

bool initProgram()
{
	bool Validated(true);
	
	glGenProgramPipelines(1, &PipelineName);

	glf::compiler Compiler;
	GLuint VertShaderName = Compiler.create(GL_VERTEX_SHADER, glf::DATA_DIRECTORY + VERT_SHADER_SOURCE, 
		"--version 420 --profile core");
	GLuint FragShaderName = Compiler.create(GL_FRAGMENT_SHADER, glf::DATA_DIRECTORY + FRAG_SHADER_SOURCE,
		"--version 420 --profile core");
	Validated = Validated && Compiler.check();

	ProgramName = glCreateProgram();
	glProgramParameteri(ProgramName, GL_PROGRAM_SEPARABLE, GL_TRUE);
	glAttachShader(ProgramName, VertShaderName);
	glAttachShader(ProgramName, FragShaderName);
	glLinkProgram(ProgramName);
	Validated = Validated && glf::checkProgram(ProgramName);

	if(Validated)
		glUseProgramStages(PipelineName, GL_VERTEX_SHADER_BIT | GL_FRAGMENT_SHADER_BIT, ProgramName);

	return Validated;
}

class name
{
public:
	enum type
	{
		TEXTURE,
		BUFFER,
		FRAMEBUFFER,
		VERTEX_ARRAY,
		QUERY,
		PROGRAM,
		SHADER,
		PIPELINE
	};

	name
	(
		type const & Type,
		std::size_t const & Index
	) :
		Type(Type),
		Index(Index)
	{}

	type getType() const
	{
		return this->Type;
	}

	std::size_t getIndex() const
	{
		return this->Index;
	}

private:
	type Type;
	std::size_t Index;
};

bool initBuffer()
{
	//name BufferName[buffer::VERTEX] = Queue->createBuffer(0, VertexSize, VertexData);

	glGenBuffers(buffer::MAX, BufferName);

	glNamedBufferDataEXT(BufferName[buffer::VERTEX], VertexSize, VertexData, GL_STATIC_DRAW);
	glNamedBufferDataEXT(BufferName[buffer::TRANSFORM], sizeof(glm::mat4), NULL, GL_DYNAMIC_DRAW);

	std::vector<DrawArraysIndirectCommand> Commands;
	Commands.resize(DrawCount);
	for(std::size_t i = 0; i < Commands.size(); ++i)
	{
		Commands[i].count = VertexCount;
		Commands[i].primCount = 2;
		Commands[i].first = 0;
		Commands[i].baseInstance = 0;
	}

	glNamedBufferDataEXT(BufferName[buffer::INDIRECT], sizeof(DrawArraysIndirectCommand) * Commands.size(), &Commands[0], GL_STATIC_DRAW);

	return true;
}

bool initVertexArray()
{
	glGenVertexArrays(1, &VertexArrayName);
	glBindVertexArray(VertexArrayName);
		glBindBuffer(GL_ARRAY_BUFFER, BufferName[buffer::VERTEX]);
		glVertexAttribPointer(glf::semantic::attr::POSITION, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), GLF_BUFFER_OFFSET(0));
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		
		glEnableVertexAttribArray(glf::semantic::attr::POSITION);
	glBindVertexArray(0);

	return true;
}

bool initDebugOutput()
{
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS_ARB);
	glDebugMessageControlARB(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE);
	glDebugMessageCallbackARB(&glf::debugOutput, NULL);

	return true;
}

bool begin()
{
	bool Success(true);

	// Validate OpenGL support
	Success = Success && glf::checkGLVersion(SAMPLE_MAJOR_VERSION, SAMPLE_MINOR_VERSION);
	Success = Success && glf::checkExtension("GL_ARB_multi_draw_indirect");

	glGenQueries(1, &QueryName);

#	if _DEBUG
	if(Success && glf::checkExtension("GL_ARB_debug_output"))
		Success = initDebugOutput();
#	endif

	if(Success)
		Success = initProgram();
	if(Success)
		Success = initBuffer();
	if(Success)
		Success = initVertexArray();

	return Success;
}

bool end()
{
	//std::vector<name> Names;
	//Queue->destroy(Names);

	glDeleteBuffers(buffer::MAX, BufferName);
	glDeleteProgramPipelines(1, &PipelineName);
	glDeleteProgram(ProgramName);
	glDeleteVertexArrays(1, &VertexArrayName);

	return true;
}

void display()
{
	utl::buffer<cmd::name, 65536> Buffer;
	Buffer.push(cmd::CLEAR, cmd::clear(
		cmd::clear::DRAWBUFFER0, 
		cmd::clear::COLOR, 
		glm::vec4(1.0, 0.5, 0.0, 1.0), 1));
	Buffer.push(cmd::CLEAR, cmd::clear(
		cmd::clear::DRAWBUFFER0, 
		cmd::clear::DEPTH, 
		glm::vec4(0.0, 0.5, 1.0, 1.0), 1));

	for(utl::buffer<cmd::name, 65536>::const_iterator it = Buffer.begin(); it != Buffer.end(); ++it)
	{
		switch(Buffer.name(it))
		{
			case cmd::CLEAR:
			{
				cmd::clear const & Command = Buffer.data<cmd::clear>(it);
				if(Command.Mode & cmd::clear::COLOR)
					glClearBufferfv(GL_COLOR, Command.Drawbuffer, &Command.Color[0]);
				if(Command.Mode & cmd::clear::DEPTH)
					glClearBufferfv(GL_DEPTH, Command.Drawbuffer, &Command.Depth);
			}
			break;
			default:
			{
				// Unknowned command
				assert(0);
			}
		}
	}

	{
		// Update the transformation matrix
		glm::mat4* Pointer = reinterpret_cast<glm::mat4*>(glMapNamedBufferRangeEXT(BufferName[buffer::TRANSFORM], 0, sizeof(glm::mat4), GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT));

		glm::mat4 Projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 2048.0f);
		glm::mat4 ViewTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -Window.TranlationCurrent.y - 512));
		glm::mat4 ViewRotateX = glm::rotate(ViewTranslate, Window.RotationCurrent.y, glm::vec3(1.f, 0.f, 0.f));
		glm::mat4 View = glm::rotate(ViewRotateX, Window.RotationCurrent.x, glm::vec3(0.f, 1.f, 0.f));
		glm::mat4 Model = glm::mat4(1.0f);

		*Pointer = Projection * View * Model;
		glUnmapNamedBufferEXT(BufferName[buffer::TRANSFORM]);
	}

	glEnable(GL_DEPTH_TEST);
	glViewportIndexedfv(0, &glm::vec4(0, 0, Window.Size.x, Window.Size.y)[0]);

	glBindBufferBase(GL_UNIFORM_BUFFER, glf::semantic::uniform::TRANSFORM0, BufferName[buffer::TRANSFORM]);
	glBindProgramPipeline(PipelineName);
	glBindVertexArray(VertexArrayName);
	glBindBuffer(GL_DRAW_INDIRECT_BUFFER, BufferName[buffer::INDIRECT]);

	glBeginQuery(GL_TIME_ELAPSED, QueryName);
		glMultiDrawArraysIndirect(GL_TRIANGLES, 0, static_cast<GLsizei>(DrawCount), 0);
	glEndQuery(GL_TIME_ELAPSED);

	GLuint QueryTime = 0;
	glGetQueryObjectuiv(QueryName, GL_QUERY_RESULT, &QueryTime);

	double InstantTime = static_cast<double>(QueryTime) / 1000.0 / 1000.0;
	static double ConvergingTime = 0;
	ConvergingTime = (ConvergingTime * 0.99 + InstantTime * 0.01);
	fprintf(stdout, "\rConverging Time: %2.4f ms, Instant Time: %2.4f ms", ConvergingTime, InstantTime);
}

static volatile GLboolean runningOpenGL = GL_TRUE;
static volatile GLboolean runningRendering = GL_TRUE;
static volatile GLboolean runningMain = GL_TRUE;

static void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error: %s\n", description);
}

std::atomic_uint AtomicCount;

unsigned int OpenGLThreadCount(0);

int opengl_thread(GLFWwindow* Window)
{
	glfwMakeContextCurrent(Window);
	glfwSwapInterval(1);

	glewExperimental = GL_TRUE;
	glewInit();
	glGetError();

	begin();

	while (runningOpenGL)
	{
		++OpenGLThreadCount;
		std::atomic_fetch_add(&AtomicCount, 1);

		display();

		glfwSwapBuffers(Window);
	}

	end();

	glfwMakeContextCurrent(nullptr);

	return 0;
}

unsigned int RenderingThreadCount(0);

int rendering_thread(GLFWwindow* Window)
{
	std::thread Thread(opengl_thread, Window);

	while (runningRendering)
	{
		++RenderingThreadCount;
		std::atomic_fetch_add(&AtomicCount, 1);
	}

	runningOpenGL = GL_FALSE;
	Thread.join();

	return 0;
}

unsigned int MainCount(0);

int main(int argc, char* argv[])
{
	std::atomic_store(&AtomicCount, 0);
	assert(AtomicCount.is_lock_free());

	glfwSetErrorCallback(error_callback);

	if (!glfwInit())
		exit(EXIT_FAILURE);

	glfwWindowHint(GLFW_VISIBLE, GL_FALSE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, SAMPLE_MAJOR_VERSION);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, SAMPLE_MINOR_VERSION);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_FALSE);

	GLFWwindow* Window = glfwCreateWindow(
		SAMPLE_SIZE_WIDTH, SAMPLE_SIZE_HEIGHT,
		argv[0],
		NULL, NULL);

	glfwWindowHint(GLFW_VISIBLE, GL_FALSE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_OPENGL_PROFILE, 0x0004);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_FALSE);

	GLFWwindow* WindowES = glfwCreateWindow(
		SAMPLE_SIZE_WIDTH, SAMPLE_SIZE_HEIGHT,
		argv[0],
		NULL, Window);

	if (!Window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwShowWindow(Window);

	std::thread Thread(rendering_thread, Window);

	while (runningMain)
	{
		++MainCount;
		std::atomic_fetch_add(&AtomicCount, 1);

		glfwWaitEvents();

		if (glfwWindowShouldClose(Window))
			runningMain = GL_FALSE;
	}

	runningRendering = GL_FALSE;
	Thread.join();

	exit(EXIT_SUCCESS);
}
