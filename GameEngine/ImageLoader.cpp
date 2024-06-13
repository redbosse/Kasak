#include "ImageLoader.h"

#define STB_IMAGE_IMPLEMENTATION

#include <iostream>

#include "include/img/stb_image.h"

int ImageLoader::Load(const char* path) {
	stbi_set_flip_vertically_on_load(true);
	data = stbi_load(path, &width, &height, &nrChannels, 0);

	return 0;
}

void ImageLoader::Free() {
	stbi_image_free(data);
}

ImageLoader::~ImageLoader() {
	Free();
}