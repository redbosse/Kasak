#pragma once

#ifndef _MeshFilter_
#include "include/glm/gtc/type_ptr.hpp"
#include "include/glm/gtc/matrix_transform.hpp"
#include "CoreVariables.h"
#include <string>
#include "include/glm/vec3.hpp"

class MeshFilter :
	public Component
{
	Mesh* _mesh = new Mesh();

public:

	vec3 m_scale = vec3(1, 1, 1);

	glm::vec3 ligth_Position = vec3(0, 1, 0.2);
	glm::vec3 ligth_Position2 = vec3(0, -1, 0);
	float angle = 0;
	float anglePerSecond = 5;

	Material* material = new Material();

	double timme = 0;

	unsigned int VAO = 0, VBO = 0, EBO = 0;

	float time_t = 0;

	void setupMesh(const Mesh& mesh) {
		_mesh = new Mesh(mesh);

		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		glBufferData(GL_ARRAY_BUFFER, _mesh->vertices.size() * sizeof(Mesh::Vertex), &_mesh->vertices[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, _mesh->vertex_indices.size() * sizeof(unsigned int),
			&_mesh->vertex_indices[0], GL_STATIC_DRAW);

		// vertex positions
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Mesh::Vertex), (void*)0);
		// vertex normals
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Mesh::Vertex), (void*)offsetof(Mesh::Vertex, normal));
		// vertex texture coords
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Mesh::Vertex), (void*)offsetof(Mesh::Vertex, texCoords));

		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Mesh::Vertex), (void*)offsetof(Mesh::Vertex, tangent));

		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Mesh::Vertex), (void*)offsetof(Mesh::Vertex, bitangent));

		glBindVertexArray(0);
	}

	void OnTransformSetup() {
	}

	void Start() {
		for (size_t i = 0; i < material->shaders.size(); i++)
		{
			Debug_Log(material->shaders[i]->getCompileInfo());
		}

		//transform->SetRotate(glm::vec3(0, 1, 0), 1.0);
	}

	void Update(float deltaTime) {
		transform->Rotate(glm::vec3(0, 1, 0), anglePerSecond * deltaTime);

		time_t += deltaTime;
	}

	void RenderFrameUpdate() {
		glBindVertexArray(VAO);

		material->use();

		for (size_t i = 0; i < material->textureID.size(); i++)
		{
			material->setInt(material->textureID[i].second, i);
		}

		for (size_t i = 0; i < material->textureID.size(); i++)
		{
			glActiveTexture(GL_TEXTURE0 + i);

			glBindTexture(GL_TEXTURE_2D, material->textureID[i].first);
		}

		material->setMat4("model", transform->ToTransformMatrix());
		material->setMat4("view", transform->cameraTransform->viewMatrix);
		material->setMat4("projection", transform->cameraTransform->projectionMatrix);
		material->setVec3("camPos", transform->cameraTransform->position);

		material->setVec3("lightPos", vec3(0, 0.5, 0.2));
		material->setVec3("lightPositions[0]", ligth_Position);
		material->setVec3("lightPositions[1]", ligth_Position2);
		material->setVec3("lightColors[0]", vec3(255, 255, 255) * 0.9f);
		material->setVec3("lightColors[1]", vec3(10, 10, 10));

		material->setMat3("normalMatrix", glm::transpose(glm::inverse(glm::mat3(transform->ToTransformMatrix()))));

		//material->setVec3("ligthColor", vec3(1, 1, 1));

		glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(_mesh->vertex_indices.size()), GL_UNSIGNED_INT, 0);

		glBindVertexArray(0);
		glUseProgram(0);

		material->deUse();
	}
};

#endif // !_MeshFilter_
#define _MeshFilter_