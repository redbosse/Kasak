#pragma once
#include "GLAPI.h"
#include <vector>
#include "ImageData.h"
#include "Debuger.h"

using namespace std;

class TextureManager
{
public:

	vector<pair<unsigned char, const char*>> texture_ids;

	inline unsigned char getTextureID(size_t index) {
		return texture_ids.at(index).first;
	}

	inline const char* getTextureName(size_t index) {
		return texture_ids.at(index).second;
	}

	void bindTexture(const ImageData& data, const char* texture_name, bool isMipMap = true) {
		unsigned int textureID;

		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, data.width, data.height, 0, GL_RGB, GL_UNSIGNED_BYTE, data.data);

		if (isMipMap) {
			glGenerateMipmap(GL_TEXTURE_2D);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		}
		else
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		}

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		Debug_Log(texture_name);
		Debug_Log(textureID);

		texture_ids.push_back(pair<unsigned char, const char*>(textureID, texture_name));
	}
};
