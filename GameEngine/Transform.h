#pragma once
#include "include/glm/vec3.hpp"
#include "include/glm/mat4x4.hpp"
#include "include/glm/gtx/quaternion.hpp"
#include "include/glm/gtx/transform.hpp"
#include "include/glm/gtc/matrix_transform.hpp"

using namespace glm;

class Transform
{
public:

	vec3 position = vec3(0.0, 0.0, 0.0);
	vec3 localposition = vec3(0.0, 0.0, 0.0);
	quat rotation = quat();
	quat localRotation = quat();
	vec3 scale = vec3(1.0, 1.0, 1.0);
	vec3 localScale = vec3(1.0, 1.0, 1.0);
	mat4 transform_matrix;

	mat4 viewMatrix = mat4();
	mat4 projectionMatrix = mat4();
	Transform* cameraTransform;

	void SetRotate(vec3 axis, float angle) {
		rotation = glm::rotate(angle, axis);
	}

	void Rotate(vec3 axis, float angle) {
		rotation = glm::rotate(rotation, angle, axis);
	}

	mat4 ToTransformMatrix() {
		transform_matrix = glm::translate(position) * glm::toMat4(rotation) * glm::scale(scale);

		return transform_matrix;
	}
};
