#pragma once

struct LightSource {
	Vector position;
	float brightness;

	LightSource(const Vector& position, const float brightness = 60.0f) {
		this->position = position;
		this->brightness = brightness;
	} 
};