#pragma once

#include <vector>

#include "vectormath.h"
#include "color.h"
#include "ray.h"


struct MaterialProperty {
	Color surfaceColor, emissionColor;
	float transparency, reflection, refractiveIndex;
};


class Shape {
public:
	virtual ~Shape() {}

	// virtual functions because we are going to be calling these methods from
	// the Base pointer
	virtual bool intersect(Intersection& intersection) = 0;
	virtual bool doesIntersect(const Ray& ray) = 0;
	virtual Vector getNormalVector(const Point& pHit) = 0;
	virtual MaterialProperty getMaterialProperty() = 0;
};



class ShapeSet: public Shape {
protected:
	std::vector<Shape*> shapes;

public:
	ShapeSet();

	virtual ~ShapeSet();

	void addShape(Shape* shape);

	virtual bool intersect(Intersection& intersection);
	virtual bool doesIntersect(const Ray& ray);
	virtual Vector getNormalVector(const Point& pHit) { return Vector();} // because they were pure
	virtual MaterialProperty getMaterialProperty() { return MaterialProperty();} // virtual functions
};


class Plane : public Shape
{
protected:
	Point position;
	Vector normal;
	Color surfaceColor, emissionColor;
	float transparency, reflection; // from 0 and 1
	float refractiveIndex;

public:

	Plane() {}

	Plane(const Point& position, const Vector& normal,
		const Color& surfaceColor = Color(1.0f, 1.0f, 1.0f),
		const float reflection = 0.0f,
		const float transparency = 0.0f,
		const float refractiveIndex = 1.0f,
		const Color& emissionColor = Color(0.0f));

	virtual ~Plane();

	virtual Vector getNormalVector(const Point& pHit);
	virtual MaterialProperty getMaterialProperty();
	virtual bool intersect(Intersection& intersection);
	virtual bool doesIntersect(const Ray& ray);
};



class Triangle : public Plane {

public:
	Point A, B, C; // vertices in counter clockwise order
	Point position;
	Vector normal;
	Color surfaceColor, emissionColor;
	float transparency, reflection;
	float refractiveIndex;

	Triangle (const Point vertices[], 
		const Color& surfaceColor = Color(1.0f, 1.0f, 1.0f),
		const float reflection = 0.0f,
		const float transparency = 0.0f,
		const float refractiveIndex = 1.0f,
		const Color& emissionColor = Color(0.0f));

	bool intersect(Intersection& intersection);
	bool doesIntersect(const Ray& ray);

};



class Sphere : public Shape
{
protected:
	Point center;
	float radius;
	Color surfaceColor, emissionColor;      /// surface color and emission (light) 
    float transparency, reflection;         /// surface transparency and reflectivity 
    float refractiveIndex;					/// from the scale of 0 to 1

public:
	Sphere(const Point& center, float radius,
		const Color& surfaceColor = Color(1.0f, 1.0f, 1.0f),
		const float reflection = 0.0f,
		const float transparency = 0.0f,
		const float refractiveIndex = 1.5f,
		const Color& emissionColor = Color(0.0f));

	virtual ~Sphere();

	virtual Vector getNormalVector(const Point& pHit);
	virtual MaterialProperty getMaterialProperty();
	virtual bool intersect(Intersection& intersection);
	virtual bool doesIntersect(const Ray& ray);
};
