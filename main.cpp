#include <cmath>

#include "image.h"
#include "camera.h"
#include "shape.h"
#include "lightSource.h"
#include "rayTrace.h"



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
		Color(0.9f, 0.3f, 0.2f), 0.3f);
	scene.addShape(&sphere);

	Sphere sphere3(Point(7.0f, 7.0f, -5.0f), 2.0f,
		Color(0.6f, 0.8f, 0.9f), 0.4f);
	scene.addShape(&sphere3);

	Sphere sphere2(Point(5.0f, 4.0f, 0.0f), 3.0f,
		Color(0.2f, 0.1f, 1.0f), 0.4f);
	scene.addShape(&sphere2);

    // center, radius, surfaceColor, reflection,
    // transparency, emissionColor
	// Sphere lightSource(Point(-10.0f, 7.0f, 5.0f), 2.0f, 
	// 				   Color(0.0f), 0.0f, 0.0f, Color(1.0f));


    LightSource lightSource(Vector(5.0f, 8.0f, 4.0f));


	rayTrace(image, &camera, &scene, lightSource);


	std::string filename = "renderedImage.ppm";
	if (argc > 1)
		filename = argv[1];

	image.saveImagePPM(filename);
}