#include <algorithm>
#include <iostream>

#include "image.h"

#include <fstream>

Image::Image(int width, int height)
	: width(width), height(height)
{
	data = new Color[width * height];
}

Image::~Image()
{
	delete[] data;
}

int Image::getWidth() const
{
	return width;
}

int Image::getHeight() const
{
	return height;
}

Color* Image::getPixel(int x, int y)
{
	return data + (x + y * width);
}


void Image::saveImagePPM(std::string filename) const {

	// data[] contains the RGB information with 0 = dark, 1 = full bright
	// and the float betn 0 to 1 representing in between color

	try {
		std::ofstream ofs(filename, std::ios::binary | std::ios::out);

		ofs << "P6\n" << width << " " << height << " 255" << std::endl;
		for (int i=0; i<height; i++) {
			for (int j=0; j<width; j++) {
				ofs << (unsigned char)(std::min(data[i*width + j].r, 1.0f) * 255) 
					<< (unsigned char)(std::min(data[i*width + j].g, 1.0f) * 255)
					<< (unsigned char)(std::min(data[i*width + j].b, 1.0f) * 255);
			}
		}

		ofs.close();

	} catch(...) {
		std::cerr << "Something really terrible happened so " + filename + " file couldn't be created!!" << std::endl;
	}
	std::cout << "Rendered image to a " + filename + " file. (SUCCESS)" << std::endl;
}