#pragma once



const int MAX_RECUR_DEPTH = 3;


Ray reflect(const Ray& ray, const Vector& normalVec, const Point& hitPosition ) {
	
	// normal vector should be a unit vector normal to the plane
	Ray reflectedRay;
	reflectedRay.direction = ray.direction - 2 * dot(ray.direction, normalVec) * normalVec;
	reflectedRay.direction.normalize();
	reflectedRay.origin = hitPosition;

	return reflectedRay;
}


Color castRay(const Ray& ray, Shape* scene, LightSource& lightSource, int depth) {

	Color color(0.0f);

	if (depth > MAX_RECUR_DEPTH)
		return color;

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

		Color directColor(0.0f);

		if (!scene->intersect(shadowIntersection) || pow(shadowIntersection.t, 2) >= length2 )
			directColor = intersection.color * lightSource.brightness * (1.0/length2); // inverse square law

		color = directColor;		

		// Reflection

		bool reflected = false;
		Ray reflectedRay;

		MaterialProperty material = intersection.pShape->getMaterialProperty();

		if (material.reflection > 0.0f) {

			Vector normalVector = intersection.pShape->getNormalVector(hitPoint);
			reflectedRay = reflect(ray, normalVector, hitPoint);
			reflected = true;
		}

		if (reflected) {
			Color reflectedColor = castRay(reflectedRay, scene, lightSource, depth+1);
			color += reflectedColor * material.reflection; // multiplying by reflection
														   // coefficient
		}

	}

	return color;
}



void rayTrace(Image& image, Camera* camera, Shape* scene, LightSource& lightSource) {
	
	for (int x=0; x < image.getWidth(); x++) {
		for (int y=0; y < image.getHeight(); y++) {
			
			float xx = (2.0f*x) / image.getWidth() - 1.0f; // from -1 to 1
			float yy = (-2.0f*y) / image.getHeight() + 1.0f; // from 1 to -1

			Vector2 screenCoord(xx, yy);
			Ray ray = camera->makeRay(screenCoord);


			Color* pixelColor = image.getPixel(x, y);
			*pixelColor = castRay(ray, scene, lightSource, 0);
		}
	}
}