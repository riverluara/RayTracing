#pragma once
 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/constants.hpp>
#include <glm/gtx/string_cast.hpp>

#include <iostream>
using namespace std;
using namespace glm;

class Ray 
{
public:
	Ray();
	Ray(vec3 begin, vec3 dir);
	//float attenuate(vec3 object);	
	//float attenuate(float length);
public:
	vec3 beginPoint;	
	vec3 direction;
	/*vec3 color;
	vec3 attenuation;*/
};