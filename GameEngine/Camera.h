#pragma once
#include "Component.h"
class Camera :
	public Component
{
public:

	float FOV = 90.0f;
	float zNear = 0.1f;
	float aspect = 1.0f;
	float zFar = 100.0f;

	mat4 viewMatrix = mat4();
	mat4 projectionMatrix = mat4();

	void OnEnableCamera() {
	}

	void Start() {
	}

	void CameraMatrixUpdate() {
		transform->viewMatrix = glm::lookAt(transform->rotation * vec3(0, 0, 1), transform->position, vec3(0, 1, 0));
		transform->projectionMatrix = glm::perspective(FOV, aspect, zNear, zFar);
	}

	void Update(float deltaTime) {
	}

	Camera() {
		this->isCamera = true;
	}
};
