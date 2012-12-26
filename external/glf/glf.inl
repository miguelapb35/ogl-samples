#include <GL/glfw3.h>

bool check();
bool begin();
bool end();
void display();

#if (defined(WIN32))
	// GL_ARB_vertex_attrib_binding
	typedef void (GLAPIENTRY * PFNGLVERTEXARRAYBINDVERTEXBUFFEREXTPROC) (GLuint vaobj, GLuint bindingindex, GLuint buffer, GLintptr offset, GLsizei stride);
	typedef void (GLAPIENTRY * PFNGLVERTEXARRAYBINDVERTEXATTRIBFORMATEXTPROC) (GLuint vaobj, GLuint attribindex, GLint size, GLenum type, GLboolean normalized, GLuint relativeoffset);
	typedef void (GLAPIENTRY * PFNGLVERTEXARRAYBINDVERTEXATTRIBIFORMATEXTPROC) (GLuint vaobj, GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset);
	typedef void (GLAPIENTRY * PFNGLVERTEXARRAYBINDVERTEXATTRIBLFORMATEXTPROC) (GLuint vaobj, GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset);
	typedef void (GLAPIENTRY * PFNGLVERTEXARRAYBINDVERTEXATTRIBBINDINGEXTPROC) (GLuint vaobj, GLuint attribindex, GLuint bindingindex);
	typedef void (GLAPIENTRY * PFNGLVERTEXARRAYBINDVERTEXBINDINGDIVISOREXTPROC) (GLuint vaobj, GLuint bindingindex, GLuint divisor);

	PFNGLVERTEXARRAYBINDVERTEXBUFFEREXTPROC glVertexArrayBindVertexBufferEXT(0);
	PFNGLVERTEXARRAYBINDVERTEXATTRIBFORMATEXTPROC glVertexArrayVertexAttribFormatEXT(0);
	PFNGLVERTEXARRAYBINDVERTEXATTRIBIFORMATEXTPROC glVertexArrayVertexAttribIFormatEXT(0);
	PFNGLVERTEXARRAYBINDVERTEXATTRIBLFORMATEXTPROC glVertexArrayVertexAttribLFormatEXT(0);
	PFNGLVERTEXARRAYBINDVERTEXATTRIBBINDINGEXTPROC glVertexArrayVertexAttribBindingEXT(0);
	PFNGLVERTEXARRAYBINDVERTEXBINDINGDIVISOREXTPROC glVertexArrayVertexBindingDivisorEXT(0);

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

	typedef void (GLAPIENTRY * PFNGLTEXSTORAGESPARSEAMDPROC) (GLenum target, GLenum internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLsizei layers, GLbitfield flags);
	typedef void (GLAPIENTRY * PFNGLTEXTURESTORAGESPARSEAMDPROC) (GLuint texture, GLenum target, GLenum internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLsizei layers, GLbitfield flags);

	PFNGLTEXSTORAGESPARSEAMDPROC glTexStorageSparseAMD = 0;
	PFNGLTEXTURESTORAGESPARSEAMDPROC glTextureStorageSparseAMD = 0;

	// GL_KHR_debug
	typedef void (GLAPIENTRY * PFNGLPOPDEBUGGROUPPROC) (GLvoid);

	PFNGLPOPDEBUGGROUPPROC glPopDebugGroup = 0;
#endif // WIN32

namespace glf
{
	inline void logImplementationDependentLimit(GLenum Value, std::string const & String)
	{
		GLint Result(0);
		glGetIntegerv(Value, &Result);
		std::string Message(glf::format("%s: %d", String.c_str(), Result));
#       if(!defined(__APPLE__))
            glDebugMessageInsertARB(GL_DEBUG_SOURCE_APPLICATION_ARB, GL_DEBUG_TYPE_OTHER_ARB, 1, GL_DEBUG_SEVERITY_LOW_ARB, GLsizei(Message.size()), Message.c_str());
#       endif
	}

	inline void swapBuffers()
	{
		glfwSwapBuffers(Window.Handle);
		assert(glGetError() == GL_NO_ERROR); // 'glutSwapBuffers' generates an here with OpenGL 3 > core profile ... :/
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
		glewInit();
		glGetError();

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
	static void GLAPIENTRY debugOutput
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
	static void cursor_position_callback(GLFWwindow window, int x, int y)
	{
		Window.MouseCurrent = glm::ivec2(x, y);
		Window.TranlationCurrent = Window.MouseButtonFlags & glf::MOUSE_BUTTON_LEFT ? Window.TranlationOrigin + (Window.MouseCurrent - Window.MouseOrigin) / 10.f : Window.TranlationOrigin;
		Window.RotationCurrent = Window.MouseButtonFlags & glf::MOUSE_BUTTON_RIGHT ? Window.RotationOrigin + (Window.MouseCurrent - Window.MouseOrigin) : Window.RotationOrigin;
	}

	static void mouse_button_callback(GLFWwindow window, int Button, int Action)
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
		glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
		glfwWindowHint(GLFW_OPENGL_VERSION_MAJOR, Major);
		glfwWindowHint(GLFW_OPENGL_VERSION_MINOR, Minor);
		//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
		//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#		if NDEBUG
			glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_FALSE);
#		else
			glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
#		endif
		Window.Handle = glfwCreateWindow(Size.x, Size.y, GLFW_WINDOWED, argv[0], NULL);

		glfwMakeContextCurrent(Window.Handle);
		glfwSwapInterval(1);
		glfwSetInputMode(Window.Handle, GLFW_STICKY_KEYS, GL_TRUE);
		glfwSetMouseButtonCallback(Window.Handle, mouse_button_callback);
		glfwSetCursorPosCallback(Window.Handle, cursor_position_callback);

#if !defined(__APPLE__)
		glewInit();
		glGetError();
#endif//__APPLE__

		init();

		begin();

		bool Exit(false);
		while(!Exit)
		{
			display();
			glfwPollEvents();

			if (glfwGetKey(Window.Handle, GLFW_KEY_ESCAPE))
				Exit = true;
			if (glfwGetWindowParam(Window.Handle, GLFW_CLOSE_REQUESTED))
				Exit = true;
		}

		bool End = end();

		glfwTerminate();

		return End ? 0 : 1;
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
