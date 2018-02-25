#pragma once


#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include "vectormath.h"
#include "shape.h"



class ObjParser {

public:
	std::vector<Point> vertices;
	std::vector<Triangle> triangles;


	ObjParser(std::string fileName);
};


