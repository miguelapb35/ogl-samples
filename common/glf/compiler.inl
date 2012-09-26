//**********************************
// Compiler
// 21/09/2012 - 21/09/2012
//**********************************
// Christophe Riccio
// ogl-samples@g-truc.net
//**********************************
// G-Truc Creation
// www.g-truc.net
//**********************************

namespace glf
{
	inline std::string format(const char* msg, ...)
	{
		std::size_t const STRING_BUFFER(10000);
		char text[STRING_BUFFER];
		va_list list;

		if(msg == 0)
			return std::string();

		va_start(list, msg);
			vsprintf(text, msg, list);
		va_end(list);

		return std::string(text);
	}

	inline compiler::~compiler()
	{
		this->clear();
	}

	inline GLuint compiler::create(
		GLenum Type, 
		std::string const & Filename,
		std::string const & Arguments)
	{
		assert(!Filename.empty());
	
		commandline CommandLine(Filename, Arguments);

		std::string PreprocessedSource = parser()(CommandLine, Filename);
		char const * PreprocessedSourcePointer = PreprocessedSource.c_str();

		fprintf(stdout, "%s\n", PreprocessedSource.c_str());

		GLuint Name = glCreateShader(Type);
		glShaderSource(Name, 1, &PreprocessedSourcePointer, NULL);
		glCompileShader(Name);

		std::pair<files_map::iterator, bool> ResultFiles = this->ShaderFiles.insert(std::make_pair(Name, Filename));
		assert(ResultFiles.second);
		std::pair<names_map::iterator, bool> ResultNames = this->ShaderNames.insert(std::make_pair(Filename, Name));
		assert(ResultNames.second);
		std::pair<names_map::iterator, bool> ResultChecks = this->PendingChecks.insert(std::make_pair(Filename, Name));
		assert(ResultChecks.second);

		return Name;
	}

	inline bool compiler::destroy(GLuint const & Name)
	{
		files_map::iterator NameIterator = this->ShaderFiles.find(Name);
		if(NameIterator == this->ShaderFiles.end())
			return false; // Shader name not found
		std::string File = NameIterator->second;
		this->ShaderFiles.erase(NameIterator);

		// Remove from the pending checks list
		names_map::iterator PendingIterator = this->PendingChecks.find(File);
		if(PendingIterator != this->PendingChecks.end())
			this->PendingChecks.erase(PendingIterator);

		// Remove from the pending checks list
		names_map::iterator FileIterator = this->ShaderNames.find(File);
		assert(FileIterator != this->ShaderNames.end());
		this->ShaderNames.erase(FileIterator);

		return true;
	}

	// TODO Interaction with KHR_debug
	inline bool compiler::check()
	{
		bool Success(true);

		for
		(
			names_map::iterator ShaderIterator = PendingChecks.begin();
			ShaderIterator != PendingChecks.end();
			++ShaderIterator
		)
		{
			GLuint ShaderName = ShaderIterator->second;
			GLint Result = GL_FALSE;
			glGetShaderiv(ShaderName, GL_COMPILE_STATUS, &Result);

			if(Result == GL_TRUE)
			{
				int InfoLogLength;
				glGetShaderiv(ShaderName, GL_INFO_LOG_LENGTH, &InfoLogLength);
				if(InfoLogLength > 0)
				{
					std::vector<char> Buffer(InfoLogLength);
					glGetShaderInfoLog(ShaderName, InfoLogLength, NULL, &Buffer[0]);
					fprintf(stdout, "%s\n", &Buffer[0]);
				}
			}

			Success = Success && Result == GL_TRUE;
		}
	
		return Success; 
	}

	inline void compiler::clear()
	{
		for
		(
			names_map::iterator ShaderNameIterator = this->ShaderNames.begin(); 
			ShaderNameIterator != this->ShaderNames.end(); 
			++ShaderNameIterator
		)
			glDeleteShader(ShaderNameIterator->second);
		this->ShaderNames.clear();
		this->ShaderFiles.clear();
		this->PendingChecks.clear();
	}

	inline bool compiler::loadBinary
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

	inline std::string compiler::loadFile
	(
		std::string const & Filename
	)
	{
		std::string Result;
		
		std::ifstream Stream(Filename);
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

	inline bool validateProgram(GLuint ProgramName)
	{
		if(!ProgramName)
			return false;

		glValidateProgram(ProgramName);
		GLint Result = GL_FALSE;
		glGetProgramiv(ProgramName, GL_VALIDATE_STATUS, &Result);

		if(Result == GL_FALSE)
		{
			fprintf(stdout, "Validate program\n");
			int InfoLogLength;
			glGetProgramiv(ProgramName, GL_INFO_LOG_LENGTH, &InfoLogLength);
			if(InfoLogLength > 0)
			{
				std::vector<char> Buffer(InfoLogLength);
				glGetProgramInfoLog(ProgramName, InfoLogLength, NULL, &Buffer[0]);
				fprintf(stdout, "%s\n", &Buffer[0]);
			}
		}

		return Result == GL_TRUE;
	}

	inline bool checkProgram(GLuint ProgramName)
	{
		if(!ProgramName)
			return false;

		GLint Result = GL_FALSE;
		glGetProgramiv(ProgramName, GL_LINK_STATUS, &Result);

		fprintf(stdout, "Linking program\n");
		int InfoLogLength;
		glGetProgramiv(ProgramName, GL_INFO_LOG_LENGTH, &InfoLogLength);
		if(InfoLogLength > 0)
		{
			std::vector<char> Buffer(std::max(InfoLogLength, int(1)));
			glGetProgramInfoLog(ProgramName, InfoLogLength, NULL, &Buffer[0]);
			fprintf(stdout, "%s\n", &Buffer[0]);
		}

		return Result == GL_TRUE;
	}

	inline bool checkShader
	(
		GLuint ShaderName, 
		std::string const & File
	)
	{
		if(!ShaderName)
			return false;

		GLint Result = GL_FALSE;
		glGetShaderiv(ShaderName, GL_COMPILE_STATUS, &Result);

		fprintf(stdout, "Compiling shader\n%s...\n", File.c_str());
		int InfoLogLength;
		glGetShaderiv(ShaderName, GL_INFO_LOG_LENGTH, &InfoLogLength);
		if(InfoLogLength > 0)
		{
			std::vector<char> Buffer(InfoLogLength);
			glGetShaderInfoLog(ShaderName, InfoLogLength, NULL, &Buffer[0]);
			fprintf(stdout, "%s\n", &Buffer[0]);
		}

		return Result == GL_TRUE;
	}

	inline GLuint createShader
	(
		GLenum Type,
		std::string const & Source
	)
	{
		return createShader(Type, std::string(), Source);
	}

	inline GLuint createShader
	(
		GLenum Type,
		std::string const & Arguments, 
		std::string const & Source
	)
	{
		GLuint Name = 0;

		if(!Source.empty())
		{
			std::string SourceContent = glf::loadFile(Source);
			char const * SourcePointer = SourceContent.c_str();
			Name = glCreateShader(Type);
			glShaderSource(Name, 1, &SourcePointer, NULL);
			glCompileShader(Name);
		}

		return Name;
	}
}//namespace glf
