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

		// OpenGL 1.5
		glGenQueries = (PFNGLGENQUERIESPROC)glfwGetProcAddress("glGenQueries");
		glDeleteQueries = (PFNGLDELETEQUERIESPROC)glfwGetProcAddress("glDeleteQueries");
		glIsQuery = (PFNGLISQUERYPROC)glfwGetProcAddress("glIsQuery");
		glBeginQuery = (PFNGLBEGINQUERYPROC)glfwGetProcAddress("glBeginQuery");
		glEndQuery = (PFNGLENDQUERYPROC)glfwGetProcAddress("glEndQuery");
		glGetQueryiv = (PFNGLGETQUERYIVPROC)glfwGetProcAddress("glGetQueryiv");
		glGetQueryObjectiv = (PFNGLGETQUERYOBJECTIVPROC)glfwGetProcAddress("glGetQueryObjectiv");
		glGetQueryObjectuiv = (PFNGLGETQUERYOBJECTUIVPROC)glfwGetProcAddress("glGetQueryObjectuiv");

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
		glGetAttribLocation = (PFNGLGETATTRIBLOCATIONPROC)glfwGetProcAddress("glGetAttribLocation");
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
		glGetActiveAttrib = (PFNGLGETACTIVEATTRIBPROC)glfwGetProcAddress("glGetActiveAttrib");
		glGetActiveUniform = (PFNGLGETACTIVEUNIFORMPROC)glfwGetProcAddress("glGetActiveUniform");

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

		// GL_ARB_provoking_vertex
		glProvokingVertex = (PFNGLPROVOKINGVERTEXPROC)glfwGetProcAddress("glProvokingVertex");

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

		// GL_ARB_sync
		glFenceSync = (PFNGLFENCESYNCPROC)glfwGetProcAddress("glFenceSync");
		glIsSync = (PFNGLISSYNCPROC)glfwGetProcAddress("glIsSync");
		glDeleteSync = (PFNGLDELETESYNCPROC)glfwGetProcAddress("glDeleteSync");
		glClientWaitSync = (PFNGLCLIENTWAITSYNCPROC)glfwGetProcAddress("glClientWaitSync");
		glWaitSync = (PFNGLWAITSYNCPROC)glfwGetProcAddress("glWaitSync");
		glGetInteger64v = (PFNGLGETINTEGER64VPROC)glfwGetProcAddress("glGetInteger64v");
		glGetSynciv = (PFNGLGETSYNCIVPROC)glfwGetProcAddress("glGetSynciv");

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

		// GL_ARB_draw_indirect
		glDrawArraysIndirect = (PFNGLDRAWARRAYSINDIRECTPROC)glfwGetProcAddress("glDrawArraysIndirect");
		glDrawElementsIndirect = (PFNGLDRAWELEMENTSINDIRECTPROC)glfwGetProcAddress("glDrawElementsIndirect");

		// GL_ARB_gpu_shader_fp64
		glUniform1d = (PFNGLUNIFORM1DPROC)glfwGetProcAddress("glUniform1d");
		glUniform2d = (PFNGLUNIFORM2DPROC)glfwGetProcAddress("glUniform2d");
		glUniform3d = (PFNGLUNIFORM3DPROC)glfwGetProcAddress("glUniform3d");
		glUniform4d = (PFNGLUNIFORM4DPROC)glfwGetProcAddress("glUniform4d");
		glUniform1dv = (PFNGLUNIFORM1DVPROC)glfwGetProcAddress("glUniform1dv");
		glUniform2dv = (PFNGLUNIFORM2DVPROC)glfwGetProcAddress("glUniform2dv");
		glUniform3dv = (PFNGLUNIFORM3DVPROC)glfwGetProcAddress("glUniform3dv");
		glUniform4dv = (PFNGLUNIFORM4DVPROC)glfwGetProcAddress("glUniform4dv");
		glUniformMatrix2dv = (PFNGLUNIFORMMATRIX2DVPROC)glfwGetProcAddress("glUniformMatrix2dv");
		glUniformMatrix3dv = (PFNGLUNIFORMMATRIX3DVPROC)glfwGetProcAddress("glUniformMatrix3dv");
		glUniformMatrix4dv = (PFNGLUNIFORMMATRIX4DVPROC)glfwGetProcAddress("glUniformMatrix4dv");
		glUniformMatrix2x3dv = (PFNGLUNIFORMMATRIX2X3DVPROC)glfwGetProcAddress("glUniformMatrix2x3dv");
		glUniformMatrix2x4dv = (PFNGLUNIFORMMATRIX2X4DVPROC)glfwGetProcAddress("glUniformMatrix2x4dv");
		glUniformMatrix3x2dv = (PFNGLUNIFORMMATRIX3X2DVPROC)glfwGetProcAddress("glUniformMatrix3x2dv");
		glUniformMatrix3x4dv = (PFNGLUNIFORMMATRIX3X4DVPROC)glfwGetProcAddress("glUniformMatrix3x4dv");
		glUniformMatrix4x2dv = (PFNGLUNIFORMMATRIX4X2DVPROC)glfwGetProcAddress("glUniformMatrix4x2dv");
		glUniformMatrix4x3dv = (PFNGLUNIFORMMATRIX4X3DVPROC)glfwGetProcAddress("glUniformMatrix4x3dv");
		glGetUniformdv = (PFNGLGETUNIFORMDVPROC)glfwGetProcAddress("glGetUniformdv");

		// GL_ARB_shader_subroutine
		glGetSubroutineUniformLocation = (PFNGLGETSUBROUTINEUNIFORMLOCATIONPROC)glfwGetProcAddress("glGetSubroutineUniformLocation");
		glGetSubroutineIndex = (PFNGLGETSUBROUTINEINDEXPROC)glfwGetProcAddress("glGetSubroutineIndex");
		glGetActiveSubroutineUniformiv = (PFNGLGETACTIVESUBROUTINEUNIFORMIVPROC)glfwGetProcAddress("glGetActiveSubroutineUniformiv");
		glGetActiveSubroutineUniformName = (PFNGLGETACTIVESUBROUTINEUNIFORMNAMEPROC)glfwGetProcAddress("glGetActiveSubroutineUniformName");
		glGetActiveSubroutineName = (PFNGLGETACTIVESUBROUTINENAMEPROC)glfwGetProcAddress("glGetActiveSubroutineName");
		glUniformSubroutinesuiv = (PFNGLUNIFORMSUBROUTINESUIVPROC)glfwGetProcAddress("glUniformSubroutinesuiv");
		glGetUniformSubroutineuiv = (PFNGLGETUNIFORMSUBROUTINEUIVPROC)glfwGetProcAddress("glGetUniformSubroutineuiv");
		glGetProgramStageiv = (PFNGLGETPROGRAMSTAGEIVPROC)glfwGetProcAddress("glGetProgramStageiv");

		// GL_ARB_tessellation_shader
		glPatchParameteri = (PFNGLPATCHPARAMETERIPROC)glfwGetProcAddress("glPatchParameteri");
		glPatchParameterfv = (PFNGLPATCHPARAMETERFVPROC)glfwGetProcAddress("glPatchParameterfv");

		// GL_ARB_transform_feedback2
		glBindTransformFeedback = (PFNGLBINDTRANSFORMFEEDBACKPROC)glfwGetProcAddress("glBindTransformFeedback");
		glDeleteTransformFeedbacks = (PFNGLDELETETRANSFORMFEEDBACKSPROC)glfwGetProcAddress("glDeleteTransformFeedbacks");
		glGenTransformFeedbacks = (PFNGLGENTRANSFORMFEEDBACKSPROC)glfwGetProcAddress("glGenTransformFeedbacks");
		glIsTransformFeedback = (PFNGLISTRANSFORMFEEDBACKPROC)glfwGetProcAddress("glIsTransformFeedback");
		glPauseTransformFeedback = (PFNGLPAUSETRANSFORMFEEDBACKPROC)glfwGetProcAddress("glPauseTransformFeedback");
		glResumeTransformFeedback = (PFNGLRESUMETRANSFORMFEEDBACKPROC)glfwGetProcAddress("glResumeTransformFeedback");
		glDrawTransformFeedback = (PFNGLDRAWTRANSFORMFEEDBACKPROC)glfwGetProcAddress("glDrawTransformFeedback");

		// GL_ARB_transform_feedback3
		glDrawTransformFeedbackStream = (PFNGLDRAWTRANSFORMFEEDBACKSTREAMPROC)glfwGetProcAddress("glDrawTransformFeedbackStream");
		glBeginQueryIndexed = (PFNGLBEGINQUERYINDEXEDPROC)glfwGetProcAddress("glBeginQueryIndexed");
		glEndQueryIndexed = (PFNGLENDQUERYINDEXEDPROC)glfwGetProcAddress("glEndQueryIndexed");
		glGetQueryIndexediv = (PFNGLGETQUERYINDEXEDIVPROC)glfwGetProcAddress("glGetQueryIndexediv");

		// GL_ARB_separate_shader_objects
		glUseProgramStages = (PFNGLUSEPROGRAMSTAGESPROC)glfwGetProcAddress("glUseProgramStages");
		glActiveShaderProgram = (PFNGLACTIVESHADERPROGRAMPROC)glfwGetProcAddress("glActiveShaderProgram");
		glCreateShaderProgramv = (PFNGLCREATESHADERPROGRAMVPROC)glfwGetProcAddress("glCreateShaderProgramv");
		glBindProgramPipeline = (PFNGLBINDPROGRAMPIPELINEPROC)glfwGetProcAddress("glBindProgramPipeline");
		glDeleteProgramPipelines = (PFNGLDELETEPROGRAMPIPELINESPROC)glfwGetProcAddress("glDeleteProgramPipelines");
		glGenProgramPipelines = (PFNGLGENPROGRAMPIPELINESPROC)glfwGetProcAddress("glGenProgramPipelines");
		glIsProgramPipeline = (PFNGLISPROGRAMPIPELINEPROC)glfwGetProcAddress("glIsProgramPipeline");
		glGetProgramPipelineiv = (PFNGLGETPROGRAMPIPELINEIVPROC)glfwGetProcAddress("glGetProgramPipelineiv");
		glProgramUniform1i = (PFNGLPROGRAMUNIFORM1IPROC)glfwGetProcAddress("glProgramUniform1i");
		glProgramUniform1iv = (PFNGLPROGRAMUNIFORM1IVPROC)glfwGetProcAddress("glProgramUniform1iv");
		glProgramUniform1f = (PFNGLPROGRAMUNIFORM1FPROC)glfwGetProcAddress("glProgramUniform1f");
		glProgramUniform1fv = (PFNGLPROGRAMUNIFORM1FVPROC)glfwGetProcAddress("glProgramUniform1fv");
		glProgramUniform1d = (PFNGLPROGRAMUNIFORM1DPROC)glfwGetProcAddress("glProgramUniform1d");
		glProgramUniform1dv = (PFNGLPROGRAMUNIFORM1DVPROC)glfwGetProcAddress("glProgramUniform1dv");
		glProgramUniform1ui = (PFNGLPROGRAMUNIFORM1UIPROC)glfwGetProcAddress("glProgramUniform1ui");
		glProgramUniform1uiv = (PFNGLPROGRAMUNIFORM1UIVPROC)glfwGetProcAddress("glProgramUniform1uiv");
		glProgramUniform2i = (PFNGLPROGRAMUNIFORM2IPROC)glfwGetProcAddress("glProgramUniform2i");
		glProgramUniform2iv = (PFNGLPROGRAMUNIFORM2IVPROC)glfwGetProcAddress("glProgramUniform2iv");
		glProgramUniform2f = (PFNGLPROGRAMUNIFORM2FPROC)glfwGetProcAddress("glProgramUniform2f");
		glProgramUniform2fv = (PFNGLPROGRAMUNIFORM2FVPROC)glfwGetProcAddress("glProgramUniform2fv");
		glProgramUniform2d = (PFNGLPROGRAMUNIFORM2DPROC)glfwGetProcAddress("glProgramUniform2d");
		glProgramUniform2dv = (PFNGLPROGRAMUNIFORM2DVPROC)glfwGetProcAddress("glProgramUniform2dv");
		glProgramUniform2ui = (PFNGLPROGRAMUNIFORM2UIPROC)glfwGetProcAddress("glProgramUniform2ui");
		glProgramUniform2uiv = (PFNGLPROGRAMUNIFORM2UIVPROC)glfwGetProcAddress("glProgramUniform2uiv");
		glProgramUniform3i = (PFNGLPROGRAMUNIFORM3IPROC)glfwGetProcAddress("glProgramUniform3i");
		glProgramUniform3iv = (PFNGLPROGRAMUNIFORM3IVPROC)glfwGetProcAddress("glProgramUniform3iv");
		glProgramUniform3f = (PFNGLPROGRAMUNIFORM3FPROC)glfwGetProcAddress("glProgramUniform3f");
		glProgramUniform3fv = (PFNGLPROGRAMUNIFORM3FVPROC)glfwGetProcAddress("glProgramUniform3fv");
		glProgramUniform3d = (PFNGLPROGRAMUNIFORM3DPROC)glfwGetProcAddress("glProgramUniform3d");
		glProgramUniform3dv = (PFNGLPROGRAMUNIFORM3DVPROC)glfwGetProcAddress("glProgramUniform3dv");
		glProgramUniform3ui = (PFNGLPROGRAMUNIFORM3UIPROC)glfwGetProcAddress("glProgramUniform3ui");
		glProgramUniform3uiv = (PFNGLPROGRAMUNIFORM3UIVPROC)glfwGetProcAddress("glProgramUniform3uiv");
		glProgramUniform4i = (PFNGLPROGRAMUNIFORM4IPROC)glfwGetProcAddress("glProgramUniform4i");
		glProgramUniform4iv = (PFNGLPROGRAMUNIFORM4IVPROC)glfwGetProcAddress("glProgramUniform4iv");
		glProgramUniform4f = (PFNGLPROGRAMUNIFORM4FPROC)glfwGetProcAddress("glProgramUniform4f");
		glProgramUniform4fv = (PFNGLPROGRAMUNIFORM4FVPROC)glfwGetProcAddress("glProgramUniform4fv");
		glProgramUniform4d = (PFNGLPROGRAMUNIFORM4DPROC)glfwGetProcAddress("glProgramUniform4d");
		glProgramUniform4dv = (PFNGLPROGRAMUNIFORM4DVPROC)glfwGetProcAddress("glProgramUniform4dv");
		glProgramUniform4ui = (PFNGLPROGRAMUNIFORM4UIPROC)glfwGetProcAddress("glProgramUniform4ui");
		glProgramUniform4uiv = (PFNGLPROGRAMUNIFORM4UIVPROC)glfwGetProcAddress("glProgramUniform4uiv");
		glProgramUniformMatrix2fv = (PFNGLPROGRAMUNIFORMMATRIX2FVPROC)glfwGetProcAddress("glProgramUniformMatrix2fv");
		glProgramUniformMatrix3fv = (PFNGLPROGRAMUNIFORMMATRIX3FVPROC)glfwGetProcAddress("glProgramUniformMatrix3fv");
		glProgramUniformMatrix4fv = (PFNGLPROGRAMUNIFORMMATRIX4FVPROC)glfwGetProcAddress("glProgramUniformMatrix4fv");
		glProgramUniformMatrix2dv = (PFNGLPROGRAMUNIFORMMATRIX2DVPROC)glfwGetProcAddress("glProgramUniformMatrix2dv");
		glProgramUniformMatrix3dv = (PFNGLPROGRAMUNIFORMMATRIX3DVPROC)glfwGetProcAddress("glProgramUniformMatrix3dv");
		glProgramUniformMatrix4dv = (PFNGLPROGRAMUNIFORMMATRIX4DVPROC)glfwGetProcAddress("glProgramUniformMatrix4dv");
		glProgramUniformMatrix2x3fv = (PFNGLPROGRAMUNIFORMMATRIX2X3FVPROC)glfwGetProcAddress("glProgramUniformMatrix2x3fv");
		glProgramUniformMatrix3x2fv = (PFNGLPROGRAMUNIFORMMATRIX3X2FVPROC)glfwGetProcAddress("glProgramUniformMatrix3x2fv");
		glProgramUniformMatrix2x4fv = (PFNGLPROGRAMUNIFORMMATRIX2X4FVPROC)glfwGetProcAddress("glProgramUniformMatrix2x4fv");
		glProgramUniformMatrix4x2fv = (PFNGLPROGRAMUNIFORMMATRIX4X2FVPROC)glfwGetProcAddress("glProgramUniformMatrix4x2fv");
		glProgramUniformMatrix3x4fv = (PFNGLPROGRAMUNIFORMMATRIX3X4FVPROC)glfwGetProcAddress("glProgramUniformMatrix3x4fv");
		glProgramUniformMatrix4x3fv = (PFNGLPROGRAMUNIFORMMATRIX4X3FVPROC)glfwGetProcAddress("glProgramUniformMatrix4x3fv");
		glProgramUniformMatrix2x3dv = (PFNGLPROGRAMUNIFORMMATRIX2X3DVPROC)glfwGetProcAddress("glProgramUniformMatrix2x3dv");
		glProgramUniformMatrix3x2dv = (PFNGLPROGRAMUNIFORMMATRIX3X2DVPROC)glfwGetProcAddress("glProgramUniformMatrix3x2dv");
		glProgramUniformMatrix2x4dv = (PFNGLPROGRAMUNIFORMMATRIX2X4DVPROC)glfwGetProcAddress("glProgramUniformMatrix2x4dv");
		glProgramUniformMatrix4x2dv = (PFNGLPROGRAMUNIFORMMATRIX4X2DVPROC)glfwGetProcAddress("glProgramUniformMatrix4x2dv");
		glProgramUniformMatrix3x4dv = (PFNGLPROGRAMUNIFORMMATRIX3X4DVPROC)glfwGetProcAddress("glProgramUniformMatrix3x4dv");
		glProgramUniformMatrix4x3dv = (PFNGLPROGRAMUNIFORMMATRIX4X3DVPROC)glfwGetProcAddress("glProgramUniformMatrix4x3dv");
		glValidateProgramPipeline = (PFNGLVALIDATEPROGRAMPIPELINEPROC)glfwGetProcAddress("glValidateProgramPipeline");
		glGetProgramPipelineInfoLog = (PFNGLGETPROGRAMPIPELINEINFOLOGPROC)glfwGetProcAddress("glGetProgramPipelineInfoLog");

		// GL_ARB_get_program_binary
		glGetProgramBinary = (PFNGLGETPROGRAMBINARYPROC)glfwGetProcAddress("glGetProgramBinary");
		glProgramBinary = (PFNGLPROGRAMBINARYPROC)glfwGetProcAddress("glProgramBinary");
		glProgramParameteri = (PFNGLPROGRAMPARAMETERIPROC)glfwGetProcAddress("glProgramParameteri");

		// GL_ARB_vertex_attrib_64bit
		glVertexAttribL1d = (PFNGLVERTEXATTRIBL1DPROC)glfwGetProcAddress("glVertexAttribL1d");
		glVertexAttribL2d = (PFNGLVERTEXATTRIBL2DPROC)glfwGetProcAddress("glVertexAttribL2d");
		glVertexAttribL3d = (PFNGLVERTEXATTRIBL3DPROC)glfwGetProcAddress("glVertexAttribL3d");
		glVertexAttribL4d = (PFNGLVERTEXATTRIBL4DPROC)glfwGetProcAddress("glVertexAttribL4d");
		glVertexAttribL1dv = (PFNGLVERTEXATTRIBL1DVPROC)glfwGetProcAddress("glVertexAttribL1dv");
		glVertexAttribL2dv = (PFNGLVERTEXATTRIBL2DVPROC)glfwGetProcAddress("glVertexAttribL2dv");
		glVertexAttribL3dv = (PFNGLVERTEXATTRIBL3DVPROC)glfwGetProcAddress("glVertexAttribL3dv");
		glVertexAttribL4dv = (PFNGLVERTEXATTRIBL4DVPROC)glfwGetProcAddress("glVertexAttribL4dv");
		glVertexAttribLPointer = (PFNGLVERTEXATTRIBLPOINTERPROC)glfwGetProcAddress("glVertexAttribLPointer");
		glGetVertexAttribLdv = (PFNGLGETVERTEXATTRIBLDVPROC)glfwGetProcAddress("glGetVertexAttribLdv");

		// GL_ARB_viewport_array
		glViewportArrayv = (PFNGLVIEWPORTARRAYVPROC)glfwGetProcAddress("glViewportArrayv");
		glViewportIndexedf = (PFNGLVIEWPORTINDEXEDFPROC)glfwGetProcAddress("glViewportIndexedf");
		glViewportIndexedfv = (PFNGLVIEWPORTINDEXEDFVPROC)glfwGetProcAddress("glViewportIndexedfv");
		glScissorArrayv = (PFNGLSCISSORARRAYVPROC)glfwGetProcAddress("glScissorArrayv");
		glScissorIndexed = (PFNGLSCISSORINDEXEDPROC)glfwGetProcAddress("glScissorIndexed");
		glScissorIndexedv = (PFNGLSCISSORINDEXEDVPROC)glfwGetProcAddress("glScissorIndexedv");
		glDepthRangeArrayv = (PFNGLDEPTHRANGEARRAYVPROC)glfwGetProcAddress("glDepthRangeArrayv");
		glDepthRangeIndexed = (PFNGLDEPTHRANGEINDEXEDPROC)glfwGetProcAddress("glDepthRangeIndexed");
		glGetFloati_v = (PFNGLGETFLOATI_VPROC)glfwGetProcAddress("glGetFloati_v");
		glGetDoublei_v = (PFNGLGETDOUBLEI_VPROC)glfwGetProcAddress("glGetDoublei_v");

		// OpenGL 4.2

		// GL_ARB_texture_storage
		glTexStorage1D = (PFNGLTEXSTORAGE1DPROC)glfwGetProcAddress("glTexStorage1D");
		glTexStorage2D = (PFNGLTEXSTORAGE2DPROC)glfwGetProcAddress("glTexStorage2D");
		glTexStorage3D = (PFNGLTEXSTORAGE3DPROC)glfwGetProcAddress("glTexStorage3D");

		// GL_ARB_base_instance
		glDrawArraysInstancedBaseInstance = (PFNGLDRAWARRAYSINSTANCEDBASEINSTANCEPROC)glfwGetProcAddress("glDrawArraysInstancedBaseInstance");
		glDrawElementsInstancedBaseInstance = (PFNGLDRAWELEMENTSINSTANCEDBASEINSTANCEPROC)glfwGetProcAddress("glDrawElementsInstancedBaseInstance");
		glDrawElementsInstancedBaseVertexBaseInstance = (PFNGLDRAWELEMENTSINSTANCEDBASEVERTEXBASEINSTANCEPROC)glfwGetProcAddress("glDrawElementsInstancedBaseVertexBaseInstance");

		// GL_ARB_shader_image_load_store
		glBindImageTexture = (PFNGLBINDIMAGETEXTUREPROC)glfwGetProcAddress("glBindImageTexture");
		glMemoryBarrier = (PFNGLMEMORYBARRIERPROC)glfwGetProcAddress("glMemoryBarrier");

		// GL_ARB_internalformat_query
		glGetInternalformativ = (PFNGLGETINTERNALFORMATIVPROC)glfwGetProcAddress("glGetInternalformativ");

		// GL_ARB_transform_feedback_instanced
		glDrawTransformFeedbackInstanced = (PFNGLDRAWTRANSFORMFEEDBACKINSTANCEDPROC)glfwGetProcAddress("glDrawTransformFeedbackInstanced");
		glDrawTransformFeedbackStreamInstanced = (PFNGLDRAWTRANSFORMFEEDBACKSTREAMINSTANCEDPROC)glfwGetProcAddress("glDrawTransformFeedbackStreamInstanced");

		// OpenGL 4.3

		// GL_ARB_compute_shader
		glDispatchCompute = (PFNGLDISPATCHCOMPUTEPROC)glfwGetProcAddress("glDispatchCompute");
		glDispatchComputeIndirect = (PFNGLDISPATCHCOMPUTEINDIRECTPROC)glfwGetProcAddress("glDispatchComputeIndirect");

		// GL_ARB_clear_buffer_object
		glClearBufferData = (PFNGLCLEARBUFFERDATAPROC)glfwGetProcAddress("glClearBufferData");
		glClearBufferSubData = (PFNGLCLEARBUFFERSUBDATAPROC)glfwGetProcAddress("glClearBufferSubData");
		glClearNamedBufferDataEXT = (PFNGLCLEARNAMEDBUFFERDATAEXTPROC)glfwGetProcAddress("glClearNamedBufferDataEXT");
		glClearNamedBufferSubDataEXT = (PFNGLCLEARNAMEDBUFFERSUBDATAEXTPROC)glfwGetProcAddress("glClearNamedBufferSubDataEXT");

		// GL_ARB_clear_buffer_object
		glClearBufferData = (PFNGLCLEARBUFFERDATAPROC)glfwGetProcAddress("glClearBufferData");

		// GL_ARB_copy_image
		glCopyImageSubData = (PFNGLCOPYIMAGESUBDATAPROC)glfwGetProcAddress("glCopyImageSubData");

		// GL_ARB_program_interface_query
		glGetProgramInterfaceiv = (PFNGLGETPROGRAMINTERFACEIVPROC)glfwGetProcAddress("glGetProgramInterfaceiv");
		glGetProgramResourceIndex = (PFNGLGETPROGRAMRESOURCEINDEXPROC)glfwGetProcAddress("glGetProgramResourceIndex");
		glGetProgramResourceName = (PFNGLGETPROGRAMRESOURCENAMEPROC)glfwGetProcAddress("glGetProgramResourceName");
		glGetProgramResourceiv = (PFNGLGETPROGRAMRESOURCEIVPROC)glfwGetProcAddress("glGetProgramResourceiv");
		glGetProgramResourceLocation = (PFNGLGETPROGRAMRESOURCELOCATIONPROC)glfwGetProcAddress("glGetProgramResourceLocation");
		glGetProgramResourceLocationIndex = (PFNGLGETPROGRAMRESOURCELOCATIONINDEXPROC)glfwGetProcAddress("glGetProgramResourceLocationIndex");

		// GL_ARB_texture_buffer_range
		glTexBufferRange = (PFNGLTEXBUFFERRANGEPROC)glfwGetProcAddress("glTexBufferRange");
		glTextureBufferRangeEXT = (PFNGLTEXTUREBUFFERRANGEEXTPROC)glfwGetProcAddress("glTextureBufferRangeEXT");

		// GL_ARB_texture_storage_multisample
		glTexStorage2DMultisample = (PFNGLTEXSTORAGE2DMULTISAMPLEPROC)glfwGetProcAddress("glTexStorage2DMultisample");
		glTexStorage3DMultisample = (PFNGLTEXSTORAGE3DMULTISAMPLEPROC)glfwGetProcAddress("glTexStorage3DMultisample");
		glTextureStorage2DMultisampleEXT = (PFNGLTEXTURESTORAGE2DMULTISAMPLEEXTPROC)glfwGetProcAddress("glTextureStorage2DMultisampleEXT");
		glTextureStorage3DMultisampleEXT = (PFNGLTEXTURESTORAGE3DMULTISAMPLEEXTPROC)glfwGetProcAddress("glTextureStorage3DMultisampleEXT");

		// GL_ARB_texture_view
		glTextureView = (PFNGLTEXTUREVIEWPROC)glfwGetProcAddress("glTextureView");

		// GL_KHR_debug
		glDebugMessageControl = (PFNGLDEBUGMESSAGECONTROLPROC)glfwGetProcAddress("glDebugMessageControl");
		glDebugMessageInsert = (PFNGLDEBUGMESSAGEINSERTPROC)glfwGetProcAddress("glDebugMessageInsert");
		glDebugMessageCallback = (PFNGLDEBUGMESSAGECALLBACKPROC)glfwGetProcAddress("glDebugMessageCallback");
		glGetDebugMessageLog = (PFNGLGETDEBUGMESSAGELOGPROC)glfwGetProcAddress("glGetDebugMessageLog");
		glPushDebugGroup = (PFNGLPUSHDEBUGGROUPPROC)glfwGetProcAddress("glPushDebugGroup");
		glPopDebugGroup = (PFNGLPOPDEBUGGROUPPROC)glfwGetProcAddress("glPopDebugGroup");
		glObjectLabel = (PFNGLOBJECTLABELPROC)glfwGetProcAddress("glObjectLabel");
		glGetObjectLabel = (PFNGLGETOBJECTLABELPROC)glfwGetProcAddress("glGetObjectLabel");
		glObjectPtrLabel = (PFNGLOBJECTPTRLABELPROC)glfwGetProcAddress("glObjectPtrLabel");
		glGetObjectPtrLabel = (PFNGLGETOBJECTPTRLABELPROC)glfwGetProcAddress("glGetObjectPtrLabel");

		// GL_EXT_direct_state_access
		glBindMultiTextureEXT = (PFNGLBINDMULTITEXTUREEXTPROC)glfwGetProcAddress("glBindMultiTextureEXT");
		glCheckNamedFramebufferStatusEXT = (PFNGLCHECKNAMEDFRAMEBUFFERSTATUSEXTPROC)glfwGetProcAddress("glCheckNamedFramebufferStatusEXT");
		glClientAttribDefaultEXT = (PFNGLCLIENTATTRIBDEFAULTEXTPROC)glfwGetProcAddress("glClientAttribDefaultEXT");
		glCompressedTextureImage1DEXT = (PFNGLCOMPRESSEDTEXTUREIMAGE1DEXTPROC)glfwGetProcAddress("glCompressedTextureImage1DEXT");
		glCompressedTextureImage2DEXT = (PFNGLCOMPRESSEDTEXTUREIMAGE2DEXTPROC)glfwGetProcAddress("glCompressedTextureImage2DEXT");
		glCompressedTextureImage3DEXT = (PFNGLCOMPRESSEDTEXTUREIMAGE3DEXTPROC)glfwGetProcAddress("glCompressedTextureImage3DEXT");
		glCompressedTextureSubImage1DEXT = (PFNGLCOMPRESSEDTEXTURESUBIMAGE1DEXTPROC)glfwGetProcAddress("glCompressedTextureSubImage1DEXT");
		glCompressedTextureSubImage2DEXT = (PFNGLCOMPRESSEDTEXTURESUBIMAGE2DEXTPROC)glfwGetProcAddress("glCompressedTextureSubImage2DEXT");
		glCompressedTextureSubImage3DEXT = (PFNGLCOMPRESSEDTEXTURESUBIMAGE3DEXTPROC)glfwGetProcAddress("glCompressedTextureSubImage3DEXT");
		glCopyTextureImage1DEXT = (PFNGLCOPYTEXTUREIMAGE1DEXTPROC)glfwGetProcAddress("glCopyTextureImage1DEXT");
		glCopyTextureImage2DEXT = (PFNGLCOPYTEXTUREIMAGE2DEXTPROC)glfwGetProcAddress("glCopyTextureImage2DEXT");
		glCopyTextureSubImage1DEXT = (PFNGLCOPYTEXTURESUBIMAGE1DEXTPROC)glfwGetProcAddress("glCopyTextureSubImage1DEXT");
		glCopyTextureSubImage2DEXT = (PFNGLCOPYTEXTURESUBIMAGE2DEXTPROC)glfwGetProcAddress("glCopyTextureSubImage2DEXT");
		glCopyTextureSubImage3DEXT = (PFNGLCOPYTEXTURESUBIMAGE3DEXTPROC)glfwGetProcAddress("glCopyTextureSubImage3DEXT");
		glDisableVertexArrayAttribEXT = (PFNGLDISABLEVERTEXARRAYATTRIBEXTPROC)glfwGetProcAddress("glDisableVertexArrayAttribEXT");
		glDisableVertexArrayEXT = (PFNGLDISABLEVERTEXARRAYEXTPROC)glfwGetProcAddress("glDisableVertexArrayEXT");
		glEnableVertexArrayAttribEXT = (PFNGLENABLEVERTEXARRAYATTRIBEXTPROC)glfwGetProcAddress("glEnableVertexArrayAttribEXT");
		glEnableVertexArrayEXT = (PFNGLENABLEVERTEXARRAYEXTPROC)glfwGetProcAddress("glEnableVertexArrayEXT");
		glFlushMappedNamedBufferRangeEXT = (PFNGLFLUSHMAPPEDNAMEDBUFFERRANGEEXTPROC)glfwGetProcAddress("glFlushMappedNamedBufferRangeEXT");
		glFramebufferDrawBufferEXT = (PFNGLFRAMEBUFFERDRAWBUFFEREXTPROC)glfwGetProcAddress("glFramebufferDrawBufferEXT");
		glFramebufferDrawBuffersEXT = (PFNGLFRAMEBUFFERDRAWBUFFERSEXTPROC)glfwGetProcAddress("glFramebufferDrawBuffersEXT");
		glFramebufferReadBufferEXT = (PFNGLFRAMEBUFFERREADBUFFEREXTPROC)glfwGetProcAddress("glFramebufferReadBufferEXT");
		glGenerateTextureMipmapEXT = (PFNGLGENERATETEXTUREMIPMAPEXTPROC)glfwGetProcAddress("glGenerateTextureMipmapEXT");
		glGetCompressedTextureImageEXT = (PFNGLGETCOMPRESSEDTEXTUREIMAGEEXTPROC)glfwGetProcAddress("glGetCompressedTextureImageEXT");
		glGetFramebufferParameterivEXT = (PFNGLGETFRAMEBUFFERPARAMETERIVEXTPROC)glfwGetProcAddress("glGetFramebufferParameterivEXT");
		glGetNamedBufferParameterivEXT = (PFNGLGETNAMEDBUFFERPARAMETERIVEXTPROC)glfwGetProcAddress("glGetNamedBufferParameterivEXT");
		glGetNamedBufferPointervEXT = (PFNGLGETNAMEDBUFFERPOINTERVEXTPROC)glfwGetProcAddress("glGetNamedBufferPointervEXT");
		glGetNamedBufferSubDataEXT = (PFNGLGETNAMEDBUFFERSUBDATAEXTPROC)glfwGetProcAddress("glGetNamedBufferSubDataEXT");
		glGetNamedFramebufferAttachmentParameterivEXT = (PFNGLGETNAMEDFRAMEBUFFERATTACHMENTPARAMETERIVEXTPROC)glfwGetProcAddress("glGetNamedFramebufferAttachmentParameterivEXT");
		glGetNamedProgramStringEXT = (PFNGLGETNAMEDPROGRAMSTRINGEXTPROC)glfwGetProcAddress("glGetNamedProgramStringEXT");
		glGetNamedProgramivEXT = (PFNGLGETNAMEDPROGRAMIVEXTPROC)glfwGetProcAddress("glGetNamedProgramivEXT");
		glGetNamedRenderbufferParameterivEXT = (PFNGLGETNAMEDRENDERBUFFERPARAMETERIVEXTPROC)glfwGetProcAddress("glGetNamedRenderbufferParameterivEXT");
		glGetTextureImageEXT = (PFNGLGETTEXTUREIMAGEEXTPROC)glfwGetProcAddress("glGetTextureImageEXT");
		glGetTextureLevelParameterfvEXT = (PFNGLGETTEXTURELEVELPARAMETERFVEXTPROC)glfwGetProcAddress("glGetTextureLevelParameterfvEXT");
		glGetTextureLevelParameterivEXT = (PFNGLGETTEXTURELEVELPARAMETERIVEXTPROC)glfwGetProcAddress("glGetTextureLevelParameterivEXT");
		glGetTextureParameterIivEXT = (PFNGLGETTEXTUREPARAMETERIIVEXTPROC)glfwGetProcAddress("glGetTextureParameterIivEXT");
		glGetTextureParameterIuivEXT = (PFNGLGETTEXTUREPARAMETERIUIVEXTPROC)glfwGetProcAddress("glGetTextureParameterIuivEXT");
		glGetTextureParameterfvEXT = (PFNGLGETTEXTUREPARAMETERFVEXTPROC)glfwGetProcAddress("glGetTextureParameterfvEXT");
		glGetTextureParameterivEXT = (PFNGLGETTEXTUREPARAMETERIVEXTPROC)glfwGetProcAddress("glGetTextureParameterivEXT");
		glGetVertexArrayIntegeri_vEXT = (PFNGLGETVERTEXARRAYINTEGERI_VEXTPROC)glfwGetProcAddress("glGetVertexArrayIntegeri_vEXT");
		glGetVertexArrayIntegervEXT = (PFNGLGETVERTEXARRAYINTEGERVEXTPROC)glfwGetProcAddress("glGetVertexArrayIntegervEXT");
		glGetVertexArrayPointeri_vEXT = (PFNGLGETVERTEXARRAYPOINTERI_VEXTPROC)glfwGetProcAddress("glGetVertexArrayPointeri_vEXT");
		glGetVertexArrayPointervEXT = (PFNGLGETVERTEXARRAYPOINTERVEXTPROC)glfwGetProcAddress("glGetVertexArrayPointervEXT");
		glMapNamedBufferEXT = (PFNGLMAPNAMEDBUFFEREXTPROC)glfwGetProcAddress("glMapNamedBufferEXT");
		glMapNamedBufferRangeEXT = (PFNGLMAPNAMEDBUFFERRANGEEXTPROC)glfwGetProcAddress("glMapNamedBufferRangeEXT");
		glNamedBufferDataEXT = (PFNGLNAMEDBUFFERDATAEXTPROC)glfwGetProcAddress("glNamedBufferDataEXT");
		glNamedBufferSubDataEXT = (PFNGLNAMEDBUFFERSUBDATAEXTPROC)glfwGetProcAddress("glNamedBufferSubDataEXT");
		glNamedCopyBufferSubDataEXT = (PFNGLNAMEDCOPYBUFFERSUBDATAEXTPROC)glfwGetProcAddress("glNamedCopyBufferSubDataEXT");

		glNamedFramebufferRenderbufferEXT = (PFNGLNAMEDFRAMEBUFFERRENDERBUFFEREXTPROC)glfwGetProcAddress("glNamedFramebufferRenderbufferEXT");
		glNamedFramebufferTexture1DEXT = (PFNGLNAMEDFRAMEBUFFERTEXTURE1DEXTPROC)glfwGetProcAddress("glNamedFramebufferTexture1DEXT");
		glNamedFramebufferTexture2DEXT = (PFNGLNAMEDFRAMEBUFFERTEXTURE2DEXTPROC)glfwGetProcAddress("glNamedFramebufferTexture2DEXT");
		glNamedFramebufferTexture3DEXT = (PFNGLNAMEDFRAMEBUFFERTEXTURE3DEXTPROC)glfwGetProcAddress("glNamedFramebufferTexture3DEXT");
		glNamedFramebufferTextureEXT = (PFNGLNAMEDFRAMEBUFFERTEXTUREEXTPROC)glfwGetProcAddress("glNamedFramebufferTextureEXT");
		glNamedFramebufferTextureFaceEXT = (PFNGLNAMEDFRAMEBUFFERTEXTUREFACEEXTPROC)glfwGetProcAddress("glNamedFramebufferTextureFaceEXT");
		glNamedFramebufferTextureLayerEXT = (PFNGLNAMEDFRAMEBUFFERTEXTURELAYEREXTPROC)glfwGetProcAddress("glNamedFramebufferTextureLayerEXT");
		glNamedProgramStringEXT = (PFNGLNAMEDPROGRAMSTRINGEXTPROC)glfwGetProcAddress("glNamedProgramStringEXT");
		glNamedRenderbufferStorageEXT = (PFNGLNAMEDRENDERBUFFERSTORAGEEXTPROC)glfwGetProcAddress("glNamedRenderbufferStorageEXT");
		glNamedRenderbufferStorageMultisampleCoverageEXT = (PFNGLNAMEDRENDERBUFFERSTORAGEMULTISAMPLECOVERAGEEXTPROC)glfwGetProcAddress("glNamedRenderbufferStorageMultisampleCoverageEXT");
		glNamedRenderbufferStorageMultisampleEXT = (PFNGLNAMEDRENDERBUFFERSTORAGEMULTISAMPLEEXTPROC)glfwGetProcAddress("glNamedRenderbufferStorageMultisampleEXT");
		glTextureBufferEXT = (PFNGLTEXTUREBUFFEREXTPROC)glfwGetProcAddress("glTextureBufferEXT");
		glTextureImage1DEXT = (PFNGLTEXTUREIMAGE1DEXTPROC)glfwGetProcAddress("glTextureImage1DEXT");
		glTextureImage2DEXT = (PFNGLTEXTUREIMAGE2DEXTPROC)glfwGetProcAddress("glTextureImage2DEXT");
		glTextureImage3DEXT = (PFNGLTEXTUREIMAGE3DEXTPROC)glfwGetProcAddress("glTextureImage3DEXT");
		glTextureParameterIivEXT = (PFNGLTEXTUREPARAMETERIIVEXTPROC)glfwGetProcAddress("glTextureParameterIivEXT");
		glTextureParameterIuivEXT = (PFNGLTEXTUREPARAMETERIUIVEXTPROC)glfwGetProcAddress("glTextureParameterIuivEXT");
		glTextureParameterfEXT = (PFNGLTEXTUREPARAMETERFEXTPROC)glfwGetProcAddress("glTextureParameterfEXT");
		glTextureParameterfvEXT = (PFNGLTEXTUREPARAMETERFVEXTPROC)glfwGetProcAddress("glTextureParameterfvEXT");
		glTextureParameteriEXT = (PFNGLTEXTUREPARAMETERIEXTPROC)glfwGetProcAddress("glTextureParameteriEXT");
		glTextureParameterivEXT = (PFNGLTEXTUREPARAMETERIVEXTPROC)glfwGetProcAddress("glTextureParameterivEXT");
		glTextureRenderbufferEXT = (PFNGLTEXTURERENDERBUFFEREXTPROC)glfwGetProcAddress("glTextureRenderbufferEXT");
		glTextureSubImage1DEXT = (PFNGLTEXTURESUBIMAGE1DEXTPROC)glfwGetProcAddress("glTextureSubImage1DEXT");
		glTextureSubImage2DEXT = (PFNGLTEXTURESUBIMAGE2DEXTPROC)glfwGetProcAddress("glTextureSubImage2DEXT");
		glTextureSubImage3DEXT = (PFNGLTEXTURESUBIMAGE3DEXTPROC)glfwGetProcAddress("glTextureSubImage3DEXT");
		glUnmapNamedBufferEXT = (PFNGLUNMAPNAMEDBUFFEREXTPROC)glfwGetProcAddress("glUnmapNamedBufferEXT");
		glVertexArrayVertexAttribIOffsetEXT = (PFNGLVERTEXARRAYVERTEXATTRIBIOFFSETEXTPROC)glfwGetProcAddress("glVertexArrayVertexAttribIOffsetEXT");
		glVertexArrayVertexAttribOffsetEXT = (PFNGLVERTEXARRAYVERTEXATTRIBOFFSETEXTPROC)glfwGetProcAddress("glVertexArrayVertexAttribOffsetEXT");
		glVertexArrayVertexOffsetEXT = (PFNGLVERTEXARRAYVERTEXOFFSETEXTPROC)glfwGetProcAddress("glVertexArrayVertexOffsetEXT");

		glVertexArrayBindVertexBufferEXT = (PFNGLVERTEXARRAYBINDVERTEXBUFFEREXTPROC)glfwGetProcAddress("glVertexArrayBindVertexBufferEXT");
		glVertexArrayVertexAttribFormatEXT = (PFNGLVERTEXARRAYBINDVERTEXATTRIBFORMATEXTPROC)glfwGetProcAddress("glVertexArrayVertexAttribFormatEXT");
		glVertexArrayVertexAttribIFormatEXT = (PFNGLVERTEXARRAYBINDVERTEXATTRIBIFORMATEXTPROC)glfwGetProcAddress("glVertexArrayVertexAttribIFormatEXT");
		glVertexArrayVertexAttribLFormatEXT = (PFNGLVERTEXARRAYBINDVERTEXATTRIBLFORMATEXTPROC)glfwGetProcAddress("glVertexArrayVertexAttribLFormatEXT");
		glVertexArrayVertexAttribBindingEXT = (PFNGLVERTEXARRAYBINDVERTEXATTRIBBINDINGEXTPROC)glfwGetProcAddress("glVertexArrayVertexAttribBindingEXT");
		glVertexArrayVertexBindingDivisorEXT = (PFNGLVERTEXARRAYBINDVERTEXBINDINGDIVISOREXTPROC)glfwGetProcAddress("glVertexArrayVertexBindingDivisorEXT");
		glTextureStorage1DEXT = (PFNGLTEXTURESTORAGE1DEXTPROC)glfwGetProcAddress("glTextureStorage1DEXT");
		glTextureStorage2DEXT = (PFNGLTEXTURESTORAGE2DEXTPROC)glfwGetProcAddress("glTextureStorage2DEXT");
		glTextureStorage3DEXT = (PFNGLTEXTURESTORAGE3DEXTPROC)glfwGetProcAddress("glTextureStorage3DEXT");

		// GL_AMD_sample_positions
		glSetMultisamplefvAMD = (PFNGLSETMULTISAMPLEFVAMDPROC)glfwGetProcAddress("glSetMultisamplefvAMD");

		// GL_NV_shader_buffer_load
		glGetBufferParameterui64vNV = (PFNGLGETBUFFERPARAMETERUI64VNVPROC)glfwGetProcAddress("glGetBufferParameterui64vNV");
		glGetIntegerui64vNV = (PFNGLGETINTEGERUI64VNVPROC)glfwGetProcAddress("glGetIntegerui64vNV");
		glGetNamedBufferParameterui64vNV = (PFNGLGETNAMEDBUFFERPARAMETERUI64VNVPROC)glfwGetProcAddress("glGetNamedBufferParameterui64vNV");
		glIsBufferResidentNV = (PFNGLISBUFFERRESIDENTNVPROC)glfwGetProcAddress("glIsBufferResidentNV");
		glIsNamedBufferResidentNV = (PFNGLISNAMEDBUFFERRESIDENTNVPROC)glfwGetProcAddress("glIsNamedBufferResidentNV");
		glMakeBufferNonResidentNV = (PFNGLMAKEBUFFERNONRESIDENTNVPROC)glfwGetProcAddress("glMakeBufferNonResidentNV");
		glMakeBufferResidentNV = (PFNGLMAKEBUFFERRESIDENTNVPROC)glfwGetProcAddress("glMakeBufferResidentNV");
		glMakeNamedBufferNonResidentNV = (PFNGLMAKENAMEDBUFFERNONRESIDENTNVPROC)glfwGetProcAddress("glMakeNamedBufferNonResidentNV");
		glMakeNamedBufferResidentNV = (PFNGLMAKENAMEDBUFFERRESIDENTNVPROC)glfwGetProcAddress("glMakeNamedBufferResidentNV");
		glProgramUniformui64NV = (PFNGLPROGRAMUNIFORMUI64NVPROC)glfwGetProcAddress("glProgramUniformui64NV");
		glProgramUniformui64vNV = (PFNGLPROGRAMUNIFORMUI64VNVPROC)glfwGetProcAddress("glProgramUniformui64vNV");
		glUniformui64NV = (PFNGLUNIFORMUI64NVPROC)glfwGetProcAddress("glUniformui64NV");
		glUniformui64vNV = (PFNGLUNIFORMUI64VNVPROC)glfwGetProcAddress("glUniformui64vNV");

		// GL_NV_vertex_buffer_unified_memory
		glBufferAddressRangeNV = (PFNGLBUFFERADDRESSRANGENVPROC)glfwGetProcAddress("glBufferAddressRangeNV");
		glVertexAttribFormatNV = (PFNGLVERTEXATTRIBFORMATNVPROC)glfwGetProcAddress("glVertexAttribFormatNV");
		glVertexAttribIFormatNV = (PFNGLVERTEXATTRIBIFORMATNVPROC)glfwGetProcAddress("glVertexAttribIFormatNV");
		glGetIntegerui64i_vNV = (PFNGLGETINTEGERUI64I_VNVPROC)glfwGetProcAddress("glGetIntegerui64i_vNV");

		// GL_NV_texture_multisample
		glTexImage2DMultisampleCoverageNV = (PFNGLTEXIMAGE2DMULTISAMPLECOVERAGENVPROC)glfwGetProcAddress("glTexImage2DMultisampleCoverageNV");
		glTexImage3DMultisampleCoverageNV = (PFNGLTEXIMAGE3DMULTISAMPLECOVERAGENVPROC)glfwGetProcAddress("glTexImage3DMultisampleCoverageNV");
		glTextureImage2DMultisampleCoverageNV = (PFNGLTEXTUREIMAGE2DMULTISAMPLECOVERAGENVPROC)glfwGetProcAddress("glTextureImage2DMultisampleCoverageNV");
		glTextureImage2DMultisampleNV = (PFNGLTEXTUREIMAGE2DMULTISAMPLENVPROC)glfwGetProcAddress("glTextureImage2DMultisampleNV");
		glTextureImage3DMultisampleCoverageNV = (PFNGLTEXTUREIMAGE3DMULTISAMPLECOVERAGENVPROC)glfwGetProcAddress("glTextureImage3DMultisampleCoverageNV");
		glTextureImage3DMultisampleNV = (PFNGLTEXTUREIMAGE3DMULTISAMPLENVPROC)glfwGetProcAddress("glTextureImage3DMultisampleNV");

		// GL_NV_explicit_multisample
		glGetMultisamplefvNV = (PFNGLGETMULTISAMPLEFVNVPROC)glfwGetProcAddress("glGetMultisamplefvNV");
		glSampleMaskIndexedNV = (PFNGLSAMPLEMASKINDEXEDNVPROC)glfwGetProcAddress("glSampleMaskIndexedNV");
		glTexRenderbufferNV = (PFNGLTEXRENDERBUFFERNVPROC)glfwGetProcAddress("glTexRenderbufferNV");

		// GL_NV_bindless_texture
		glGetImageHandleNV = (PFNGLGETIMAGEHANDLENVPROC)glfwGetProcAddress("glGetImageHandleNV");
		glGetTextureHandleNV = (PFNGLGETTEXTUREHANDLENVPROC)glfwGetProcAddress("glGetTextureHandleNV");
		glGetTextureSamplerHandleNV = (PFNGLGETTEXTURESAMPLERHANDLENVPROC)glfwGetProcAddress("glGetTextureSamplerHandleNV");
		glIsImageHandleResidentNV = (PFNGLISIMAGEHANDLERESIDENTNVPROC)glfwGetProcAddress("glIsImageHandleResidentNV");
		glIsTextureHandleResidentNV = (PFNGLISTEXTUREHANDLERESIDENTNVPROC)glfwGetProcAddress("glIsTextureHandleResidentNV");
		glMakeImageHandleNonResidentNV = (PFNGLMAKEIMAGEHANDLENONRESIDENTNVPROC)glfwGetProcAddress("glMakeImageHandleNonResidentNV");
		glMakeImageHandleResidentNV = (PFNGLMAKEIMAGEHANDLERESIDENTNVPROC)glfwGetProcAddress("glMakeImageHandleResidentNV");
		glMakeTextureHandleNonResidentNV = (PFNGLMAKETEXTUREHANDLENONRESIDENTNVPROC)glfwGetProcAddress("glMakeTextureHandleNonResidentNV");
		glMakeTextureHandleResidentNV = (PFNGLMAKETEXTUREHANDLERESIDENTNVPROC)glfwGetProcAddress("glMakeTextureHandleResidentNV");
		glProgramUniformHandleui64NV = (PFNGLPROGRAMUNIFORMHANDLEUI64NVPROC)glfwGetProcAddress("glProgramUniformHandleui64NV");
		glProgramUniformHandleui64vNV = (PFNGLPROGRAMUNIFORMHANDLEUI64VNVPROC)glfwGetProcAddress("glProgramUniformHandleui64vNV");
		glUniformHandleui64NV = (PFNGLUNIFORMHANDLEUI64NVPROC)glfwGetProcAddress("glUniformHandleui64NV");
		glUniformHandleui64vNV = (PFNGLUNIFORMHANDLEUI64VNVPROC)glfwGetProcAddress("glUniformHandleui64vNV");

		// GL_EXT_texture_compression_s3tc
		#define GL_COMPRESSED_RGB_S3TC_DXT1_EXT			0x83F0
		#define GL_COMPRESSED_RGBA_S3TC_DXT1_EXT		0x83F1
		#define GL_COMPRESSED_RGBA_S3TC_DXT3_EXT		0x83F2
		#define GL_COMPRESSED_RGBA_S3TC_DXT5_EXT		0x83F3

		// GL_ARB_debug_output
		glDebugMessageControlARB = (PFNGLDEBUGMESSAGECONTROLPROC)glfwGetProcAddress("glDebugMessageControlARB");
		glDebugMessageInsertARB = (PFNGLDEBUGMESSAGEINSERTARBPROC)glfwGetProcAddress("glDebugMessageInsertARB");
		glDebugMessageCallbackARB = (PFNGLDEBUGMESSAGECALLBACKPROC)glfwGetProcAddress("glDebugMessageCallbackARB");
		glGetDebugMessageLogARB = (PFNGLGETDEBUGMESSAGELOGARBPROC)glfwGetProcAddress("glGetDebugMessageLogARB");

		// GL_AMD_sparse_texture
		glTexStorageSparseAMD = (PFNGLTEXSTORAGESPARSEAMDPROC)glfwGetProcAddress("glTexStorageSparseAMD");
		glTextureStorageSparseAMD = (PFNGLTEXTURESTORAGESPARSEAMDPROC)glfwGetProcAddress("glTextureStorageSparseAMD");
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
