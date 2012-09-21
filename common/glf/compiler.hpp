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

#ifndef GLF_COMPILER_INCLUDED
#define GLF_COMPILER_INCLUDED

#include "glf.hpp"

namespace glf
{
	class compiler
	{
		typedef std::map<std::string, GLuint> names_map;
		typedef std::map<GLuint, std::string> files_map;

		class commandline
		{
			enum profile
			{
				CORE,
				COMPATIBILITY
			};

		public:
			commandline(std::string const & Arguments) :
				Profile("core"),
				Version(420)
			{}

			void parseArguments
			(
				std::string const & Arguments
			)
			{
				std::stringstream Stream(Arguments);
				std::string Param;
				std::vector<std::string> Defines;
				std::vector<std::string> Includes;

				while(Stream >> Param)
				{
					std::size_t Found = Param.find("-D");
					if(Found != std::string::npos)
					{
						std::string Define;
						Stream >> Define;
						Defines.push_back(Define.substr(2));
					}
					else if(Param == "--define")
					{
						std::string Define;
						Stream >> Define;
						Defines.push_back(Define);
					}
					else if((Param == "--version") || (Param == "-v"))
						Stream >> Version;
					else if((Param == "--profile") || (Param == "-p"))
						Stream >> Profile;
					else if (Param == "--include" || Param == "-i")
					{
						std::string Include;
						Stream >> Include;
						Includes.push_back(Include);
					}
		/*
					else 
					{
						std::stringstream err;
						err << "unknown parameter type: \"" << Param << "\"";
						glDebugMessageInsertARB(
							GL_DEBUG_SOURCE_APPLICATION_ARB, GL_DEBUG_TYPE_OTHER_ARB, 1, GL_DEBUG_SEVERITY_LOW_ARB, 
							-1, std::string(std::string("unknown parameter type: \"") << Param << std::string("\"")).c_str());
					}
					if(!Stream) 
					{
						glDebugMessageInsertARB(
							GL_DEBUG_SOURCE_APPLICATION_ARB, GL_DEBUG_TYPE_OTHER_ARB, 1, GL_DEBUG_SEVERITY_LOW_ARB, 
							-1, std::string(std::string("error parsing parameter: \"") << Param << std::string("\"")).c_str());
					}
		*/
				}
			}

			int getVersion() const
			{
				return this->Version;
			}

			std::string getProfile() const
			{
				return this->Profile;
			}

			std::string getDefines() const
			{
				std::string Result;
				for(std::size_t i = 0; i < this->Defines.size(); ++i)
					Result += Defines[i];
				return Result;
			}

		private:
			std::string Profile;
			int Version;
			std::vector<std::string> Defines;
		};

	public:
		~compiler();

		GLuint create(GLenum Type, std::string const & Filename);
		GLuint create(GLenum Type, std::string const & Arguments, std::string const & Filename);
		bool destroy(GLuint const & Name);

		bool check();
		// TODO: Not defined
		bool check(GLuint const & Name);
		void clear();

	private:
		bool loadBinary(
			std::string const & Filename,
			GLenum & Format,
			std::vector<glm::byte> & Data,
			GLint & Size);
		std::string loadFile(
			std::string const & Filename);

		names_map ShaderNames;
		files_map ShaderFiles;
		names_map PendingChecks;
	};
}//namespace glf

#include "compiler.inl"

#endif//GLF_COMPILER_INCLUDED
