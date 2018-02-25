#include <cmath>

#include "shape.h"
#include "vectormath.h"


ShapeSet::ShapeSet() {

}

ShapeSet::~ShapeSet() {

}

void ShapeSet::addShape(Shape* shape) {
	shapes.push_back(shape);
}


bool ShapeSet::intersect(Intersection& intersection) {

	bool intersects = false;

	for (const auto&  shape: shapes) {
		if (shape->intersect(intersection))
			intersects = true;
	}

	return intersects;
}



bool ShapeSet::doesIntersect(const Ray& ray) {

	for (const auto& shape: shapes) {
		if (shape->doesIntersect(ray))
			return true;
	}

	return false;
}


Plane::Plane(const Point& position, const Vector& normal,
		const Color& surfaceColor,
		const float reflection,
		const float transparency,
		const Color& emissionColor):
			position(position),
			normal(normal),
			surfaceColor(surfaceColor),
			emissionColor(emissionColor)
{
	this->transparency = std::max(0.0f, std::min(transparency, 1.0f)); // between 0 and 1
	this->reflection = std::max(0.0f, std::min(reflection, 1.0f)); // between 0 and 1
}

Plane::~Plane()
{
}


Vector Plane::getNormalVector(const Point& pHit) {
	
	Vector normVector = pHit + normal; // r = p + t*d;
	return normVector.normalized();
}


MaterialProperty Plane::getMaterialProperty() {
	MaterialProperty mp;
	mp.surfaceColor = surfaceColor;
	mp.emissionColor = emissionColor;
	mp.transparency = transparency;
	mp.reflection = reflection;
	return mp;
}


bool Plane::intersect(Intersection& intersection) {

	// First, check if we intersect
	float dDotN = dot(intersection.ray.direction, normal);

	if (dDotN == 0.0f)
	{
		// We just assume the ray is not embedded in the plane
		return false;
	}

	// Find point of intersection
	float t = dot(position - intersection.ray.origin, normal) / dDotN;

	if (t <= RAY_T_MIN || t >= intersection.t)
	{
		// Outside relevant range
		return false;
	}

	intersection.t = t;
	intersection.pShape = this;
	intersection.color = surfaceColor;

	return true;

}

bool Plane::doesIntersect(const Ray& ray) {

	// First, check if we intersect
	float dDotN = dot(ray.direction, normal);

	if (dDotN == 0.0f)
	{
		// We just assume the ray is not embedded in the plane
		return false;
	}

	// Find point of intersection
	float t = dot(position - ray.origin, normal) / dDotN;

	if (t <= RAY_T_MIN || t >= ray.tMax)
	{
		// Outside relevant range
		return false;
	}

	return true;

}



Triangle::Triangle (const Point vertices[], 
		const Color& surfaceColor,
		const float reflection,
		const float transparency,
		const Color& emissionColor):
			surfaceColor(surfaceColor),
			emissionColor(emissionColor)
{
	this->transparency = std::max(0.0f, std::min(transparency, 1.0f)); // between 0 and 1
	this->reflection = std::max(0.0f, std::min(reflection, 1.0f)); // between 0 and 1

	A = vertices[0];
	B = vertices[1];
	C = vertices[2];


	this->position = B;

	this->normal = cross(C-B, A-B);
	this->normal.normalize();
}
	


bool Triangle::intersect(Intersection& intersection) {

	// First, check if we intersect
	float dDotN = dot(intersection.ray.direction, normal);

	if (dDotN == 0.0f)
	{
		// We just assume the ray is not embedded in the plane
		return false;
	}

	// Find point of intersection
	float t = dot(position - intersection.ray.origin, normal) / dDotN;

	if (t <= RAY_T_MIN || t >= intersection.t)
	{
		// Outside relevant range
		return false;
	}

	Point Q = intersection.ray.calculate(t);

	// by looking at the sign of the the cross product we can identify
	// if the point lies to the left or right of the vector
	// so checking where Q lies relative to the edges of the triangle
	// we can verify whether Q lies inside the triangle

	if ( dot( cross(C-B, Q-B), normal ) >= 0 &&
		 dot( cross(A-C, Q-C), normal ) >= 0 &&
		 dot( cross(B-A, Q-A), normal ) >= 0) 
	{
		// Q lies inside the triangle ABC

		intersection.t = t;
		intersection.pShape = this;
		intersection.color = surfaceColor;

		return true;	
	}

	return false;
}


bool Triangle::doesIntersect(const Ray& ray) {

// First, check if we intersect
	float dDotN = dot(ray.direction, normal);

	if (dDotN == 0.0f)
	{
		// We just assume the ray is not embedded in the plane
		return false;
	}

	// Find point of intersection
	float t = dot(position - ray.origin, normal) / dDotN;

	if (t <= RAY_T_MIN || t >= ray.tMax)
	{
		// Outside relevant range
		return false;
	}

	Point Q = ray.calculate(t);

	if ( dot( cross(C-B, Q-B), normal ) >= 0 &&
		 dot( cross(A-C, Q-C), normal ) >= 0 &&
		 dot( cross(B-A, Q-A), normal ) >= 0) 
	{
		// Q lies inside the triangle ABC
		return true;	
	}

	return false;
}



Sphere::Sphere(const Point& center, float radius,
	const Color& surfaceColor,
	const float reflection,
	const float transparency,
	const Color& emissionColor):
		center(center),
		radius(radius),
		surfaceColor(surfaceColor),
		emissionColor(emissionColor)
{
	this->transparency = std::max(0.0f, std::min(transparency, 1.0f)); // between 0 and 1
	this->reflection = std::max(0.0f, std::min(reflection, 1.0f)); // between 0 and 1
}

Sphere::~Sphere()
{
}


Vector Sphere::getNormalVector(const Point& pHit) {

	Vector normVector = pHit - center;
	return normVector.normalized();
}


MaterialProperty Sphere::getMaterialProperty() {
	MaterialProperty mp;
	mp.surfaceColor = surfaceColor;
	mp.emissionColor = emissionColor;
	mp.transparency = transparency;
	mp.reflection = reflection;
	return mp;
}



bool Sphere::intersect(Intersection& intersection) {

	// bring sphere at the origin first
	Ray newRay = intersection.ray;
	newRay.origin -= center;

	// quadratic coefficients
	float a = dot(newRay.direction, newRay.direction);
	float b = 2 * dot(newRay.origin, newRay.direction);
	float c = dot(newRay.origin, newRay.origin) - radius*radius;

	// check if we intersect
	float discriminant = b*b - 4*a*c;

	if (discriminant < 0.0f)
		return false;

	if (2*a == 0.0f) 
		return false; // infinite solution

	// two points of intersections, t0 <= t1
	float t0 = (-b - sqrt(discriminant)) / 2*a;
	float t1 = (-b + sqrt(discriminant)) / 2*a;

	// first check if smaller t is valid
	if (t0 > RAY_T_MIN && t0 < intersection.t) {
		intersection.t = t0;
	}
	else if (t1 > RAY_T_MIN && t1 < intersection.t) {
		intersection.t = t1;
	}
	else {
		return false;
	}

	// finish populating intersection
	intersection.pShape = this;
	intersection.color = surfaceColor;

	return true;

}


bool Sphere::doesIntersect(const Ray& ray) {

	// bring sphere at the origin first
	Ray newRay = ray;
	newRay.origin -= center;

	// quadratic coefficients
	float a = dot(newRay.direction, newRay.direction);
	float b = 2 * dot(newRay.origin, newRay.direction);
	float c = dot(newRay.origin, newRay.origin) - radius*radius;

	// check if we intersect
	float discriminant = b*b - 4*a*c;

	if (discriminant < 0.0f)
		return false;

	if (2*a == 0.0f) 
		return false; // infinite solution

	// two points of intersections, t0 <= t1
	float t0 = (-b - sqrt(discriminant)) / 2*a;
	float t1 = (-b + sqrt(discriminant)) / 2*a;

	if (t0 > RAY_T_MIN && t0 < ray.tMax)
		return true;

	if (t1 > RAY_T_MIN && t1 < ray.tMax)
		return true;

	return false;

}