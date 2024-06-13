#pragma once
#include "Object.h"
#include "Component.h"
#include <vector>
#include <string>
#include "Debuger.h"
#include "Camera.h"

using namespace std;

class MainObject :
	public Object
{
public:
	Transform* transform = new Transform();
	Camera* cameraReference = nullptr;

	vector<Component*> components = vector<Component*>();

	void SetCamera(Camera* camera) {
		cameraReference = camera;
	}

	void AddComponent(Component* component) {
		component->transform = transform;

		component->OnTransformSetup();

		components.push_back(component);
	}

	Component& GetComponent(Component& component, const char* typeName) {
		for (size_t i = 0; i < components.size(); i++)
		{
			if (components[i]->name == typeName)
			{
				component = *components[i];
				return component;
			}
		}
	}

	virtual void Awake() {
		for (size_t i = 0; i < components.size(); i++)
		{
			components.at(i)->Awake();
		}
	}
	virtual void Start() {
		for (size_t i = 0; i < components.size(); i++)
		{
			components.at(i)->Start();
		}
	}

	virtual void UpdateViewAndProjectionMatrix() {
		if (cameraReference == nullptr) return;

		transform->cameraTransform = cameraReference->transform;
	}

	void CameraMatrixUpdate() {
		for (size_t i = 0; i < components.size(); i++)
		{
			if (components.at(i)->isCamera)
				components.at(i)->CameraMatrixUpdate();
		}
	}

	virtual void LateUpdate() {
		for (size_t i = 0; i < components.size(); i++)
		{
			components.at(i)->LateUpdate();
		}
	}

	virtual void RenderFrameUpdate() {
		for (size_t i = 0; i < components.size(); i++)
		{
			components.at(i)->RenderFrameUpdate();
		}
	}

	virtual void Update(float deltaTime) {
		for (size_t i = 0; i < components.size(); i++)
		{
			components.at(i)->Update(deltaTime);
		}
	}

	virtual void FixedUpdate() {
		for (size_t i = 0; i < components.size(); i++)
		{
			components.at(i)->FixedUpdate();
		}
	}

	virtual const char* ToString() {
		return "MainObject";
	}
};
