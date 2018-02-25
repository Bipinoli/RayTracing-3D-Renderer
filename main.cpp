#include <cmath>

#include "image.h"
#include "camera.h"
#include "shape.h"
#include "lightSource.h"
#include "rayTrace.h"
#include "rayCast.h"
#include "objParser.h"


int main(int argc, char** argv)
{
	int width = 1920;
	int height = 1080;

	Image image(width, height);
	PerspectiveCamera camera(Point(-5.0f, 1.0f, 0.0f),
		Vector(0.0f, 1.0f, 0.0f), Vector(), M_PI / 4,
		(float)width / (float)height);

	ShapeSet scene;

	Plane floor(Point(0.0f, 0.0f, 0.0f), Vector(),
		Color(0.4f, 1.0f, 0.4f), 0.1f);
	scene.addShape(&floor);

	Sphere sphere(Point(0.0f, 1.0f, 0.0f), 1.0f,
		Color(0.9f, 0.3f, 0.2f), 0.7f);
	scene.addShape(&sphere);

	Sphere sphere3(Point(7.0f, 7.0f, -5.0f), 2.0f,
		Color(0.6f, 0.8f, 0.9f), 1.0f);
	scene.addShape(&sphere3);

	Sphere sphere2(Point(5.0f, 4.0f, 0.0f), 3.0f,
		Color(0.2f, 0.1f, 1.0f), 0.8f);
	scene.addShape(&sphere2);

	Sphere sphere4(Point(3.0f, 3.0f, 7.0f), 2.3f,
		Color(0.8f, 0.8f, 0.0f), 0.7f);
	scene.addShape(&sphere4);

	Sphere sphere5(Point(5.0f, 3.0f, -8.0f), 2.7f,
		Color(0.0f, 0.0f, 0.0f), 1.0f, 0.2f, 1.5f); // 0.6 refrac coeff, 1.5 refrac Index
	scene.addShape(&sphere5);

	Sphere sphere6(Point(-2.0f, 1.3f, 1.2f), 1.0f,
		Color(0.02f, 0.0f, 0.0f), 0.01f, 1.0f, 1.5f); // 0.6 refrac coeff, 1.5 refrac Index
	scene.addShape(&sphere6);

	// ---------------------------------------------
    // center, radius, surfaceColor, reflection,
    // transparency, emissionColor
	// Sphere lightSource(Point(-10.0f, 7.0f, 5.0f), 2.0f, 
	// 				   Color(0.0f), 0.0f, 0.0f, Color(1.0f));


	// Point vertices[] = {
	// 						Point(3.0f, 4.0f, 2.0f),
	// 						Point(3.0f, -4.0f, 2.0f),
	// 						Point(6.0f, 2.0f, 2.0f)
	// 					};

 //    Point vertices2[] = {
	// 						Point(3.0f, 4.0f, 2.0f),
	// 						Point(3.0f, -4.0f, 2.0f),
	// 						Point(6.0f, 8.0f, 2.0f)
	// 					};

	// Triangle triangle2(vertices2, Color(0.9f, 0.3f, 0.3f));
	// scene.addShape(&triangle2);

	// Triangle triangle(vertices, Color(0.2f, 0.3f, 0.9f));
	// scene.addShape(&triangle);


	// ObjParser objParser("pumpkin.obj");

	// // for (auto& it: objParser.triangles)
	// // 	scene.addShape(&it);

	// for (int i=0; i<100; i++)
	// 	scene.addShape(&objParser.triangles[i]);


    LightSource lightSource(Vector(5.0f, 15.0f, 4.0f), 270.0f);


	rayTrace(image, &camera, &scene, lightSource);
    // rayCast(image, &camera, &scene, lightSource);

	std::string filename = "renderedImage.ppm";
	if (argc > 1)
		filename = argv[1];

	image.saveImagePPM(filename);
}