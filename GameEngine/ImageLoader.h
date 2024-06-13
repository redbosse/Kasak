#pragma once

#include <iostream>

using namespace std;

class ImageLoader
{
public:
	int width = 0, height = 0, nrChannels = 0;
	unsigned char* data;

	int Load(const char* path);

	void Free();

	~ImageLoader();
};
