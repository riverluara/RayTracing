#include "Ray.h"

Ray::Ray()
{

}

Ray::Ray(vec3 begin, vec3 dir)
{
	beginPoint = begin;
	direction = dir;
}
