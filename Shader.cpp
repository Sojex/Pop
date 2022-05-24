#include "Shader.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "Renderer.h"



Shader::Shader(const std::string& filepath)
	: m_FilePath(filepath), m_RendererID(0)
{
    ShaderProgramSource source = ParseShader(filepath);

    m_RendererID = CreateShader(source.VertexSource, source.FragmentSource);

}

Shader::~Shader()
{
    GLCall(glDeleteProgram(m_RendererID));
}


static inline bool substring_in_string(const std::string& String, const std::string& substring) {
    return String.find(substring) != std::string::npos;
}



ShaderProgramSource Shader::ParseShader(const std::string& filepath) {
    std::ifstream stream(filepath);
    std::stringstream vertex;
    std::stringstream fragment;
    std::stringstream* active_ss = nullptr;
    std::string line;

    while (getline(stream, line)) {
        if (substring_in_string(line, "#shader")) {
            if (substring_in_string(line, "vertex")) {
                active_ss = &vertex;
            }
            else if (substring_in_string(line, "fragment")) {
                active_ss = &fragment;
            }
        }
        else if (active_ss) {
            *active_ss << line << "\n";
        }
    }

    return { vertex.str(), fragment.str() };
}


unsigned int Shader::compileShader(unsigned int type, const std::string& source)
{

    unsigned int id = glCreateShader(type);


    const char* src = source.c_str();
    //specifies source of the shader
    //theshader, count(how many source codes are specified), memory address of source variable, length(if null is determined to be null terminated) 
    glShaderSource(id, 1, &src, nullptr);

    glCompileShader(id);



    // ERROR HANDLING
    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);

    if (result == GL_FALSE)
    {
        int length;

        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);

        char* message = (char*)alloca(length * sizeof(char));

        glGetShaderInfoLog(id, length, &length, message);

        std::cout << "FAILED TO COMPILE" << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << "shader" << std::endl;
        std::cout << message << std::endl;

        glDeleteShader(id);
        return 0;
    }

    return id;
}

unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
	unsigned int program = glCreateProgram();

	unsigned int vs = compileShader(GL_VERTEX_SHADER, vertexShader);

	unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);

	glAttachShader(program, vs);

	glAttachShader(program, fs);

	//links program 

	glLinkProgram(program);

	glValidateProgram(program);

	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}

void Shader::Bind() const
{
    GLCall(glUseProgram(m_RendererID));
}

void Shader::Unbind() const
{
    GLCall(glUseProgram(0));

}

void Shader::setUniform1i(const std::string& name, int value)
{
    GLCall(glUniform1i(GetUniformLocation(name), value));
}

void Shader::setUniform1f(const std::string& name, float value)
{
    GLCall(glUniform1f(GetUniformLocation(name), value));
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
    GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}

void Shader::SetUniformMat4f(const std::string& name, const glm::mat4& matrix)
{
    GLCall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
}

int Shader::GetUniformLocation(const std::string& name)
{
    if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
    {
        return m_UniformLocationCache[name];
    }

    GLCall(int location = glGetUniformLocation(m_RendererID, name.c_str()));
    if (location == -1)
    {
        std::cout << "WARNING: UNIFORM " << name << " doesn't exist!" << std::endl;
    }
    m_UniformLocationCache[name] = location;
    
    return location;
}
