#pragma once

#include <string>

#include "color.h"

class Image
{
protected:
	int width, height;
	Color* data;

public:
	Image(int width, int height);

	virtual ~Image();
	
	int getWidth() const;
	int getHeight() const;

	Color* getPixel(int x, int y);


	void saveImagePPM(std::string filename) const;

};
