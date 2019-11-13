#pragma once
#include <glm/glm.hpp>

class IMaterial
{
public:
	vec3 color;

	// material
	float ambient;
	float diffuse;
	float phong;
};