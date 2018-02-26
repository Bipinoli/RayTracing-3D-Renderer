#pragma once

struct Color
{
	float r, g, b;

	Color();
	Color(float l);
	Color(float r, float g, float b);

	virtual ~Color();

	Color& operator =(const Color& c);
	Color& operator +=(const Color& c);
	Color& operator *=(const Color& c);
	Color& operator *=(float f);
};

inline Color operator +(const Color& c1, const Color& c2)
{
	return Color(c1.r + c2.r,
		c1.g + c2.g,
		c1.b + c2.b);
}

inline Color operator *(const Color& c1, const Color& c2)
{
	return Color(c1.r * c2.r,
		c1.g * c2.g,
		c1.b * c2.b);
}

inline Color operator *(const Color& c, float f)
{
	return Color(c.r * f,
		c.g * f,
		c.b * f);
}

inline Color operator *(float f, const Color& c)
{
	return Color(f * c.r,
		f * c.g,
		f * c.b);
}
