//**********************************
// OpenGL Framebuffer Object
// 22/10/2012 - 11/08/2013
//**********************************
// Christophe Riccio
// ogl-samples@g-truc.net
//**********************************
// G-Truc Creation
// www.g-truc.net
//**********************************


#include <glf/glf.hpp>
#include <glf/compiler.hpp>

namespace
{
	char const * SAMPLE_NAME("OpenGL Framebuffer Object");
	char const * VERT_SHADER_SOURCE_TEXTURE("gl-440/fbo-texture-2d.vert");
	char const * FRAG_SHADER_SOURCE_TEXTURE("gl-440/fbo-texture-2d.frag");
	char const * VERT_SHADER_SOURCE_SPLASH("gl-440/fbo-splash.vert");
	char const * FRAG_SHADER_SOURCE_SPLASH("gl-440/fbo-splash.frag");
	char const * TEXTURE_DIFFUSE("kueken1-dxt1.dds");
	int const SAMPLE_SIZE_WIDTH(640);
	int const SAMPLE_SIZE_HEIGHT(480);
	int const SAMPLE_MAJOR_VERSION(4);
	int const SAMPLE_MINOR_VERSION(4);

	glf::window Window(glm::ivec2(SAMPLE_SIZE_WIDTH, SAMPLE_SIZE_HEIGHT));

	GLsizei const VertexCount(4);
	GLsizeiptr const VertexSize = VertexCount * sizeof(glf::vertex_v2fv2f);
	glf::vertex_v2fv2f const VertexData[VertexCount] =
	{
		glf::vertex_v2fv2f(glm::vec2(-1.0f,-1.0f), glm::vec2(0.0f, 1.0f)),
		glf::vertex_v2fv2f(glm::vec2( 1.0f,-1.0f), glm::vec2(1.0f, 1.0f)),
		glf::vertex_v2fv2f(glm::vec2( 1.0f, 1.0f), glm::vec2(1.0f, 0.0f)),
		glf::vertex_v2fv2f(glm::vec2(-1.0f, 1.0f), glm::vec2(0.0f, 0.0f))
	};

	GLsizei const ElementCount(6);
	GLsizeiptr const ElementSize = ElementCount * sizeof(GLushort);
	GLushort const ElementData[ElementCount] =
	{
		0, 1, 2, 
		2, 3, 0
	};

	namespace buffer
	{
		enum type
		{
			VERTEX,
			ELEMENT,
			TRANSFORM,
			MAX
		};
	}//namespace buffer

	namespace texture
	{
		enum type
		{
			DIFFUSE,
			COLORBUFFER,
			RENDERBUFFER,
			MAX
		};
	}//namespace texture
	
	namespace pipeline
	{
		enum type
		{
			TEXTURE,
			SPLASH,
			MAX
		};
	}//namespace pipeline

	GLuint FramebufferName(0);
	std::vector<GLuint> ProgramName(pipeline::MAX);
	std::vector<GLuint> VertexArrayName(pipeline::MAX);
	std::vector<GLuint> BufferName(buffer::MAX);
	std::vector<GLuint> TextureName(texture::MAX);
	std::vector<GLuint> PipelineName(pipeline::MAX);
	glm::mat4* UniformPointer(NULL);
}//namespace

struct caps
{
private:
	struct version
	{
		GLint Major;					// GL_MINOR_VERSION
		GLint Minor;					// GL_MAJOR_VERSION
		GLint NumExtensions;			// GL_NUM_EXTENSIONS
		bool ARB_shader_group_vote;
		bool ARB_sparse_texture;
	} VersionData;

	void initVersion()
	{
		glGetIntegerv(GL_MINOR_VERSION, &VersionData.Minor);
		glGetIntegerv(GL_MAJOR_VERSION, &VersionData.Major);
		glGetIntegerv(GL_NUM_EXTENSIONS, &VersionData.NumExtensions);
	}

	struct debug
	{
		GLint ContextFlags; // GL_CONTEXT_FLAGS
		GLint MaxDebugGroupStackDepth; // GL_MAX_DEBUG_GROUP_STACK_DEPTH
		GLint MaxLabelLength; // GL_MAX_LABEL_LENGTH
		GLint MaxComputeShaderStorageBlocks; // GL_MAX_SERVER_WAIT_TIMEOUT
	} DebugData;

	void initDebug()
	{
		glGetIntegerv(GL_CONTEXT_FLAGS, &DebugData.ContextFlags);
		glGetIntegerv(GL_MAX_DEBUG_GROUP_STACK_DEPTH, &DebugData.MaxDebugGroupStackDepth);
		glGetIntegerv(GL_MAX_LABEL_LENGTH, &DebugData.MaxLabelLength);
	}

	struct combined
	{
		GLint MaxCombinedShaderStorageBlocks; // GL_MAX_COMBINED_SHADER_STORAGE_BLOCKS
		GLint MaxCombinedComputeUniformComponents; // GL_MAX_COMBINED_COMPUTE_UNIFORM_COMPONENTS
		GLint MaxCombinedAtomicCounters; // GL_MAX_COMBINED_ATOMIC_COUNTERS
	} CombinedData;

	void initCombined()
	{
		glGetIntegerv(GL_MAX_COMBINED_SHADER_STORAGE_BLOCKS, &CombinedData.MaxCombinedShaderStorageBlocks);
		glGetIntegerv(GL_MAX_COMBINED_COMPUTE_UNIFORM_COMPONENTS, &CombinedData.MaxCombinedComputeUniformComponents);
		glGetIntegerv(GL_MAX_COMBINED_ATOMIC_COUNTERS, &CombinedData.MaxCombinedAtomicCounters);
	}

	struct compute
	{
		GLint MaxComputeShaderStorageBlocks;	// GL_MAX_COMPUTE_SHADER_STORAGE_BLOCKS
		GLint MaxComputeUniformBlocks;			// GL_MAX_COMPUTE_UNIFORM_BLOCKS
		GLint MaxComputeTextureImageUnits;		// GL_MAX_COMPUTE_TEXTURE_IMAGE_UNITS
		GLint MaxComputeUniformComponents;		// GL_MAX_COMPUTE_UNIFORM_COMPONENTS
		GLint MaxComputeAtomicCounters;			// GL_MAX_COMPUTE_ATOMIC_COUNTERS
		GLint MaxComputeAtomicCounterBuffers;	// GL_MAX_COMPUTE_ATOMIC_COUNTER_BUFFERS
		GLint MaxComputeWorkGroupInvocations;	// GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS
		GLint MaxComputeWorkGroupCount;			// GL_MAX_COMPUTE_WORK_GROUP_COUNT
		GLint MaxComputeWorkGroupSize;			// GL_MAX_COMPUTE_WORK_GROUP_SIZE
	} ComputeData;

	void initCompute()
	{
		glGetIntegerv(GL_MAX_COMPUTE_SHADER_STORAGE_BLOCKS, &ComputeData.MaxComputeShaderStorageBlocks);
		glGetIntegerv(GL_MAX_COMPUTE_UNIFORM_BLOCKS, &ComputeData.MaxComputeUniformBlocks);
		glGetIntegerv(GL_MAX_COMPUTE_TEXTURE_IMAGE_UNITS, &ComputeData.MaxComputeTextureImageUnits);
		glGetIntegerv(GL_MAX_COMPUTE_UNIFORM_COMPONENTS, &ComputeData.MaxComputeUniformComponents);
		glGetIntegerv(GL_MAX_COMPUTE_ATOMIC_COUNTERS, &ComputeData.MaxComputeAtomicCounters);
		glGetIntegerv(GL_MAX_COMPUTE_ATOMIC_COUNTER_BUFFERS, &ComputeData.MaxComputeAtomicCounterBuffers);
		glGetIntegerv(GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS, &ComputeData.MaxComputeWorkGroupInvocations);
		glGetIntegerv(GL_MAX_COMPUTE_WORK_GROUP_COUNT, &ComputeData.MaxComputeWorkGroupCount);
		glGetIntegerv(GL_MAX_COMPUTE_WORK_GROUP_SIZE, &ComputeData.MaxComputeWorkGroupSize);
	}

	struct vertex
	{
		GLint MaxComputeShaderStorageBlocks; // GL_MAX_VERTEX_ATOMIC_COUNTERS
		GLint MaxComputeShaderStorageBlocks; // GL_MAX_VERTEX_SHADER_STORAGE_BLOCKS
		GLint MaxComputeShaderStorageBlocks; // GL_MAX_VERTEX_ATTRIBS
		GLint MaxComputeShaderStorageBlocks; // GL_MAX_VERTEX_OUTPUT_COMPONENTS
		GLint MaxComputeShaderStorageBlocks; // GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS
		GLint MaxComputeShaderStorageBlocks; // GL_MAX_VERTEX_UNIFORM_COMPONENTS
		GLint MaxComputeShaderStorageBlocks; // GL_MAX_VERTEX_UNIFORM_VECTORS
		GLint MaxComputeShaderStorageBlocks; // GL_MAX_VERTEX_UNIFORM_BLOCKS
		GLint MaxComputeShaderStorageBlocks; // GL_MAX_COMBINED_VERTEX_UNIFORM_COMPONENTS
	} VertexData;

	void initVertex()
	{

	}

	struct control
	{
		GLint MaxComputeShaderStorageBlocks; // GL_MAX_TESS_CONTROL_ATOMIC_COUNTERS
		GLint MaxComputeShaderStorageBlocks; // GL_MAX_TESS_CONTROL_SHADER_STORAGE_BLOCKS
		GLint MaxComputeShaderStorageBlocks; // GL_MAX_TESS_CONTROL_INPUT_COMPONENTS
		GLint MaxComputeShaderStorageBlocks; // GL_MAX_TESS_CONTROL_OUTPUT_COMPONENTS
		GLint MaxComputeShaderStorageBlocks; // GL_MAX_TESS_CONTROL_TEXTURE_IMAGE_UNITS
		GLint MaxComputeShaderStorageBlocks; // GL_MAX_TESS_CONTROL_UNIFORM_BLOCKS
		GLint MaxComputeShaderStorageBlocks; // GL_MAX_TESS_CONTROL_UNIFORM_COMPONENTS
		GLint MaxComputeShaderStorageBlocks; // GL_MAX_COMBINED_TESS_CONTROL_UNIFORM_COMPONENTS
	} ControlData;

	void initControl()
	{

	}

	struct evaluation
	{
		GLint MaxComputeShaderStorageBlocks; // GL_MAX_TESS_EVALUATION_ATOMIC_COUNTERS
		GLint MaxComputeShaderStorageBlocks; // GL_MAX_TESS_EVALUATION_SHADER_STORAGE_BLOCKS
		GLint MaxComputeShaderStorageBlocks; // GL_MAX_TESS_EVALUATION_INPUT_COMPONENTS
		GLint MaxComputeShaderStorageBlocks; // GL_MAX_TESS_EVALUATION_OUTPUT_COMPONENTS
		GLint MaxComputeShaderStorageBlocks; // GL_MAX_TESS_EVALUATION_TEXTURE_IMAGE_UNITS
		GLint MaxComputeShaderStorageBlocks; // GL_MAX_TESS_EVALUATION_UNIFORM_BLOCKS
		GLint MaxComputeShaderStorageBlocks; // GL_MAX_TESS_EVALUATION_UNIFORM_COMPONENTS
		GLint MaxComputeShaderStorageBlocks; // GL_MAX_COMBINED_TESS_EVALUATION_UNIFORM_COMPONENTS
	} EvaluationData;

	void initEvaluation()
	{

	}

	struct geometry
	{
		GLint MaxComputeShaderStorageBlocks; // GL_MAX_GEOMETRY_ATOMIC_COUNTERS
		GLint MaxComputeShaderStorageBlocks; // GL_MAX_GEOMETRY_SHADER_STORAGE_BLOCKS
		GLint MaxComputeShaderStorageBlocks; // GL_MAX_GEOMETRY_INPUT_COMPONENTS
		GLint MaxComputeShaderStorageBlocks; // GL_MAX_GEOMETRY_OUTPUT_COMPONENTS
		GLint MaxComputeShaderStorageBlocks; // GL_MAX_GEOMETRY_TEXTURE_IMAGE_UNITS
		GLint MaxComputeShaderStorageBlocks; // GL_MAX_GEOMETRY_UNIFORM_BLOCKS
		GLint MaxComputeShaderStorageBlocks; // GL_MAX_GEOMETRY_UNIFORM_COMPONENTS
		GLint MaxComputeShaderStorageBlocks; // GL_MAX_COMBINED_GEOMETRY_UNIFORM_COMPONENTS
		GLint MaxVertexStreams; // GL_MAX_VERTEX_STREAMS
	} GeometryData;

	void initGeometry()
	{

	}

	struct fragment
	{
		GLint MaxComputeShaderStorageBlocks; // GL_MAX_FRAGMENT_ATOMIC_COUNTERS
		GLint MaxComputeShaderStorageBlocks; // GL_MAX_FRAGMENT_SHADER_STORAGE_BLOCKS
		GLint MaxComputeShaderStorageBlocks; // GL_MAX_FRAGMENT_INPUT_COMPONENTS
		GLint MaxComputeShaderStorageBlocks; // GL_MAX_FRAGMENT_UNIFORM_COMPONENTS
		GLint MaxComputeShaderStorageBlocks; // GL_MAX_FRAGMENT_UNIFORM_VECTORS
		GLint MaxComputeShaderStorageBlocks; // GL_MAX_FRAGMENT_UNIFORM_BLOCKS
		GLint MaxComputeShaderStorageBlocks; // GL_MAX_COMBINED_FRAGMENT_UNIFORM_COMPONENTS
		GLint MaxComputeShaderStorageBlocks; // GL_MAX_DRAW_BUFFERS
		GLint MaxComputeShaderStorageBlocks; // GL_MAX_DUAL_SOURCE_DRAW_BUFFERS
	} FragmentData;

	void initFragment()
	{

	}

	struct pulling
	{
		GLint MaxComputeShaderStorageBlocks; // GL_MAX_ELEMENTS_INDICES
		GLint MaxComputeShaderStorageBlocks; // GL_MAX_ELEMENTS_VERTICES
		GLint MaxComputeShaderStorageBlocks; // GL_MAX_VERTEX_ATTRIB_RELATIVE_OFFSET
		GLint MaxComputeShaderStorageBlocks; // GL_MAX_VERTEX_ATTRIB_BINDINGS
		GLint MaxComputeShaderStorageBlocks; // GL_MAX_ELEMENT_INDEX
	} PullingData;

	void initPulling()
	{

	}

	struct rasterizer
	{
		GLint MaxComputeShaderStorageBlocks; // GL_SUBPIXEL_BITS
		GLint MaxComputeShaderStorageBlocks; // GL_MAX_CLIP_DISTANCES
		GLint MaxComputeShaderStorageBlocks; // GL_MAX_VIEWPORT_DIMS
		GLint MaxComputeShaderStorageBlocks; // GL_MAX_VIEWPORTS
		GLint MaxComputeShaderStorageBlocks; // GL_VIEWPORT_SUBPIXEL_BITS
		GLint MaxComputeShaderStorageBlocks; // GL_VIEWPORT_BOUNDS_RANGE // returns two values
	} RasterizerData;

	void initRasterizer()
	{

	}

	struct framebuffer
	{
		GLint MaxComputeShaderStorageBlocks; // GL_MAX_FRAMEBUFFER_WIDTH
		GLint MaxComputeShaderStorageBlocks; // GL_MAX_FRAMEBUFFER_HEIGHT
		GLint MaxComputeShaderStorageBlocks; // GL_MAX_FRAMEBUFFER_LAYERS
		GLint MaxComputeShaderStorageBlocks; // GL_MAX_FRAMEBUFFER_SAMPLES
		GLint MaxComputeShaderStorageBlocks; // GL_MAX_RENDERBUFFER_SIZE
		GLint MaxComputeShaderStorageBlocks; // GL_MAX_SAMPLE_MASK_WORDS
	} FramebufferData;

	void initFramebuffer()
	{

	}

	struct buffer
	{
		GLint MaxComputeShaderStorageBlocks; // GL_MIN_MAP_BUFFER_ALIGNMENT
	} BufferData;

	void initBuffer()
	{

	}

	struct texture
	{
		GLint MaxComputeShaderStorageBlocks; // GL_MAX_TEXTURE_IMAGE_UNITS
		GLint MaxComputeShaderStorageBlocks; // GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS
		GLint MaxComputeShaderStorageBlocks; // GL_MAX_TEXTURE_LOD_BIAS
		GLint MaxComputeShaderStorageBlocks; // GL_MAX_TEXTURE_SIZE
		GLint MaxComputeShaderStorageBlocks; // GL_MAX_RECTANGLE_TEXTURE_SIZE
		GLint MaxComputeShaderStorageBlocks; // GL_MAX_3D_TEXTURE_SIZE
		GLint MaxComputeShaderStorageBlocks; // GL_MAX_ARRAY_TEXTURE_LAYERS
		GLint MaxComputeShaderStorageBlocks; // GL_MAX_CUBE_MAP_TEXTURE_SIZE
		GLint MaxComputeShaderStorageBlocks; // GL_MAX_COLOR_TEXTURE_SAMPLES
		GLint MaxComputeShaderStorageBlocks; // GL_MAX_DEPTH_TEXTURE_SAMPLES
		GLint MaxComputeShaderStorageBlocks; // GL_MAX_INTEGER_SAMPLES
		GLint MaxComputeShaderStorageBlocks; // GL_MAX_TEXTURE_BUFFER_SIZE
		GLint MaxComputeShaderStorageBlocks; // GL_NUM_COMPRESSED_TEXTURE_FORMATS
	} TextureData;

	void initTexture()
	{

	}

	struct program
	{
		GLint MaxComputeShaderStorageBlocks; // GL_MAX_PROGRAM_TEXEL_OFFSET
		GLint MaxComputeShaderStorageBlocks; // GL_MIN_PROGRAM_TEXEL_OFFSET
		GLint MaxComputeShaderStorageBlocks; // GL_MAX_COMBINED_UNIFORM_BLOCKS
		GLint MaxComputeShaderStorageBlocks; // GL_MAX_UNIFORM_BUFFER_BINDINGS
		GLint MaxComputeShaderStorageBlocks; // GL_MAX_UNIFORM_BLOCK_SIZE
		GLint MaxComputeShaderStorageBlocks; // GL_MAX_UNIFORM_LOCATIONS
		GLint MaxComputeShaderStorageBlocks; // GL_MAX_VARYING_COMPONENTS
		GLint MaxComputeShaderStorageBlocks; // GL_MAX_VARYING_VECTORS
		GLint MaxComputeShaderStorageBlocks; // GL_MAX_VARYING_FLOATS
		GLint MaxComputeShaderStorageBlocks; // GL_MAX_SHADER_STORAGE_BUFFER_BINDINGS
		GLint MaxComputeShaderStorageBlocks; // GL_SHADER_STORAGE_BUFFER_OFFSET_ALIGNMENT
		GLint MaxComputeShaderStorageBlocks; // GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT
		GLint MaxComputeShaderStorageBlocks; // GL_NUM_PROGRAM_BINARY_FORMATS
		GLint MaxComputeShaderStorageBlocks; // GL_NUM_SHADER_BINARY_FORMATS
		GLint MaxComputeShaderStorageBlocks; // GL_PROGRAM_BINARY_FORMATS
	} ProgramData;

	void initProgram()
	{

	}

public:
	caps() :
		Version(VersionData),
		Debug(DebugData),
		Combined(CombinedData),
		Compute(ComputeData),
		Vertex(VertexData),
		Control(ControlData),
		Evaluation(EvaluationData),
		Geometry(GeometryData),
		Fragment(FragmentData),
		Pulling(PullingData),
		Framebuffer(FramebufferData),
		Rasterizer(RasterizerData),
		Buffer(BufferData),
		Texture(TextureData),
		Program(ProgramData)
	{
		this->initVersion();
		this->initDebug();
		this->initCombined();
		this->initCompute();
		this->initVertex();
		this->initControl();
		this->initEvaluation();
		this->initGeometry();
		this->initFragment();
		this->initPulling();
		this->initRasterizer();
		this->initFramebuffer();
		this->initBuffer();
		this->initTexture();
		this->initProgram();
	}

	version const & Version;
	debug const & Debug;
	combined const & Combined;
	compute const & Compute;
	vertex const & Vertex;
	control const & Control;
	evaluation const & Evaluation;
	geometry const & Geometry;
	fragment const & Fragment;
	pulling const & Pulling;
	rasterizer const & Rasterizer;
	framebuffer const & Framebuffer;
	buffer const & Buffer;
	texture const & Texture;
	program const & Program;
};

bool initCaps()
{
	caps Caps;
	
}

bool initProgram()
{
	bool Validated(true);
	
	glGenProgramPipelines(pipeline::MAX, &PipelineName[0]);

	if(Validated)
	{
		glf::compiler Compiler;
		GLuint VertShaderName = Compiler.create(GL_VERTEX_SHADER, 
			glf::DATA_DIRECTORY + VERT_SHADER_SOURCE_TEXTURE, 
			"--version 440 --profile core");
		GLuint FragShaderName = Compiler.create(GL_FRAGMENT_SHADER, 
			glf::DATA_DIRECTORY + FRAG_SHADER_SOURCE_TEXTURE,
			"--version 440 --profile core");
		Validated = Validated && Compiler.check();

		ProgramName[pipeline::TEXTURE] = glCreateProgram();
		glProgramParameteri(ProgramName[pipeline::TEXTURE], GL_PROGRAM_SEPARABLE, GL_TRUE);
		glAttachShader(ProgramName[pipeline::TEXTURE], VertShaderName);
		glAttachShader(ProgramName[pipeline::TEXTURE], FragShaderName);
		glLinkProgram(ProgramName[pipeline::TEXTURE]);
		glDeleteShader(VertShaderName);
		glDeleteShader(FragShaderName);

		Validated = Validated && glf::checkProgram(ProgramName[pipeline::TEXTURE]);
	}

	if(Validated)
		glUseProgramStages(PipelineName[pipeline::TEXTURE], GL_VERTEX_SHADER_BIT | GL_FRAGMENT_SHADER_BIT, ProgramName[pipeline::TEXTURE]);

	if(Validated)
	{
		glf::compiler Compiler;
		GLuint VertShaderName = Compiler.create(GL_VERTEX_SHADER, 
			glf::DATA_DIRECTORY + VERT_SHADER_SOURCE_SPLASH, 
			"--version 440 --profile core");
		GLuint FragShaderName = Compiler.create(GL_FRAGMENT_SHADER, 
			glf::DATA_DIRECTORY + FRAG_SHADER_SOURCE_SPLASH,
			"--version 440 --profile core");
		Validated = Validated && Compiler.check();

		ProgramName[pipeline::SPLASH] = glCreateProgram();
		glProgramParameteri(ProgramName[pipeline::SPLASH], GL_PROGRAM_SEPARABLE, GL_TRUE);
		glAttachShader(ProgramName[pipeline::SPLASH], VertShaderName);
		glAttachShader(ProgramName[pipeline::SPLASH], FragShaderName);
		glLinkProgram(ProgramName[pipeline::SPLASH]);
		glDeleteShader(VertShaderName);
		glDeleteShader(FragShaderName);

		Validated = Validated && glf::checkProgram(ProgramName[pipeline::SPLASH]);
	}

	if(Validated)
		glUseProgramStages(PipelineName[pipeline::SPLASH], GL_VERTEX_SHADER_BIT | GL_FRAGMENT_SHADER_BIT, ProgramName[pipeline::SPLASH]);

	return Validated;
}

bool initBuffer()
{
	glGenBuffers(buffer::MAX, &BufferName[0]);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, BufferName[buffer::ELEMENT]);
	glBufferStorage(GL_ELEMENT_ARRAY_BUFFER, ElementSize, ElementData, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ARRAY_BUFFER, BufferName[buffer::VERTEX]);
	glBufferStorage(GL_ARRAY_BUFFER, VertexSize, VertexData, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	GLint UniformBufferOffset(0);

	glGetIntegerv(
		GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT,
		&UniformBufferOffset);

	GLint UniformBlockSize = glm::max(GLint(sizeof(glm::mat4)), UniformBufferOffset);

	glBindBuffer(GL_UNIFORM_BUFFER, BufferName[buffer::TRANSFORM]);
	glBufferStorage(GL_UNIFORM_BUFFER, UniformBlockSize, NULL, GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	return true;
}

bool initTexture()
{
	bool Validated(true);

	gli::texture2D Texture(gli::loadStorageDDS(glf::DATA_DIRECTORY + TEXTURE_DIFFUSE));
	assert(!Texture.empty());

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glGenTextures(texture::MAX, &TextureName[0]);

	glBindTexture(GL_TEXTURE_2D, TextureName[texture::DIFFUSE]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, GLint(Texture.levels() - 1));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_R, GL_RED);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_G, GL_GREEN);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_B, GL_BLUE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_A, GL_ALPHA);
	glTexStorage2D(GL_TEXTURE_2D, GLint(Texture.levels()), GL_COMPRESSED_RGB_S3TC_DXT1_EXT, GLsizei(Texture.dimensions().x), GLsizei(Texture.dimensions().y));

	for(std::size_t Level = 0; Level < Texture.levels(); ++Level)
	{
		glCompressedTexSubImage2D(
			GL_TEXTURE_2D,
			GLint(Level),
			0, 0,
			GLsizei(Texture[Level].dimensions().x), 
			GLsizei(Texture[Level].dimensions().y), 
			GL_COMPRESSED_RGB_S3TC_DXT1_EXT, 
			GLsizei(Texture[Level].size()), 
			Texture[Level].data());
	}
	
	glBindTexture(GL_TEXTURE_2D, TextureName[texture::COLORBUFFER]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
	glTexStorage2D(GL_TEXTURE_2D, GLint(1), GL_RGBA8, GLsizei(Window.Size.x), GLsizei(Window.Size.y));

	glBindTexture(GL_TEXTURE_2D, TextureName[texture::RENDERBUFFER]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
	glTexStorage2D(GL_TEXTURE_2D, GLint(1), GL_DEPTH_COMPONENT24, GLsizei(Window.Size.x), GLsizei(Window.Size.y));

	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

	return Validated;
}

bool initVertexArray()
{
	glGenVertexArrays(pipeline::MAX, &VertexArrayName[0]);
	glBindVertexArray(VertexArrayName[pipeline::TEXTURE]);
		glVertexAttribFormat(glf::semantic::attr::POSITION, 2, GL_FLOAT, GL_FALSE, 0);
		glVertexAttribBinding(glf::semantic::attr::POSITION, glf::semantic::buffer::STATIC);
		glEnableVertexAttribArray(glf::semantic::attr::POSITION);

		glVertexAttribFormat(glf::semantic::attr::TEXCOORD, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2));
		glVertexAttribBinding(glf::semantic::attr::TEXCOORD, glf::semantic::buffer::STATIC);
		glEnableVertexAttribArray(glf::semantic::attr::TEXCOORD);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, BufferName[buffer::ELEMENT]);
	glBindVertexArray(0);

	glBindVertexArray(VertexArrayName[pipeline::SPLASH]);
	glBindVertexArray(0);

	return true;
}

bool initFramebuffer()
{
	bool Validated(true);

	glGenFramebuffers(1, &FramebufferName);
	glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, TextureName[texture::COLORBUFFER], 0);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, TextureName[texture::RENDERBUFFER], 0);

	if(glf::checkFramebuffer(FramebufferName))
		return false;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	return true;
}

bool initDebugOutput()
{
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageControlARB(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE);
	glDebugMessageCallbackARB(&glf::debugOutput, NULL);

	return true;
}

bool begin()
{
	bool Validated(true);
	Validated = Validated && glf::checkGLVersion(SAMPLE_MAJOR_VERSION, SAMPLE_MINOR_VERSION);

	if(Validated)
		Validated = initDebugOutput();
	if(Validated)
		Validated = initProgram();
	if(Validated)
		Validated = initBuffer();
	if(Validated)
		Validated = initVertexArray();
	if(Validated)
		Validated = initTexture();
	if(Validated)
		Validated = initFramebuffer();

	glBindBuffer(GL_UNIFORM_BUFFER, BufferName[buffer::TRANSFORM]);
	UniformPointer = (glm::mat4*)glMapBufferRange(
		GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4),
		GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_INVALIDATE_BUFFER_BIT | GL_MAP_UNSYNCHRONIZED_BIT | GL_MAP_FLUSH_EXPLICIT_BIT);

	return Validated;
}

bool end()
{
	bool Validated(true);

	glDeleteProgramPipelines(pipeline::MAX, &PipelineName[0]);
	glDeleteProgram(ProgramName[pipeline::SPLASH]);
	glDeleteProgram(ProgramName[pipeline::TEXTURE]);
	glDeleteBuffers(buffer::MAX, &BufferName[0]);
	glDeleteFramebuffers(1, &FramebufferName);
	glDeleteTextures(texture::MAX, &TextureName[0]);
	glDeleteVertexArrays(pipeline::MAX, &VertexArrayName[0]);

	return Validated;
}

void display()
{
	{
		// Compute the MVP (Model View Projection matrix)
		float Aspect = (Window.Size.x * 0.50f) / (Window.Size.y * 0.50f);
		glm::mat4 Projection = glm::perspective(45.0f, Aspect, 0.1f, 100.0f);
		glm::mat4 ViewTranslateZ = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -Window.TranlationCurrent.y));
		glm::mat4 ViewRotateX = glm::rotate(ViewTranslateZ, Window.RotationCurrent.y, glm::vec3(1.f, 0.f, 0.f));
		glm::mat4 ViewRotateY = glm::rotate(ViewRotateX, Window.RotationCurrent.x, glm::vec3(0.f, 1.f, 0.f));
		glm::mat4 View = ViewRotateY;
		glm::mat4 Model = glm::mat4(1.0f);
		glm::mat4 MVP = Projection * View * Model;

		*UniformPointer = MVP;
	}

	glFlushMappedBufferRange(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4));

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glViewportIndexedf(0, 0, 0, GLfloat(Window.Size.x), GLfloat(Window.Size.y));

	glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);
	float Depth(1.0f);
	glClearBufferfv(GL_DEPTH , 0, &Depth);
	glClearTexImage(TextureName[texture::COLORBUFFER], 0, 
		GL_RGBA, GL_UNSIGNED_BYTE, &glm::u8vec4(255, 127,   0, 255));
	glClearTexSubImage(TextureName[texture::COLORBUFFER], 0, 
		64, 64, 0,
		64, 64, 1,
		GL_RGBA, GL_UNSIGNED_BYTE, &glm::u8vec4(  0, 127, 255, 255));
	glClearTexSubImage(TextureName[texture::COLORBUFFER], 0, 
		256, 0, 0,
		64, 64, 1,
		GL_RGBA, GL_UNSIGNED_BYTE, &glm::u8vec4(  0, 127, 255, 255));
	glClearTexSubImage(TextureName[texture::COLORBUFFER], 0, 
		128, 384, 0,
		64, 64, 1,
		GL_RGBA, GL_UNSIGNED_BYTE, &glm::u8vec4(  0, 127, 255, 255));
	glClearTexSubImage(TextureName[texture::COLORBUFFER], 0, 
		512, 256, 0,
		64, 64, 1,
		GL_RGBA, GL_UNSIGNED_BYTE, &glm::u8vec4(  0, 127, 255, 255));


	// Bind rendering objects
	glBindProgramPipeline(PipelineName[pipeline::TEXTURE]);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, TextureName[texture::DIFFUSE]);
	glBindVertexArray(VertexArrayName[pipeline::TEXTURE]);
	glBindVertexBuffer(glf::semantic::buffer::STATIC, BufferName[buffer::VERTEX], 0, GLsizei(sizeof(glf::vertex_v2fv2f)));
	glBindBufferBase(GL_UNIFORM_BUFFER, glf::semantic::uniform::TRANSFORM0, BufferName[buffer::TRANSFORM]);

	glDrawElementsInstancedBaseVertexBaseInstance(
		GL_TRIANGLES, ElementCount, GL_UNSIGNED_SHORT, 0, 2, 0, 0);

	glDisable(GL_DEPTH_TEST);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glBindProgramPipeline(PipelineName[pipeline::SPLASH]);
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(VertexArrayName[pipeline::SPLASH]);
	glBindTexture(GL_TEXTURE_2D, TextureName[texture::COLORBUFFER]);

	glDrawArraysInstanced(GL_TRIANGLES, 0, 3, 1);

	glf::swapBuffers();
}

int main(int argc, char* argv[])
{
	return glf::run(
		argc, argv,
		glm::ivec2(::SAMPLE_SIZE_WIDTH, ::SAMPLE_SIZE_HEIGHT), 
		GLF_CONTEXT_CORE_PROFILE_BIT, 
		::SAMPLE_MAJOR_VERSION, ::SAMPLE_MINOR_VERSION);
}
