#pragma once

#include "include/glm/vec3.hpp"
#include "include/glm/mat4x4.hpp"

#include <vector>
#include <string>

using namespace std;
using namespace glm;

class Mesh
{
public:

	string name = "";

	struct Vertex {
		vec3 position;
		vec3 normal;
		vec3 bitangent;
		vec3 tangent;
		vec3 texCoords;

		Vertex(const vec3& position, const vec3& normal, const vec3& texCoords)
			: position(position), normal(normal), texCoords(texCoords)
		{
		}

		Vertex() = default;
	};

	struct BoundingBox {
		vec3 boundingMax;
		vec3 boundingMin;

		BoundingBox() = default;

		BoundingBox(const vec3& boundingMax, const vec3& boundingMin)
			: boundingMax(boundingMax), boundingMin(boundingMin)
		{
		}
	};

	struct Texture {
		unsigned short id;
		string texture_type;
	};

	Mesh() {}

	Mesh(const Mesh& mesh) {
		this->vertices = mesh.vertices;
		this->vertex_indices = mesh.vertex_indices;
	}

	vector<Vertex> vertices;
	vector<int> vertex_indices;
	BoundingBox boundings;
	mat4 pivot = mat4();
};
