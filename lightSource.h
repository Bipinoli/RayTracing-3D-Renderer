#pragma once

struct LightSource {
	Vector position;
	float brightness;

	LightSource(const Vector& position, const float brightness = 100.0f) {
		this->position = position;
		this->brightness = brightness;
	} 
};