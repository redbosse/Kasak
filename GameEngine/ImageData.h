#pragma once
#ifndef _IMAGE_DATA_

#include "ImageLoader.h"

class ImageData
{
public:

	unsigned char* data;
	int height;
	int width;
	int channels;
	size_t length;

	void fromImageLoader(const ImageLoader& loader) {
		length = sizeof(loader.data) * sizeof(unsigned char);

		data = loader.data;

		height = loader.height;
		width = loader.width;
		channels = loader.nrChannels;
	}

	~ImageData() {
		free(data);
	}
};

#endif

#define _IMAGE_DATA_