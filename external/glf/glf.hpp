#ifndef GLF_INCLUDED
#define GLF_INCLUDED

//#pragma warning(disable : once)

#ifdef WIN32
#	define WIN32_LEAN_AND_MEAN
#	include <windows.h>
#	include <GL/gl.h>
#	include <GL/glcorearb.h>
//#	include <GL/glext.h>
#elif defined(linux) || defined(__linux)
#	include <GL/glew.h>
#	define GL_GLEXT_PROTOTYPES 1
#	include <GL/gl.h>
#	include <GL/glext.h>
#elif defined(__APPLE__)
#	include <OpenGL/gl3.h>
#   include <GLUT/glut.h>
#else
#	error "Unsupported platform"
#endif
#include <GL/glfw.h>

#if (defined(WIN32))
	// OpenGL 1.0
	// PFNGLENABLEPROC glEnable(0);
	// PFNGLVIEWPORTPROC glViewport(0);

	// OpenGL 1.1
	// PFNGLGETINTEGERVPROC glGetIntegerv(0);
	// PFNGLGETERRORPROC glGetError(0);

	// OpenGL 1.2 
	PFNGLBLENDCOLORPROC glBlendColor(0);
	PFNGLBLENDEQUATIONPROC glBlendEquation(0);
	PFNGLDRAWRANGEELEMENTSPROC glDrawRangeElements(0);
	PFNGLTEXIMAGE3DPROC glTexImage3D(0);
	PFNGLTEXSUBIMAGE3DPROC glTexSubImage3D(0);
	PFNGLCOPYTEXSUBIMAGE3DPROC glCopyTexSubImage3D(0);

	// OpenGL 1.3
	PFNGLACTIVETEXTUREPROC glActiveTexture(0);
	PFNGLSAMPLECOVERAGEPROC glSampleCoverage(0);
	PFNGLCOMPRESSEDTEXIMAGE3DPROC glCompressedTexImage3D(0);
	PFNGLCOMPRESSEDTEXIMAGE2DPROC glCompressedTexImage2D(0);
	PFNGLCOMPRESSEDTEXIMAGE1DPROC glCompressedTexImage1D(0);
	PFNGLCOMPRESSEDTEXSUBIMAGE3DPROC glCompressedTexSubImage3D(0);
	PFNGLCOMPRESSEDTEXSUBIMAGE2DPROC glCompressedTexSubImage2D(0);
	PFNGLCOMPRESSEDTEXSUBIMAGE1DPROC glCompressedTexSubImage1D(0);
	PFNGLGETCOMPRESSEDTEXIMAGEPROC glGetCompressedTexImage(0);

	// OpenGL 1.4
	PFNGLBLENDFUNCSEPARATEPROC glBlendFuncSeparate(0);
	PFNGLPOINTPARAMETERFPROC glPointParameterf(0);
	PFNGLPOINTPARAMETERFVPROC glPointParameterfv(0);
	PFNGLPOINTPARAMETERIPROC glPointParameteri(0);
	PFNGLPOINTPARAMETERIVPROC glPointParameteriv(0);

	// OpenGL 1.5
	PFNGLGENQUERIESPROC glGenQueries(0);
	PFNGLDELETEQUERIESPROC glDeleteQueries(0);
	PFNGLISQUERYPROC glIsQuery(0);
	PFNGLBEGINQUERYPROC glBeginQuery(0);
	PFNGLENDQUERYPROC glEndQuery(0);
	PFNGLGETQUERYIVPROC glGetQueryiv(0);
	PFNGLGETQUERYOBJECTIVPROC glGetQueryObjectiv(0);
	PFNGLGETQUERYOBJECTUIVPROC glGetQueryObjectuiv(0);
	PFNGLBINDBUFFERPROC glBindBuffer(0);
	PFNGLDELETEBUFFERSPROC glDeleteBuffers(0);
	PFNGLGENBUFFERSPROC glGenBuffers(0);
	PFNGLISBUFFERPROC glIsBuffer(0);
	PFNGLBUFFERDATAPROC glBufferData(0);
	PFNGLBUFFERSUBDATAPROC glBufferSubData(0);
	PFNGLGETBUFFERSUBDATAPROC glGetBufferSubData(0);
	PFNGLUNMAPBUFFERPROC glUnmapBuffer(0);
	PFNGLGETBUFFERPARAMETERIVPROC glGetBufferParameteriv(0);
	PFNGLGETBUFFERPOINTERVPROC glGetBufferPointerv(0);

	// OpenGL 2.0
	PFNGLBLENDEQUATIONSEPARATEPROC glBlendEquationSeparate(0);
	PFNGLDRAWBUFFERSPROC glDrawBuffers(0);
	PFNGLSTENCILOPSEPARATEPROC glStencilOpSeparate(0);
	PFNGLSTENCILFUNCSEPARATEPROC glStencilFuncSeparate(0);
	PFNGLSTENCILMASKSEPARATEPROC glStencilMaskSeparate(0);

	// Program and shader
	PFNGLCREATESHADERPROC glCreateShader(0);
	PFNGLDELETESHADERPROC glDeleteShader(0);
	PFNGLCREATEPROGRAMPROC glCreateProgram(0);
	PFNGLDELETEPROGRAMPROC glDeleteProgram(0);
	PFNGLSHADERSOURCEPROC glShaderSource(0);
	PFNGLCOMPILESHADERPROC glCompileShader(0);
	PFNGLGETSHADERIVPROC glGetShaderiv(0);
	PFNGLVALIDATEPROGRAMPROC glValidateProgram(0);
	PFNGLGETPROGRAMIVPROC glGetProgramiv(0);
	PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog(0);
	PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog(0);
	PFNGLATTACHSHADERPROC glAttachShader(0);
	PFNGLLINKPROGRAMPROC glLinkProgram(0);
	PFNGLUSEPROGRAMPROC glUseProgram(0);
	PFNGLGETVERTEXATTRIBDVPROC glGetVertexAttribdv(0);
	PFNGLGETVERTEXATTRIBFVPROC glGetVertexAttribfv(0);
	PFNGLGETVERTEXATTRIBIVPROC glGetVertexAttribiv(0);
	PFNGLGETVERTEXATTRIBPOINTERVPROC glGetVertexAttribPointerv(0);
	PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer(0);
	PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray(0);
	PFNGLBINDATTRIBLOCATIONPROC glBindAttribLocation(0);
	PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation(0);
	PFNGLGETATTRIBLOCATIONPROC glGetAttribLocation(0);
	PFNGLUNIFORM1FPROC glUniform1f(0);
	PFNGLUNIFORM2FPROC glUniform2f(0);
	PFNGLUNIFORM3FPROC glUniform3f(0);
	PFNGLUNIFORM4FPROC glUniform4f(0);
	PFNGLUNIFORM1IPROC glUniform1i(0);
	PFNGLUNIFORM2IPROC glUniform2i(0);
	PFNGLUNIFORM3IPROC glUniform3i(0);
	PFNGLUNIFORM4IPROC glUniform4i(0);
	PFNGLUNIFORM1FVPROC glUniform1fv(0);
	PFNGLUNIFORM2FVPROC glUniform2fv(0);
	PFNGLUNIFORM3FVPROC glUniform3fv(0);
	PFNGLUNIFORM4FVPROC glUniform4fv(0);
	PFNGLUNIFORM1IVPROC glUniform1iv(0);
	PFNGLUNIFORM2IVPROC glUniform2iv(0);
	PFNGLUNIFORM3IVPROC glUniform3iv(0);
	PFNGLUNIFORM4IVPROC glUniform4iv(0);
	PFNGLUNIFORMMATRIX2FVPROC glUniformMatrix2fv(0);
	PFNGLUNIFORMMATRIX3FVPROC glUniformMatrix3fv(0);
	PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv(0);
	PFNGLGETACTIVEATTRIBPROC glGetActiveAttrib(0);
	PFNGLGETACTIVEUNIFORMPROC glGetActiveUniform(0);
		
	// OpenGL 3.0
	PFNGLCOLORMASKIPROC glColorMaski(0);
	PFNGLGETBOOLEANI_VPROC glGetBooleani_v(0);
	PFNGLGETINTEGERI_VPROC glGetIntegeri_v(0);
	PFNGLENABLEIPROC glEnablei(0);
	PFNGLDISABLEIPROC glDisablei(0);
	PFNGLISENABLEDIPROC glIsEnabledi(0);
	PFNGLBEGINTRANSFORMFEEDBACKPROC glBeginTransformFeedback(0);
	PFNGLENDTRANSFORMFEEDBACKPROC glEndTransformFeedback(0);
	PFNGLBINDBUFFERRANGEPROC glBindBufferRange(0);
	PFNGLBINDBUFFERBASEPROC glBindBufferBase(0);
	PFNGLTRANSFORMFEEDBACKVARYINGSPROC glTransformFeedbackVaryings(0);
	PFNGLGETTRANSFORMFEEDBACKVARYINGPROC glGetTransformFeedbackVarying(0);
	PFNGLCLAMPCOLORPROC glClampColor(0);
	PFNGLBEGINCONDITIONALRENDERPROC glBeginConditionalRender(0);
	PFNGLENDCONDITIONALRENDERPROC glEndConditionalRender(0);
	PFNGLVERTEXATTRIBIPOINTERPROC glVertexAttribIPointer(0);
	PFNGLGETVERTEXATTRIBIIVPROC glGetVertexAttribIiv(0);
	PFNGLGETVERTEXATTRIBIUIVPROC glGetVertexAttribIuiv(0);
	PFNGLVERTEXATTRIBI1IPROC glVertexAttribI1i(0);
	PFNGLVERTEXATTRIBI2IPROC glVertexAttribI2i(0);
	PFNGLVERTEXATTRIBI3IPROC glVertexAttribI3i(0);
	PFNGLVERTEXATTRIBI4IPROC glVertexAttribI4i(0);
	PFNGLVERTEXATTRIBI1UIPROC glVertexAttribI1ui(0);
	PFNGLVERTEXATTRIBI2UIPROC glVertexAttribI2ui(0);
	PFNGLVERTEXATTRIBI3UIPROC glVertexAttribI3ui(0);
	PFNGLVERTEXATTRIBI4UIPROC glVertexAttribI4ui(0);
	PFNGLVERTEXATTRIBI1IVPROC glVertexAttribI1iv(0);
	PFNGLVERTEXATTRIBI2IVPROC glVertexAttribI2iv(0);
	PFNGLVERTEXATTRIBI3IVPROC glVertexAttribI3iv(0);
	PFNGLVERTEXATTRIBI4IVPROC glVertexAttribI4iv(0);
	PFNGLVERTEXATTRIBI1UIVPROC glVertexAttribI1uiv(0);
	PFNGLVERTEXATTRIBI2UIVPROC glVertexAttribI2uiv(0);
	PFNGLVERTEXATTRIBI3UIVPROC glVertexAttribI3uiv(0);
	PFNGLVERTEXATTRIBI4UIVPROC glVertexAttribI4uiv(0);
	PFNGLVERTEXATTRIBI4BVPROC glVertexAttribI4bv(0);
	PFNGLVERTEXATTRIBI4SVPROC glVertexAttribI4sv(0);
	PFNGLVERTEXATTRIBI4UBVPROC glVertexAttribI4ubv(0);
	PFNGLVERTEXATTRIBI4USVPROC glVertexAttribI4usv(0);
	PFNGLGETUNIFORMUIVPROC glGetUniformuiv(0);
	PFNGLBINDFRAGDATALOCATIONPROC glBindFragDataLocation(0);
	PFNGLGETFRAGDATALOCATIONPROC glGetFragDataLocation(0);
	PFNGLUNIFORM1UIPROC glUniform1ui(0);
	PFNGLUNIFORM2UIPROC glUniform2ui(0);
	PFNGLUNIFORM3UIPROC glUniform3ui(0);
	PFNGLUNIFORM4UIPROC glUniform4ui(0);
	PFNGLUNIFORM1UIVPROC glUniform1uiv(0);
	PFNGLUNIFORM2UIVPROC glUniform2uiv(0);
	PFNGLUNIFORM3UIVPROC glUniform3uiv(0);
	PFNGLUNIFORM4UIVPROC glUniform4uiv(0);
	PFNGLTEXPARAMETERIIVPROC glTexParameterIiv(0);
	PFNGLTEXPARAMETERIUIVPROC glTexParameterIuiv(0);
	PFNGLGETTEXPARAMETERIIVPROC glGetTexParameterIiv(0);
	PFNGLGETTEXPARAMETERIUIVPROC glGetTexParameterIuiv(0);
	PFNGLCLEARBUFFERIVPROC glClearBufferiv(0);
	PFNGLCLEARBUFFERUIVPROC glClearBufferuiv(0);
	PFNGLCLEARBUFFERFVPROC glClearBufferfv(0);
	PFNGLCLEARBUFFERFIPROC glClearBufferfi(0);
	PFNGLGETSTRINGIPROC glGetStringi(0);

	// GL_ARB_framebuffer_object
	PFNGLISRENDERBUFFERPROC glIsRenderbuffer(0);
	PFNGLBINDRENDERBUFFERPROC glBindRenderbuffer(0);
	PFNGLDELETERENDERBUFFERSPROC glDeleteRenderbuffers(0);
	PFNGLGENRENDERBUFFERSPROC glGenRenderbuffers(0);
	PFNGLRENDERBUFFERSTORAGEPROC glRenderbufferStorage(0);
	PFNGLGETRENDERBUFFERPARAMETERIVPROC glGetRenderbufferParameteriv(0);
	PFNGLISFRAMEBUFFERPROC glIsFramebuffer(0);
	PFNGLBINDFRAMEBUFFERPROC glBindFramebuffer(0);
	PFNGLDELETEFRAMEBUFFERSPROC glDeleteFramebuffers(0);
	PFNGLGENFRAMEBUFFERSPROC glGenFramebuffers(0);
	PFNGLCHECKFRAMEBUFFERSTATUSPROC glCheckFramebufferStatus(0);
	PFNGLFRAMEBUFFERTEXTURE1DPROC glFramebufferTexture1D(0);
	PFNGLFRAMEBUFFERTEXTURE2DPROC glFramebufferTexture2D(0);
	PFNGLFRAMEBUFFERTEXTURE3DPROC glFramebufferTexture3D(0);
	PFNGLFRAMEBUFFERRENDERBUFFERPROC glFramebufferRenderbuffer(0);
	PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC glGetFramebufferAttachmentParameteriv(0);
	PFNGLGENERATEMIPMAPPROC glGenerateMipmap(0);
	PFNGLBLITFRAMEBUFFERPROC glBlitFramebuffer(0);
	PFNGLRENDERBUFFERSTORAGEMULTISAMPLEPROC glRenderbufferStorageMultisample(0);
	PFNGLFRAMEBUFFERTEXTURELAYERPROC glFramebufferTextureLayer(0);

	// GL_ARB_vertex_array_object
	PFNGLBINDVERTEXARRAYPROC glBindVertexArray(0);
	PFNGLDELETEVERTEXARRAYSPROC glDeleteVertexArrays(0);
	PFNGLGENVERTEXARRAYSPROC glGenVertexArrays(0);
	PFNGLISVERTEXARRAYPROC glIsVertexArray(0);

	// GL_ARB_provoking_vertex
	PFNGLPROVOKINGVERTEXPROC glProvokingVertex(0);

	// OpenGL 3.1
	PFNGLDRAWARRAYSINSTANCEDPROC glDrawArraysInstanced(0);
	PFNGLDRAWELEMENTSINSTANCEDPROC glDrawElementsInstanced(0);
	PFNGLTEXBUFFERPROC glTexBuffer(0);
	PFNGLPRIMITIVERESTARTINDEXPROC glPrimitiveRestartIndex(0);

	// GL_ARB_copy_buffer
	PFNGLCOPYBUFFERSUBDATAPROC glCopyBufferSubData(0);

	// OpenGL 3.2
	PFNGLGETINTEGER64I_VPROC glGetInteger64i_v(0);
	PFNGLGETBUFFERPARAMETERI64VPROC glGetBufferParameteri64v(0);
	PFNGLFRAMEBUFFERTEXTUREPROC glFramebufferTexture(0);

	// GL_ARB_map_buffer_range
	PFNGLMAPBUFFERRANGEPROC glMapBufferRange(0);
	PFNGLFLUSHMAPPEDBUFFERRANGEPROC glFlushMappedBufferRange(0);

	// GL_ARB_draw_elements_base_vertex
	PFNGLDRAWELEMENTSBASEVERTEXPROC glDrawElementsBaseVertex(0);
	PFNGLDRAWRANGEELEMENTSBASEVERTEXPROC glDrawRangeElementsBaseVertex(0);
	PFNGLDRAWELEMENTSINSTANCEDBASEVERTEXPROC glDrawElementsInstancedBaseVertex(0);
	PFNGLMULTIDRAWELEMENTSBASEVERTEXPROC glMultiDrawElementsBaseVertex(0);

	// GL_ARB_uniform_buffer_object
	PFNGLGETUNIFORMINDICESPROC glGetUniformIndices(0);
	PFNGLGETACTIVEUNIFORMSIVPROC glGetActiveUniformsiv(0);
	PFNGLGETACTIVEUNIFORMNAMEPROC glGetActiveUniformName(0);
	PFNGLGETUNIFORMBLOCKINDEXPROC glGetUniformBlockIndex(0);
	PFNGLGETACTIVEUNIFORMBLOCKIVPROC glGetActiveUniformBlockiv(0);
	PFNGLGETACTIVEUNIFORMBLOCKNAMEPROC glGetActiveUniformBlockName(0);
	PFNGLUNIFORMBLOCKBINDINGPROC glUniformBlockBinding(0);

	// GL_ARB_texture_multisample
	PFNGLTEXIMAGE2DMULTISAMPLEPROC glTexImage2DMultisample(0);
	PFNGLTEXIMAGE3DMULTISAMPLEPROC glTexImage3DMultisample(0);
	PFNGLGETMULTISAMPLEFVPROC glGetMultisamplefv(0);
	PFNGLSAMPLEMASKIPROC glSampleMaski(0);

	// GL_ARB_sync
	PFNGLFENCESYNCPROC glFenceSync(0);
	PFNGLISSYNCPROC glIsSync(0);
	PFNGLDELETESYNCPROC glDeleteSync(0);
	PFNGLCLIENTWAITSYNCPROC glClientWaitSync(0);
	PFNGLWAITSYNCPROC glWaitSync(0);
	PFNGLGETINTEGER64VPROC glGetInteger64v(0);
	PFNGLGETSYNCIVPROC glGetSynciv(0);

	// OpenGL 3.3
	PFNGLVERTEXATTRIBDIVISORPROC glVertexAttribDivisor(0);

	// GL_ARB_sampler_objects
	PFNGLGENSAMPLERSPROC glGenSamplers(0);
	PFNGLDELETESAMPLERSPROC glDeleteSamplers(0);
	PFNGLISSAMPLERPROC glIsSampler(0);
	PFNGLBINDSAMPLERPROC glBindSampler(0);
	PFNGLSAMPLERPARAMETERIPROC glSamplerParameteri(0);
	PFNGLSAMPLERPARAMETERIVPROC glSamplerParameteriv(0);
	PFNGLSAMPLERPARAMETERFPROC glSamplerParameterf(0);
	PFNGLSAMPLERPARAMETERFVPROC glSamplerParameterfv(0);
	PFNGLSAMPLERPARAMETERIIVPROC glSamplerParameterIiv(0);
	PFNGLSAMPLERPARAMETERIUIVPROC glSamplerParameterIuiv(0);
	PFNGLGETSAMPLERPARAMETERIVPROC glGetSamplerParameteriv(0);
	PFNGLGETSAMPLERPARAMETERIIVPROC glGetSamplerParameterIiv(0);
	PFNGLGETSAMPLERPARAMETERFVPROC glGetSamplerParameterfv(0);
	PFNGLGETSAMPLERPARAMETERIUIVPROC glGetSamplerParameterIuiv(0);

	// OpenGL 4.0
	PFNGLMINSAMPLESHADINGPROC glMinSampleShading(0);
	PFNGLBLENDEQUATIONIPROC glBlendEquationi(0);
	PFNGLBLENDEQUATIONSEPARATEIPROC glBlendEquationSeparatei(0);
	PFNGLBLENDFUNCIPROC glBlendFunci(0);
	PFNGLBLENDFUNCSEPARATEIPROC glBlendFuncSeparatei(0);

	// GL_ARB_draw_indirect
	PFNGLDRAWARRAYSINDIRECTPROC glDrawArraysIndirect(0);
	PFNGLDRAWELEMENTSINDIRECTPROC glDrawElementsIndirect(0);

	// GL_ARB_gpu_shader_fp64
	PFNGLUNIFORM1DPROC glUniform1d(0);
	PFNGLUNIFORM2DPROC glUniform2d(0);
	PFNGLUNIFORM3DPROC glUniform3d(0);
	PFNGLUNIFORM4DPROC glUniform4d(0);
	PFNGLUNIFORM1DVPROC glUniform1dv(0);
	PFNGLUNIFORM2DVPROC glUniform2dv(0);
	PFNGLUNIFORM3DVPROC glUniform3dv(0);
	PFNGLUNIFORM4DVPROC glUniform4dv(0);
	PFNGLUNIFORMMATRIX2DVPROC glUniformMatrix2dv(0);
	PFNGLUNIFORMMATRIX3DVPROC glUniformMatrix3dv(0);
	PFNGLUNIFORMMATRIX4DVPROC glUniformMatrix4dv(0);
	PFNGLUNIFORMMATRIX2X3DVPROC glUniformMatrix2x3dv(0);
	PFNGLUNIFORMMATRIX2X4DVPROC glUniformMatrix2x4dv(0);
	PFNGLUNIFORMMATRIX3X2DVPROC glUniformMatrix3x2dv(0);
	PFNGLUNIFORMMATRIX3X4DVPROC glUniformMatrix3x4dv(0);
	PFNGLUNIFORMMATRIX4X2DVPROC glUniformMatrix4x2dv(0);
	PFNGLUNIFORMMATRIX4X3DVPROC glUniformMatrix4x3dv(0);
	PFNGLGETUNIFORMDVPROC glGetUniformdv(0);

	// GL_ARB_shader_subroutine
	PFNGLGETSUBROUTINEUNIFORMLOCATIONPROC glGetSubroutineUniformLocation(0);
	PFNGLGETSUBROUTINEINDEXPROC glGetSubroutineIndex(0);
	PFNGLGETACTIVESUBROUTINEUNIFORMIVPROC glGetActiveSubroutineUniformiv(0);
	PFNGLGETACTIVESUBROUTINEUNIFORMNAMEPROC glGetActiveSubroutineUniformName(0);
	PFNGLGETACTIVESUBROUTINENAMEPROC glGetActiveSubroutineName(0);
	PFNGLUNIFORMSUBROUTINESUIVPROC glUniformSubroutinesuiv(0);
	PFNGLGETUNIFORMSUBROUTINEUIVPROC glGetUniformSubroutineuiv(0);
	PFNGLGETPROGRAMSTAGEIVPROC glGetProgramStageiv(0);

	// GL_ARB_tessellation_shader
	PFNGLPATCHPARAMETERIPROC glPatchParameteri(0);
	PFNGLPATCHPARAMETERFVPROC glPatchParameterfv(0);

	// GL_ARB_transform_feedback2
	PFNGLBINDTRANSFORMFEEDBACKPROC glBindTransformFeedback(0);
	PFNGLDELETETRANSFORMFEEDBACKSPROC glDeleteTransformFeedbacks(0);
	PFNGLGENTRANSFORMFEEDBACKSPROC glGenTransformFeedbacks(0);
	PFNGLISTRANSFORMFEEDBACKPROC glIsTransformFeedback(0);
	PFNGLPAUSETRANSFORMFEEDBACKPROC glPauseTransformFeedback(0);
	PFNGLRESUMETRANSFORMFEEDBACKPROC glResumeTransformFeedback(0);
	PFNGLDRAWTRANSFORMFEEDBACKPROC glDrawTransformFeedback(0);

	// GL_ARB_transform_feedback3
	PFNGLDRAWTRANSFORMFEEDBACKSTREAMPROC glDrawTransformFeedbackStream(0);
	PFNGLBEGINQUERYINDEXEDPROC glBeginQueryIndexed(0);
	PFNGLENDQUERYINDEXEDPROC glEndQueryIndexed(0);
	PFNGLGETQUERYINDEXEDIVPROC glGetQueryIndexediv(0);

	// GL_ARB_get_program_binary
	PFNGLGETPROGRAMBINARYPROC glGetProgramBinary(0);
	PFNGLPROGRAMBINARYPROC glProgramBinary(0);
	PFNGLPROGRAMPARAMETERIPROC glProgramParameteri(0);

	// GL_ARB_separate_shader_objects
	PFNGLUSEPROGRAMSTAGESPROC glUseProgramStages(0);
	PFNGLACTIVESHADERPROGRAMPROC glActiveShaderProgram(0);
	PFNGLCREATESHADERPROGRAMVPROC glCreateShaderProgramv(0);
	PFNGLBINDPROGRAMPIPELINEPROC glBindProgramPipeline(0);
	PFNGLDELETEPROGRAMPIPELINESPROC glDeleteProgramPipelines(0);
	PFNGLGENPROGRAMPIPELINESPROC glGenProgramPipelines(0);
	PFNGLISPROGRAMPIPELINEPROC glIsProgramPipeline(0);
	PFNGLGETPROGRAMPIPELINEIVPROC glGetProgramPipelineiv(0);
	PFNGLPROGRAMUNIFORM1IPROC glProgramUniform1i(0);
	PFNGLPROGRAMUNIFORM1IVPROC glProgramUniform1iv(0);
	PFNGLPROGRAMUNIFORM1FPROC glProgramUniform1f(0);
	PFNGLPROGRAMUNIFORM1FVPROC glProgramUniform1fv(0);
	PFNGLPROGRAMUNIFORM1DPROC glProgramUniform1d(0);
	PFNGLPROGRAMUNIFORM1DVPROC glProgramUniform1dv(0);
	PFNGLPROGRAMUNIFORM1UIPROC glProgramUniform1ui(0);
	PFNGLPROGRAMUNIFORM1UIVPROC glProgramUniform1uiv(0);
	PFNGLPROGRAMUNIFORM2IPROC glProgramUniform2i(0);
	PFNGLPROGRAMUNIFORM2IVPROC glProgramUniform2iv(0);
	PFNGLPROGRAMUNIFORM2FPROC glProgramUniform2f(0);
	PFNGLPROGRAMUNIFORM2FVPROC glProgramUniform2fv(0);
	PFNGLPROGRAMUNIFORM2DPROC glProgramUniform2d(0);
	PFNGLPROGRAMUNIFORM2DVPROC glProgramUniform2dv(0);
	PFNGLPROGRAMUNIFORM2UIPROC glProgramUniform2ui(0);
	PFNGLPROGRAMUNIFORM2UIVPROC glProgramUniform2uiv(0);
	PFNGLPROGRAMUNIFORM3IPROC glProgramUniform3i(0);
	PFNGLPROGRAMUNIFORM3IVPROC glProgramUniform3iv(0);
	PFNGLPROGRAMUNIFORM3FPROC glProgramUniform3f(0);
	PFNGLPROGRAMUNIFORM3FVPROC glProgramUniform3fv(0);
	PFNGLPROGRAMUNIFORM3DPROC glProgramUniform3d(0);
	PFNGLPROGRAMUNIFORM3DVPROC glProgramUniform3dv(0);
	PFNGLPROGRAMUNIFORM3UIPROC glProgramUniform3ui(0);
	PFNGLPROGRAMUNIFORM3UIVPROC glProgramUniform3uiv(0);
	PFNGLPROGRAMUNIFORM4IPROC glProgramUniform4i(0);
	PFNGLPROGRAMUNIFORM4IVPROC glProgramUniform4iv(0);
	PFNGLPROGRAMUNIFORM4FPROC glProgramUniform4f(0);
	PFNGLPROGRAMUNIFORM4FVPROC glProgramUniform4fv(0);
	PFNGLPROGRAMUNIFORM4DPROC glProgramUniform4d(0);
	PFNGLPROGRAMUNIFORM4DVPROC glProgramUniform4dv(0);
	PFNGLPROGRAMUNIFORM4UIPROC glProgramUniform4ui(0);
	PFNGLPROGRAMUNIFORM4UIVPROC glProgramUniform4uiv(0);
	PFNGLPROGRAMUNIFORMMATRIX2FVPROC glProgramUniformMatrix2fv(0);
	PFNGLPROGRAMUNIFORMMATRIX3FVPROC glProgramUniformMatrix3fv(0);
	PFNGLPROGRAMUNIFORMMATRIX4FVPROC glProgramUniformMatrix4fv(0);
	PFNGLPROGRAMUNIFORMMATRIX2DVPROC glProgramUniformMatrix2dv(0);
	PFNGLPROGRAMUNIFORMMATRIX3DVPROC glProgramUniformMatrix3dv(0);
	PFNGLPROGRAMUNIFORMMATRIX4DVPROC glProgramUniformMatrix4dv(0);
	PFNGLPROGRAMUNIFORMMATRIX2X3FVPROC glProgramUniformMatrix2x3fv(0);
	PFNGLPROGRAMUNIFORMMATRIX3X2FVPROC glProgramUniformMatrix3x2fv(0);
	PFNGLPROGRAMUNIFORMMATRIX2X4FVPROC glProgramUniformMatrix2x4fv(0);
	PFNGLPROGRAMUNIFORMMATRIX4X2FVPROC glProgramUniformMatrix4x2fv(0);
	PFNGLPROGRAMUNIFORMMATRIX3X4FVPROC glProgramUniformMatrix3x4fv(0);
	PFNGLPROGRAMUNIFORMMATRIX4X3FVPROC glProgramUniformMatrix4x3fv(0);
	PFNGLPROGRAMUNIFORMMATRIX2X3DVPROC glProgramUniformMatrix2x3dv(0);
	PFNGLPROGRAMUNIFORMMATRIX3X2DVPROC glProgramUniformMatrix3x2dv(0);
	PFNGLPROGRAMUNIFORMMATRIX2X4DVPROC glProgramUniformMatrix2x4dv(0);
	PFNGLPROGRAMUNIFORMMATRIX4X2DVPROC glProgramUniformMatrix4x2dv(0);
	PFNGLPROGRAMUNIFORMMATRIX3X4DVPROC glProgramUniformMatrix3x4dv(0);
	PFNGLPROGRAMUNIFORMMATRIX4X3DVPROC glProgramUniformMatrix4x3dv(0);
	PFNGLVALIDATEPROGRAMPIPELINEPROC glValidateProgramPipeline(0);
	PFNGLGETPROGRAMPIPELINEINFOLOGPROC glGetProgramPipelineInfoLog(0);

	// GL_ARB_vertex_attrib_64bit
	PFNGLVERTEXATTRIBL1DPROC glVertexAttribL1d(0);
	PFNGLVERTEXATTRIBL2DPROC glVertexAttribL2d(0);
	PFNGLVERTEXATTRIBL3DPROC glVertexAttribL3d(0);
	PFNGLVERTEXATTRIBL4DPROC glVertexAttribL4d(0);
	PFNGLVERTEXATTRIBL1DVPROC glVertexAttribL1dv(0);
	PFNGLVERTEXATTRIBL2DVPROC glVertexAttribL2dv(0);
	PFNGLVERTEXATTRIBL3DVPROC glVertexAttribL3dv(0);
	PFNGLVERTEXATTRIBL4DVPROC glVertexAttribL4dv(0);
	PFNGLVERTEXATTRIBLPOINTERPROC glVertexAttribLPointer(0);
	PFNGLGETVERTEXATTRIBLDVPROC glGetVertexAttribLdv(0);

	// GL_ARB_viewport_array
	PFNGLVIEWPORTARRAYVPROC glViewportArrayv(0);
	PFNGLVIEWPORTINDEXEDFPROC glViewportIndexedf(0);
	PFNGLVIEWPORTINDEXEDFVPROC glViewportIndexedfv(0);
	PFNGLSCISSORARRAYVPROC glScissorArrayv(0);
	PFNGLSCISSORINDEXEDPROC glScissorIndexed(0);
	PFNGLSCISSORINDEXEDVPROC glScissorIndexedv(0);
	PFNGLDEPTHRANGEARRAYVPROC glDepthRangeArrayv(0);
	PFNGLDEPTHRANGEINDEXEDPROC glDepthRangeIndexed(0);
	PFNGLGETFLOATI_VPROC glGetFloati_v(0);
	PFNGLGETDOUBLEI_VPROC glGetDoublei_v(0);

	// OpenGL 4.2
	// GL_ARB_texture_storage
	PFNGLTEXSTORAGE1DPROC glTexStorage1D(0);
	PFNGLTEXSTORAGE2DPROC glTexStorage2D(0);
	PFNGLTEXSTORAGE3DPROC glTexStorage3D(0);

	// GL_ARB_base_instance
	PFNGLDRAWARRAYSINSTANCEDBASEINSTANCEPROC glDrawArraysInstancedBaseInstance(0);
	PFNGLDRAWELEMENTSINSTANCEDBASEINSTANCEPROC glDrawElementsInstancedBaseInstance(0);
	PFNGLDRAWELEMENTSINSTANCEDBASEVERTEXBASEINSTANCEPROC glDrawElementsInstancedBaseVertexBaseInstance(0);

	// GL_ARB_shader_image_load_store
	PFNGLBINDIMAGETEXTUREPROC glBindImageTexture(0);
	PFNGLMEMORYBARRIERPROC glMemoryBarrier(0);

	// GL_ARB_internalformat_query
	PFNGLGETINTERNALFORMATIVPROC glGetInternalformativ(0);

	// GL_ARB_transform_feedback_instanced
	PFNGLDRAWTRANSFORMFEEDBACKINSTANCEDPROC glDrawTransformFeedbackInstanced(0);
	PFNGLDRAWTRANSFORMFEEDBACKSTREAMINSTANCEDPROC glDrawTransformFeedbackStreamInstanced(0);

	// OpenGL 4.3
	// GL_ARB_vertex_attrib_binding
	typedef void (APIENTRY * PFNGLVERTEXARRAYBINDVERTEXBUFFEREXTPROC) (GLuint vaobj, GLuint bindingindex, GLuint buffer, GLintptr offset, GLsizei stride);
	typedef void (APIENTRY * PFNGLVERTEXARRAYBINDVERTEXATTRIBFORMATEXTPROC) (GLuint vaobj, GLuint attribindex, GLint size, GLenum type, GLboolean normalized, GLuint relativeoffset);
	typedef void (APIENTRY * PFNGLVERTEXARRAYBINDVERTEXATTRIBIFORMATEXTPROC) (GLuint vaobj, GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset);
	typedef void (APIENTRY * PFNGLVERTEXARRAYBINDVERTEXATTRIBLFORMATEXTPROC) (GLuint vaobj, GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset);
	typedef void (APIENTRY * PFNGLVERTEXARRAYBINDVERTEXATTRIBBINDINGEXTPROC) (GLuint vaobj, GLuint attribindex, GLuint bindingindex);
	typedef void (APIENTRY * PFNGLVERTEXARRAYBINDVERTEXBINDINGDIVISOREXTPROC) (GLuint vaobj, GLuint bindingindex, GLuint divisor);

	PFNGLVERTEXARRAYBINDVERTEXBUFFEREXTPROC glVertexArrayBindVertexBufferEXT(0);
	PFNGLVERTEXARRAYBINDVERTEXATTRIBFORMATEXTPROC glVertexArrayVertexAttribFormatEXT(0);
	PFNGLVERTEXARRAYBINDVERTEXATTRIBIFORMATEXTPROC glVertexArrayVertexAttribIFormatEXT(0);
	PFNGLVERTEXARRAYBINDVERTEXATTRIBLFORMATEXTPROC glVertexArrayVertexAttribLFormatEXT(0);
	PFNGLVERTEXARRAYBINDVERTEXATTRIBBINDINGEXTPROC glVertexArrayVertexAttribBindingEXT(0);
	PFNGLVERTEXARRAYBINDVERTEXBINDINGDIVISOREXTPROC glVertexArrayVertexBindingDivisorEXT(0);

	// GL_ARB_clear_buffer_object
	PFNGLCLEARBUFFERDATAPROC glClearBufferData(0);
	PFNGLCLEARBUFFERSUBDATAPROC glClearBufferSubData(0);
	PFNGLCLEARNAMEDBUFFERDATAEXTPROC glClearNamedBufferDataEXT(0);
	PFNGLCLEARNAMEDBUFFERSUBDATAEXTPROC glClearNamedBufferSubDataEXT(0);

	// GL_ARB_compute_shader
	#define GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS 0x90EB

	PFNGLDISPATCHCOMPUTEPROC glDispatchCompute(0);
	PFNGLDISPATCHCOMPUTEINDIRECTPROC glDispatchComputeIndirect(0);

	// GL_ARB_copy_image
	PFNGLCOPYIMAGESUBDATAPROC glCopyImageSubData(0);

	// GL_ARB_multi_draw_indirect
	PFNGLMULTIDRAWARRAYSINDIRECTPROC glMultiDrawArraysIndirect(0);
	PFNGLMULTIDRAWELEMENTSINDIRECTPROC glMultiDrawElementsIndirect(0);

	// GL_ARB_program_interface_query
	PFNGLGETPROGRAMINTERFACEIVPROC glGetProgramInterfaceiv(0);
	PFNGLGETPROGRAMRESOURCEINDEXPROC glGetProgramResourceIndex(0);
	PFNGLGETPROGRAMRESOURCENAMEPROC glGetProgramResourceName(0);
	PFNGLGETPROGRAMRESOURCEIVPROC glGetProgramResourceiv(0);
	PFNGLGETPROGRAMRESOURCELOCATIONPROC glGetProgramResourceLocation(0);
	PFNGLGETPROGRAMRESOURCELOCATIONINDEXPROC glGetProgramResourceLocationIndex(0);

	// GL_ARB_texture_buffer_range
	PFNGLTEXBUFFERRANGEPROC glTexBufferRange(0);
	PFNGLTEXTUREBUFFERRANGEEXTPROC glTextureBufferRangeEXT(0);

	// GL_ARB_texture_storage_multisample
	PFNGLTEXSTORAGE2DMULTISAMPLEPROC glTexStorage2DMultisample(0);
	PFNGLTEXSTORAGE3DMULTISAMPLEPROC glTexStorage3DMultisample(0);
	PFNGLTEXTURESTORAGE2DMULTISAMPLEEXTPROC glTextureStorage2DMultisampleEXT(0);
	PFNGLTEXTURESTORAGE3DMULTISAMPLEEXTPROC glTextureStorage3DMultisampleEXT(0);

	// GL_ARB_texture_view
	PFNGLTEXTUREVIEWPROC glTextureView(0);

	// GL_KHR_debug
	PFNGLDEBUGMESSAGECONTROLPROC glDebugMessageControl(0);
	PFNGLDEBUGMESSAGEINSERTPROC glDebugMessageInsert(0);
	PFNGLDEBUGMESSAGECALLBACKPROC glDebugMessageCallback(0);
	PFNGLGETDEBUGMESSAGELOGPROC glGetDebugMessageLog(0);
	PFNGLPUSHDEBUGGROUPPROC glPushDebugGroup(0);
	PFNGLPOPDEBUGGROUPPROC glPopDebugGroup(0);
	PFNGLOBJECTLABELPROC glObjectLabel(0);
	PFNGLGETOBJECTLABELPROC glGetObjectLabel(0);
	PFNGLOBJECTPTRLABELPROC glObjectPtrLabel(0);
	PFNGLGETOBJECTPTRLABELPROC glGetObjectPtrLabel(0);

	// GL_EXT_direct_state_access
	typedef void (APIENTRY * PFNGLBINDMULTITEXTUREEXTPROC) (GLenum texunit, GLenum target, GLuint texture);
	typedef GLenum (APIENTRY * PFNGLCHECKNAMEDFRAMEBUFFERSTATUSEXTPROC) (GLuint framebuffer, GLenum target);
	typedef void (APIENTRY * PFNGLCLIENTATTRIBDEFAULTEXTPROC) (GLbitfield mask);
	typedef void (APIENTRY * PFNGLCOMPRESSEDMULTITEXIMAGE1DEXTPROC) (GLenum texunit, GLenum target, GLint level, GLenum internalformat, GLsizei width, GLint border, GLsizei imageSize, const void* data);
	typedef void (APIENTRY * PFNGLCOMPRESSEDMULTITEXIMAGE2DEXTPROC) (GLenum texunit, GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const void* data);
	typedef void (APIENTRY * PFNGLCOMPRESSEDMULTITEXIMAGE3DEXTPROC) (GLenum texunit, GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const void* data);
	typedef void (APIENTRY * PFNGLCOMPRESSEDMULTITEXSUBIMAGE1DEXTPROC) (GLenum texunit, GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLsizei imageSize, const void* data);
	typedef void (APIENTRY * PFNGLCOMPRESSEDMULTITEXSUBIMAGE2DEXTPROC) (GLenum texunit, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const void* data);
	typedef void (APIENTRY * PFNGLCOMPRESSEDMULTITEXSUBIMAGE3DEXTPROC) (GLenum texunit, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const void* data);
	typedef void (APIENTRY * PFNGLCOMPRESSEDTEXTUREIMAGE1DEXTPROC) (GLuint texture, GLenum target, GLint level, GLenum internalformat, GLsizei width, GLint border, GLsizei imageSize, const void* data);
	typedef void (APIENTRY * PFNGLCOMPRESSEDTEXTUREIMAGE2DEXTPROC) (GLuint texture, GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const void* data);
	typedef void (APIENTRY * PFNGLCOMPRESSEDTEXTUREIMAGE3DEXTPROC) (GLuint texture, GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const void* data);
	typedef void (APIENTRY * PFNGLCOMPRESSEDTEXTURESUBIMAGE1DEXTPROC) (GLuint texture, GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLsizei imageSize, const void* data);
	typedef void (APIENTRY * PFNGLCOMPRESSEDTEXTURESUBIMAGE2DEXTPROC) (GLuint texture, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const void* data);
	typedef void (APIENTRY * PFNGLCOMPRESSEDTEXTURESUBIMAGE3DEXTPROC) (GLuint texture, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const void* data);
	typedef void (APIENTRY * PFNGLCOPYMULTITEXIMAGE1DEXTPROC) (GLenum texunit, GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLint border);
	typedef void (APIENTRY * PFNGLCOPYMULTITEXIMAGE2DEXTPROC) (GLenum texunit, GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border);
	typedef void (APIENTRY * PFNGLCOPYMULTITEXSUBIMAGE1DEXTPROC) (GLenum texunit, GLenum target, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width);
	typedef void (APIENTRY * PFNGLCOPYMULTITEXSUBIMAGE2DEXTPROC) (GLenum texunit, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height);
	typedef void (APIENTRY * PFNGLCOPYMULTITEXSUBIMAGE3DEXTPROC) (GLenum texunit, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height);
	typedef void (APIENTRY * PFNGLCOPYTEXTUREIMAGE1DEXTPROC) (GLuint texture, GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLint border);
	typedef void (APIENTRY * PFNGLCOPYTEXTUREIMAGE2DEXTPROC) (GLuint texture, GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border);
	typedef void (APIENTRY * PFNGLCOPYTEXTURESUBIMAGE1DEXTPROC) (GLuint texture, GLenum target, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width);
	typedef void (APIENTRY * PFNGLCOPYTEXTURESUBIMAGE2DEXTPROC) (GLuint texture, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height);
	typedef void (APIENTRY * PFNGLCOPYTEXTURESUBIMAGE3DEXTPROC) (GLuint texture, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height);
	typedef void (APIENTRY * PFNGLDISABLECLIENTSTATEINDEXEDEXTPROC) (GLenum array, GLuint index);
	typedef void (APIENTRY * PFNGLDISABLECLIENTSTATEIEXTPROC) (GLenum array, GLuint index);
	typedef void (APIENTRY * PFNGLDISABLEVERTEXARRAYATTRIBEXTPROC) (GLuint vaobj, GLuint index);
	typedef void (APIENTRY * PFNGLDISABLEVERTEXARRAYEXTPROC) (GLuint vaobj, GLenum array);
	typedef void (APIENTRY * PFNGLENABLECLIENTSTATEINDEXEDEXTPROC) (GLenum array, GLuint index);
	typedef void (APIENTRY * PFNGLENABLECLIENTSTATEIEXTPROC) (GLenum array, GLuint index);
	typedef void (APIENTRY * PFNGLENABLEVERTEXARRAYATTRIBEXTPROC) (GLuint vaobj, GLuint index);
	typedef void (APIENTRY * PFNGLENABLEVERTEXARRAYEXTPROC) (GLuint vaobj, GLenum array);
	typedef void (APIENTRY * PFNGLFLUSHMAPPEDNAMEDBUFFERRANGEEXTPROC) (GLuint buffer, GLintptr offset, GLsizeiptr length);
	typedef void (APIENTRY * PFNGLFRAMEBUFFERDRAWBUFFEREXTPROC) (GLuint framebuffer, GLenum mode);
	typedef void (APIENTRY * PFNGLFRAMEBUFFERDRAWBUFFERSEXTPROC) (GLuint framebuffer, GLsizei n, const GLenum* bufs);
	typedef void (APIENTRY * PFNGLFRAMEBUFFERREADBUFFEREXTPROC) (GLuint framebuffer, GLenum mode);
	typedef void (APIENTRY * PFNGLGENERATEMULTITEXMIPMAPEXTPROC) (GLenum texunit, GLenum target);
	typedef void (APIENTRY * PFNGLGENERATETEXTUREMIPMAPEXTPROC) (GLuint texture, GLenum target);
	typedef void (APIENTRY * PFNGLGETCOMPRESSEDMULTITEXIMAGEEXTPROC) (GLenum texunit, GLenum target, GLint level, void* img);
	typedef void (APIENTRY * PFNGLGETCOMPRESSEDTEXTUREIMAGEEXTPROC) (GLuint texture, GLenum target, GLint level, void* img);
	typedef void (APIENTRY * PFNGLGETDOUBLEINDEXEDVEXTPROC) (GLenum target, GLuint index, GLdouble* params);
	typedef void (APIENTRY * PFNGLGETDOUBLEI_VEXTPROC) (GLenum pname, GLuint index, GLdouble* params);
	typedef void (APIENTRY * PFNGLGETFLOATINDEXEDVEXTPROC) (GLenum target, GLuint index, GLfloat* params);
	typedef void (APIENTRY * PFNGLGETFLOATI_VEXTPROC) (GLenum pname, GLuint index, GLfloat* params);
	typedef void (APIENTRY * PFNGLGETFRAMEBUFFERPARAMETERIVEXTPROC) (GLuint framebuffer, GLenum pname, GLint* param);
	typedef void (APIENTRY * PFNGLGETMULTITEXENVFVEXTPROC) (GLenum texunit, GLenum target, GLenum pname, GLfloat* params);
	typedef void (APIENTRY * PFNGLGETMULTITEXENVIVEXTPROC) (GLenum texunit, GLenum target, GLenum pname, GLint* params);
	typedef void (APIENTRY * PFNGLGETMULTITEXGENDVEXTPROC) (GLenum texunit, GLenum coord, GLenum pname, GLdouble* params);
	typedef void (APIENTRY * PFNGLGETMULTITEXGENFVEXTPROC) (GLenum texunit, GLenum coord, GLenum pname, GLfloat* params);
	typedef void (APIENTRY * PFNGLGETMULTITEXGENIVEXTPROC) (GLenum texunit, GLenum coord, GLenum pname, GLint* params);
	typedef void (APIENTRY * PFNGLGETMULTITEXIMAGEEXTPROC) (GLenum texunit, GLenum target, GLint level, GLenum format, GLenum type, void* pixels);
	typedef void (APIENTRY * PFNGLGETMULTITEXLEVELPARAMETERFVEXTPROC) (GLenum texunit, GLenum target, GLint level, GLenum pname, GLfloat* params);
	typedef void (APIENTRY * PFNGLGETMULTITEXLEVELPARAMETERIVEXTPROC) (GLenum texunit, GLenum target, GLint level, GLenum pname, GLint* params);
	typedef void (APIENTRY * PFNGLGETMULTITEXPARAMETERIIVEXTPROC) (GLenum texunit, GLenum target, GLenum pname, GLint* params);
	typedef void (APIENTRY * PFNGLGETMULTITEXPARAMETERIUIVEXTPROC) (GLenum texunit, GLenum target, GLenum pname, GLuint* params);
	typedef void (APIENTRY * PFNGLGETMULTITEXPARAMETERFVEXTPROC) (GLenum texunit, GLenum target, GLenum pname, GLfloat* params);
	typedef void (APIENTRY * PFNGLGETMULTITEXPARAMETERIVEXTPROC) (GLenum texunit, GLenum target, GLenum pname, GLint* params);
	typedef void (APIENTRY * PFNGLGETNAMEDBUFFERPARAMETERIVEXTPROC) (GLuint buffer, GLenum pname, GLint* params);
	typedef void (APIENTRY * PFNGLGETNAMEDBUFFERPOINTERVEXTPROC) (GLuint buffer, GLenum pname, void** params);
	typedef void (APIENTRY * PFNGLGETNAMEDBUFFERSUBDATAEXTPROC) (GLuint buffer, GLintptr offset, GLsizeiptr size, void* data);
	typedef void (APIENTRY * PFNGLGETNAMEDFRAMEBUFFERATTACHMENTPARAMETERIVEXTPROC) (GLuint framebuffer, GLenum attachment, GLenum pname, GLint* params);
	typedef void (APIENTRY * PFNGLGETNAMEDPROGRAMLOCALPARAMETERIIVEXTPROC) (GLuint program, GLenum target, GLuint index, GLint* params);
	typedef void (APIENTRY * PFNGLGETNAMEDPROGRAMLOCALPARAMETERIUIVEXTPROC) (GLuint program, GLenum target, GLuint index, GLuint* params);
	typedef void (APIENTRY * PFNGLGETNAMEDPROGRAMLOCALPARAMETERDVEXTPROC) (GLuint program, GLenum target, GLuint index, GLdouble* params);
	typedef void (APIENTRY * PFNGLGETNAMEDPROGRAMLOCALPARAMETERFVEXTPROC) (GLuint program, GLenum target, GLuint index, GLfloat* params);
	typedef void (APIENTRY * PFNGLGETNAMEDPROGRAMSTRINGEXTPROC) (GLuint program, GLenum target, GLenum pname, void* string);
	typedef void (APIENTRY * PFNGLGETNAMEDPROGRAMIVEXTPROC) (GLuint program, GLenum target, GLenum pname, GLint* params);
	typedef void (APIENTRY * PFNGLGETNAMEDRENDERBUFFERPARAMETERIVEXTPROC) (GLuint renderbuffer, GLenum pname, GLint* params);
	typedef void (APIENTRY * PFNGLGETPOINTERINDEXEDVEXTPROC) (GLenum target, GLuint index, GLvoid** params);
	typedef void (APIENTRY * PFNGLGETPOINTERI_VEXTPROC) (GLenum pname, GLuint index, GLvoid** params);
	typedef void (APIENTRY * PFNGLGETTEXTUREIMAGEEXTPROC) (GLuint texture, GLenum target, GLint level, GLenum format, GLenum type, void* pixels);
	typedef void (APIENTRY * PFNGLGETTEXTURELEVELPARAMETERFVEXTPROC) (GLuint texture, GLenum target, GLint level, GLenum pname, GLfloat* params);
	typedef void (APIENTRY * PFNGLGETTEXTURELEVELPARAMETERIVEXTPROC) (GLuint texture, GLenum target, GLint level, GLenum pname, GLint* params);
	typedef void (APIENTRY * PFNGLGETTEXTUREPARAMETERIIVEXTPROC) (GLuint texture, GLenum target, GLenum pname, GLint* params);
	typedef void (APIENTRY * PFNGLGETTEXTUREPARAMETERIUIVEXTPROC) (GLuint texture, GLenum target, GLenum pname, GLuint* params);
	typedef void (APIENTRY * PFNGLGETTEXTUREPARAMETERFVEXTPROC) (GLuint texture, GLenum target, GLenum pname, GLfloat* params);
	typedef void (APIENTRY * PFNGLGETTEXTUREPARAMETERIVEXTPROC) (GLuint texture, GLenum target, GLenum pname, GLint* params);
	typedef void (APIENTRY * PFNGLGETVERTEXARRAYINTEGERI_VEXTPROC) (GLuint vaobj, GLuint index, GLenum pname, GLint* param);
	typedef void (APIENTRY * PFNGLGETVERTEXARRAYINTEGERVEXTPROC) (GLuint vaobj, GLenum pname, GLint* param);
	typedef void (APIENTRY * PFNGLGETVERTEXARRAYPOINTERI_VEXTPROC) (GLuint vaobj, GLuint index, GLenum pname, GLvoid** param);
	typedef void (APIENTRY * PFNGLGETVERTEXARRAYPOINTERVEXTPROC) (GLuint vaobj, GLenum pname, GLvoid** param);
	typedef GLvoid * (APIENTRY * PFNGLMAPNAMEDBUFFEREXTPROC) (GLuint buffer, GLenum access);
	typedef GLvoid * (APIENTRY * PFNGLMAPNAMEDBUFFERRANGEEXTPROC) (GLuint buffer, GLintptr offset, GLsizeiptr length, GLbitfield access);
	typedef void (APIENTRY * PFNGLMATRIXFRUSTUMEXTPROC) (GLenum matrixMode, GLdouble l, GLdouble r, GLdouble b, GLdouble t, GLdouble n, GLdouble f);
	typedef void (APIENTRY * PFNGLMATRIXLOADIDENTITYEXTPROC) (GLenum matrixMode);
	typedef void (APIENTRY * PFNGLMATRIXLOADTRANSPOSEDEXTPROC) (GLenum matrixMode, const GLdouble* m);
	typedef void (APIENTRY * PFNGLMATRIXLOADTRANSPOSEFEXTPROC) (GLenum matrixMode, const GLfloat* m);
	typedef void (APIENTRY * PFNGLMATRIXLOADDEXTPROC) (GLenum matrixMode, const GLdouble* m);
	typedef void (APIENTRY * PFNGLMATRIXLOADFEXTPROC) (GLenum matrixMode, const GLfloat* m);
	typedef void (APIENTRY * PFNGLMATRIXMULTTRANSPOSEDEXTPROC) (GLenum matrixMode, const GLdouble* m);
	typedef void (APIENTRY * PFNGLMATRIXMULTTRANSPOSEFEXTPROC) (GLenum matrixMode, const GLfloat* m);
	typedef void (APIENTRY * PFNGLMATRIXMULTDEXTPROC) (GLenum matrixMode, const GLdouble* m);
	typedef void (APIENTRY * PFNGLMATRIXMULTFEXTPROC) (GLenum matrixMode, const GLfloat* m);
	typedef void (APIENTRY * PFNGLMATRIXORTHOEXTPROC) (GLenum matrixMode, GLdouble l, GLdouble r, GLdouble b, GLdouble t, GLdouble n, GLdouble f);
	typedef void (APIENTRY * PFNGLMATRIXPOPEXTPROC) (GLenum matrixMode);
	typedef void (APIENTRY * PFNGLMATRIXPUSHEXTPROC) (GLenum matrixMode);
	typedef void (APIENTRY * PFNGLMATRIXROTATEDEXTPROC) (GLenum matrixMode, GLdouble angle, GLdouble x, GLdouble y, GLdouble z);
	typedef void (APIENTRY * PFNGLMATRIXROTATEFEXTPROC) (GLenum matrixMode, GLfloat angle, GLfloat x, GLfloat y, GLfloat z);
	typedef void (APIENTRY * PFNGLMATRIXSCALEDEXTPROC) (GLenum matrixMode, GLdouble x, GLdouble y, GLdouble z);
	typedef void (APIENTRY * PFNGLMATRIXSCALEFEXTPROC) (GLenum matrixMode, GLfloat x, GLfloat y, GLfloat z);
	typedef void (APIENTRY * PFNGLMATRIXTRANSLATEDEXTPROC) (GLenum matrixMode, GLdouble x, GLdouble y, GLdouble z);
	typedef void (APIENTRY * PFNGLMATRIXTRANSLATEFEXTPROC) (GLenum matrixMode, GLfloat x, GLfloat y, GLfloat z);
	typedef void (APIENTRY * PFNGLMULTITEXBUFFEREXTPROC) (GLenum texunit, GLenum target, GLenum internalformat, GLuint buffer);
	typedef void (APIENTRY * PFNGLMULTITEXCOORDPOINTEREXTPROC) (GLenum texunit, GLint size, GLenum type, GLsizei stride, const void* pointer);
	typedef void (APIENTRY * PFNGLMULTITEXENVFEXTPROC) (GLenum texunit, GLenum target, GLenum pname, GLfloat param);
	typedef void (APIENTRY * PFNGLMULTITEXENVFVEXTPROC) (GLenum texunit, GLenum target, GLenum pname, const GLfloat* params);
	typedef void (APIENTRY * PFNGLMULTITEXENVIEXTPROC) (GLenum texunit, GLenum target, GLenum pname, GLint param);
	typedef void (APIENTRY * PFNGLMULTITEXENVIVEXTPROC) (GLenum texunit, GLenum target, GLenum pname, const GLint* params);
	typedef void (APIENTRY * PFNGLMULTITEXGENDEXTPROC) (GLenum texunit, GLenum coord, GLenum pname, GLdouble param);
	typedef void (APIENTRY * PFNGLMULTITEXGENDVEXTPROC) (GLenum texunit, GLenum coord, GLenum pname, const GLdouble* params);
	typedef void (APIENTRY * PFNGLMULTITEXGENFEXTPROC) (GLenum texunit, GLenum coord, GLenum pname, GLfloat param);
	typedef void (APIENTRY * PFNGLMULTITEXGENFVEXTPROC) (GLenum texunit, GLenum coord, GLenum pname, const GLfloat* params);
	typedef void (APIENTRY * PFNGLMULTITEXGENIEXTPROC) (GLenum texunit, GLenum coord, GLenum pname, GLint param);
	typedef void (APIENTRY * PFNGLMULTITEXGENIVEXTPROC) (GLenum texunit, GLenum coord, GLenum pname, const GLint* params);
	typedef void (APIENTRY * PFNGLMULTITEXIMAGE1DEXTPROC) (GLenum texunit, GLenum target, GLint level, GLint internalformat, GLsizei width, GLint border, GLenum format, GLenum type, const void* pixels);
	typedef void (APIENTRY * PFNGLMULTITEXIMAGE2DEXTPROC) (GLenum texunit, GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void* pixels);
	typedef void (APIENTRY * PFNGLMULTITEXIMAGE3DEXTPROC) (GLenum texunit, GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const void* pixels);
	typedef void (APIENTRY * PFNGLMULTITEXPARAMETERIIVEXTPROC) (GLenum texunit, GLenum target, GLenum pname, const GLint* params);
	typedef void (APIENTRY * PFNGLMULTITEXPARAMETERIUIVEXTPROC) (GLenum texunit, GLenum target, GLenum pname, const GLuint* params);
	typedef void (APIENTRY * PFNGLMULTITEXPARAMETERFEXTPROC) (GLenum texunit, GLenum target, GLenum pname, GLfloat param);
	typedef void (APIENTRY * PFNGLMULTITEXPARAMETERFVEXTPROC) (GLenum texunit, GLenum target, GLenum pname, const GLfloat* param);
	typedef void (APIENTRY * PFNGLMULTITEXPARAMETERIEXTPROC) (GLenum texunit, GLenum target, GLenum pname, GLint param);
	typedef void (APIENTRY * PFNGLMULTITEXPARAMETERIVEXTPROC) (GLenum texunit, GLenum target, GLenum pname, const GLint* param);
	typedef void (APIENTRY * PFNGLMULTITEXRENDERBUFFEREXTPROC) (GLenum texunit, GLenum target, GLuint renderbuffer);
	typedef void (APIENTRY * PFNGLMULTITEXSUBIMAGE1DEXTPROC) (GLenum texunit, GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const void* pixels);
	typedef void (APIENTRY * PFNGLMULTITEXSUBIMAGE2DEXTPROC) (GLenum texunit, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void* pixels);
	typedef void (APIENTRY * PFNGLMULTITEXSUBIMAGE3DEXTPROC) (GLenum texunit, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void* pixels);
	typedef void (APIENTRY * PFNGLNAMEDBUFFERDATAEXTPROC) (GLuint buffer, GLsizeiptr size, const void* data, GLenum usage);
	typedef void (APIENTRY * PFNGLNAMEDBUFFERSUBDATAEXTPROC) (GLuint buffer, GLintptr offset, GLsizeiptr size, const void* data);
	typedef void (APIENTRY * PFNGLNAMEDCOPYBUFFERSUBDATAEXTPROC) (GLuint readBuffer, GLuint writeBuffer, GLintptr readOffset, GLintptr writeOffset, GLsizeiptr size);
	typedef void (APIENTRY * PFNGLNAMEDFRAMEBUFFERRENDERBUFFEREXTPROC) (GLuint framebuffer, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);
	typedef void (APIENTRY * PFNGLNAMEDFRAMEBUFFERTEXTURE1DEXTPROC) (GLuint framebuffer, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
	typedef void (APIENTRY * PFNGLNAMEDFRAMEBUFFERTEXTURE2DEXTPROC) (GLuint framebuffer, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
	typedef void (APIENTRY * PFNGLNAMEDFRAMEBUFFERTEXTURE3DEXTPROC) (GLuint framebuffer, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLint zoffset);
	typedef void (APIENTRY * PFNGLNAMEDFRAMEBUFFERTEXTUREEXTPROC) (GLuint framebuffer, GLenum attachment, GLuint texture, GLint level);
	typedef void (APIENTRY * PFNGLNAMEDFRAMEBUFFERTEXTUREFACEEXTPROC) (GLuint framebuffer, GLenum attachment, GLuint texture, GLint level, GLenum face);
	typedef void (APIENTRY * PFNGLNAMEDFRAMEBUFFERTEXTURELAYEREXTPROC) (GLuint framebuffer, GLenum attachment, GLuint texture, GLint level, GLint layer);
	typedef void (APIENTRY * PFNGLNAMEDPROGRAMLOCALPARAMETER4DEXTPROC) (GLuint program, GLenum target, GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
	typedef void (APIENTRY * PFNGLNAMEDPROGRAMLOCALPARAMETER4DVEXTPROC) (GLuint program, GLenum target, GLuint index, const GLdouble* params);
	typedef void (APIENTRY * PFNGLNAMEDPROGRAMLOCALPARAMETER4FEXTPROC) (GLuint program, GLenum target, GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
	typedef void (APIENTRY * PFNGLNAMEDPROGRAMLOCALPARAMETER4FVEXTPROC) (GLuint program, GLenum target, GLuint index, const GLfloat* params);
	typedef void (APIENTRY * PFNGLNAMEDPROGRAMLOCALPARAMETERI4IEXTPROC) (GLuint program, GLenum target, GLuint index, GLint x, GLint y, GLint z, GLint w);
	typedef void (APIENTRY * PFNGLNAMEDPROGRAMLOCALPARAMETERI4IVEXTPROC) (GLuint program, GLenum target, GLuint index, const GLint* params);
	typedef void (APIENTRY * PFNGLNAMEDPROGRAMLOCALPARAMETERI4UIEXTPROC) (GLuint program, GLenum target, GLuint index, GLuint x, GLuint y, GLuint z, GLuint w);
	typedef void (APIENTRY * PFNGLNAMEDPROGRAMLOCALPARAMETERI4UIVEXTPROC) (GLuint program, GLenum target, GLuint index, const GLuint* params);
	typedef void (APIENTRY * PFNGLNAMEDPROGRAMLOCALPARAMETERS4FVEXTPROC) (GLuint program, GLenum target, GLuint index, GLsizei count, const GLfloat* params);
	typedef void (APIENTRY * PFNGLNAMEDPROGRAMLOCALPARAMETERSI4IVEXTPROC) (GLuint program, GLenum target, GLuint index, GLsizei count, const GLint* params);
	typedef void (APIENTRY * PFNGLNAMEDPROGRAMLOCALPARAMETERSI4UIVEXTPROC) (GLuint program, GLenum target, GLuint index, GLsizei count, const GLuint* params);
	typedef void (APIENTRY * PFNGLNAMEDPROGRAMSTRINGEXTPROC) (GLuint program, GLenum target, GLenum format, GLsizei len, const void* string);
	typedef void (APIENTRY * PFNGLNAMEDRENDERBUFFERSTORAGEEXTPROC) (GLuint renderbuffer, GLenum internalformat, GLsizei width, GLsizei height);
	typedef void (APIENTRY * PFNGLNAMEDRENDERBUFFERSTORAGEMULTISAMPLECOVERAGEEXTPROC) (GLuint renderbuffer, GLsizei coverageSamples, GLsizei colorSamples, GLenum internalformat, GLsizei width, GLsizei height);
	typedef void (APIENTRY * PFNGLNAMEDRENDERBUFFERSTORAGEMULTISAMPLEEXTPROC) (GLuint renderbuffer, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height);
	typedef void (APIENTRY * PFNGLPUSHCLIENTATTRIBDEFAULTEXTPROC) (GLbitfield mask);
	typedef void (APIENTRY * PFNGLTEXTUREBUFFEREXTPROC) (GLuint texture, GLenum target, GLenum internalformat, GLuint buffer);
	typedef void (APIENTRY * PFNGLTEXTUREIMAGE1DEXTPROC) (GLuint texture, GLenum target, GLint level, GLint internalformat, GLsizei width, GLint border, GLenum format, GLenum type, const void* pixels);
	typedef void (APIENTRY * PFNGLTEXTUREIMAGE2DEXTPROC) (GLuint texture, GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void* pixels);
	typedef void (APIENTRY * PFNGLTEXTUREIMAGE3DEXTPROC) (GLuint texture, GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const void* pixels);
	typedef void (APIENTRY * PFNGLTEXTUREPARAMETERIIVEXTPROC) (GLuint texture, GLenum target, GLenum pname, const GLint* params);
	typedef void (APIENTRY * PFNGLTEXTUREPARAMETERIUIVEXTPROC) (GLuint texture, GLenum target, GLenum pname, const GLuint* params);
	typedef void (APIENTRY * PFNGLTEXTUREPARAMETERFEXTPROC) (GLuint texture, GLenum target, GLenum pname, GLfloat param);
	typedef void (APIENTRY * PFNGLTEXTUREPARAMETERFVEXTPROC) (GLuint texture, GLenum target, GLenum pname, const GLfloat* param);
	typedef void (APIENTRY * PFNGLTEXTUREPARAMETERIEXTPROC) (GLuint texture, GLenum target, GLenum pname, GLint param);
	typedef void (APIENTRY * PFNGLTEXTUREPARAMETERIVEXTPROC) (GLuint texture, GLenum target, GLenum pname, const GLint* param);
	typedef void (APIENTRY * PFNGLTEXTURERENDERBUFFEREXTPROC) (GLuint texture, GLenum target, GLuint renderbuffer);
	typedef void (APIENTRY * PFNGLTEXTURESUBIMAGE1DEXTPROC) (GLuint texture, GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const void* pixels);
	typedef void (APIENTRY * PFNGLTEXTURESUBIMAGE2DEXTPROC) (GLuint texture, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void* pixels);
	typedef void (APIENTRY * PFNGLTEXTURESUBIMAGE3DEXTPROC) (GLuint texture, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void* pixels);
	typedef GLboolean (APIENTRY * PFNGLUNMAPNAMEDBUFFEREXTPROC) (GLuint buffer);
	typedef void (APIENTRY * PFNGLVERTEXARRAYVERTEXATTRIBIOFFSETEXTPROC) (GLuint vaobj, GLuint buffer, GLuint index, GLint size, GLenum type, GLsizei stride, GLintptr offset);
	typedef void (APIENTRY * PFNGLVERTEXARRAYVERTEXATTRIBOFFSETEXTPROC) (GLuint vaobj, GLuint buffer, GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, GLintptr offset);
	typedef void (APIENTRY * PFNGLVERTEXARRAYVERTEXOFFSETEXTPROC) (GLuint vaobj, GLuint buffer, GLint size, GLenum type, GLsizei stride, GLintptr offset);

	PFNGLTEXTURESTORAGE1DEXTPROC glTextureStorage1DEXT(0);
	PFNGLTEXTURESTORAGE2DEXTPROC glTextureStorage2DEXT(0);
	PFNGLTEXTURESTORAGE3DEXTPROC glTextureStorage3DEXT(0);

	PFNGLBINDMULTITEXTUREEXTPROC glBindMultiTextureEXT(0);
	PFNGLCHECKNAMEDFRAMEBUFFERSTATUSEXTPROC glCheckNamedFramebufferStatusEXT(0);
	PFNGLCLIENTATTRIBDEFAULTEXTPROC glClientAttribDefaultEXT(0);
	PFNGLCOMPRESSEDTEXTUREIMAGE1DEXTPROC glCompressedTextureImage1DEXT(0);
	PFNGLCOMPRESSEDTEXTUREIMAGE2DEXTPROC glCompressedTextureImage2DEXT(0);
	PFNGLCOMPRESSEDTEXTUREIMAGE3DEXTPROC glCompressedTextureImage3DEXT(0);
	PFNGLCOMPRESSEDTEXTURESUBIMAGE1DEXTPROC glCompressedTextureSubImage1DEXT(0);
	PFNGLCOMPRESSEDTEXTURESUBIMAGE2DEXTPROC glCompressedTextureSubImage2DEXT(0);
	PFNGLCOMPRESSEDTEXTURESUBIMAGE3DEXTPROC glCompressedTextureSubImage3DEXT(0);
	PFNGLCOPYTEXTUREIMAGE1DEXTPROC glCopyTextureImage1DEXT(0);
	PFNGLCOPYTEXTUREIMAGE2DEXTPROC glCopyTextureImage2DEXT(0);
	PFNGLCOPYTEXTURESUBIMAGE1DEXTPROC glCopyTextureSubImage1DEXT(0);
	PFNGLCOPYTEXTURESUBIMAGE2DEXTPROC glCopyTextureSubImage2DEXT(0);
	PFNGLCOPYTEXTURESUBIMAGE3DEXTPROC glCopyTextureSubImage3DEXT(0);
	PFNGLDISABLEVERTEXARRAYATTRIBEXTPROC glDisableVertexArrayAttribEXT(0);
	PFNGLDISABLEVERTEXARRAYEXTPROC glDisableVertexArrayEXT(0);
	PFNGLENABLEVERTEXARRAYATTRIBEXTPROC glEnableVertexArrayAttribEXT(0);
	PFNGLENABLEVERTEXARRAYEXTPROC glEnableVertexArrayEXT(0);
	PFNGLFLUSHMAPPEDNAMEDBUFFERRANGEEXTPROC glFlushMappedNamedBufferRangeEXT(0);
	PFNGLFRAMEBUFFERDRAWBUFFEREXTPROC glFramebufferDrawBufferEXT(0);
	PFNGLFRAMEBUFFERDRAWBUFFERSEXTPROC glFramebufferDrawBuffersEXT(0);
	PFNGLFRAMEBUFFERREADBUFFEREXTPROC glFramebufferReadBufferEXT(0);
	PFNGLGENERATETEXTUREMIPMAPEXTPROC glGenerateTextureMipmapEXT(0);
	PFNGLGETCOMPRESSEDTEXTUREIMAGEEXTPROC glGetCompressedTextureImageEXT(0);
	PFNGLGETFRAMEBUFFERPARAMETERIVEXTPROC glGetFramebufferParameterivEXT(0);
	PFNGLGETNAMEDBUFFERPARAMETERIVEXTPROC glGetNamedBufferParameterivEXT(0);
	PFNGLGETNAMEDBUFFERPOINTERVEXTPROC glGetNamedBufferPointervEXT(0);
	PFNGLGETNAMEDBUFFERSUBDATAEXTPROC glGetNamedBufferSubDataEXT(0);
	PFNGLGETNAMEDFRAMEBUFFERATTACHMENTPARAMETERIVEXTPROC glGetNamedFramebufferAttachmentParameterivEXT(0);
	PFNGLGETNAMEDPROGRAMSTRINGEXTPROC glGetNamedProgramStringEXT(0);
	PFNGLGETNAMEDPROGRAMIVEXTPROC glGetNamedProgramivEXT(0);
	PFNGLGETNAMEDRENDERBUFFERPARAMETERIVEXTPROC glGetNamedRenderbufferParameterivEXT(0);
	PFNGLGETTEXTUREIMAGEEXTPROC glGetTextureImageEXT(0);
	PFNGLGETTEXTURELEVELPARAMETERFVEXTPROC glGetTextureLevelParameterfvEXT(0);
	PFNGLGETTEXTURELEVELPARAMETERIVEXTPROC glGetTextureLevelParameterivEXT(0);
	PFNGLGETTEXTUREPARAMETERIIVEXTPROC glGetTextureParameterIivEXT(0);
	PFNGLGETTEXTUREPARAMETERIUIVEXTPROC glGetTextureParameterIuivEXT(0);
	PFNGLGETTEXTUREPARAMETERFVEXTPROC glGetTextureParameterfvEXT(0);
	PFNGLGETTEXTUREPARAMETERIVEXTPROC glGetTextureParameterivEXT(0);
	PFNGLGETVERTEXARRAYINTEGERI_VEXTPROC glGetVertexArrayIntegeri_vEXT(0);
	PFNGLGETVERTEXARRAYINTEGERVEXTPROC glGetVertexArrayIntegervEXT(0);
	PFNGLGETVERTEXARRAYPOINTERI_VEXTPROC glGetVertexArrayPointeri_vEXT(0);
	PFNGLGETVERTEXARRAYPOINTERVEXTPROC glGetVertexArrayPointervEXT(0);
	PFNGLMAPNAMEDBUFFEREXTPROC glMapNamedBufferEXT(0);
	PFNGLMAPNAMEDBUFFERRANGEEXTPROC glMapNamedBufferRangeEXT(0);
	PFNGLNAMEDBUFFERDATAEXTPROC glNamedBufferDataEXT(0);
	PFNGLNAMEDBUFFERSUBDATAEXTPROC glNamedBufferSubDataEXT(0);
	PFNGLNAMEDCOPYBUFFERSUBDATAEXTPROC glNamedCopyBufferSubDataEXT(0);
	PFNGLNAMEDFRAMEBUFFERRENDERBUFFEREXTPROC glNamedFramebufferRenderbufferEXT(0);
	PFNGLNAMEDFRAMEBUFFERTEXTURE1DEXTPROC glNamedFramebufferTexture1DEXT(0);
	PFNGLNAMEDFRAMEBUFFERTEXTURE2DEXTPROC glNamedFramebufferTexture2DEXT(0);
	PFNGLNAMEDFRAMEBUFFERTEXTURE3DEXTPROC glNamedFramebufferTexture3DEXT(0);
	PFNGLNAMEDFRAMEBUFFERTEXTUREEXTPROC glNamedFramebufferTextureEXT(0);
	PFNGLNAMEDFRAMEBUFFERTEXTUREFACEEXTPROC glNamedFramebufferTextureFaceEXT(0);
	PFNGLNAMEDFRAMEBUFFERTEXTURELAYEREXTPROC glNamedFramebufferTextureLayerEXT(0);
	PFNGLNAMEDPROGRAMSTRINGEXTPROC glNamedProgramStringEXT(0);
	PFNGLNAMEDRENDERBUFFERSTORAGEEXTPROC glNamedRenderbufferStorageEXT(0);
	PFNGLNAMEDRENDERBUFFERSTORAGEMULTISAMPLECOVERAGEEXTPROC glNamedRenderbufferStorageMultisampleCoverageEXT(0);
	PFNGLNAMEDRENDERBUFFERSTORAGEMULTISAMPLEEXTPROC glNamedRenderbufferStorageMultisampleEXT(0);
	PFNGLTEXTUREBUFFEREXTPROC glTextureBufferEXT(0);
	PFNGLTEXTUREIMAGE1DEXTPROC glTextureImage1DEXT(0);
	PFNGLTEXTUREIMAGE2DEXTPROC glTextureImage2DEXT(0);
	PFNGLTEXTUREIMAGE3DEXTPROC glTextureImage3DEXT(0);
	PFNGLTEXTUREPARAMETERIIVEXTPROC glTextureParameterIivEXT(0);
	PFNGLTEXTUREPARAMETERIUIVEXTPROC glTextureParameterIuivEXT(0);
	PFNGLTEXTUREPARAMETERFEXTPROC glTextureParameterfEXT(0);
	PFNGLTEXTUREPARAMETERFVEXTPROC glTextureParameterfvEXT(0);
	PFNGLTEXTUREPARAMETERIEXTPROC glTextureParameteriEXT(0);
	PFNGLTEXTUREPARAMETERIVEXTPROC glTextureParameterivEXT(0);
	PFNGLTEXTURERENDERBUFFEREXTPROC glTextureRenderbufferEXT(0);
	PFNGLTEXTURESUBIMAGE1DEXTPROC glTextureSubImage1DEXT(0);
	PFNGLTEXTURESUBIMAGE2DEXTPROC glTextureSubImage2DEXT(0);
	PFNGLTEXTURESUBIMAGE3DEXTPROC glTextureSubImage3DEXT(0);
	PFNGLUNMAPNAMEDBUFFEREXTPROC glUnmapNamedBufferEXT(0);
	PFNGLVERTEXARRAYVERTEXATTRIBIOFFSETEXTPROC glVertexArrayVertexAttribIOffsetEXT(0);
	PFNGLVERTEXARRAYVERTEXATTRIBOFFSETEXTPROC glVertexArrayVertexAttribOffsetEXT(0);
	PFNGLVERTEXARRAYVERTEXOFFSETEXTPROC glVertexArrayVertexOffsetEXT(0);

	// GL_AMD_sample_positions
	#define GL_SUBSAMPLE_DISTANCE_AMD 0x883F

	typedef void (APIENTRY * PFNGLSETMULTISAMPLEFVAMDPROC) (GLenum pname, GLuint index, const GLfloat* val);

	PFNGLSETMULTISAMPLEFVAMDPROC glSetMultisamplefvAMD(0);

	// GL_EXT_texture_sRGB_decode

	#define GL_TEXTURE_SRGB_DECODE_EXT 0x8A48
	#define GL_DECODE_EXT 0x8A49
	#define GL_SKIP_DECODE_EXT 0x8A4A

	// GL_NV_shader_buffer_load
	#define GL_BUFFER_GPU_ADDRESS_NV 0x8F1D
	#define GL_GPU_ADDRESS_NV 0x8F34
	#define GL_MAX_SHADER_BUFFER_ADDRESS_NV 0x8F35

	typedef void (APIENTRY * PFNGLGETBUFFERPARAMETERUI64VNVPROC) (GLenum target, GLenum pname, GLuint64EXT* params);
	typedef void (APIENTRY * PFNGLGETINTEGERUI64VNVPROC) (GLenum value, GLuint64EXT* result);
	typedef void (APIENTRY * PFNGLGETNAMEDBUFFERPARAMETERUI64VNVPROC) (GLuint buffer, GLenum pname, GLuint64EXT* params);
	typedef GLboolean (APIENTRY * PFNGLISBUFFERRESIDENTNVPROC) (GLenum target);
	typedef GLboolean (APIENTRY * PFNGLISNAMEDBUFFERRESIDENTNVPROC) (GLuint buffer);
	typedef void (APIENTRY * PFNGLMAKEBUFFERNONRESIDENTNVPROC) (GLenum target);
	typedef void (APIENTRY * PFNGLMAKEBUFFERRESIDENTNVPROC) (GLenum target, GLenum access);
	typedef void (APIENTRY * PFNGLMAKENAMEDBUFFERNONRESIDENTNVPROC) (GLuint buffer);
	typedef void (APIENTRY * PFNGLMAKENAMEDBUFFERRESIDENTNVPROC) (GLuint buffer, GLenum access);
	typedef void (APIENTRY * PFNGLPROGRAMUNIFORMUI64NVPROC) (GLuint program, GLint location, GLuint64EXT value);
	typedef void (APIENTRY * PFNGLPROGRAMUNIFORMUI64VNVPROC) (GLuint program, GLint location, GLsizei count, const GLuint64EXT* value);
	typedef void (APIENTRY * PFNGLUNIFORMUI64NVPROC) (GLint location, GLuint64EXT value);
	typedef void (APIENTRY * PFNGLUNIFORMUI64VNVPROC) (GLint location, GLsizei count, const GLuint64EXT* value);

	PFNGLGETBUFFERPARAMETERUI64VNVPROC glGetBufferParameterui64vNV(0);
	PFNGLGETINTEGERUI64VNVPROC glGetIntegerui64vNV(0);
	PFNGLGETNAMEDBUFFERPARAMETERUI64VNVPROC glGetNamedBufferParameterui64vNV(0);
	PFNGLISBUFFERRESIDENTNVPROC glIsBufferResidentNV(0);
	PFNGLISNAMEDBUFFERRESIDENTNVPROC glIsNamedBufferResidentNV(0);
	PFNGLMAKEBUFFERNONRESIDENTNVPROC glMakeBufferNonResidentNV(0);
	PFNGLMAKEBUFFERRESIDENTNVPROC glMakeBufferResidentNV(0);
	PFNGLMAKENAMEDBUFFERNONRESIDENTNVPROC glMakeNamedBufferNonResidentNV(0);
	PFNGLMAKENAMEDBUFFERRESIDENTNVPROC glMakeNamedBufferResidentNV(0);
	PFNGLPROGRAMUNIFORMUI64NVPROC glProgramUniformui64NV(0);
	PFNGLPROGRAMUNIFORMUI64VNVPROC glProgramUniformui64vNV(0);
	PFNGLUNIFORMUI64NVPROC glUniformui64NV(0);
	PFNGLUNIFORMUI64VNVPROC glUniformui64vNV(0);

	// GL_NV_vertex_buffer_unified_memory
	typedef void (APIENTRY * PFNGLBUFFERADDRESSRANGENVPROC) (GLenum pname, GLuint index, GLuint64EXT address, GLsizeiptr length);
	typedef void (APIENTRY * PFNGLVERTEXATTRIBFORMATNVPROC) (GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride);
	typedef void (APIENTRY * PFNGLVERTEXATTRIBIFORMATNVPROC) (GLuint index, GLint size, GLenum type, GLsizei stride);
	typedef void (APIENTRY * PFNGLGETINTEGERUI64I_VNVPROC) (GLenum value, GLuint index, GLuint64EXT result[]);

	#define GL_VERTEX_ATTRIB_ARRAY_UNIFIED_NV 0x8F1E
	#define GL_ELEMENT_ARRAY_UNIFIED_NV 0x8F1F
	#define GL_VERTEX_ATTRIB_ARRAY_ADDRESS_NV 0x8F20
	#define GL_ELEMENT_ARRAY_ADDRESS_NV 0x8F29
	#define GL_VERTEX_ATTRIB_ARRAY_LENGTH_NV 0x8F2A
	#define GL_ELEMENT_ARRAY_LENGTH_NV 0x8F33
	#define GL_DRAW_INDIRECT_UNIFIED_NV 0x8F40
	#define GL_DRAW_INDIRECT_ADDRESS_NV 0x8F41
	#define GL_DRAW_INDIRECT_LENGTH_NV 0x8F42

	PFNGLBUFFERADDRESSRANGENVPROC glBufferAddressRangeNV(0);
	PFNGLVERTEXATTRIBFORMATNVPROC glVertexAttribFormatNV(0);
	PFNGLVERTEXATTRIBIFORMATNVPROC glVertexAttribIFormatNV(0);
	PFNGLGETINTEGERUI64I_VNVPROC glGetIntegerui64i_vNV(0);

	// GL_AMD_depth_clamp_separate
	#define GL_DEPTH_CLAMP_NEAR_AMD 0x901E
	#define GL_DEPTH_CLAMP_FAR_AMD 0x901F

	// GL_NV_texture_multisample
	typedef void (APIENTRY * PFNGLTEXIMAGE2DMULTISAMPLECOVERAGENVPROC) (GLenum target, GLsizei coverageSamples, GLsizei colorSamples, GLint internalFormat, GLsizei width, GLsizei height, GLboolean fixedSampleLocations);
	typedef void (APIENTRY * PFNGLTEXIMAGE3DMULTISAMPLECOVERAGENVPROC) (GLenum target, GLsizei coverageSamples, GLsizei colorSamples, GLint internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedSampleLocations);
	typedef void (APIENTRY * PFNGLTEXTUREIMAGE2DMULTISAMPLECOVERAGENVPROC) (GLuint texture, GLenum target, GLsizei coverageSamples, GLsizei colorSamples, GLint internalFormat, GLsizei width, GLsizei height, GLboolean fixedSampleLocations);
	typedef void (APIENTRY * PFNGLTEXTUREIMAGE2DMULTISAMPLENVPROC) (GLuint texture, GLenum target, GLsizei samples, GLint internalFormat, GLsizei width, GLsizei height, GLboolean fixedSampleLocations);
	typedef void (APIENTRY * PFNGLTEXTUREIMAGE3DMULTISAMPLECOVERAGENVPROC) (GLuint texture, GLenum target, GLsizei coverageSamples, GLsizei colorSamples, GLint internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedSampleLocations);
	typedef void (APIENTRY * PFNGLTEXTUREIMAGE3DMULTISAMPLENVPROC) (GLuint texture, GLenum target, GLsizei samples, GLint internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedSampleLocations);

	PFNGLTEXIMAGE2DMULTISAMPLECOVERAGENVPROC glTexImage2DMultisampleCoverageNV(0);
	PFNGLTEXIMAGE3DMULTISAMPLECOVERAGENVPROC glTexImage3DMultisampleCoverageNV(0);
	PFNGLTEXTUREIMAGE2DMULTISAMPLECOVERAGENVPROC glTextureImage2DMultisampleCoverageNV(0);
	PFNGLTEXTUREIMAGE2DMULTISAMPLENVPROC glTextureImage2DMultisampleNV(0);
	PFNGLTEXTUREIMAGE3DMULTISAMPLECOVERAGENVPROC glTextureImage3DMultisampleCoverageNV(0);
	PFNGLTEXTUREIMAGE3DMULTISAMPLENVPROC glTextureImage3DMultisampleNV(0);

	// GL_NV_bindless_texture
	typedef GLuint64 (APIENTRY * PFNGLGETIMAGEHANDLENVPROC) (GLuint texture, GLint level, GLboolean layered, GLint layer, GLenum format);
	typedef GLuint64 (APIENTRY * PFNGLGETTEXTUREHANDLENVPROC) (GLuint texture);
	typedef GLuint64 (APIENTRY * PFNGLGETTEXTURESAMPLERHANDLENVPROC) (GLuint texture, GLuint sampler);
	typedef GLboolean (APIENTRY * PFNGLISIMAGEHANDLERESIDENTNVPROC) (GLuint64 handle);
	typedef GLboolean (APIENTRY * PFNGLISTEXTUREHANDLERESIDENTNVPROC) (GLuint64 handle);
	typedef void (APIENTRY * PFNGLMAKEIMAGEHANDLENONRESIDENTNVPROC) (GLuint64 handle);
	typedef void (APIENTRY * PFNGLMAKEIMAGEHANDLERESIDENTNVPROC) (GLuint64 handle, GLenum access);
	typedef void (APIENTRY * PFNGLMAKETEXTUREHANDLENONRESIDENTNVPROC) (GLuint64 handle);
	typedef void (APIENTRY * PFNGLMAKETEXTUREHANDLERESIDENTNVPROC) (GLuint64 handle);
	typedef void (APIENTRY * PFNGLPROGRAMUNIFORMHANDLEUI64NVPROC) (GLuint program, GLint location, GLuint64 value);
	typedef void (APIENTRY * PFNGLPROGRAMUNIFORMHANDLEUI64VNVPROC) (GLuint program, GLint location, GLsizei count, const GLuint64* values);
	typedef void (APIENTRY * PFNGLUNIFORMHANDLEUI64NVPROC) (GLint location, GLuint64 value);
	typedef void (APIENTRY * PFNGLUNIFORMHANDLEUI64VNVPROC) (GLint location, GLsizei count, const GLuint64* value);

	PFNGLGETIMAGEHANDLENVPROC glGetImageHandleNV(0);
	PFNGLGETTEXTUREHANDLENVPROC glGetTextureHandleNV(0);
	PFNGLGETTEXTURESAMPLERHANDLENVPROC glGetTextureSamplerHandleNV(0);
	PFNGLISIMAGEHANDLERESIDENTNVPROC glIsImageHandleResidentNV(0);
	PFNGLISTEXTUREHANDLERESIDENTNVPROC glIsTextureHandleResidentNV(0);
	PFNGLMAKEIMAGEHANDLENONRESIDENTNVPROC glMakeImageHandleNonResidentNV(0);
	PFNGLMAKEIMAGEHANDLERESIDENTNVPROC glMakeImageHandleResidentNV(0);
	PFNGLMAKETEXTUREHANDLENONRESIDENTNVPROC glMakeTextureHandleNonResidentNV(0);
	PFNGLMAKETEXTUREHANDLERESIDENTNVPROC glMakeTextureHandleResidentNV(0);
	PFNGLPROGRAMUNIFORMHANDLEUI64NVPROC glProgramUniformHandleui64NV(0);
	PFNGLPROGRAMUNIFORMHANDLEUI64VNVPROC glProgramUniformHandleui64vNV(0);
	PFNGLUNIFORMHANDLEUI64NVPROC glUniformHandleui64NV(0);
	PFNGLUNIFORMHANDLEUI64VNVPROC glUniformHandleui64vNV(0);

	// GL_EXT_texture_filter_anisotropic
	#define GL_TEXTURE_MAX_ANISOTROPY_EXT 0x84FE
	#define GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT 0x84FF

	// GL_NV_explicit_multisample
	typedef void (APIENTRY * PFNGLGETMULTISAMPLEFVNVPROC) (GLenum pname, GLuint index, GLfloat* val);
	typedef void (APIENTRY * PFNGLSAMPLEMASKINDEXEDNVPROC) (GLuint index, GLbitfield mask);
	typedef void (APIENTRY * PFNGLTEXRENDERBUFFERNVPROC) (GLenum target, GLuint renderbuffer);

	PFNGLGETMULTISAMPLEFVNVPROC glGetMultisamplefvNV(0);
	PFNGLSAMPLEMASKINDEXEDNVPROC glSampleMaskIndexedNV(0);
	PFNGLTEXRENDERBUFFERNVPROC glTexRenderbufferNV(0);

	#define GL_SAMPLE_POSITION_NV								0x8E50
	#define GL_SAMPLE_MASK_NV									0x8E51
	#define GL_SAMPLE_MASK_VALUE_NV								0x8E52
	#define GL_TEXTURE_BINDING_RENDERBUFFER_NV					0x8E53
	#define GL_TEXTURE_RENDERBUFFER_DATA_STORE_BINDING_NV		0x8E54
	#define GL_MAX_SAMPLE_MASK_WORDS_NV							0x8E59
	#define GL_TEXTURE_RENDERBUFFER_NV							0x8E55
	#define GL_SAMPLER_RENDERBUFFER_NV							0x8E56
	#define GL_INT_SAMPLER_RENDERBUFFER_NV						0x8E57
	#define GL_UNSIGNED_INT_SAMPLER_RENDERBUFFER_NV				0x8E58

	// GL_AMD_blend_minmax_factor
	#define GL_FACTOR_MIN_AMD 0x901C
	#define GL_FACTOR_MAX_AMD 0x901D

	// GL_AMD_sparse_texture
	#define GL_TEXTURE_STORAGE_SPARSE_BIT_AMD 0x00000001

	#define GL_VIRTUAL_PAGE_SIZE_X_AMD 0x9195
	#define GL_VIRTUAL_PAGE_SIZE_Y_AMD 0x9196
	#define GL_VIRTUAL_PAGE_SIZE_Z_AMD 0x9197

	#define GL_MAX_SPARSE_TEXTURE_SIZE_AMD 0x9198
	#define GL_MAX_SPARSE_3D_TEXTURE_SIZE_AMD 0x9199
	#define GL_MAX_SPARSE_ARRAY_TEXTURE_LAYERS_AMD 0x919A

	#define GL_MIN_SPARSE_LEVEL_AMD 0x919B
	#define GL_MIN_LOD_WARNING_AMD 0x919C

	typedef void (APIENTRY * PFNGLTEXSTORAGESPARSEAMDPROC) (GLenum target, GLenum internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLsizei layers, GLbitfield flags);
	typedef void (APIENTRY * PFNGLTEXTURESTORAGESPARSEAMDPROC) (GLuint texture, GLenum target, GLenum internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLsizei layers, GLbitfield flags);
	
	PFNGLTEXSTORAGESPARSEAMDPROC glTexStorageSparseAMD = 0;
	PFNGLTEXTURESTORAGESPARSEAMDPROC glTextureStorageSparseAMD = 0;

	// GL_ARB_debug_output
	PFNGLDEBUGMESSAGECONTROLARBPROC glDebugMessageControlARB(0);
	PFNGLDEBUGMESSAGEINSERTARBPROC glDebugMessageInsertARB(0);
	PFNGLDEBUGMESSAGECALLBACKARBPROC glDebugMessageCallbackARB(0);
	PFNGLGETDEBUGMESSAGELOGARBPROC glGetDebugMessageLogARB(0);
#endif

#include "common.hpp"
#include "vertex.hpp"
#include "compiler.hpp"
#include "sementics.hpp"
#include "error.hpp"
//#include "window.hpp"

namespace glf
{
	enum mouse_button
	{
		MOUSE_BUTTON_NONE = 0,
		MOUSE_BUTTON_LEFT = (1 << 0),
		MOUSE_BUTTON_RIGHT = (1 << 1),
		MOUSE_BUTTON_MIDDLE = (1 << 2)
	};

	struct window
	{
		window(glm::ivec2 const & Size) :
			Size(Size),
			MouseOrigin(Size >> 1),
			MouseCurrent(Size >> 1),
			TranlationOrigin(0, 4),
			TranlationCurrent(0, 4),
			RotationOrigin(0), 
			RotationCurrent(0),
			MouseButtonFlags(0)
		{
			memset(KeyPressed, 0, sizeof(KeyPressed));	
		}

		glm::ivec2 Size;
		glm::vec2 MouseOrigin;
		glm::vec2 MouseCurrent;
		glm::vec2 TranlationOrigin;
		glm::vec2 TranlationCurrent;
		glm::vec2 RotationOrigin;
		glm::vec2 RotationCurrent;
		int MouseButtonFlags;
		std::size_t KeyPressed[256];
	};

	int version(int Major, int Minor);
	int run();

	struct vertexattrib
	{
		vertexattrib() :
			Enabled(GL_FALSE),
			//Binding(0),
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
			//GLint Binding,
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
			//Binding(Binding),
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
		//GLint Binding;
		GLint Size;
		GLint Stride;
		GLint Type;
		GLint Normalized;
		GLint Integer;
		GLint Long;
		GLint Divisor;
		GLvoid* Pointer;
	};

	bool operator== (vertexattrib const & A, vertexattrib const & B)
	{
		return A.Enabled == B.Enabled && 
			A.Size == B.Size && 
			A.Stride == B.Stride && 
			A.Type == B.Type && 
			A.Normalized == B.Normalized && 
			A.Integer == B.Integer && 
			A.Long == B.Long;
	}

	bool operator!= (vertexattrib const & A, vertexattrib const & B)
	{
		return !(A == B);
	}

}//namespace glf

namespace 
{
	extern glf::window Window;
}//namespace 

#define GLF_BUFFER_OFFSET(i) ((char *)NULL + (i))
#ifndef GL_EXTERNAL_VIRTUAL_MEMORY_BUFFER_AMD
#define GL_EXTERNAL_VIRTUAL_MEMORY_BUFFER_AMD 0x9160
#endif

#ifndef WGL_CONTEXT_CORE_PROFILE_BIT_ARB
#define WGL_CONTEXT_CORE_PROFILE_BIT_ARB 0x00000001
#endif

#ifndef WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB
#define WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB 0x00000002
#endif

#ifndef WGL_CONTEXT_ES2_PROFILE_BIT_EXT
#define WGL_CONTEXT_ES2_PROFILE_BIT_EXT 0x00000004
#endif

#ifndef GLX_CONTEXT_CORE_PROFILE_BIT_ARB
#define GLX_CONTEXT_CORE_PROFILE_BIT_ARB 0x00000001
#endif

#ifndef GLX_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB
#define GLX_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB 0x00000002
#endif

#ifndef WGLX_CONTEXT_ES2_PROFILE_BIT_EXT
#define WGLX_CONTEXT_ES2_PROFILE_BIT_EXT 0x00000004
#endif

#include "glf.inl"

#endif//GLF_WINDOW_INCLUDED
