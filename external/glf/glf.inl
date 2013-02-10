#include <GL/glfw.h>

bool check();
bool begin();
bool end();
void display();

namespace glf
{
	inline void logImplementationDependentLimit(GLenum Value, std::string const & String)
	{
		GLint Result(0);
		glGetIntegerv(Value, &Result);
		std::string Message(glf::format("%s: %d", String.c_str(), Result));
#		if(!defined(__APPLE__))
			glDebugMessageInsertARB(GL_DEBUG_SOURCE_APPLICATION_ARB, GL_DEBUG_TYPE_OTHER_ARB, 1, GL_DEBUG_SEVERITY_LOW_ARB, GLsizei(Message.size()), Message.c_str());
#		endif
	}

	inline void swapBuffers()
	{
		glfwSwapBuffers();
		//assert(glGetError() == GL_NO_ERROR); // 'glutSwapBuffers' generates an here with OpenGL 3 > core profile ... :/
	}

	inline bool checkGLVersion(GLint MajorVersionRequire, GLint MinorVersionRequire)
	{
		GLint MajorVersionContext = 0;
		GLint MinorVersionContext = 0;
		glGetIntegerv(GL_MAJOR_VERSION, &MajorVersionContext);
		glGetIntegerv(GL_MINOR_VERSION, &MinorVersionContext);
		return glf::version(MajorVersionContext, MinorVersionContext) 
			>= glf::version(MajorVersionRequire, MinorVersionRequire);
	}

	inline bool checkExtension(char const * String)
	{
		GLint ExtensionCount = 0;
		glGetIntegerv(GL_NUM_EXTENSIONS, &ExtensionCount);
		for(GLint i = 0; i < ExtensionCount; ++i)
			if(std::string((char const*)glGetStringi(GL_EXTENSIONS, i)) == std::string(String))
				return true;
		return false;
	}

	inline int version(int Major, int Minor)
	{
		return Major * 100 + Minor * 10;
	}

	inline void init()
	{
#if (defined(WIN32))
		// OpenGL 1.0
		// glEnable = (PFNGLENABLEPROC)glfwGetProcAddress("glEnable");
		// glViewport = (PFNGLVIEWPORTPROC)glfwGetProcAddress("glViewport");
		
		// OpenGL 1.1
		// glGetIntegerv = (PFNGLGETINTEGERVPROC)glfwGetProcAddress("glGetIntegerv");
		// glGetError = (PFNGLGETERRORPROC)glfwGetProcAddress("glGetError");

		// OpenGL 1.2
		glBlendColor = (PFNGLBLENDCOLORPROC)glfwGetProcAddress("glBlendColor");
		glBlendEquation = (PFNGLBLENDEQUATIONPROC)glfwGetProcAddress("glBlendEquation");
		glDrawRangeElements = (PFNGLDRAWRANGEELEMENTSPROC)glfwGetProcAddress("glDrawRangeElements");
		glTexImage3D = (PFNGLTEXIMAGE3DPROC)glfwGetProcAddress("glTexImage3D");
		glTexSubImage3D = (PFNGLTEXSUBIMAGE3DPROC)glfwGetProcAddress("glTexSubImage3D");
		glCopyTexSubImage3D = (PFNGLCOPYTEXSUBIMAGE3DPROC)glfwGetProcAddress("glCopyTexSubImage3D");

		// OpenGL 1.3
		glActiveTexture = (PFNGLACTIVETEXTUREPROC)glfwGetProcAddress("glActiveTexture");
		glSampleCoverage = (PFNGLSAMPLECOVERAGEPROC)glfwGetProcAddress("glSampleCoverage");
		glCompressedTexImage3D = (PFNGLCOMPRESSEDTEXIMAGE3DPROC)glfwGetProcAddress("glCompressedTexImage3D");
		glCompressedTexImage2D = (PFNGLCOMPRESSEDTEXIMAGE2DPROC)glfwGetProcAddress("glCompressedTexImage2D");
		glCompressedTexImage1D = (PFNGLCOMPRESSEDTEXIMAGE1DPROC)glfwGetProcAddress("glCompressedTexImage1D");
		glCompressedTexSubImage3D = (PFNGLCOMPRESSEDTEXSUBIMAGE3DPROC)glfwGetProcAddress("glCompressedTexSubImage3D");
		glCompressedTexSubImage2D = (PFNGLCOMPRESSEDTEXSUBIMAGE2DPROC)glfwGetProcAddress("glCompressedTexSubImage2D");
		glCompressedTexSubImage1D = (PFNGLCOMPRESSEDTEXSUBIMAGE1DPROC)glfwGetProcAddress("glCompressedTexSubImage1D");
		glGetCompressedTexImage = (PFNGLGETCOMPRESSEDTEXIMAGEPROC)glfwGetProcAddress("glGetCompressedTexImage");

		// OpenGL 1.4
		glBlendFuncSeparate = (PFNGLBLENDFUNCSEPARATEPROC)glfwGetProcAddress("glBlendFuncSeparate");
		glPointParameterf = (PFNGLPOINTPARAMETERFPROC)glfwGetProcAddress("glPointParameterf");
		glPointParameterfv = (PFNGLPOINTPARAMETERFVPROC)glfwGetProcAddress("glPointParameterfv");
		glPointParameteri = (PFNGLPOINTPARAMETERIPROC)glfwGetProcAddress("glPointParameteri");
		glPointParameteriv = (PFNGLPOINTPARAMETERIVPROC)glfwGetProcAddress("glPointParameteriv");

		// GL_ARB_vertex_buffer_object
		glBindBuffer = (PFNGLBINDBUFFERPROC)glfwGetProcAddress("glBindBuffer");
		glDeleteBuffers = (PFNGLDELETEBUFFERSPROC)glfwGetProcAddress("glDeleteBuffers");
		glGenBuffers = (PFNGLGENBUFFERSPROC)glfwGetProcAddress("glGenBuffers");
		glIsBuffer = (PFNGLISBUFFERPROC)glfwGetProcAddress("glIsBuffer");
		glBufferData = (PFNGLBUFFERDATAPROC)glfwGetProcAddress("glBufferData");
		glBufferSubData = (PFNGLBUFFERSUBDATAPROC)glfwGetProcAddress("glBufferSubData");
		glGetBufferSubData = (PFNGLGETBUFFERSUBDATAPROC)glfwGetProcAddress("glGetBufferSubData");
		glUnmapBuffer = (PFNGLUNMAPBUFFERPROC)glfwGetProcAddress("glUnmapBuffer");
		glGetBufferParameteriv = (PFNGLGETBUFFERPARAMETERIVPROC)glfwGetProcAddress("glGetBufferParameteriv");
		glGetBufferPointerv = (PFNGLGETBUFFERPOINTERVPROC)glfwGetProcAddress("glGetBufferPointerv");

		// OpenGL 2.0
		glBlendEquationSeparate = (PFNGLBLENDEQUATIONSEPARATEPROC)glfwGetProcAddress("glBlendEquationSeparate");
		glDrawBuffers = (PFNGLDRAWBUFFERSPROC)glfwGetProcAddress("glDrawBuffers");
		glStencilOpSeparate = (PFNGLSTENCILOPSEPARATEPROC)glfwGetProcAddress("glStencilOpSeparate");
		glStencilFuncSeparate = (PFNGLSTENCILFUNCSEPARATEPROC)glfwGetProcAddress("glStencilFuncSeparate");
		glStencilMaskSeparate = (PFNGLSTENCILMASKSEPARATEPROC)glfwGetProcAddress("glStencilMaskSeparate");

		glCreateProgram = (PFNGLCREATEPROGRAMPROC)glfwGetProcAddress("glCreateProgram");
		glCreateShader = (PFNGLCREATESHADERPROC)glfwGetProcAddress("glCreateShader");
		glDeleteProgram = (PFNGLDELETEPROGRAMPROC)glfwGetProcAddress("glDeleteProgram");
		glDeleteShader = (PFNGLDELETESHADERPROC)glfwGetProcAddress("glDeleteShader");
		glShaderSource = (PFNGLSHADERSOURCEPROC)glfwGetProcAddress("glShaderSource");
		glCompileShader = (PFNGLCOMPILESHADERPROC)glfwGetProcAddress("glCompileShader");
		glGetShaderiv = (PFNGLGETSHADERIVPROC)glfwGetProcAddress("glGetShaderiv");
		glValidateProgram = (PFNGLVALIDATEPROGRAMPROC)glfwGetProcAddress("glValidateProgram");
		glGetProgramiv = (PFNGLGETPROGRAMIVPROC)glfwGetProcAddress("glGetProgramiv");
		glGetProgramInfoLog = (PFNGLGETPROGRAMINFOLOGPROC)glfwGetProcAddress("glGetProgramInfoLog");
		glGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC)glfwGetProcAddress("glGetShaderInfoLog");
		glAttachShader = (PFNGLATTACHSHADERPROC)glfwGetProcAddress("glAttachShader");
		glLinkProgram = (PFNGLLINKPROGRAMPROC)glfwGetProcAddress("glLinkProgram");
		glUseProgram = (PFNGLUSEPROGRAMPROC)glfwGetProcAddress("glUseProgram");
		glGetVertexAttribdv = (PFNGLGETVERTEXATTRIBDVPROC)glfwGetProcAddress("glGetVertexAttribdv");
		glGetVertexAttribfv = (PFNGLGETVERTEXATTRIBFVPROC)glfwGetProcAddress("glGetVertexAttribfv");
		glGetVertexAttribiv = (PFNGLGETVERTEXATTRIBIVPROC)glfwGetProcAddress("glGetVertexAttribiv");
		glGetVertexAttribPointerv = (PFNGLGETVERTEXATTRIBPOINTERVPROC)glfwGetProcAddress("glGetVertexAttribPointerv");
		glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC)glfwGetProcAddress("glVertexAttribPointer");
		glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC)glfwGetProcAddress("glEnableVertexAttribArray");
		glBindAttribLocation = (PFNGLBINDATTRIBLOCATIONPROC)glfwGetProcAddress("glBindAttribLocation");
		glGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC)glfwGetProcAddress("glGetUniformLocation");
		glUniform1f = (PFNGLUNIFORM1FPROC)glfwGetProcAddress("glUniform1f");
		glUniform2f = (PFNGLUNIFORM2FPROC)glfwGetProcAddress("glUniform2f");
		glUniform3f = (PFNGLUNIFORM3FPROC)glfwGetProcAddress("glUniform3f");
		glUniform4f = (PFNGLUNIFORM4FPROC)glfwGetProcAddress("glUniform4f");
		glUniform1i = (PFNGLUNIFORM1IPROC)glfwGetProcAddress("glUniform1i");
		glUniform2i = (PFNGLUNIFORM2IPROC)glfwGetProcAddress("glUniform2i");
		glUniform3i = (PFNGLUNIFORM3IPROC)glfwGetProcAddress("glUniform3i");
		glUniform4i = (PFNGLUNIFORM4IPROC)glfwGetProcAddress("glUniform4i");
		glUniform1fv = (PFNGLUNIFORM1FVPROC)glfwGetProcAddress("glUniform1fv");
		glUniform2fv = (PFNGLUNIFORM2FVPROC)glfwGetProcAddress("glUniform2fv");
		glUniform3fv = (PFNGLUNIFORM3FVPROC)glfwGetProcAddress("glUniform3fv");
		glUniform4fv = (PFNGLUNIFORM4FVPROC)glfwGetProcAddress("glUniform4fv");
		glUniform1iv = (PFNGLUNIFORM1IVPROC)glfwGetProcAddress("glUniform1iv");
		glUniform2iv = (PFNGLUNIFORM2IVPROC)glfwGetProcAddress("glUniform2iv");
		glUniform3iv = (PFNGLUNIFORM3IVPROC)glfwGetProcAddress("glUniform3iv");
		glUniform4iv = (PFNGLUNIFORM4IVPROC)glfwGetProcAddress("glUniform4iv");
		glUniformMatrix2fv = (PFNGLUNIFORMMATRIX2FVPROC)glfwGetProcAddress("glUniformMatrix2fv");
		glUniformMatrix3fv = (PFNGLUNIFORMMATRIX3FVPROC)glfwGetProcAddress("glUniformMatrix3fv");
		glUniformMatrix4fv = (PFNGLUNIFORMMATRIX4FVPROC)glfwGetProcAddress("glUniformMatrix4fv");

		// OpenGL 3.0
		glColorMaski = (PFNGLCOLORMASKIPROC)glfwGetProcAddress("glColorMaski");
		glGetBooleani_v = (PFNGLGETBOOLEANI_VPROC)glfwGetProcAddress("glGetBooleani_v");
		glGetIntegeri_v = (PFNGLGETINTEGERI_VPROC)glfwGetProcAddress("glGetIntegeri_v");
		glEnablei = (PFNGLENABLEIPROC)glfwGetProcAddress("glEnablei");
		glDisablei = (PFNGLDISABLEIPROC)glfwGetProcAddress("glDisablei");
		glIsEnabledi = (PFNGLISENABLEDIPROC)glfwGetProcAddress("glIsEnabledi");
		glBeginTransformFeedback = (PFNGLBEGINTRANSFORMFEEDBACKPROC)glfwGetProcAddress("glBeginTransformFeedback");
		glEndTransformFeedback = (PFNGLENDTRANSFORMFEEDBACKPROC)glfwGetProcAddress("glEndTransformFeedback");
		glBindBufferRange = (PFNGLBINDBUFFERRANGEPROC)glfwGetProcAddress("glBindBufferRange");
		glBindBufferBase = (PFNGLBINDBUFFERBASEPROC)glfwGetProcAddress("glBindBufferBase");
		glTransformFeedbackVaryings = (PFNGLTRANSFORMFEEDBACKVARYINGSPROC)glfwGetProcAddress("glTransformFeedbackVaryings");
		glGetTransformFeedbackVarying = (PFNGLGETTRANSFORMFEEDBACKVARYINGPROC)glfwGetProcAddress("glGetTransformFeedbackVarying");
		glClampColor = (PFNGLCLAMPCOLORPROC)glfwGetProcAddress("glClampColor");
		glBeginConditionalRender = (PFNGLBEGINCONDITIONALRENDERPROC)glfwGetProcAddress("glBeginConditionalRender");
		glEndConditionalRender = (PFNGLENDCONDITIONALRENDERPROC)glfwGetProcAddress("glEndConditionalRender");
		glVertexAttribIPointer = (PFNGLVERTEXATTRIBIPOINTERPROC)glfwGetProcAddress("glVertexAttribIPointer");
		glGetVertexAttribIiv = (PFNGLGETVERTEXATTRIBIIVPROC)glfwGetProcAddress("glGetVertexAttribIiv");
		glGetVertexAttribIuiv = (PFNGLGETVERTEXATTRIBIUIVPROC)glfwGetProcAddress("glGetVertexAttribIuiv");
		glVertexAttribI1i = (PFNGLVERTEXATTRIBI1IPROC)glfwGetProcAddress("glVertexAttribI1i");
		glVertexAttribI2i = (PFNGLVERTEXATTRIBI2IPROC)glfwGetProcAddress("glVertexAttribI2i");
		glVertexAttribI3i = (PFNGLVERTEXATTRIBI3IPROC)glfwGetProcAddress("glVertexAttribI3i");
		glVertexAttribI4i = (PFNGLVERTEXATTRIBI4IPROC)glfwGetProcAddress("glVertexAttribI4i");
		glVertexAttribI1ui = (PFNGLVERTEXATTRIBI1UIPROC)glfwGetProcAddress("glVertexAttribI1ui");
		glVertexAttribI2ui = (PFNGLVERTEXATTRIBI2UIPROC)glfwGetProcAddress("glVertexAttribI2ui");
		glVertexAttribI3ui = (PFNGLVERTEXATTRIBI3UIPROC)glfwGetProcAddress("glVertexAttribI3ui");
		glVertexAttribI4ui = (PFNGLVERTEXATTRIBI4UIPROC)glfwGetProcAddress("glVertexAttribI4ui");
		glVertexAttribI1iv = (PFNGLVERTEXATTRIBI1IVPROC)glfwGetProcAddress("glVertexAttribI1iv");
		glVertexAttribI2iv = (PFNGLVERTEXATTRIBI2IVPROC)glfwGetProcAddress("glVertexAttribI2iv");
		glVertexAttribI3iv = (PFNGLVERTEXATTRIBI3IVPROC)glfwGetProcAddress("glVertexAttribI3iv");
		glVertexAttribI4iv = (PFNGLVERTEXATTRIBI4IVPROC)glfwGetProcAddress("glVertexAttribI4iv");

		glVertexAttribI1uiv = (PFNGLVERTEXATTRIBI1UIVPROC)glfwGetProcAddress("glVertexAttribI1uiv");
		glVertexAttribI2uiv = (PFNGLVERTEXATTRIBI2UIVPROC)glfwGetProcAddress("glVertexAttribI2uiv");
		glVertexAttribI3uiv = (PFNGLVERTEXATTRIBI3UIVPROC)glfwGetProcAddress("glVertexAttribI3uiv");
		glVertexAttribI4uiv = (PFNGLVERTEXATTRIBI4UIVPROC)glfwGetProcAddress("glVertexAttribI4uiv");
		glVertexAttribI4bv = (PFNGLVERTEXATTRIBI4BVPROC)glfwGetProcAddress("glVertexAttribI4bv");
		glVertexAttribI4sv = (PFNGLVERTEXATTRIBI4SVPROC)glfwGetProcAddress("glVertexAttribI4sv");
		glVertexAttribI4ubv = (PFNGLVERTEXATTRIBI4UBVPROC)glfwGetProcAddress("glVertexAttribI4ubv");
		glVertexAttribI4usv = (PFNGLVERTEXATTRIBI4USVPROC)glfwGetProcAddress("glVertexAttribI4usv");
		glGetUniformuiv = (PFNGLGETUNIFORMUIVPROC)glfwGetProcAddress("glGetUniformuiv");
		glBindFragDataLocation = (PFNGLBINDFRAGDATALOCATIONPROC)glfwGetProcAddress("glBindFragDataLocation");
		glGetFragDataLocation = (PFNGLGETFRAGDATALOCATIONPROC)glfwGetProcAddress("glGetFragDataLocation");
		glUniform1ui = (PFNGLUNIFORM1UIPROC)glfwGetProcAddress("glUniform1ui");
		glUniform2ui = (PFNGLUNIFORM2UIPROC)glfwGetProcAddress("glUniform2ui");
		glUniform3ui = (PFNGLUNIFORM3UIPROC)glfwGetProcAddress("glUniform3ui");
		glUniform4ui = (PFNGLUNIFORM4UIPROC)glfwGetProcAddress("glUniform4ui");
		glUniform1uiv = (PFNGLUNIFORM1UIVPROC)glfwGetProcAddress("glUniform1uiv");
		glUniform2uiv = (PFNGLUNIFORM2UIVPROC)glfwGetProcAddress("glUniform2uiv");
		glUniform3uiv = (PFNGLUNIFORM3UIVPROC)glfwGetProcAddress("glUniform3uiv");
		glUniform4uiv = (PFNGLUNIFORM4UIVPROC)glfwGetProcAddress("glUniform4uiv");
		glTexParameterIiv = (PFNGLTEXPARAMETERIIVPROC)glfwGetProcAddress("glTexParameterIiv");
		glTexParameterIuiv = (PFNGLTEXPARAMETERIUIVPROC)glfwGetProcAddress("glTexParameterIuiv");
		glGetTexParameterIiv = (PFNGLGETTEXPARAMETERIIVPROC)glfwGetProcAddress("glGetTexParameterIiv");
		glGetTexParameterIuiv = (PFNGLGETTEXPARAMETERIUIVPROC)glfwGetProcAddress("glGetTexParameterIuiv");
		glClearBufferiv = (PFNGLCLEARBUFFERIVPROC)glfwGetProcAddress("glClearBufferiv");
		glClearBufferuiv = (PFNGLCLEARBUFFERUIVPROC)glfwGetProcAddress("glClearBufferuiv");
		glClearBufferfv = (PFNGLCLEARBUFFERFVPROC)glfwGetProcAddress("glClearBufferfv");
		glClearBufferfi = (PFNGLCLEARBUFFERFIPROC)glfwGetProcAddress("glClearBufferfi");
		glGetStringi = (PFNGLGETSTRINGIPROC)glfwGetProcAddress("glGetStringi");

		// GL_ARB_framebuffer_object
		glIsRenderbuffer = (PFNGLISRENDERBUFFERPROC)glfwGetProcAddress("glIsRenderbuffer");
		glBindRenderbuffer = (PFNGLBINDRENDERBUFFERPROC)glfwGetProcAddress("glBindRenderbuffer");
		glDeleteRenderbuffers = (PFNGLDELETERENDERBUFFERSPROC)glfwGetProcAddress("glDeleteRenderbuffers");
		glGenRenderbuffers = (PFNGLGENRENDERBUFFERSPROC)glfwGetProcAddress("glGenRenderbuffers");
		glRenderbufferStorage = (PFNGLRENDERBUFFERSTORAGEPROC)glfwGetProcAddress("glRenderbufferStorage");
		glGetRenderbufferParameteriv = (PFNGLGETRENDERBUFFERPARAMETERIVPROC)glfwGetProcAddress("glGetRenderbufferParameteriv");
		glIsFramebuffer = (PFNGLISFRAMEBUFFERPROC)glfwGetProcAddress("glIsFramebuffer");
		glBindFramebuffer = (PFNGLBINDFRAMEBUFFERPROC)glfwGetProcAddress("glBindFramebuffer");
		glDeleteFramebuffers = (PFNGLDELETEFRAMEBUFFERSPROC)glfwGetProcAddress("glDeleteFramebuffers");
		glGenFramebuffers = (PFNGLGENFRAMEBUFFERSPROC)glfwGetProcAddress("glGenFramebuffers");
		glCheckFramebufferStatus = (PFNGLCHECKFRAMEBUFFERSTATUSPROC)glfwGetProcAddress("glCheckFramebufferStatus");
		glFramebufferTexture1D = (PFNGLFRAMEBUFFERTEXTURE1DPROC)glfwGetProcAddress("glFramebufferTexture1D");
		glFramebufferTexture2D = (PFNGLFRAMEBUFFERTEXTURE2DPROC)glfwGetProcAddress("glFramebufferTexture2D");
		glFramebufferTexture3D = (PFNGLFRAMEBUFFERTEXTURE3DPROC)glfwGetProcAddress("glFramebufferTexture3D");
		glFramebufferRenderbuffer = (PFNGLFRAMEBUFFERRENDERBUFFERPROC)glfwGetProcAddress("glFramebufferRenderbuffer");
		glGetFramebufferAttachmentParameteriv = (PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC)glfwGetProcAddress("glGetFramebufferAttachmentParameteriv");
		glGenerateMipmap = (PFNGLGENERATEMIPMAPPROC)glfwGetProcAddress("glGenerateMipmap");
		glBlitFramebuffer = (PFNGLBLITFRAMEBUFFERPROC)glfwGetProcAddress("glBlitFramebuffer");
		glRenderbufferStorageMultisample = (PFNGLRENDERBUFFERSTORAGEMULTISAMPLEPROC)glfwGetProcAddress("glRenderbufferStorageMultisample");
		glFramebufferTextureLayer = (PFNGLFRAMEBUFFERTEXTURELAYERPROC)glfwGetProcAddress("glFramebufferTextureLayer");

		// GL_ARB_vertex_array_object
		glBindVertexArray = (PFNGLBINDVERTEXARRAYPROC)glfwGetProcAddress("glBindVertexArray");
		glDeleteVertexArrays = (PFNGLDELETEVERTEXARRAYSPROC)glfwGetProcAddress("glDeleteVertexArrays");
		glGenVertexArrays = (PFNGLGENVERTEXARRAYSPROC)glfwGetProcAddress("glGenVertexArrays");
		glIsVertexArray = (PFNGLISVERTEXARRAYPROC)glfwGetProcAddress("glIsVertexArray");

		// OpenGL 3.1
		glDrawArraysInstanced = (PFNGLDRAWARRAYSINSTANCEDPROC)glfwGetProcAddress("glDrawArraysInstanced");
		glDrawElementsInstanced = (PFNGLDRAWELEMENTSINSTANCEDPROC)glfwGetProcAddress("glDrawElementsInstanced");
		glTexBuffer = (PFNGLTEXBUFFERPROC)glfwGetProcAddress("glTexBuffer");
		glPrimitiveRestartIndex = (PFNGLPRIMITIVERESTARTINDEXPROC)glfwGetProcAddress("glPrimitiveRestartIndex");

		// GL_ARB_copy_buffer
		glCopyBufferSubData = (PFNGLCOPYBUFFERSUBDATAPROC)glfwGetProcAddress("glCopyBufferSubData");

		// OpenGL 3.2
		glGetStringi = (PFNGLGETSTRINGIPROC)glfwGetProcAddress("glGetStringi");
		glBindFragDataLocation = (PFNGLBINDFRAGDATALOCATIONPROC)glfwGetProcAddress("glBindFragDataLocation");
		glGetInteger64i_v = (PFNGLGETINTEGER64I_VPROC)glfwGetProcAddress("glGetInteger64i_v");
		glGetBufferParameteri64v = (PFNGLGETBUFFERPARAMETERI64VPROC)glfwGetProcAddress("glGetBufferParameteri64v");
		glFramebufferTexture = (PFNGLFRAMEBUFFERTEXTUREPROC)glfwGetProcAddress("glFramebufferTexture");

		// GL_ARB_map_buffer_range
		glMapBufferRange = (PFNGLMAPBUFFERRANGEPROC)glfwGetProcAddress("glMapBufferRange");
		glFlushMappedBufferRange = (PFNGLFLUSHMAPPEDBUFFERRANGEPROC)glfwGetProcAddress("glFlushMappedBufferRange");

		// GL_ARB_draw_elements_base_vertex
		glDrawElementsBaseVertex = (PFNGLDRAWELEMENTSBASEVERTEXPROC)glfwGetProcAddress("glDrawElementsBaseVertex");
		glDrawRangeElementsBaseVertex = (PFNGLDRAWRANGEELEMENTSBASEVERTEXPROC)glfwGetProcAddress("glDrawRangeElementsBaseVertex");
		glDrawElementsInstancedBaseVertex = (PFNGLDRAWELEMENTSINSTANCEDBASEVERTEXPROC)glfwGetProcAddress("glDrawElementsInstancedBaseVertex");
		glMultiDrawElementsBaseVertex = (PFNGLMULTIDRAWELEMENTSBASEVERTEXPROC)glfwGetProcAddress("glMultiDrawElementsBaseVertex");

		// GL_ARB_uniform_buffer_object
		glGetUniformIndices = (PFNGLGETUNIFORMINDICESPROC)glfwGetProcAddress("glGetUniformIndices");
		glGetActiveUniformsiv = (PFNGLGETACTIVEUNIFORMSIVPROC)glfwGetProcAddress("glGetActiveUniformsiv");
		glGetActiveUniformName = (PFNGLGETACTIVEUNIFORMNAMEPROC)glfwGetProcAddress("glGetActiveUniformName");
		glGetUniformBlockIndex = (PFNGLGETUNIFORMBLOCKINDEXPROC)glfwGetProcAddress("glGetUniformBlockIndex");
		glGetActiveUniformBlockiv = (PFNGLGETACTIVEUNIFORMBLOCKIVPROC)glfwGetProcAddress("glGetActiveUniformBlockiv");
		glGetActiveUniformBlockName = (PFNGLGETACTIVEUNIFORMBLOCKNAMEPROC)glfwGetProcAddress("glGetActiveUniformBlockName");
		glUniformBlockBinding = (PFNGLUNIFORMBLOCKBINDINGPROC)glfwGetProcAddress("glUniformBlockBinding");

		// GL_ARB_texture_multisample
		glTexImage2DMultisample = (PFNGLTEXIMAGE2DMULTISAMPLEPROC)glfwGetProcAddress("glTexImage2DMultisample");
		glTexImage3DMultisample = (PFNGLTEXIMAGE3DMULTISAMPLEPROC)glfwGetProcAddress("glTexImage3DMultisample");
		glGetMultisamplefv = (PFNGLGETMULTISAMPLEFVPROC)glfwGetProcAddress("glGetMultisamplefv");
		glSampleMaski = (PFNGLSAMPLEMASKIPROC)glfwGetProcAddress("glSampleMaski");

		// OpenGL 3.3
		glVertexAttribDivisor = (PFNGLVERTEXATTRIBDIVISORPROC)glfwGetProcAddress("glVertexAttribDivisor");

		// GL_ARB_sampler_objects
		glGenSamplers = (PFNGLGENSAMPLERSPROC)glfwGetProcAddress("glGenSamplers");
		glDeleteSamplers = (PFNGLDELETESAMPLERSPROC)glfwGetProcAddress("glDeleteSamplers");
		glIsSampler = (PFNGLISSAMPLERPROC)glfwGetProcAddress("glIsSampler");
		glBindSampler = (PFNGLBINDSAMPLERPROC)glfwGetProcAddress("glBindSampler");
		glSamplerParameteri = (PFNGLSAMPLERPARAMETERIPROC)glfwGetProcAddress("glSamplerParameteri");
		glSamplerParameteriv = (PFNGLSAMPLERPARAMETERIVPROC)glfwGetProcAddress("glSamplerParameteriv");
		glSamplerParameterf = (PFNGLSAMPLERPARAMETERFPROC)glfwGetProcAddress("glSamplerParameterf");
		glSamplerParameterfv = (PFNGLSAMPLERPARAMETERFVPROC)glfwGetProcAddress("glSamplerParameterfv");
		glSamplerParameterIiv = (PFNGLSAMPLERPARAMETERIIVPROC)glfwGetProcAddress("glSamplerParameterIiv");
		glSamplerParameterIuiv = (PFNGLSAMPLERPARAMETERIUIVPROC)glfwGetProcAddress("glSamplerParameterIuiv");
		glGetSamplerParameteriv = (PFNGLGETSAMPLERPARAMETERIVPROC)glfwGetProcAddress("glGetSamplerParameteriv");
		glGetSamplerParameterIiv = (PFNGLGETSAMPLERPARAMETERIIVPROC)glfwGetProcAddress("glGetSamplerParameterIiv");
		glGetSamplerParameterfv = (PFNGLGETSAMPLERPARAMETERFVPROC)glfwGetProcAddress("glGetSamplerParameterfv");
		glGetSamplerParameterIuiv = (PFNGLGETSAMPLERPARAMETERIUIVPROC)glfwGetProcAddress("glGetSamplerParameterIuiv");

		// OpenGL 4.0
		glMinSampleShading = (PFNGLMINSAMPLESHADINGPROC)glfwGetProcAddress("glMinSampleShading");
		glBlendEquationi = (PFNGLBLENDEQUATIONIPROC)glfwGetProcAddress("glBlendEquationi");
		glBlendEquationSeparatei = (PFNGLBLENDEQUATIONSEPARATEIPROC)glfwGetProcAddress("glBlendEquationSeparatei");
		glBlendFunci = (PFNGLBLENDFUNCIPROC)glfwGetProcAddress("glBlendFunci");
		glBlendFuncSeparatei = (PFNGLBLENDFUNCSEPARATEIPROC)glfwGetProcAddress("glBlendFuncSeparatei");

		// GL_NV_explicit_multisample
		glGetMultisamplefvNV = (PFNGLGETMULTISAMPLEFVNVPROC)glfwGetProcAddress("glGetMultisamplefvNV");
		glSampleMaskIndexedNV = (PFNGLSAMPLEMASKINDEXEDNVPROC)glfwGetProcAddress("glSampleMaskIndexedNV");
		glTexRenderbufferNV = (PFNGLTEXRENDERBUFFERNVPROC)glfwGetProcAddress("glTexRenderbufferNV");

		// GL_EXT_texture_compression_s3tc
		#define GL_COMPRESSED_RGB_S3TC_DXT1_EXT			0x83F0
		#define GL_COMPRESSED_RGBA_S3TC_DXT1_EXT		0x83F1
		#define GL_COMPRESSED_RGBA_S3TC_DXT3_EXT		0x83F2
		#define GL_COMPRESSED_RGBA_S3TC_DXT5_EXT		0x83F3

		glDebugMessageControlARB = (PFNGLDEBUGMESSAGECONTROLPROC)glfwGetProcAddress("glDebugMessageControlARB");
		glDebugMessageCallbackARB = (PFNGLDEBUGMESSAGECALLBACKPROC)glfwGetProcAddress("glDebugMessageCallbackARB");

		glVertexArrayBindVertexBufferEXT = (PFNGLVERTEXARRAYBINDVERTEXBUFFEREXTPROC)glfwGetProcAddress("glVertexArrayBindVertexBufferEXT");
		glVertexArrayVertexAttribFormatEXT = (PFNGLVERTEXARRAYBINDVERTEXATTRIBFORMATEXTPROC)glfwGetProcAddress("glVertexArrayVertexAttribFormatEXT");
		glVertexArrayVertexAttribIFormatEXT = (PFNGLVERTEXARRAYBINDVERTEXATTRIBIFORMATEXTPROC)glfwGetProcAddress("glVertexArrayVertexAttribIFormatEXT");
		glVertexArrayVertexAttribLFormatEXT = (PFNGLVERTEXARRAYBINDVERTEXATTRIBLFORMATEXTPROC)glfwGetProcAddress("glVertexArrayVertexAttribLFormatEXT");
		glVertexArrayVertexAttribBindingEXT = (PFNGLVERTEXARRAYBINDVERTEXATTRIBBINDINGEXTPROC)glfwGetProcAddress("glVertexArrayVertexAttribBindingEXT");
		glVertexArrayVertexBindingDivisorEXT = (PFNGLVERTEXARRAYBINDVERTEXBINDINGDIVISOREXTPROC)glfwGetProcAddress("glVertexArrayVertexBindingDivisorEXT");

		glTexStorageSparseAMD = (PFNGLTEXSTORAGESPARSEAMDPROC)glfwGetProcAddress("glTexStorageSparseAMD");
		glTextureStorageSparseAMD = (PFNGLTEXTURESTORAGESPARSEAMDPROC)glfwGetProcAddress("glTextureStorageSparseAMD");

		glPopDebugGroup = (PFNGLPOPDEBUGGROUPPROC)glfwGetProcAddress("glPopDebugGroup");
#endif//WIN32
	}

	inline bool saveBinary
	(
		std::string const & Filename, 
		GLenum const & Format,
		std::vector<glm::byte> const & Data,
		GLint const & Size
	)
	{
		FILE* File = fopen(Filename.c_str(), "wb");

		if(File)
		{
			fwrite(&Format, sizeof(GLenum), 1, File);
			fwrite(&Size, sizeof(Size), 1, File);
			fwrite(&Data[0], Size, 1, File);
			fclose(File);
			return true;
		}
		return false;
	}

	inline bool loadBinary
	(
		std::string const & Filename,
		GLenum & Format,
		std::vector<glm::byte> & Data,
		GLint & Size
	)
	{
		FILE* File = fopen(Filename.c_str(), "rb");

		if(File)
		{
			fread(&Format, sizeof(GLenum), 1, File);
			fread(&Size, sizeof(Size), 1, File);
			Data.resize(Size);
			fread(&Data[0], Size, 1, File);
			fclose(File);
			return true;
		}
		return false;
	}

	inline std::string loadFile
	(
		std::string const & Filename
	)
	{
		std::string Result;
		
		std::ifstream Stream(Filename.c_str());
		if(!Stream.is_open())
			return Result;

		Stream.seekg(0, std::ios::end);
		Result.reserve(Stream.tellg());
		Stream.seekg(0, std::ios::beg);
		
		Result.assign(
			(std::istreambuf_iterator<char>(Stream)),
			std::istreambuf_iterator<char>());

		return Result;
	}

	#if !defined(__APPLE__)
	static void APIENTRY debugOutput
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
			else if(source == GL_DEBUG_SOURCE_OTHER_ARB)
				strcpy(debSource, "Other");
 
			if(type == GL_DEBUG_TYPE_ERROR_ARB)
				strcpy(debType, "error");
			else if(type == GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR_ARB)
				strcpy(debType, "deprecated behavior");
			else if(type == GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR_ARB)
				strcpy(debType, "undefined behavior");
			else if(type == GL_DEBUG_TYPE_PORTABILITY_ARB)
				strcpy(debType, "portability");
			else if(type == GL_DEBUG_TYPE_PERFORMANCE_ARB)
				strcpy(debType, "performance");
			else if(type == GL_DEBUG_TYPE_OTHER_ARB)
				strcpy(debType, "message");
 
			if(severity == GL_DEBUG_SEVERITY_HIGH_ARB)
			{
				strcpy(debSev, "high");
				Error = true;
			}
			else if(severity == GL_DEBUG_SEVERITY_MEDIUM_ARB)
				strcpy(debSev, "medium");
			else if(severity == GL_DEBUG_SEVERITY_LOW_ARB)
				strcpy(debSev, "low");

			 fprintf(stderr,"%s: %s(%s) %d: %s\n", debSource, debType, debSev, id, message);
			 //assert(!Error);
			 //fclose(f);
		}
	}
/*
	void checkDebugOutput()
	{
			unsigned int count = 10; // max. num. of messages that will be read from the log
			int bufsize = 2048;
	 
			unsigned int* sources      = new unsigned int[count];
			unsigned int* types        = new unsigned int[count];
			unsigned int* ids   = new unsigned int[count];
			unsigned int* severities = new unsigned int[count];
			int* lengths = new int[count];
	 
			char* messageLog = new char[bufsize];
	 
			unsigned int retVal = glGetDebugMessageLogARB(count, bufsize, sources, types, ids, severities, lengths, messageLog);
			if(retVal > 0)
			{
					unsigned int pos = 0;
					for(unsigned int i=0; i<retVal; i++)
					{
						debugOutput(sources[i], types[i], ids[i], severities[i], NULL, &messageLog[pos], NULL);
						pos += lengths[i];
					}
			}
			delete [] sources;
			delete [] types;
			delete [] ids;
			delete [] severities;
			delete [] lengths;
			delete [] messageLog;
	}
*/
#endif
	static void cursor_position_callback(int x, int y)
	{
		Window.MouseCurrent = glm::ivec2(x, y);
		Window.TranlationCurrent = Window.MouseButtonFlags & glf::MOUSE_BUTTON_LEFT ? Window.TranlationOrigin + (Window.MouseCurrent - Window.MouseOrigin) / 10.f : Window.TranlationOrigin;
		Window.RotationCurrent = Window.MouseButtonFlags & glf::MOUSE_BUTTON_RIGHT ? Window.RotationOrigin + (Window.MouseCurrent - Window.MouseOrigin) : Window.RotationOrigin;
	}

	static void mouse_button_callback(int Button, int Action)
	{
		switch(Action)
		{
			case GLFW_PRESS:
			{
				Window.MouseOrigin = Window.MouseCurrent;
				switch(Button)
				{
					case GLFW_MOUSE_BUTTON_LEFT:
					{
						Window.MouseButtonFlags |= glf::MOUSE_BUTTON_LEFT;
						Window.TranlationOrigin = Window.TranlationCurrent;
					}
					break;
					case GLFW_MOUSE_BUTTON_MIDDLE:
					{
						Window.MouseButtonFlags |= glf::MOUSE_BUTTON_MIDDLE;
					}
					break;
					case GLFW_MOUSE_BUTTON_RIGHT:
					{
						Window.MouseButtonFlags |= glf::MOUSE_BUTTON_RIGHT;
						Window.RotationOrigin = Window.RotationCurrent;
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
						Window.TranlationOrigin += (Window.MouseCurrent - Window.MouseOrigin) / 10.f;
						Window.MouseButtonFlags &= ~glf::MOUSE_BUTTON_LEFT;
					}
					break;
					case GLFW_MOUSE_BUTTON_MIDDLE:
					{
						Window.MouseButtonFlags &= ~glf::MOUSE_BUTTON_MIDDLE;
					}
					break;
					case GLFW_MOUSE_BUTTON_RIGHT:
					{
						Window.RotationOrigin += Window.MouseCurrent - Window.MouseOrigin;
						Window.MouseButtonFlags &= ~glf::MOUSE_BUTTON_RIGHT;
					}
					break;
				}
			}
			break;
		}
	}

	static void GLFWCALL key_callback(int key, int action)
	{
		switch(action)
		{
			case GLFW_PRESS:
			{
				Window.KeyPressed[key] = 1;
				break;
			}
			case GLFW_RELEASE:
			{
				Window.KeyPressed[key] = 0;
				break;
			}
		}
	}

	static int GLFWCALL close_callback()
	{
		Window.KeyPressed[GLFW_KEY_ESC] = 1;
		return GL_FALSE;
	}

	static void displayProxy()
	{
		static int FrameID = 0;
		++FrameID;
#ifdef GLF_AUTO_STATUS
		if(FrameID > 10)
			exit(end() ? 0 : 1);
#endif//GLF_AUTO_STATUS
		display();
	}

	inline int run
	(
		int argc, char* argv[], 
		glm::ivec2 const & Size, 
		int Profile,
		int Major, int Minor
	)
	{
		glfwInit();

		glfwEnable(GLFW_AUTO_POLL_EVENTS); /* No explicit call to glfwPollEvents() */

		glfwOpenWindowHint(GLFW_WINDOW_NO_RESIZE, GL_FALSE);
		glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, Major);
		glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, Minor);
#		if defined(__APPLE__)
			glfwOpenWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#		elif defined(__INTEL__)
			glfwOpenWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
#		else
			//glfwOpenWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
			glfwOpenWindowHint(GLFW_OPENGL_PROFILE, Profile == WGL_CONTEXT_CORE_PROFILE_BIT_ARB ? GLFW_OPENGL_CORE_PROFILE : GLFW_OPENGL_COMPAT_PROFILE);
#		endif
			//glfwOpenWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#		if defined(NDEBUG)
			glfwOpenWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_FALSE);
#		else
			glfwOpenWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
#		endif
		GLboolean Result = glfwOpenWindow(Size.x, Size.y, 8, 8, 8, 8, 24, 0, GLFW_WINDOW);
		assert(Result == GL_TRUE);

		glfwSetWindowTitle(argv[0]);
		glfwSetMouseButtonCallback(mouse_button_callback);
		glfwSetMousePosCallback(cursor_position_callback);
		glfwSetWindowCloseCallback(close_callback);
		glfwSetKeyCallback(key_callback);

		init();

		bool Run = begin();

		if(Run)
		{
			do
			{
				display();
			}
			while(Window.KeyPressed[GLFW_KEY_ESC] != 1);

			Run = end();
			glfwCloseWindow();
		}

		glfwTerminate();

		return Run ? 0 : 1;
	}

	bool validateVAO(GLuint VertexArrayName, std::vector<glf::vertexattrib> const & Expected)
	{
		bool Success = true;
#if !defined(__APPLE__)
		GLint MaxVertexAttrib(0);
		glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &MaxVertexAttrib);

		glBindVertexArray(VertexArrayName);
		for(GLuint AttribLocation = 0; AttribLocation < glm::min(GLuint(MaxVertexAttrib), GLuint(Expected.size())); ++AttribLocation)
		{
			glf::vertexattrib VertexAttrib;
			glGetVertexAttribiv(AttribLocation, GL_VERTEX_ATTRIB_ARRAY_ENABLED, &VertexAttrib.Enabled);
			//glGetVertexAttribiv(AttribLocation, GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING, &VertexAttrib.Binding);
			glGetVertexAttribiv(AttribLocation, GL_VERTEX_ATTRIB_ARRAY_SIZE, &VertexAttrib.Size);
			glGetVertexAttribiv(AttribLocation, GL_VERTEX_ATTRIB_ARRAY_STRIDE, &VertexAttrib.Stride);
			glGetVertexAttribiv(AttribLocation, GL_VERTEX_ATTRIB_ARRAY_TYPE, &VertexAttrib.Type);
			glGetVertexAttribiv(AttribLocation, GL_VERTEX_ATTRIB_ARRAY_NORMALIZED, &VertexAttrib.Normalized);
			glGetVertexAttribiv(AttribLocation, GL_VERTEX_ATTRIB_ARRAY_INTEGER, &VertexAttrib.Integer);
			glGetVertexAttribiv(AttribLocation, GL_VERTEX_ATTRIB_ARRAY_LONG, &VertexAttrib.Long);
			glGetVertexAttribiv(AttribLocation, GL_VERTEX_ATTRIB_ARRAY_DIVISOR, &VertexAttrib.Divisor);
			glGetVertexAttribPointerv(AttribLocation, GL_VERTEX_ATTRIB_ARRAY_POINTER, &VertexAttrib.Pointer);
			Success = Success && (VertexAttrib == Expected[AttribLocation]);
			assert(Success);
		}
		glBindVertexArray(0);
#endif//!defined(__APPLE__)
		return Success;
	}

}//namespace glf
