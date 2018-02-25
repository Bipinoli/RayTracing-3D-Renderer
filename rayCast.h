#pragma once

#include <iostream>


Color castRay(const Ray& ray, Shape* scene, LightSource& lightSource) {

	Color color(0.0f);

	Intersection intersection(ray);

	if (scene->intersect(intersection)) {
		// determine shadow

		Point hitPoint = ray.calculate(intersection.t);

		Ray shadowRay = Ray();
		shadowRay.origin = hitPoint;
		shadowRay.direction = lightSource.position - shadowRay.origin;
		float length2 = dot(shadowRay.direction, shadowRay.direction);
		shadowRay.direction.normalize();

		Intersection shadowIntersection(shadowRay);

		//  if no intersection than no shadow
		//  but if it intersected but away from the light source than also no shadow

		if (!scene->intersect(shadowIntersection) || pow(shadowIntersection.t, 2) >= length2 )
			color = intersection.color * lightSource.brightness * (1.0/length2); // inverse square law	

	}

	return color;
}



void rayCast(Image& image, Camera* camera, Shape* scene, LightSource& lightSource) {
	
	std::cout << " rayCasting " << std::endl;

	for (int x=0; x < image.getWidth(); x++) {
		for (int y=0; y < image.getHeight(); y++) {
			
			float xx = (2.0f*x) / image.getWidth() - 1.0f; // from -1 to 1
			float yy = (-2.0f*y) / image.getHeight() + 1.0f; // from 1 to -1

			Vector2 screenCoord(xx, yy);
			Ray ray = camera->makeRay(screenCoord);


			Color* pixelColor = image.getPixel(x, y);
			*pixelColor = castRay(ray, scene, lightSource);
		}
	}
}