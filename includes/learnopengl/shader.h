#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h> // include glad to get all the required OpenGL headers
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
using namespace std;

class Shader
{
public:
	// the program ID
	unsigned int ID;
	// constructor reads and builds the shader
	// ---------------------------------------
	Shader(const char* vertexPath, const char* fragmentPath);
	// use/activate the shader
	void Use();
	// utility uniform functions
	void SetBool(const string &name, bool value) const;
	void SetInt(const string &name, int value) const;
	void SetFloat(const string &name, float value) const;

private:
	// utility function for checking shader compilation/linking errors
	// ---------------------------------------------------------------
	void CheckCompileErrors(unsigned int shader, string type);

};

Shader::Shader(const char* vertexPath, const char *fragmentPath)
{
	// 1. retrieve the vertex/fragment source code from filePath
	string vertexCode;
	string fragmentCode;
	ifstream vShaderFile;
	ifstream fShaderFile;
	// ensure ifstream objects can throw exceptions
	vShaderFile.exceptions(ifstream::failbit | ifstream::badbit);
	fShaderFile.exceptions(ifstream::failbit | ifstream::badbit);
	try
	{
		// open files
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		stringstream vShaderStream, fShaderStream;
		// read file's buffer contents into streams
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		// close file handlers
		vShaderFile.close();
		fShaderFile.close();
		// convert stream into string
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch (ifstream::failure& e)
	{
		cerr << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY::READ\n" << strerror(errno) << endl;
	}

	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();

	// 2. compile shaders
	unsigned int vertex, fragment;
	// vertex shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	CheckCompileErrors(vertex, "VERTEX");

	// fragment shader
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	CheckCompileErrors(fragment, "FRAGMENT");

	// shader program
	ID = glCreateProgram();
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
	glLinkProgram(ID);
	CheckCompileErrors(ID, "PROGRAM");

	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

// activate the shader
// -------------------
void Shader::Use() {
	glUseProgram(ID);
}

// utility uniform functions
// -------------------------
void Shader::SetBool(const string &name, bool value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::SetInt(const string &name, int value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::SetFloat(const string &name, float value) const
{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::CheckCompileErrors(unsigned int shader, string type)
{
	int success;
	char infoLog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << infoLog;
		}
	}
	else
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << infoLog << endl;
		}
	}
}

#endif
