#pragma once

#ifndef _Core_

#include "GLAPI.h"
#include "MainObject.h"
#include "include/glm/gtx/transform.hpp"
#include "include/glm/mat4x4.hpp"
#include "include/glm/vec3.hpp"
#include "include/glm/gtc/type_ptr.hpp"
#include "Time.h"
#include "Object.h"
#include "Debuger.h"

#include <iostream>
#include <string>
#include <thread>

using namespace std;

class RenderCore
{
private:
	GLFWwindow* window;

	bool isExit = false;

	vector<MainObject*> objects;

public:
	bool isInit = false;

	//vector<Ligth*> ligths;

	vec2 resolution;

	void AddObject(MainObject* obj) {
		objects.push_back(obj);
	}

	int CreateWindowContext(int width, int height, const char* title) {
		if (!glfwInit())
			return -1;

		window = glfwCreateWindow(width, height, title, NULL, NULL);
		if (!window)
		{
			glfwTerminate();
			return -1;
		}

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);

		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

		glfwMakeContextCurrent(window);

		glfwSetDropCallback(window, DragAndDropContext);

		glewExperimental = GL_TRUE;
		if (glewInit() != GLEW_OK)
		{
			return -1;
		}
	}

	static void DragAndDropContext(GLFWwindow* window, int count, const char** paths) {
		int i;
		for (i = 0; i < count; i++) {
			Debug_Log(paths[i]);
		}
	}

	enum RenderParameters {
		DepthTest = 0,

		FrontCullRender,
		BackCullRender,
		BothCullRender,
		NoCullRender
	};

	void RenderEnable(RenderParameters parameter) const {
		if (isInit) {
			switch (parameter)
			{
			case RenderParameters::DepthTest:

				glEnable(GL_DEPTH_TEST);
				glDepthFunc(GL_LESS);

				break;

			case RenderParameters::BackCullRender:

				glEnable(GL_CULL_FACE);
				glCullFace(GL_BACK);

				break;

			case RenderParameters::FrontCullRender:

				glEnable(GL_CULL_FACE);
				glCullFace(GL_FRONT);

				break;

			case RenderParameters::BothCullRender:
				glEnable(GL_CULL_FACE);
				glCullFace(GL_FRONT_AND_BACK);

				break;

			case RenderParameters::NoCullRender:
				glDisable(GL_CULL_FACE);

				break;

			default:
				break;
			}
		}
	}

	void SetCamera(Camera* camera) {
		for (size_t i = 0; i < objects.size(); i++)
		{
			objects.at(i)->SetCamera(camera);
		}
	}

	void StartGameUpdate() {
		for (size_t i = 0; i < objects.size(); i++)
		{
			objects.at(i)->Awake();
		}

		for (size_t i = 0; i < objects.size(); i++)
		{
			objects.at(i)->Start();
		}

		double delta_time = 0;

		while (!isExit)
		{
			isExit = glfwWindowShouldClose(window);

			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			for (size_t i = 0; i < objects.size(); i++)
			{
				objects.at(i)->LateUpdate();
			}
			/*
			for (size_t i = 0; i < objects.size(); i++)
			{
				objects.at(i)->FixedUpdate();
			}
			*/

			for (size_t i = 0; i < objects.size(); i++)
			{
				objects.at(i)->CameraMatrixUpdate();
			}

			for (size_t i = 0; i < objects.size(); i++)
			{
				objects.at(i)->UpdateViewAndProjectionMatrix();
			}

			for (size_t i = 0; i < objects.size(); i++)
			{
				objects.at(i)->RenderFrameUpdate();
			}

			float delta = glfwGetTime() - delta_time;

			for (size_t i = 0; i < objects.size(); i++)
			{
				objects.at(i)->Update(delta);
			}

			delta_time = glfwGetTime();

			glfwSwapBuffers(window);
			glfwPollEvents();

			std::this_thread::sleep_for(10ms);
		}

		glfwTerminate();
	}
};

class Material
{
public:
	enum  ShaderType
	{
		vertex,
		fragment,
		geometry
	};

	class Shader {
		GLchar info[512];

	public:

		GLuint shader;

		const GLchar* source;
		GLint success;

		Shader() {}
		Shader(const char* source, ShaderType type) {
			setSource(source);
			Create(type);
		}

		void setSource(const char* src) {
			source = src;
		}

		void Create(ShaderType type) {
			switch (type)
			{
			case ShaderType::fragment:

				shader = glCreateShader(GL_FRAGMENT_SHADER);

				break;
			case ShaderType::geometry:

				shader = glCreateShader(GL_GEOMETRY_SHADER);

				break;
			case ShaderType::vertex:

				shader = glCreateShader(GL_VERTEX_SHADER);

				break;

			default:
				break;
			}

			glShaderSource(shader, 1, &source, NULL);
			glCompileShader(shader);
			glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		}

		const char* getCompileInfo() {
			if (success) {
				return "Shader Compiled";
			}
			else {
				glGetShaderInfoLog(shader, 512, NULL, info);

				return info;
			}
		}

		void deleteShader() {
			glDeleteShader(shader);
		}

		void attach(GLuint program) {
			glAttachShader(program, shader);
		}
	};

	GLuint programm;

	GLint isError = 0;
	GLchar error_info[512];

	vector<Shader*> shaders;
	vector<pair<unsigned int, const char*>> textureID;

	void AddTexture(unsigned int texture_id, const char* texture_name) {
		textureID.push_back(pair<unsigned int, const char*>(texture_id, texture_name));
	}

	void use() {
		glUseProgram(programm);
	}

	void deUse() { glUseProgram(0); }

	void CreateShaderProgramm() {
		programm = glCreateProgram();

		for (size_t i = 0; i < shaders.size(); i++)
		{
			shaders.at(i)->attach(programm);
		}

		glLinkProgram(programm);

		glGetProgramiv(programm, GL_LINK_STATUS, &isError);
		if (!isError) {
			glGetProgramInfoLog(programm, 512, NULL, error_info);
		}

		for (size_t i = 0; i < shaders.size(); i++)
		{
			shaders.at(i)->deleteShader();
		}
	}

	void setFloat(const char* value_name, float value) const {
		glUniform1f(glGetUniformLocation(programm, value_name), (float)value);
	}

	void setInt(const char* value_name, int value) const {
		glUniform1i(glGetUniformLocation(programm, value_name), (int)value);
	}

	void setBool(const char* value_name, bool value) const {
		glUniform1i(glGetUniformLocation(programm, value_name), (bool)value);
	}

	void setVec3(const char* value_name, glm::vec3 value) const {
		glUniform3f(glGetUniformLocation(programm, value_name), (GLfloat)value.x, (GLfloat)value.y, (GLfloat)value.z);
	}

	void setVec4(const char* value_name, glm::vec4 value) const {
		glUniform4f(glGetUniformLocation(programm, value_name), (GLfloat)value.x, (GLfloat)value.y, (GLfloat)value.z, (GLfloat)value.w);
	}

	void setMat4(const char* value_name, glm::mat4 value) const {
		glUniformMatrix4fv(glGetUniformLocation(programm, value_name), 1, GL_FALSE, value_ptr(value));
	}

	void setMat3(const char* value_name, glm::mat3 value) const {
		glUniformMatrix3fv(glGetUniformLocation(programm, value_name), 1, GL_FALSE, value_ptr(value));
	}
};

#endif // !_Core_
#define _Core_