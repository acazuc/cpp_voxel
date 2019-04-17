#ifndef SHADER_H
# define SHADER_H

# include <librender/Shader/Program.h>
# include <string>

using librender::ProgramLocation;
using librender::Program;

namespace voxel
{

	class Shader
	{

	protected:
		void loadProgram(std::string vertex, std::string fragment);
		void loadUniform(ProgramLocation &location, std::string name);
		void loadAttrib(ProgramLocation &location, std::string name);

	public:
		Program *program;
		Shader();
		~Shader();

	};

}

#endif
