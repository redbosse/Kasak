#pragma once
#include "Mesh.h"
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>

#include "assimp/cimport.h"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

#include "Debuger.h"

using namespace std;

class ModelLoader
{
private:
	const aiScene* scene = NULL;

	static inline vec3 aiVec3DtoVec3GLM(const aiVector3D& in_vec) {
		return vec3(in_vec.x, in_vec.y, in_vec.z);
	}

	static inline vec3 aiFacetoVec3GLM(const aiFace& in_face) {
		return vec3(in_face.mIndices[0], in_face.mIndices[1], in_face.mIndices[2]);
	}

	static inline glm::mat4 aiMatrixtoMat4GLM(const aiMatrix4x4& from)
	{
		glm::mat4 to;

		to[0][0] = from.a1; to[1][0] = from.a2; to[2][0] = from.a3; to[3][0] = from.a4;
		to[0][1] = from.b1; to[1][1] = from.b2; to[2][1] = from.b3; to[3][1] = from.b4;
		to[0][2] = from.c1; to[1][2] = from.c2; to[2][2] = from.c3; to[3][2] = from.c4;
		to[0][3] = from.d1; to[1][3] = from.d2; to[2][3] = from.d3; to[3][3] = from.d4;
		return to;
	}

public:
	int AssimpLoader(const char* path) {
		scene = aiImportFile(path, aiProcessPreset_TargetRealtime_MaxQuality);

		if (scene)
		{
			scene = aiApplyPostProcessing(scene, aiProcess_Triangulate);

			if (scene->mNumMeshes > 0) {
				aiMesh* aMesh = scene->mMeshes[0];

				mesh = Mesh();

				mesh.boundings = Mesh::BoundingBox(aiVec3DtoVec3GLM(aMesh->mAABB.mMax), aiVec3DtoVec3GLM(aMesh->mAABB.mMin));
				mesh.name = aMesh->mName.C_Str();
				mesh.pivot = aiMatrixtoMat4GLM(scene->mRootNode->mTransformation);

				mesh.vertices = vector<Mesh::Vertex>(aMesh->mNumVertices);

				for (size_t i = 0; i < aMesh->mNumVertices; i++)
				{
					Mesh::Vertex vertex = Mesh::Vertex(vec3(), vec3(), vec3());

					vertex.position = aiVec3DtoVec3GLM(aMesh->mVertices[i]);
					vertex.normal = aiVec3DtoVec3GLM(aMesh->mNormals[i]);

					vertex.bitangent = aiVec3DtoVec3GLM(aMesh->mBitangents[i]);
					vertex.tangent = aiVec3DtoVec3GLM(aMesh->mTangents[i]);

					vertex.texCoords = aiVec3DtoVec3GLM(aMesh->mTextureCoords[0][i]);

					mesh.vertices[i] = vertex;
				}

				mesh.vertex_indices = vector<int>();

				for (size_t i = 0; i < aMesh->mNumFaces; i++)
				{
					if (aMesh->mFaces[i].mNumIndices == 3)
					{
						mesh.vertex_indices.push_back(aMesh->mFaces[i].mIndices[0]);
						mesh.vertex_indices.push_back(aMesh->mFaces[i].mIndices[1]);
						mesh.vertex_indices.push_back(aMesh->mFaces[i].mIndices[2]);
					}
					else {
						Debug_Log("Face is not 3!");
					}
				}
			}

			return 0;
		}

		return 1;
	}

	Mesh mesh;
};
