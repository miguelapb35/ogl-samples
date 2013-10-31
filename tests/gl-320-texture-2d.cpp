//**********************************
// OpenGL Texture 2D
// 27/08/2009 - 11/03/2011
//**********************************
// Christophe Riccio
// ogl-samples@g-truc.net
//**********************************
// G-Truc Creation
// www.g-truc.net
//**********************************

#include <glf/glf.hpp>

namespace
{
	char const * SAMPLE_NAME("OpenGL Texture 2D");
	char const * VERT_SHADER_SOURCE("gl-320/texture-2d.vert");
	char const * FRAG_SHADER_SOURCE("gl-320/texture-2d.frag");
	char const * TEXTURE_DIFFUSE("kueken1-bgr8.dds");
	int const SAMPLE_SIZE_WIDTH(640);
	int const SAMPLE_SIZE_HEIGHT(480);
	int const SAMPLE_MAJOR_VERSION(3);
	int const SAMPLE_MINOR_VERSION(2);

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

	GLuint VertexArrayName(0);
	GLuint ProgramName(0);
	GLuint TextureName(0);
	std::vector<GLuint> BufferName(buffer::MAX);

	GLint UniformTransform(0);
	GLint UniformDiffuse(0);
}//namespace

struct caps
{
	enum profile
	{
		CORE = 0x00000001,
		COMPATIBILITY = 0x00000002,
		ES = 0x00000004
	};

private:

	struct version
	{
		version(profile const & Profile) :
			ARB_multitexture(false),
			ARB_transpose_matrix(false),
			ARB_seamless_cubemap_per_texture(false),
			ARB_shader_draw_parameters(false),
			ARB_shader_group_vote(false),
			ARB_sparse_texture(false)
		{}
		GLint Major;					// GL_MINOR_VERSION
		GLint Minor;					// GL_MAJOR_VERSION
		GLint NumExtensions;			// GL_NUM_EXTENSIONS
		profile Profile;
		bool ARB_multitexture;
		bool ARB_transpose_matrix;

		bool ARB_fragment_shader;
		bool ARB_vertex_shader;

		bool ARB_geometry_shader;
		bool ARB_uniform_buffer_object;
		bool ARB_blend_func_extended;

		bool ARB_tessellation_shader;
		bool ARB_texture_buffer_object_rgb32;
		bool ARB_transform_feedback2;
		bool ARB_transform_feedback3;

		bool ARB_map_buffer_alignment;
		bool ARB_shader_atomic_counters;
		bool ARB_shader_image_load_store;

		bool ARB_shader_storage_buffer_object;

		bool ARB_compute_shader;
		bool ARB_copy_image;
		bool ARB_texture_view;
		bool ARB_vertex_attrib_binding;
		bool ARB_robustness_isolation;
		bool ARB_ES3_compatibility;
		bool ARB_explicit_uniform_location;
		bool ARB_indirect_parameters;
		bool ARB_seamless_cubemap_per_texture;
		bool ARB_shader_draw_parameters;
		bool ARB_shader_group_vote;
		bool ARB_sparse_texture;
	} VersionData;

	inline bool check(GLint MajorVersionRequire, GLint MinorVersionRequire)
	{
		return (VersionData.Major * 100 + VersionData.Minor * 10)
			>= (MajorVersionRequire * 100 + MinorVersionRequire * 10);
	}

	void initVersion()
	{
		memset(&VersionData, 0, sizeof(VersionData));

		glGetIntegerv(GL_MINOR_VERSION, &VersionData.Minor);
		glGetIntegerv(GL_MAJOR_VERSION, &VersionData.Major);
		glGetIntegerv(GL_NUM_EXTENSIONS, &VersionData.NumExtensions);

		for (GLint i = 0; i < VersionData.NumExtensions; ++i)
		{
			const char* Extension = reinterpret_cast<const char*>(glGetStringi(GL_EXTENSIONS, i));
			if(!strcmp("GL_ARB_multitexture", Extension))
				VersionData.ARB_multitexture = true;
			else if(!strcmp("GL_ARB_transpose_matrix", Extension))
				VersionData.ARB_transpose_matrix = true;
			else if(!strcmp("GL_ARB_fragment_shader", Extension))
				VersionData.ARB_fragment_shader = true;
			else if(!strcmp("GL_ARB_vertex_shader", Extension))
				VersionData.ARB_vertex_shader = true;

			else if(!strcmp("GL_ARB_geometry_shader", Extension))
				VersionData.ARB_geometry_shader = true;
			else if(!strcmp("GL_ARB_uniform_buffer_object", Extension))
				VersionData.ARB_uniform_buffer_object = true;
			else if(!strcmp("GL_ARB_blend_func_extended", Extension))
				VersionData.ARB_blend_func_extended = true;

			else if(!strcmp("GL_ARB_tessellation_shader", Extension))
				VersionData.ARB_tessellation_shader = true;
			else if(!strcmp("GL_ARB_texture_buffer_object_rgb32", Extension))
				VersionData.ARB_texture_buffer_object_rgb32 = true;
			else if(!strcmp("GL_ARB_transform_feedback2", Extension))
				VersionData.ARB_transform_feedback2 = true;
			else if(!strcmp("GL_ARB_transform_feedback3", Extension))
				VersionData.ARB_transform_feedback3 = true;

			else if(!strcmp("GL_ARB_shader_atomic_counters", Extension))
				VersionData.ARB_shader_atomic_counters = true;
			else if(!strcmp("GL_ARB_shader_storage_buffer_object", Extension))
				VersionData.ARB_shader_storage_buffer_object = true;

			else if(!strcmp("GL_ARB_compute_shader", Extension))
				VersionData.ARB_compute_shader = true;
			else if(!strcmp("GL_ARB_copy_image", Extension))
				VersionData.ARB_copy_image = true;
			else if(!strcmp("GL_ARB_texture_view", Extension))
				VersionData.ARB_texture_view = true;
			else if(!strcmp("GL_ARB_vertex_attrib_binding", Extension))
				VersionData.ARB_vertex_attrib_binding = true;
			else if(!strcmp("GL_ARB_robustness_isolation", Extension))
				VersionData.ARB_robustness_isolation = true;
			else if(!strcmp("GL_ARB_explicit_uniform_location", Extension))
				VersionData.ARB_explicit_uniform_location = true;

			else if(!strcmp("GL_ARB_uniform_buffer_object", Extension))
				VersionData.ARB_uniform_buffer_object = true;

			else if(!strcmp("GL_ARB_shader_atomic_counters", Extension))
				VersionData.ARB_shader_atomic_counters = true;
			else if(!strcmp("GL_ARB_shader_image_load_store", Extension))
				VersionData.ARB_shader_image_load_store = true;

			else if(!strcmp("GL_ARB_shader_atomic_counters", Extension))
				VersionData.ARB_shader_atomic_counters = true;

			else if(!strcmp("GL_ARB_shader_storage_buffer_object", Extension))
				VersionData.ARB_shader_storage_buffer_object = true;

			else if(!strcmp("GL_ARB_indirect_parameters", Extension))
				VersionData.ARB_indirect_parameters = true;
			else if(!strcmp("GL_ARB_seamless_cubemap_per_texture", Extension))
				VersionData.ARB_seamless_cubemap_per_texture = true;
			else if(!strcmp("GL_ARB_shader_draw_parameters", Extension))
				VersionData.ARB_shader_draw_parameters = true;
			else if(!strcmp("GL_ARB_shader_group_vote", Extension))
				VersionData.ARB_shader_group_vote = true;
			else if(!strcmp("GL_ARB_sparse_texture", Extension))
				VersionData.ARB_sparse_texture = true;
		}
	}

	struct debug
	{
		GLint CONTEXT_FLAGS;
		GLint MAX_DEBUG_GROUP_STACK_DEPTH;
		GLint MAX_LABEL_LENGTH;
		GLint MAX_SERVER_WAIT_TIMEOUT;
	} DebugData;

	void initDebug()
	{
		memset(&DebugData, 0, sizeof(DebugData));

		glGetIntegerv(GL_CONTEXT_FLAGS, &DebugData.CONTEXT_FLAGS);
		glGetIntegerv(GL_MAX_DEBUG_GROUP_STACK_DEPTH, &DebugData.MAX_DEBUG_GROUP_STACK_DEPTH);
		glGetIntegerv(GL_MAX_LABEL_LENGTH, &DebugData.MAX_LABEL_LENGTH);
		glGetIntegerv(GL_MAX_SERVER_WAIT_TIMEOUT, &DebugData.MAX_SERVER_WAIT_TIMEOUT);
	}

	struct compute
	{
		GLint MAX_COMPUTE_SHADER_STORAGE_BLOCKS;
		GLint MAX_COMPUTE_UNIFORM_BLOCKS;
		GLint MAX_COMPUTE_TEXTURE_IMAGE_UNITS;
		GLint MAX_COMPUTE_IMAGE_UNIFORMS;
		GLint MAX_COMPUTE_UNIFORM_COMPONENTS;
		GLint MAX_COMBINED_COMPUTE_UNIFORM_COMPONENTS;
		GLint MAX_COMPUTE_ATOMIC_COUNTERS;
		GLint MAX_COMPUTE_ATOMIC_COUNTER_BUFFERS;
		GLint MAX_COMPUTE_SHARED_MEMORY_SIZE;
		GLint MAX_COMPUTE_WORK_GROUP_INVOCATIONS;
		GLint MAX_COMPUTE_WORK_GROUP_COUNT;
		GLint MAX_COMPUTE_WORK_GROUP_SIZE;
	} ComputeData;

	void initCompute()
	{
		memset(&ComputeData, 0, sizeof(ComputeData));

		if(check(4, 3) || VersionData.ARB_compute_shader)
		{
			glGetIntegerv(GL_MAX_COMPUTE_TEXTURE_IMAGE_UNITS, &ComputeData.MAX_COMPUTE_TEXTURE_IMAGE_UNITS);
			glGetIntegerv(GL_MAX_COMPUTE_UNIFORM_COMPONENTS, &ComputeData.MAX_COMPUTE_UNIFORM_COMPONENTS);
			glGetIntegerv(GL_MAX_COMPUTE_SHARED_MEMORY_SIZE, &ComputeData.MAX_COMPUTE_SHARED_MEMORY_SIZE);
			glGetIntegerv(GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS, &ComputeData.MAX_COMPUTE_WORK_GROUP_INVOCATIONS);
			glGetIntegerv(GL_MAX_COMPUTE_WORK_GROUP_COUNT, &ComputeData.MAX_COMPUTE_WORK_GROUP_COUNT);
			glGetIntegerv(GL_MAX_COMPUTE_WORK_GROUP_SIZE, &ComputeData.MAX_COMPUTE_WORK_GROUP_SIZE);
		}

		if(check(4, 3) || (VersionData.ARB_compute_shader && VersionData.ARB_uniform_buffer_object))
		{
			glGetIntegerv(GL_MAX_COMPUTE_UNIFORM_BLOCKS, &ComputeData.MAX_COMPUTE_UNIFORM_BLOCKS);
			glGetIntegerv(GL_MAX_COMBINED_COMPUTE_UNIFORM_COMPONENTS, &ComputeData.MAX_COMBINED_COMPUTE_UNIFORM_COMPONENTS);
		}

		if(check(4, 3) || (VersionData.ARB_compute_shader && VersionData.ARB_shader_image_load_store))
			glGetIntegerv(GL_MAX_COMPUTE_IMAGE_UNIFORMS, &ComputeData.MAX_COMPUTE_IMAGE_UNIFORMS);

		if(check(4, 3) || (VersionData.ARB_compute_shader && VersionData.ARB_shader_atomic_counters))
		{
			glGetIntegerv(GL_MAX_COMPUTE_ATOMIC_COUNTERS, &ComputeData.MAX_COMPUTE_ATOMIC_COUNTERS);
			glGetIntegerv(GL_MAX_COMPUTE_ATOMIC_COUNTER_BUFFERS, &ComputeData.MAX_COMPUTE_ATOMIC_COUNTER_BUFFERS);
		}

		if(check(4, 3) || VersionData.ARB_compute_shader && VersionData.ARB_shader_storage_buffer_object)
			glGetIntegerv(GL_MAX_COMPUTE_SHADER_STORAGE_BLOCKS, &ComputeData.MAX_COMPUTE_SHADER_STORAGE_BLOCKS);
	}

	struct vertex
	{
		GLint MAX_VERTEX_ATOMIC_COUNTERS;
		GLint MAX_VERTEX_SHADER_STORAGE_BLOCKS;
		GLint MAX_VERTEX_ATTRIBS;
		GLint MAX_VERTEX_OUTPUT_COMPONENTS;
		GLint MAX_VERTEX_TEXTURE_IMAGE_UNITS;
		GLint MAX_VERTEX_UNIFORM_COMPONENTS;
		GLint MAX_VERTEX_UNIFORM_VECTORS;
		GLint MAX_VERTEX_UNIFORM_BLOCKS;
		GLint MAX_COMBINED_VERTEX_UNIFORM_COMPONENTS;
	} VertexData;

	void initVertex()
	{
		memset(&VertexData, 0, sizeof(VertexData));

		if(check(2, 1) || Version.ARB_vertex_shader)
		{
			glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &VertexData.MAX_VERTEX_ATTRIBS);
			glGetIntegerv(GL_MAX_VERTEX_OUTPUT_COMPONENTS, &VertexData.MAX_VERTEX_OUTPUT_COMPONENTS);
			glGetIntegerv(GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS, &VertexData.MAX_VERTEX_TEXTURE_IMAGE_UNITS);
			glGetIntegerv(GL_MAX_VERTEX_UNIFORM_COMPONENTS, &VertexData.MAX_VERTEX_UNIFORM_COMPONENTS);
			glGetIntegerv(GL_MAX_VERTEX_UNIFORM_VECTORS, &VertexData.MAX_VERTEX_UNIFORM_VECTORS);
		}
		if(check(3, 2) || (Version.ARB_vertex_shader && Version.ARB_uniform_buffer_object))
		{
			glGetIntegerv(GL_MAX_VERTEX_UNIFORM_BLOCKS, &VertexData.MAX_VERTEX_UNIFORM_BLOCKS);
			glGetIntegerv(GL_MAX_COMBINED_VERTEX_UNIFORM_COMPONENTS, &VertexData.MAX_COMBINED_VERTEX_UNIFORM_COMPONENTS);
		}
		if(check(4, 2) || (Version.ARB_vertex_shader && Version.ARB_shader_atomic_counters))
			glGetIntegerv(GL_MAX_VERTEX_ATOMIC_COUNTERS, &VertexData.MAX_VERTEX_ATOMIC_COUNTERS);
		if(check(4, 3) || (Version.ARB_vertex_shader && Version.ARB_shader_storage_buffer_object))
			glGetIntegerv(GL_MAX_VERTEX_SHADER_STORAGE_BLOCKS, &VertexData.MAX_VERTEX_SHADER_STORAGE_BLOCKS);
	}

	struct control
	{
		GLint MAX_TESS_CONTROL_ATOMIC_COUNTERS;
		GLint MAX_TESS_CONTROL_SHADER_STORAGE_BLOCKS;
		GLint MAX_TESS_CONTROL_INPUT_COMPONENTS;
		GLint MAX_TESS_CONTROL_OUTPUT_COMPONENTS;
		GLint MAX_TESS_CONTROL_TEXTURE_IMAGE_UNITS;
		GLint MAX_TESS_CONTROL_UNIFORM_BLOCKS;
		GLint MAX_TESS_CONTROL_UNIFORM_COMPONENTS;
		GLint MAX_COMBINED_TESS_CONTROL_UNIFORM_COMPONENTS;
	} ControlData;

	void initControl()
	{
		memset(&ControlData, 0, sizeof(ControlData));

		if(check(4, 0) || VersionData.ARB_tessellation_shader)
		{
			glGetIntegerv(GL_MAX_TESS_CONTROL_INPUT_COMPONENTS, &ControlData.MAX_TESS_CONTROL_INPUT_COMPONENTS);
			glGetIntegerv(GL_MAX_TESS_CONTROL_OUTPUT_COMPONENTS, &ControlData.MAX_TESS_CONTROL_OUTPUT_COMPONENTS);
			glGetIntegerv(GL_MAX_TESS_CONTROL_TEXTURE_IMAGE_UNITS, &ControlData.MAX_TESS_CONTROL_TEXTURE_IMAGE_UNITS);
			glGetIntegerv(GL_MAX_TESS_CONTROL_UNIFORM_COMPONENTS, &ControlData.MAX_TESS_CONTROL_UNIFORM_COMPONENTS);
		}
		if(check(4, 0) || (VersionData.ARB_tessellation_shader && VersionData.ARB_uniform_buffer_object))
		{
			glGetIntegerv(GL_MAX_TESS_CONTROL_UNIFORM_BLOCKS, &ControlData.MAX_TESS_CONTROL_UNIFORM_BLOCKS);
			glGetIntegerv(GL_MAX_COMBINED_TESS_CONTROL_UNIFORM_COMPONENTS, &ControlData.MAX_COMBINED_TESS_CONTROL_UNIFORM_COMPONENTS);
		}
		if(check(4, 2) || (VersionData.ARB_tessellation_shader && VersionData.ARB_shader_atomic_counters))
			glGetIntegerv(GL_MAX_TESS_CONTROL_ATOMIC_COUNTERS, &ControlData.MAX_TESS_CONTROL_ATOMIC_COUNTERS);
		if(check(4, 3) || (VersionData.ARB_tessellation_shader && VersionData.ARB_shader_storage_buffer_object))
			glGetIntegerv(GL_MAX_TESS_CONTROL_SHADER_STORAGE_BLOCKS, &ControlData.MAX_TESS_CONTROL_SHADER_STORAGE_BLOCKS);
	}

	struct evaluation
	{
		GLint MAX_TESS_EVALUATION_ATOMIC_COUNTERS;
		GLint MAX_TESS_EVALUATION_SHADER_STORAGE_BLOCKS;
		GLint MAX_TESS_EVALUATION_INPUT_COMPONENTS;
		GLint MAX_TESS_EVALUATION_OUTPUT_COMPONENTS;
		GLint MAX_TESS_EVALUATION_TEXTURE_IMAGE_UNITS;
		GLint MAX_TESS_EVALUATION_UNIFORM_BLOCKS;
		GLint MAX_TESS_EVALUATION_UNIFORM_COMPONENTS;
		GLint MAX_COMBINED_TESS_EVALUATION_UNIFORM_COMPONENTS;
	} EvaluationData;

	void initEvaluation()
	{
		memset(&EvaluationData, 0, sizeof(EvaluationData));

		if(check(4, 0) || VersionData.ARB_tessellation_shader)
		{
			glGetIntegerv(GL_MAX_TESS_EVALUATION_INPUT_COMPONENTS, &EvaluationData.MAX_TESS_EVALUATION_INPUT_COMPONENTS);
			glGetIntegerv(GL_MAX_TESS_EVALUATION_OUTPUT_COMPONENTS, &EvaluationData.MAX_TESS_EVALUATION_OUTPUT_COMPONENTS);
			glGetIntegerv(GL_MAX_TESS_EVALUATION_TEXTURE_IMAGE_UNITS, &EvaluationData.MAX_TESS_EVALUATION_TEXTURE_IMAGE_UNITS);
			glGetIntegerv(GL_MAX_TESS_EVALUATION_UNIFORM_COMPONENTS, &EvaluationData.MAX_TESS_EVALUATION_UNIFORM_COMPONENTS);
		}
		if(check(4, 0) || (VersionData.ARB_tessellation_shader && VersionData.ARB_uniform_buffer_object))
		{
			glGetIntegerv(GL_MAX_TESS_EVALUATION_UNIFORM_BLOCKS, &EvaluationData.MAX_TESS_EVALUATION_UNIFORM_BLOCKS);
			glGetIntegerv(GL_MAX_COMBINED_TESS_EVALUATION_UNIFORM_COMPONENTS, &EvaluationData.MAX_COMBINED_TESS_EVALUATION_UNIFORM_COMPONENTS);
		}
		if(check(4, 2) || (VersionData.ARB_tessellation_shader && VersionData.ARB_shader_atomic_counters))
			glGetIntegerv(GL_MAX_TESS_EVALUATION_ATOMIC_COUNTERS, &EvaluationData.MAX_TESS_EVALUATION_ATOMIC_COUNTERS);
		if(check(4, 3) || (VersionData.ARB_tessellation_shader && VersionData.ARB_shader_storage_buffer_object))
			glGetIntegerv(GL_MAX_TESS_EVALUATION_SHADER_STORAGE_BLOCKS, &EvaluationData.MAX_TESS_EVALUATION_SHADER_STORAGE_BLOCKS);
	}

	struct geometry
	{
		GLint MAX_GEOMETRY_ATOMIC_COUNTERS;
		GLint MAX_GEOMETRY_SHADER_STORAGE_BLOCKS;
		GLint MAX_GEOMETRY_INPUT_COMPONENTS;
		GLint MAX_GEOMETRY_OUTPUT_COMPONENTS;
		GLint MAX_GEOMETRY_TEXTURE_IMAGE_UNITS;
		GLint MAX_GEOMETRY_UNIFORM_BLOCKS;
		GLint MAX_GEOMETRY_UNIFORM_COMPONENTS;
		GLint MAX_COMBINED_GEOMETRY_UNIFORM_COMPONENTS;
		GLint MAX_VERTEX_STREAMS;
	} GeometryData;

	void initGeometry()
	{
		memset(&GeometryData, 0, sizeof(GeometryData));

		if(check(3, 2) || VersionData.ARB_geometry_shader)
		{
			glGetIntegerv(GL_MAX_GEOMETRY_INPUT_COMPONENTS, &GeometryData.MAX_GEOMETRY_INPUT_COMPONENTS);
			glGetIntegerv(GL_MAX_GEOMETRY_OUTPUT_COMPONENTS, &GeometryData.MAX_GEOMETRY_OUTPUT_COMPONENTS);
			glGetIntegerv(GL_MAX_GEOMETRY_TEXTURE_IMAGE_UNITS, &GeometryData.MAX_GEOMETRY_TEXTURE_IMAGE_UNITS);
			glGetIntegerv(GL_MAX_GEOMETRY_UNIFORM_COMPONENTS, &GeometryData.MAX_GEOMETRY_UNIFORM_COMPONENTS);
		}
		if(check(3, 2) || (VersionData.ARB_geometry_shader && VersionData.ARB_uniform_buffer_object))
		{
			glGetIntegerv(GL_MAX_GEOMETRY_UNIFORM_BLOCKS, &GeometryData.MAX_GEOMETRY_UNIFORM_BLOCKS);
			glGetIntegerv(GL_MAX_COMBINED_GEOMETRY_UNIFORM_COMPONENTS, &GeometryData.MAX_COMBINED_GEOMETRY_UNIFORM_COMPONENTS);
		}
		if(check(4, 0) || (VersionData.ARB_geometry_shader && VersionData.ARB_transform_feedback3))
			glGetIntegerv(GL_MAX_VERTEX_STREAMS, &GeometryData.MAX_VERTEX_STREAMS);
		if(check(4, 2) || (VersionData.ARB_geometry_shader && VersionData.ARB_shader_atomic_counters))
			glGetIntegerv(GL_MAX_GEOMETRY_ATOMIC_COUNTERS, &GeometryData.MAX_GEOMETRY_ATOMIC_COUNTERS);
		if(check(4, 3) || (VersionData.ARB_geometry_shader && VersionData.ARB_shader_storage_buffer_object))
			glGetIntegerv(GL_MAX_GEOMETRY_SHADER_STORAGE_BLOCKS, &GeometryData.MAX_GEOMETRY_SHADER_STORAGE_BLOCKS);
	}

	struct fragment
	{
		GLint MAX_FRAGMENT_ATOMIC_COUNTERS;
		GLint MAX_FRAGMENT_SHADER_STORAGE_BLOCKS;
		GLint MAX_FRAGMENT_INPUT_COMPONENTS;
		GLint MAX_FRAGMENT_UNIFORM_COMPONENTS;
		GLint MAX_FRAGMENT_UNIFORM_VECTORS;
		GLint MAX_FRAGMENT_UNIFORM_BLOCKS;
		GLint MAX_COMBINED_FRAGMENT_UNIFORM_COMPONENTS;
		GLint MAX_DRAW_BUFFERS;
		GLint MAX_DUAL_SOURCE_DRAW_BUFFERS;
	} FragmentData;

	void initFragment()
	{
		memset(&FragmentData, 0, sizeof(FragmentData));

		if(check(2, 1))
			glGetIntegerv(GL_MAX_DRAW_BUFFERS, &FragmentData.MAX_DRAW_BUFFERS);

		if(check(2, 1) || VersionData.ARB_fragment_shader)
		{
			glGetIntegerv(GL_MAX_FRAGMENT_INPUT_COMPONENTS, &FragmentData.MAX_FRAGMENT_INPUT_COMPONENTS);
			glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_COMPONENTS, &FragmentData.MAX_FRAGMENT_UNIFORM_COMPONENTS);
			glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_VECTORS, &FragmentData.MAX_FRAGMENT_UNIFORM_VECTORS);
		}
		if(check(3, 2) || (VersionData.ARB_fragment_shader && VersionData.ARB_uniform_buffer_object))
		{
			glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_BLOCKS, &FragmentData.MAX_FRAGMENT_UNIFORM_BLOCKS);
			glGetIntegerv(GL_MAX_COMBINED_FRAGMENT_UNIFORM_COMPONENTS, &FragmentData.MAX_COMBINED_FRAGMENT_UNIFORM_COMPONENTS);
		}
		if(check(3, 3) || (VersionData.ARB_blend_func_extended))
			glGetIntegerv(GL_MAX_DUAL_SOURCE_DRAW_BUFFERS, &FragmentData.MAX_DUAL_SOURCE_DRAW_BUFFERS);
		if(check(4, 2) || (VersionData.ARB_fragment_shader && VersionData.ARB_shader_atomic_counters))
			glGetIntegerv(GL_MAX_FRAGMENT_ATOMIC_COUNTERS, &FragmentData.MAX_FRAGMENT_ATOMIC_COUNTERS);
		if(check(4, 3) || (VersionData.ARB_fragment_shader && VersionData.ARB_shader_storage_buffer_object))
			glGetIntegerv(GL_MAX_FRAGMENT_SHADER_STORAGE_BLOCKS, &FragmentData.MAX_FRAGMENT_SHADER_STORAGE_BLOCKS);
	}

	struct pulling
	{
		GLint MAX_ELEMENTS_INDICES;
		GLint MAX_ELEMENTS_VERTICES;
		GLint MAX_VERTEX_ATTRIB_RELATIVE_OFFSET;
		GLint MAX_VERTEX_ATTRIB_BINDINGS;
		GLint MAX_ELEMENT_INDEX;
	} PullingData;

	void initPulling()
	{
		memset(&PullingData, 0, sizeof(PullingData));

		glGetIntegerv(GL_MAX_ELEMENTS_INDICES, &PullingData.MAX_ELEMENTS_INDICES);
		glGetIntegerv(GL_MAX_ELEMENTS_VERTICES, &PullingData.MAX_ELEMENTS_VERTICES);
		glGetIntegerv(GL_MAX_VERTEX_ATTRIB_RELATIVE_OFFSET, &PullingData.MAX_VERTEX_ATTRIB_RELATIVE_OFFSET);
		glGetIntegerv(GL_MAX_VERTEX_ATTRIB_BINDINGS, &PullingData.MAX_VERTEX_ATTRIB_BINDINGS);
		glGetIntegerv(GL_MAX_ELEMENT_INDEX, &PullingData.MAX_ELEMENT_INDEX);
	}

	struct rasterizer
	{
		GLint SUBPIXEL_BITS;
		GLint MAX_CLIP_DISTANCES;
		GLfloat MAX_VIEWPORT_DIMS;
		GLint MAX_VIEWPORTS;
		GLint VIEWPORT_SUBPIXEL_BITS;
		GLfloat VIEWPORT_BOUNDS_RANGE[2];
	} RasterizerData;

	void initRasterizer()
	{
		memset(&RasterizerData, 0, sizeof(RasterizerData));

		glGetIntegerv(GL_SUBPIXEL_BITS, &RasterizerData.SUBPIXEL_BITS);
		glGetIntegerv(GL_MAX_CLIP_DISTANCES, &RasterizerData.MAX_CLIP_DISTANCES);
		glGetFloatv(GL_MAX_VIEWPORT_DIMS, &RasterizerData.MAX_VIEWPORT_DIMS);
		glGetIntegerv(GL_MAX_VIEWPORTS, &RasterizerData.MAX_VIEWPORTS);
		glGetIntegerv(GL_VIEWPORT_SUBPIXEL_BITS, &RasterizerData.VIEWPORT_SUBPIXEL_BITS);
		glGetFloatv(GL_VIEWPORT_BOUNDS_RANGE, RasterizerData.VIEWPORT_BOUNDS_RANGE);
	}

	struct framebuffer
	{
		GLint MAX_FRAMEBUFFER_WIDTH;
		GLint MAX_FRAMEBUFFER_HEIGHT;
		GLint MAX_FRAMEBUFFER_LAYERS;
		GLint MAX_FRAMEBUFFER_SAMPLES;
		GLint MAX_RENDERBUFFER_SIZE;
		GLint MAX_SAMPLE_MASK_WORDS;
	} FramebufferData;

	void initFramebuffer()
	{
		memset(&FramebufferData, 0, sizeof(FramebufferData));

		glGetIntegerv(GL_MAX_FRAMEBUFFER_WIDTH, &FramebufferData.MAX_FRAMEBUFFER_WIDTH);
		glGetIntegerv(GL_MAX_FRAMEBUFFER_HEIGHT, &FramebufferData.MAX_FRAMEBUFFER_HEIGHT);
		glGetIntegerv(GL_MAX_FRAMEBUFFER_LAYERS, &FramebufferData.MAX_FRAMEBUFFER_LAYERS);
		glGetIntegerv(GL_MAX_FRAMEBUFFER_SAMPLES, &FramebufferData.MAX_FRAMEBUFFER_SAMPLES);
		glGetIntegerv(GL_MAX_RENDERBUFFER_SIZE, &FramebufferData.MAX_RENDERBUFFER_SIZE);
		glGetIntegerv(GL_MAX_SAMPLE_MASK_WORDS, &FramebufferData.MAX_SAMPLE_MASK_WORDS);
	}

	struct buffer
	{
		GLint MAX_TRANSFORM_FEEDBACK_BUFFERS;
		GLint MIN_MAP_BUFFER_ALIGNMENT;
	} BufferData;

	void initBuffer()
	{
		memset(&BufferData, 0, sizeof(BufferData));

		if(check(4, 0) || (VersionData.ARB_transform_feedback3))
			glGetIntegerv(GL_MAX_TRANSFORM_FEEDBACK_BUFFERS, &BufferData.MAX_TRANSFORM_FEEDBACK_BUFFERS);
		if(check(4, 2) || (VersionData.ARB_map_buffer_alignment))
			glGetIntegerv(GL_MIN_MAP_BUFFER_ALIGNMENT, &BufferData.MIN_MAP_BUFFER_ALIGNMENT);
	}

	struct texture
	{
		GLint MAX_TEXTURE_IMAGE_UNITS;
		GLint MAX_COMBINED_TEXTURE_IMAGE_UNITS;
		GLint MAX_TEXTURE_LOD_BIAS;
		GLint MAX_TEXTURE_SIZE;
		GLint MAX_RECTANGLE_TEXTURE_SIZE;
		GLint MAX_3D_TEXTURE_SIZE;
		GLint MAX_ARRAY_TEXTURE_LAYERS;
		GLint MAX_CUBE_MAP_TEXTURE_SIZE;
		GLint MAX_COLOR_TEXTURE_SAMPLES;
		GLint MAX_DEPTH_TEXTURE_SAMPLES;
		GLint MAX_INTEGER_SAMPLES;
		GLint MAX_TEXTURE_BUFFER_SIZE;
		GLint NUM_COMPRESSED_TEXTURE_FORMATS;
		GLint MAX_TEXTURE_MAX_ANISOTROPY_EXT;

		bool COMPRESSED_RGB_S3TC_DXT1_EXT;
		bool COMPRESSED_RGBA_S3TC_DXT1_EXT;
		bool COMPRESSED_RGBA_S3TC_DXT3_EXT;
		bool COMPRESSED_RGBA_S3TC_DXT5_EXT;
		bool COMPRESSED_RED_RGTC1;
		bool COMPRESSED_SIGNED_RED_RGTC1;
		bool COMPRESSED_RG_RGTC2;
		bool COMPRESSED_SIGNED_RG_RGTC2;
		bool COMPRESSED_RGBA_BPTC_UNORM;
		bool COMPRESSED_SRGB_ALPHA_BPTC_UNORM;
		bool COMPRESSED_RGB_BPTC_SIGNED_FLOAT;
		bool COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT;
		bool COMPRESSED_RGBA_ASTC_4x4_KHR;
		bool COMPRESSED_RGBA_ASTC_5x4_KHR;
		bool COMPRESSED_RGBA_ASTC_5x5_KHR;
		bool COMPRESSED_RGBA_ASTC_6x5_KHR;
		bool COMPRESSED_RGBA_ASTC_6x6_KHR;
		bool COMPRESSED_RGBA_ASTC_8x5_KHR;
		bool COMPRESSED_RGBA_ASTC_8x6_KHR;
		bool COMPRESSED_RGBA_ASTC_8x8_KHR;
		bool COMPRESSED_RGBA_ASTC_10x5_KHR;
		bool COMPRESSED_RGBA_ASTC_10x6_KHR;
		bool COMPRESSED_RGBA_ASTC_10x8_KHR;
		bool COMPRESSED_RGBA_ASTC_10x10_KHR;
		bool COMPRESSED_RGBA_ASTC_12x10_KHR;
		bool COMPRESSED_RGBA_ASTC_12x12_KHR;
		bool COMPRESSED_RGB8_ETC2;
		bool COMPRESSED_SRGB8_ETC2;
		bool COMPRESSED_RGB8_PUNCHTHROUGH_ALPHA1_ETC2;
		bool COMPRESSED_SRGB8_PUNCHTHROUGH_ALPHA1_ETC2;
		bool COMPRESSED_RGBA8_ETC2_EAC;
		bool COMPRESSED_SRGB8_ALPHA8_ETC2_EAC;
		bool COMPRESSED_R11_EAC;
		bool COMPRESSED_SIGNED_R11_EAC;
		bool COMPRESSED_RG11_EAC;
		bool COMPRESSED_SIGNED_RG11_EAC;
		bool PALETTE4_RGB8_OES;
		bool PALETTE4_RGBA8_OES;
		bool PALETTE4_R5_G6_B5_OES;
		bool PALETTE4_RGBA4_OES;
		bool PALETTE4_RGB5_A1_OES;
		bool PALETTE8_RGB8_OES;
		bool PALETTE8_RGBA8_OES;
		bool PALETTE8_R5_G6_B5_OES;
		bool PALETTE8_RGBA4_OES;
		bool PALETTE8_RGB5_A1_OES;
		bool ETC1_RGB8_OES;
	} TextureData;

#	ifndef GL_ETC1_RGB8_OES
#		define GL_ETC1_RGB8_OES	0x8D64
#	endif

	void initTexture()
	{
		memset(&TextureData, 0, sizeof(TextureData));

		glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &TextureData.MAX_TEXTURE_IMAGE_UNITS);
		glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &TextureData.MAX_COMBINED_TEXTURE_IMAGE_UNITS);
		glGetIntegerv(GL_MAX_TEXTURE_LOD_BIAS, &TextureData.MAX_TEXTURE_LOD_BIAS);
		glGetIntegerv(GL_MAX_TEXTURE_SIZE, &TextureData.MAX_TEXTURE_SIZE);
		glGetIntegerv(GL_MAX_RECTANGLE_TEXTURE_SIZE, &TextureData.MAX_RECTANGLE_TEXTURE_SIZE);
		glGetIntegerv(GL_MAX_3D_TEXTURE_SIZE, &TextureData.MAX_3D_TEXTURE_SIZE);
		glGetIntegerv(GL_MAX_ARRAY_TEXTURE_LAYERS, &TextureData.MAX_ARRAY_TEXTURE_LAYERS);
		glGetIntegerv(GL_MAX_CUBE_MAP_TEXTURE_SIZE, &TextureData.MAX_CUBE_MAP_TEXTURE_SIZE);
		glGetIntegerv(GL_MAX_COLOR_TEXTURE_SAMPLES, &TextureData.MAX_COLOR_TEXTURE_SAMPLES);
		glGetIntegerv(GL_MAX_DEPTH_TEXTURE_SAMPLES, &TextureData.MAX_DEPTH_TEXTURE_SAMPLES);
		glGetIntegerv(GL_MAX_INTEGER_SAMPLES, &TextureData.MAX_INTEGER_SAMPLES);
		glGetIntegerv(GL_MAX_TEXTURE_BUFFER_SIZE, &TextureData.MAX_TEXTURE_BUFFER_SIZE);
		glGetIntegerv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &TextureData.MAX_TEXTURE_MAX_ANISOTROPY_EXT);
		glGetIntegerv(GL_NUM_COMPRESSED_TEXTURE_FORMATS, &TextureData.NUM_COMPRESSED_TEXTURE_FORMATS);

		std::vector<GLint> Formats;
		Formats.resize(static_cast<std::size_t>(TextureData.NUM_COMPRESSED_TEXTURE_FORMATS));
		glGetIntegerv(GL_COMPRESSED_TEXTURE_FORMATS, &Formats[0]);
		for(std::size_t i = 0; i < Formats.size(); ++i)
		{
			switch(Formats[i])
			{
			case GL_COMPRESSED_RGB_S3TC_DXT1_EXT:
				TextureData.COMPRESSED_RGB_S3TC_DXT1_EXT = true;
				break;
			case GL_COMPRESSED_RGBA_S3TC_DXT1_EXT:
				TextureData.COMPRESSED_RGBA_S3TC_DXT1_EXT = true;
				break;
			case GL_COMPRESSED_RGBA_S3TC_DXT3_EXT:
				TextureData.COMPRESSED_RGBA_S3TC_DXT3_EXT = true;
				break;
			case GL_COMPRESSED_RGBA_S3TC_DXT5_EXT:
				TextureData.COMPRESSED_RGBA_S3TC_DXT5_EXT = true;
				break;
			case GL_COMPRESSED_RED_RGTC1:
				TextureData.COMPRESSED_RED_RGTC1 = true;
				break;
			case GL_COMPRESSED_SIGNED_RED_RGTC1:
				TextureData.COMPRESSED_SIGNED_RED_RGTC1 = true;
				break;
			case GL_COMPRESSED_RG_RGTC2:
				TextureData.COMPRESSED_RG_RGTC2 = true;
				break;
			case GL_COMPRESSED_SIGNED_RG_RGTC2:
				TextureData.COMPRESSED_SIGNED_RG_RGTC2 = true;
				break;
			case GL_COMPRESSED_RGBA_BPTC_UNORM:
				TextureData.COMPRESSED_RGBA_BPTC_UNORM = true;
				break;
			case GL_COMPRESSED_SRGB_ALPHA_BPTC_UNORM:
				TextureData.COMPRESSED_SRGB_ALPHA_BPTC_UNORM = true;
				break;
			case GL_COMPRESSED_RGB_BPTC_SIGNED_FLOAT:
				TextureData.COMPRESSED_RGB_BPTC_SIGNED_FLOAT = true;
				break;
			case GL_COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT:
				TextureData.COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT = true;
				break;
			case GL_COMPRESSED_RGBA_ASTC_4x4_KHR:
				TextureData.COMPRESSED_RGBA_ASTC_4x4_KHR = true;
				break;
			case GL_COMPRESSED_RGBA_ASTC_5x4_KHR:
				TextureData.COMPRESSED_RGBA_ASTC_5x4_KHR = true;
				break;
			case GL_COMPRESSED_RGBA_ASTC_5x5_KHR:
				TextureData.COMPRESSED_RGBA_ASTC_5x5_KHR = true;
				break;
			case GL_COMPRESSED_RGBA_ASTC_6x5_KHR:
				TextureData.COMPRESSED_RGBA_ASTC_6x5_KHR = true;
				break;
			case GL_COMPRESSED_RGBA_ASTC_6x6_KHR:
				TextureData.COMPRESSED_RGBA_ASTC_6x6_KHR = true;
				break;
			case GL_COMPRESSED_RGBA_ASTC_8x5_KHR:
				TextureData.COMPRESSED_RGBA_ASTC_8x5_KHR = true;
				break;
			case GL_COMPRESSED_RGBA_ASTC_8x6_KHR:
				TextureData.COMPRESSED_RGBA_ASTC_8x6_KHR = true;
				break;
			case GL_COMPRESSED_RGBA_ASTC_8x8_KHR:
				TextureData.COMPRESSED_RGBA_ASTC_8x8_KHR = true;
				break;
			case GL_COMPRESSED_RGBA_ASTC_10x5_KHR:
				TextureData.COMPRESSED_RGBA_ASTC_10x5_KHR = true;
				break;
			case GL_COMPRESSED_RGBA_ASTC_10x6_KHR:
				TextureData.COMPRESSED_RGBA_ASTC_10x6_KHR = true;
				break;
			case GL_COMPRESSED_RGBA_ASTC_10x8_KHR:
				TextureData.COMPRESSED_RGBA_ASTC_10x8_KHR = true;
				break;
			case GL_COMPRESSED_RGBA_ASTC_10x10_KHR:
				TextureData.COMPRESSED_RGBA_ASTC_10x10_KHR = true;
				break;
			case GL_COMPRESSED_RGBA_ASTC_12x10_KHR:
				TextureData.COMPRESSED_RGBA_ASTC_12x10_KHR = true;
				break;
			case GL_COMPRESSED_RGBA_ASTC_12x12_KHR:
				TextureData.COMPRESSED_RGBA_ASTC_12x12_KHR = true;
				break;
			case GL_COMPRESSED_RGB8_ETC2:
				TextureData.COMPRESSED_RGB8_ETC2 = true;
				break;
			case GL_COMPRESSED_SRGB8_ETC2:
				TextureData.COMPRESSED_SRGB8_ETC2 = true;
				break;
			case GL_COMPRESSED_RGB8_PUNCHTHROUGH_ALPHA1_ETC2:
				TextureData.COMPRESSED_RGB8_PUNCHTHROUGH_ALPHA1_ETC2 = true;
				break;
			case GL_COMPRESSED_SRGB8_PUNCHTHROUGH_ALPHA1_ETC2:
				TextureData.COMPRESSED_SRGB8_PUNCHTHROUGH_ALPHA1_ETC2 = true;
				break;
			case GL_COMPRESSED_RGBA8_ETC2_EAC:
				TextureData.COMPRESSED_RGBA8_ETC2_EAC = true;
				break;
			case GL_COMPRESSED_SRGB8_ALPHA8_ETC2_EAC:
				TextureData.COMPRESSED_SRGB8_ALPHA8_ETC2_EAC = true;
				break;
			case GL_COMPRESSED_R11_EAC:
				TextureData.COMPRESSED_R11_EAC = true;
				break;
			case GL_COMPRESSED_SIGNED_R11_EAC:
				TextureData.COMPRESSED_SIGNED_R11_EAC = true;
				break;
			case GL_COMPRESSED_RG11_EAC :
				TextureData.COMPRESSED_RG11_EAC  = true;
				break;
			case GL_COMPRESSED_SIGNED_RG11_EAC:
				TextureData.COMPRESSED_SIGNED_RG11_EAC = true;
				break;
			case GL_PALETTE4_RGB8_OES:
				TextureData.PALETTE4_RGB8_OES = true;
				break;
			case GL_PALETTE4_RGBA8_OES:
				TextureData.PALETTE4_RGBA8_OES = true;
				break;
			case GL_PALETTE4_R5_G6_B5_OES:
				TextureData.PALETTE4_R5_G6_B5_OES = true;
				break;
			case GL_PALETTE4_RGBA4_OES:
				TextureData.PALETTE4_RGBA4_OES = true;
				break;
			case GL_PALETTE4_RGB5_A1_OES:
				TextureData.PALETTE4_RGB5_A1_OES = true;
				break;
			case GL_PALETTE8_RGB8_OES:
				TextureData.PALETTE8_RGB8_OES = true;
				break;
			case GL_PALETTE8_RGBA8_OES:
				TextureData.PALETTE8_RGBA8_OES = true;
				break;
			case GL_PALETTE8_R5_G6_B5_OES:
				TextureData.PALETTE8_R5_G6_B5_OES = true;
				break;
			case GL_PALETTE8_RGBA4_OES:
				TextureData.PALETTE8_RGBA4_OES = true;
				break;
			case GL_PALETTE8_RGB5_A1_OES:
				TextureData.PALETTE8_RGB5_A1_OES = true;
				break;
			case GL_ETC1_RGB8_OES:
				TextureData.ETC1_RGB8_OES = true;
				break;
			default:
				//assert(0);
				break;
			}
		}
	}

	struct program
	{
		GLint MAX_COMBINED_ATOMIC_COUNTERS;
		GLint MAX_COMBINED_SHADER_STORAGE_BLOCKS;
		GLint MAX_PROGRAM_TEXEL_OFFSET;
		GLint MIN_PROGRAM_TEXEL_OFFSET;
		GLint MAX_COMBINED_UNIFORM_BLOCKS;
		GLint MAX_UNIFORM_BUFFER_BINDINGS;
		GLint MAX_UNIFORM_BLOCK_SIZE;
		GLint MAX_UNIFORM_LOCATIONS;
		GLint MAX_VARYING_COMPONENTS;
		GLint MAX_VARYING_VECTORS;
		GLint MAX_VARYING_FLOATS;
		GLint MAX_SHADER_STORAGE_BUFFER_BINDINGS;
		GLint MAX_SHADER_STORAGE_BLOCK_SIZE;
		GLint MAX_COMBINED_SHADER_OUTPUT_RESOURCES;
		GLint SHADER_STORAGE_BUFFER_OFFSET_ALIGNMENT;
		GLint UNIFORM_BUFFER_OFFSET_ALIGNMENT;
		GLint NUM_PROGRAM_BINARY_FORMATS;
		GLint NUM_SHADER_BINARY_FORMATS;
		GLint PROGRAM_BINARY_FORMATS;
	} ProgramData;

	void initProgram()
	{
		glGetIntegerv(GL_MAX_COMBINED_ATOMIC_COUNTERS, &ProgramData.MAX_COMBINED_ATOMIC_COUNTERS);
		glGetIntegerv(GL_MAX_COMBINED_SHADER_STORAGE_BLOCKS, &ProgramData.MAX_COMBINED_SHADER_STORAGE_BLOCKS);
		glGetIntegerv(GL_MAX_PROGRAM_TEXEL_OFFSET, &ProgramData.MAX_PROGRAM_TEXEL_OFFSET);
		glGetIntegerv(GL_MIN_PROGRAM_TEXEL_OFFSET, &ProgramData.MIN_PROGRAM_TEXEL_OFFSET);
		glGetIntegerv(GL_MAX_COMBINED_UNIFORM_BLOCKS, &ProgramData.MAX_COMBINED_UNIFORM_BLOCKS);
		glGetIntegerv(GL_MAX_UNIFORM_BUFFER_BINDINGS, &ProgramData.MAX_UNIFORM_BUFFER_BINDINGS);
		glGetIntegerv(GL_MAX_UNIFORM_BLOCK_SIZE, &ProgramData.MAX_UNIFORM_BLOCK_SIZE);
		glGetIntegerv(GL_MAX_UNIFORM_LOCATIONS, &ProgramData.MAX_UNIFORM_LOCATIONS);
		glGetIntegerv(GL_MAX_VARYING_COMPONENTS, &ProgramData.MAX_VARYING_COMPONENTS);
		glGetIntegerv(GL_MAX_VARYING_VECTORS, &ProgramData.MAX_VARYING_VECTORS);
		glGetIntegerv(GL_MAX_VARYING_FLOATS, &ProgramData.MAX_VARYING_FLOATS);
		glGetIntegerv(GL_MAX_SHADER_STORAGE_BUFFER_BINDINGS, &ProgramData.MAX_SHADER_STORAGE_BUFFER_BINDINGS);
		glGetIntegerv(GL_MAX_SHADER_STORAGE_BLOCK_SIZE, &ProgramData.MAX_SHADER_STORAGE_BLOCK_SIZE);
		glGetIntegerv(GL_MAX_COMBINED_SHADER_OUTPUT_RESOURCES, &ProgramData.MAX_COMBINED_SHADER_OUTPUT_RESOURCES);
		glGetIntegerv(GL_SHADER_STORAGE_BUFFER_OFFSET_ALIGNMENT, &ProgramData.SHADER_STORAGE_BUFFER_OFFSET_ALIGNMENT);
		glGetIntegerv(GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT, &ProgramData.SHADER_STORAGE_BUFFER_OFFSET_ALIGNMENT);
		glGetIntegerv(GL_NUM_PROGRAM_BINARY_FORMATS, &ProgramData.NUM_PROGRAM_BINARY_FORMATS);
		glGetIntegerv(GL_NUM_SHADER_BINARY_FORMATS, &ProgramData.NUM_SHADER_BINARY_FORMATS);
		glGetIntegerv(GL_PROGRAM_BINARY_FORMATS, &ProgramData.PROGRAM_BINARY_FORMATS);
	}

public:
	caps(profile const & Profile) :
		VersionData(Profile),
		Version(VersionData),
		Debug(DebugData),
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

bool initDebugOutput()
{
#	ifdef GL_ARB_debug_output
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS_ARB);
		glDebugMessageControlARB(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE);
		glDebugMessageCallbackARB(&glf::debugOutput, NULL);
#	endif

	return true;
}

bool initProgram()
{
	bool Validated(true);
	
	glf::compiler Compiler;

	if(Validated)
	{
		GLuint VertShaderName = Compiler.create(GL_VERTEX_SHADER, glf::DATA_DIRECTORY + VERT_SHADER_SOURCE, "--version 150 --profile core");
		GLuint FragShaderName = Compiler.create(GL_FRAGMENT_SHADER, glf::DATA_DIRECTORY + FRAG_SHADER_SOURCE, "--version 150 --profile core");
		Validated = Validated && Compiler.check();

		ProgramName = glCreateProgram();
		glAttachShader(ProgramName, VertShaderName);
		glAttachShader(ProgramName, FragShaderName);
		glBindAttribLocation(ProgramName, glf::semantic::attr::POSITION, "Position");
		glBindAttribLocation(ProgramName, glf::semantic::attr::TEXCOORD, "Texcoord");
		glBindFragDataLocation(ProgramName, glf::semantic::frag::COLOR, "Color");
		glLinkProgram(ProgramName);
		glDeleteShader(VertShaderName);
		glDeleteShader(FragShaderName);
		Validated = Validated && glf::checkProgram(ProgramName);
	}

	if(Validated)
	{
		UniformTransform = glGetUniformBlockIndex(ProgramName, "transform");
		UniformDiffuse = glGetUniformLocation(ProgramName, "Diffuse");
	}
	
	Validated = Validated && glf::checkError("initProgram");
	
	return Validated;
}

bool initBuffer()
{
	glGenBuffers(buffer::MAX, &BufferName[0]);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, BufferName[buffer::ELEMENT]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, ElementSize, ElementData, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ARRAY_BUFFER, BufferName[buffer::VERTEX]);
	glBufferData(GL_ARRAY_BUFFER, VertexSize, VertexData, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	GLint UniformBufferOffset(0);

	glGetIntegerv(
		GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT,
		&UniformBufferOffset);

	GLint UniformBlockSize = glm::max(GLint(sizeof(glm::mat4)), UniformBufferOffset);

	glBindBuffer(GL_UNIFORM_BUFFER, BufferName[buffer::TRANSFORM]);
	glBufferData(GL_UNIFORM_BUFFER, UniformBlockSize, NULL, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	return glf::checkError("initBuffer");
}

bool initTexture()
{
	gli::texture2D Texture(gli::loadStorageDDS(glf::DATA_DIRECTORY + TEXTURE_DIFFUSE));
	assert(!Texture.empty());

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glGenTextures(1, &TextureName);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, TextureName);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, GLint(Texture.levels() - 1));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	for(gli::texture2D::size_type Level = 0; Level < Texture.levels(); ++Level)
	{
		glTexImage2D(
			GL_TEXTURE_2D,
			GLint(Level),
			GL_RGBA8,
			GLsizei(Texture[Level].dimensions().x),
			GLsizei(Texture[Level].dimensions().y),
			0,
			GL_BGR,
			GL_UNSIGNED_BYTE,
			Texture[Level].data());
	}

	if(Texture.levels() == 1)
		glGenerateMipmap(GL_TEXTURE_2D);
	
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

	return glf::checkError("initTexture");
}

bool initVertexArray()
{
	glf::checkError("initVertexArray 0");
	
	glGenVertexArrays(1, &VertexArrayName);
	glBindVertexArray(VertexArrayName);
		glBindBuffer(GL_ARRAY_BUFFER, BufferName[buffer::VERTEX]);
		glVertexAttribPointer(glf::semantic::attr::POSITION, 2, GL_FLOAT, GL_FALSE, sizeof(glf::vertex_v2fv2f), GLF_BUFFER_OFFSET(0));
		glVertexAttribPointer(glf::semantic::attr::TEXCOORD, 2, GL_FLOAT, GL_FALSE, sizeof(glf::vertex_v2fv2f), GLF_BUFFER_OFFSET(sizeof(glm::vec2)));
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glEnableVertexAttribArray(glf::semantic::attr::POSITION);
		glEnableVertexAttribArray(glf::semantic::attr::TEXCOORD);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, BufferName[buffer::ELEMENT]);
	glBindVertexArray(0);

	return glf::checkError("initVertexArray");
}

bool begin()
{
	bool Validated = glf::checkGLVersion(SAMPLE_MAJOR_VERSION, SAMPLE_MINOR_VERSION);

	caps Caps(caps::CORE);

	if(Validated && glf::checkExtension("GL_ARB_debug_output"))
		Validated = initDebugOutput();
	if(Validated)
		Validated = initBuffer();
	if(Validated)
		Validated = initTexture();
	if(Validated)
		Validated = initProgram();
	if(Validated)
		Validated = initVertexArray();

	return Validated && glf::checkError("begin");
}

bool end()
{
	glDeleteBuffers(buffer::MAX, &BufferName[0]);
	glDeleteProgram(ProgramName);
	glDeleteTextures(1, &TextureName);
	glDeleteVertexArrays(1, &VertexArrayName);

	return glf::checkError("end");
}

void display()
{
	// Update of the uniform buffer
	{
		glBindBuffer(GL_UNIFORM_BUFFER, BufferName[buffer::TRANSFORM]);
		glm::mat4* Pointer = (glm::mat4*)glMapBufferRange(
			GL_UNIFORM_BUFFER, 0,	sizeof(glm::mat4),
			GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);

		glm::mat4 Projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
		glm::mat4 ViewTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -Window.TranlationCurrent.y));
		glm::mat4 ViewRotateX = glm::rotate(ViewTranslate, Window.RotationCurrent.y, glm::vec3(1.f, 0.f, 0.f));
		glm::mat4 View = glm::rotate(ViewRotateX, Window.RotationCurrent.x, glm::vec3(0.f, 1.f, 0.f));
		glm::mat4 Model = glm::mat4(1.0f);
		
		*Pointer = Projection * View * Model;

		// Make sure the uniform buffer is uploaded
		glUnmapBuffer(GL_UNIFORM_BUFFER);
	}

	glViewport(0, 0, Window.Size.x, Window.Size.y);
	glClearBufferfv(GL_COLOR, 0, &glm::vec4(1.0f, 0.5f, 0.0f, 1.0f)[0]);

	glUseProgram(ProgramName);
	glUniform1i(UniformDiffuse, 0);
	glUniformBlockBinding(ProgramName, UniformTransform, glf::semantic::uniform::TRANSFORM0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, TextureName);
	glBindBufferBase(GL_UNIFORM_BUFFER, glf::semantic::uniform::TRANSFORM0, BufferName[buffer::TRANSFORM]);
	glBindVertexArray(VertexArrayName);

	glDrawElementsInstancedBaseVertex(
		GL_TRIANGLES, ElementCount, GL_UNSIGNED_SHORT, 0, 1, 0);

	glf::checkError("display");
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
