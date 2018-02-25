#pragma once


#include <cmath>
#include <iostream>
using namespace std;


const int MAX_RECUR_DEPTH = 10;
const float ka = 0.73; // ambient light coeff
const int ns = 64; // exponent of cos for specular light
				  // generally multiple of 2
const float ks = 0.3; // specular light coeff


Ray reflect(const Ray& ray, const Vector& normalVec, const Point& hitPosition ) {
	
	// normal vector should be a unit vector normal to the plane
	Ray reflectedRay;
	reflectedRay.direction = ray.direction - 2 * dot(ray.direction, normalVec) * normalVec;
	reflectedRay.direction.normalize();
	reflectedRay.origin = hitPosition;

	return reflectedRay;
}


Ray refract(const Ray& ray, const Vector& normalVec, const Point& hitPosition,
			 const float n1, const float n2) 
 {
	// normal vector should be a unit vector normal to the plane
	const float n = n1/n2;
	const float cosI = -dot(normalVec, ray.direction);
	const float sinR2 = n*n * (1.0 - cosI*cosI);
	if (sinR2 >= 1.0) {
		// total internal reflection, returning invalid ray
		Ray invalidRay;
		invalidRay.invalid = true;
		return invalidRay;
	} 

	const float cosR = sqrt(1.0 - sinR2);

	Ray refractedRay;
	refractedRay.direction = n * ray.direction + (n*cosI - cosR) * normalVec;
	refractedRay.direction.normalize();
	refractedRay.origin = hitPosition;

	return refractedRay;
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

		Vector normalVector = intersection.pShape->getNormalVector(hitPoint);
		MaterialProperty material = intersection.pShape->getMaterialProperty();

		Intersection shadowIntersection(shadowRay);

		//  if no intersection than no shadow
		//  but if it intersected but away from the light source than also no shadow

		Color directColor(0.0f);


		// reflection ray of -shadow light (i.e light from source):
		//  needed inorder to find the true direction of specular light
		// on viewing from other direction intensity is multiplied by cos of the angle 
		// cos is raised to the power of ns for better modelling purpose
		Ray specularRay = shadowRay;
		specularRay.direction = -specularRay.direction;
		specularRay = reflect(specularRay, normalVector, hitPoint);


		if (!scene->intersect(shadowIntersection) || pow(shadowIntersection.t, 2) >= length2 ) 
		{
			directColor = ka * intersection.color 
						  +
						 	intersection.color * lightSource.brightness 
						  	  * dot(shadowRay.direction, normalVector) 
						  	  * (1.0/length2) 
						  +
						  	ks * pow( dot(specularRay.direction, normalVector), ns) * intersection.color;

			// inverse square law + lambert cosine law + specular cos^ns law
			// ambient + diffused + specular lights
		}
		
		else if (scene->intersect(shadowIntersection) && pow(shadowIntersection.t, 2) < length2) {
			// if intersecting object is transparent then certain light enters in
			// but this is very crude solution as there might be other opaque 
			// objects beyond it still blocking the object
			directColor = ka * Color(1.0f)
						  +
						 	Color(1.0f) * lightSource.brightness * (1.0/length2);
			directColor *= material.transparency;
		}

		color = directColor;		

		// For reflection and refraction

		bool rayHittingFromInsideObject = false;
		if (dot(normalVector, ray.direction) >= 0) {
			 // this means ray was hitting from inside the object
			normalVector = -normalVector;
			rayHittingFromInsideObject = true;
		}

		// Reflection

		bool reflected = false;
		Ray reflectedRay;

		if (material.reflection > 0.0f) {

			reflectedRay = reflect(ray, normalVector, hitPoint);
			reflected = true;
		}

		if (reflected) {
			Color reflectedColor = castRay(reflectedRay, scene, lightSource, depth+1);
			color += reflectedColor * material.reflection; // multiplying by reflection
														   // coefficient
		}

		bool refracted = false;
		Ray refractedRay;

		// Refraction
		if (material.transparency > 0.0f) { 

			float n1 = 1.0f; // air's refrc index 
			float n2 = material.refractiveIndex; 

			if (rayHittingFromInsideObject) {
				float temp = n1;
				n1 = n2;
				n2 = temp;
			}

			refractedRay = refract(ray, normalVector, hitPoint, n1, n2);
			if (!refractedRay.invalid)
				refracted = true;
		}

		if (refracted) {

			Color refractedColor = castRay(refractedRay, scene, lightSource, depth+1);
			// cout << "refractedColor: " << refractedColor.r << ", " << refractedColor.g  
			// 	 <<	", " << refractedColor.b << endl;
			color += refractedColor * material.transparency; // multiplying by refraction bias											   // coefficient
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