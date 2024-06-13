#pragma once
#include "Transform.h"
#include <typeinfo>
#include <string>

class Component
{
public:
	const char* name;
	bool isCamera = false;

	Component() {
		name = TypeName();
	}

	Transform* transform = new Transform();

	virtual void OnTransformSetup() {};
	virtual void Awake() {};
	virtual void Start() {};
	virtual void CameraMatrixUpdate() {};
	virtual void RenderFrameUpdate() {};
	virtual void Update(float deltaTime) {};
	virtual void LateUpdate() {};
	virtual void FixedUpdate() {};

	const char* TypeName() {
		return typeid(this).name();
	}
};
