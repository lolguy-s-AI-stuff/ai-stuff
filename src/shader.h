#include <fstream>
#include <string>
#include <sstream>
struct ShaderProgramSource
{
	std::string VertexSource;
	std::string FragmentSource;
};

enum ShaderType
{
	NONE = -1, VERTEX = 0, FRAGMENT = 1
};

ShaderProgramSource ParseShader(const std::string& filepath)
{

	std::ifstream stream(filepath);
	std::string line;
	std::stringstream ss[2];
	ShaderType type = NONE;

	if (!stream.is_open())
		std::cout << "[Error]failed to open the shader" << std::endl;

	while (getline(stream, line))
	{
		if (line.find("#shader") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos)
				type = VERTEX;
			else if (line.find("fragment") != std::string::npos)
				type = FRAGMENT;
		}
		else
		{
			ss[(int) type] << line << '\n';
		}
	}
	std::cout << ss[0].str() << std::endl;
	std::cout << ss[1].str() << std::endl;

	ShaderProgramSource s = {ss[0].str(), ss[1].str()};
	return s;
}

unsigned int CompileShader(unsigned int type, const std::string& source)
{
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	// Error handling
	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	std::cout << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader compile status: " << result << std::endl;
	if (result == GL_FALSE)
	{
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*) alloca(length * sizeof(char));
		glGetShaderInfoLog(id, length, &length, message);
		std::cout
			<< "Failed to compile "
			<< (type == GL_VERTEX_SHADER ? "vertex" : "fragment")
			<< "shader"
			<< std::endl;
		std::cout << message << std::endl;
		glDeleteShader(id);
		return 0;
	}

	return id;
}

unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
	// create a shader program
	unsigned int program = glCreateProgram();
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	glAttachShader(program, vs);
	glAttachShader(program, fs);

	glLinkProgram(program);

	GLint program_linked;

	glGetProgramiv(program, GL_LINK_STATUS, &program_linked);
	std::cout << "Program link status: " << program_linked << std::endl;
	if (program_linked != GL_TRUE)
	{
		GLsizei log_length = 0;
		GLchar message[1024];
		glGetProgramInfoLog(program, 1024, &log_length, message);
		std::cout << "Failed to link program" << std::endl;
		std::cout << message << std::endl;
	}

	glValidateProgram(program);

	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}
