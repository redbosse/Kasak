#pragma once
#ifndef _CoreVariables_

#include "Component.h"
#include "Mesh.h"
#include "Time.h"
#include "MainObject.h"
#include "RenderCore.h"
#include "Camera.h"

#ifdef _ECORE_

RenderCore core;

void InitEngine(vec2 resolution, string window_title) {
	if (core.CreateWindowContext(resolution.x, resolution.y, window_title.c_str()) != -1) {
		core.isInit = true;

		core.resolution = resolution;
	}
}

#endif

#endif // !_CoreVariables_
#define _CoreVariables_