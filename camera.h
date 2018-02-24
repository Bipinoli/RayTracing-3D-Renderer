#pragma once

#include "vectormath.h"
#include "ray.h"

class Camera
{
public:
	virtual ~Camera() { }

	virtual Ray makeRay(Vector2 point) const = 0;
};

class PerspectiveCamera : public Camera
{
protected:
	Point origin;
	Vector forward;
	Vector up;
	Vector right;

	float h, w;

public:
	PerspectiveCamera(Point origin, Vector target,
		Vector upguide, float fov, float aspectRatio);

	virtual Ray makeRay(Vector2 point) const;
};