#pragma once
#ifndef _object_

class Object
{
protected:
	Object() {}

public:
	virtual void Awake() = 0;
	virtual void Start() = 0;
	virtual void CameraMatrixUpdate() = 0;
	virtual void UpdateViewAndProjectionMatrix() = 0;
	virtual void Update(float deltaTime) = 0;
	virtual void FixedUpdate() = 0;
	virtual void LateUpdate() = 0;
	virtual const char* ToString() = 0;
};
#endif

#define _object_
