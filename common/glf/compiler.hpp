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
		public:
			commandline(std::string const & Arguments);

		private:

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
