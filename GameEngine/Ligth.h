#pragma once
#include "Component.h"
#include "MeshFilter.h"

class Ligth :
	public Component
{
public:
	float intensity = 1;

	MeshFilter* filter;

	void RenderFrameUpdate() {
		//filter->ligthPosition = transform->position;
	}
};
