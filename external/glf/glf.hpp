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

	// GL_ARB_vertex_buffer_object
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

	// GL_KHR_debug
	typedef void (APIENTRY * PFNGLPOPDEBUGGROUPPROC) (GLvoid);

	PFNGLPOPDEBUGGROUPPROC glPopDebugGroup = 0;
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
