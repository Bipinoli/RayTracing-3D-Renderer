#include "objParser.h"


ObjParser::ObjParser(std::string fileName) {

	std::ifstream objFile(fileName);
	std::stringstream ss;
	std::string line;

	if (objFile.is_open()) {
		while (getline(objFile, line)) {
			
			ss.clear();
			ss << line;

			char c;
			ss >> c;
			if (c == 'v') {
				float coords[3];
				for (int i=0; i<3; i++) {
					ss >> coords[i];
				}
				vertices.push_back(Point(coords[0], coords[1], coords[2]));
			}

			else {
				int a, b, c;
				ss >> a >> b >> c;
				a--; b--; c--; 

				Color surfaceColor(0.9f, 0.2f, 0.1f);

				Point verts[] = { vertices[a], vertices[b], vertices[c]};

				triangles.push_back(Triangle( verts, surfaceColor));
			}


		}
		objFile.close();
	}


	std::cout << fileName << " parsed successfully. " << std::endl;
	std::cout << "vertices : " << vertices.size() << std::endl;
	std::cout << "triangles : " << triangles.size() << std::endl;
	std::cout << "-------------------------------------------------" << std::endl;
}