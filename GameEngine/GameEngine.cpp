#pragma once

#include "include/glm/vec3.hpp"
#include "include/glm/gtc/type_ptr.hpp"
#include "include/glm/ext/matrix_transform.hpp"
#include "include/glm/trigonometric.hpp"

#define _ECORE_
#define toRadians(degrees) degrees * 0.01745329251994329576923690768489

#include "CoreVariables.h"
#include "MeshFilter.h"
#include "CustomObject.h"

#include "ImageData.h"
#include "TextureManager.h"
#include "ModelLoader.h"

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include "RenderCore.h"
#include "Camera.h"
#include "Ligth.h"

using namespace glm;
using namespace std;

int main(void)
{
	InitEngine(vec2(1920, 1080), "Game");
	core.RenderEnable(RenderCore::RenderParameters::DepthTest);
	core.RenderEnable(RenderCore::RenderParameters::BackCullRender);

	Mesh* mesh = new Mesh();

	ImageLoader* albedo = new ImageLoader();
	albedo->Load("textures/PBR/albedo.jpg");

	ImageData* albedo_data = new ImageData();
	albedo_data->fromImageLoader(*albedo);

	ImageLoader* normal = new ImageLoader();
	normal->Load("textures/PBR/normal.jpg");

	ImageData* normal_data = new ImageData();
	normal_data->fromImageLoader(*normal);

	ImageLoader* rough = new ImageLoader();
	rough->Load("textures/PBR/roughness.jpg");

	ImageData* rough_data = new ImageData();
	rough_data->fromImageLoader(*rough);

	ImageLoader* ao = new ImageLoader();
	ao->Load("textures/PBR/ao.jpg");

	ImageData* ao_data = new ImageData();
	ao_data->fromImageLoader(*ao);

	ImageLoader* metallic = new ImageLoader();
	metallic->Load("textures/PBR/metallic.jpg");

	ImageData* metallic_data = new ImageData();
	metallic_data->fromImageLoader(*metallic);

	ifstream fs1("shaders/PBR/frag.vt");
	ifstream fs2("shaders/PBR/vert.vt");

	stringstream frag;

	frag << fs1.rdbuf();

	stringstream vert;

	vert << fs2.rdbuf();

	fs1.close();
	fs2.close();

	string v = vert.str();
	string f = frag.str();

	char* vt = const_cast<char*>(v.c_str());
	char* fr = const_cast<char*>(f.c_str());

	Material* material = new Material();

	Material::Shader* shadervt = new Material::Shader(vt, Material::ShaderType::vertex);
	Material::Shader* shaderfr = new Material::Shader(fr, Material::ShaderType::fragment);

	material->shaders.push_back(shaderfr);
	material->shaders.push_back(shadervt);

	material->CreateShaderProgramm();

	ModelLoader lder;
	int error_code = lder.AssimpLoader("model/drone.fbx");

	Debug_Log(lder.mesh.name);
	Debug_Log(error_code);

	TextureManager* t_manager = new TextureManager();

	t_manager->bindTexture(*albedo_data, "albedo", true);
	t_manager->bindTexture(*normal_data, "normal", true);
	t_manager->bindTexture(*rough_data, "rough", true);
	t_manager->bindTexture(*ao_data, "ao", true);
	t_manager->bindTexture(*metallic_data, "metallic", true);

	material->AddTexture(t_manager->getTextureID(0), "albedoMap");

	material->AddTexture(t_manager->getTextureID(1), "normalMap");

	material->AddTexture(t_manager->getTextureID(2), "roughnessMap");

	material->AddTexture(t_manager->getTextureID(3), "aoMap");

	material->AddTexture(t_manager->getTextureID(4), "metallicMap");

	MainObject* obj = new MainObject();

	MeshFilter* meshFilter = new MeshFilter();

	meshFilter->setupMesh(lder.mesh);

	meshFilter->material = material;

	meshFilter->anglePerSecond = 0.4f;

	obj->transform->position = vec3(-0.0, -0.15, 0.4);

	obj->transform->scale = vec3(1, 1, 1) * 1.0f;

	obj->transform->SetRotate(vec3(0, 1, 0), 110);

	obj->AddComponent(meshFilter);

	MainObject* obj2 = new MainObject();

	MeshFilter* meshFilter2 = new MeshFilter();

	meshFilter2->setupMesh(lder.mesh);

	meshFilter2->material = material;

	meshFilter2->anglePerSecond = 0.1;

	obj2->transform->position = vec3(0.6, -0.8, 0.5);

	obj2->transform->scale = vec3(1, 1, 1) * 1.0f;

	obj2->transform->SetRotate(vec3(0, 1, 0), 110);

	obj2->AddComponent(meshFilter2);

	///

	MainObject* camera = new MainObject();

	Camera* cameraComponent = new Camera();

	camera->AddComponent(cameraComponent);

	camera->transform->position = vec3(0, 0, 0.7);
	camera->transform->scale = vec3(1, 1, 1);

	cameraComponent->FOV = toRadians(60);
	cameraComponent->aspect = core.resolution.x / core.resolution.y;

	core.AddObject(camera);
	core.AddObject(obj);
	core.AddObject(obj2);

	//core.ligths.push_back(ligthComponent);

	core.SetCamera(cameraComponent);

	//Debug_Log(cameraComponent);

	core.StartGameUpdate();

	return 0;
}