#pragma once
#include"CoreVariables.h"

class Scene
{
	vector<MainObject*> game_objects;
public:

	void AddObject(MainObject* object) {
		game_objects.push_back(object);
	}

	void FixedUpdate() {
		for (size_t i = 0; i < game_objects.size(); i++)
		{
			game_objects.at(i)->FixedUpdate();
		}
	}
	void Update(float deltaTime) {
		for (size_t i = 0; i < game_objects.size(); i++)
		{
			game_objects.at(i)->Update(deltaTime);
		}
	}
	void LateUpdate() {
		for (size_t i = 0; i < game_objects.size(); i++)
		{
			game_objects.at(i)->LateUpdate();
		}
	}
};
