#include "Scene.h"
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/constants.hpp>

Scene::Scene()
{
	
}

void Scene::initScene(Box* b, Sphere* s, Camera c, Light l)
{
	box = b;
	sphere = s;
	camera = c;
	light = l;
	eye = vec3(camera.eye.x, camera.eye.y, camera.eye.z);
	width = 640;
	height = 480;
	buffer.resize(width * height);
	isSceneChanged = true;

	light.pos = vec3(camera.mvMat * vec4(light.pos, 1));

	for (int i = 0; i < 2; ++i)
		sphere[i].pos = vec3(camera.mvMat * vec4(sphere[i].pos, 1));


	for (int i = 0; i < 2; ++i)
	{
		mat4 S = scale(0.5f * (box[i].maxPos - box[i].minPos));
		mat4 T = translate(0.5f * (box[i].maxPos + box[i].minPos));

		box[i].projMat = camera.mvMat * T * box[i].rotMat * S;
		box[i].invProjMat = inverse(box[i].projMat);
	}

	//Ray r;

	//r.beginPoint = vec3(0, 0, 0);
	//r.direction = vec3(0, 0, -1);

	//Box bx;
	//bx.minPos = vec3(-1, -1, -1);
	//bx.maxPos = vec3(1, 1, 1);
	//bx.rotMat = scale(vec3(1, 1, 1));
	//bx.projMat = camera.mvMat * bx.rotMat;
	//bx.invProjMat = inverse(bx.projMat);

	//cout << getIntersectionBox(r, &bx).t << endl;
}



RayHit Scene::getIntersectionSphere(Ray& ray, Sphere* sphere)
{
	vec3 v(sphere->pos);

	float a = dot(ray.direction, ray.direction);
	float b = 2 * dot(ray.direction, (ray.beginPoint - v));
	float c = dot(v, v) - sphere->radius * sphere->radius + dot(ray.beginPoint, ray.beginPoint);
	float delta = b * b - 4 * a * c;
	
	if (delta > 0)
	{
		float t = (-b - sqrtf(delta)) / (2 * a);

		if (t > 0)
		{
			RayHit hit;

			hit.obj = sphere;
			hit.t = t;
			hit.point = ray.beginPoint + t * ray.direction;
			hit.normal = normalize(hit.point - sphere->pos);

			return hit;
		}
	}

	return RayHit();
}

RayHit Scene::getIntersectionBox(Ray& ray, Box* box)
{
	vec3 P = vec3(box->invProjMat * vec4(ray.beginPoint, 1));
	vec3 L = normalize(vec3(box->invProjMat * vec4(ray.beginPoint + ray.direction, 1)) - P);

	vec3 minP = vec3(-1, -1, -1);
	vec3 maxP = vec3(1, 1, 1);
	vec3 x = vec3(1, 0, 0);
	vec3 y = vec3(0, 1, 0);
	vec3 z = vec3(0, 0, 1);


	float minT = -1;
	vec3 n;
	vec3 p;

	float t = dot(minP - P, -x) / dot(L, -x);
	if (t > 0)
	{
		vec3 intersection = P + t * L;

		if (box->Contains(intersection) && (minT < 0 || t < minT))
		{
			minT = t;
			n = -x;
			p = intersection;
		}
	}

	t = dot(minP - P, -y) / dot(L, -y);
	if (t > 0 && (minT < 0 || t < minT))
	{
		vec3 intersection = P + t * L;

		if (box->Contains(intersection) && (minT < 0 || t < minT))
		{
			minT = t;
			n = -y;
			p = intersection;
		}
	}

	t = dot(minP - P, -z) / dot(L, -z);
	if (t > 0 && (minT < 0 || t < minT))
	{
		vec3 intersection = P + t * L;

		if (box->Contains(intersection) && (minT < 0 || t < minT))
		{
			minT = t;
			n = -z;
			p = intersection;
		}
	}

	t = dot(maxP - P, x) / dot(L, x);
	if (t > 0)
	{
		vec3 intersection = P + t * L;

		if (box->Contains(intersection) && (minT < 0 || t < minT))
		{
			minT = t;
			n = x;
			p = intersection;
		}
	}

	t = dot(maxP - P, y) / dot(L, y);
	if (t > 0 && (minT < 0 || t < minT))
	{
		vec3 intersection = P + t * L;

		if (box->Contains(intersection) && (minT < 0 || t < minT))
		{
			minT = t;
			n = y;
			p = intersection;
		}
	}

	t = dot(maxP - P, z) / dot(L, z);
	if (t > 0 && (minT < 0 || t < minT))
	{
		vec3 intersection = P + t * L;

		if (box->Contains(intersection) && (minT < 0 || t < minT))
		{
			minT = t;
			n = z;
			p = intersection;
		}
	}


	if (minT > 0)
	{
		RayHit hit;

		hit.obj = box;
		hit.t = minT;
		hit.point = vec3(box->projMat * vec4(p, 1));
		hit.normal = normalize(vec3(box->projMat * vec4(p + n, 1)) - hit.point);

		return hit;
	}


	return RayHit();
}

RayHit Scene::findNearestObject(Ray & ray)
{
	RayHit hit;

	//1. find sphere
	for (int i = 0; i < 2; i++)
	{
		RayHit temp = getIntersectionSphere(ray, sphere + i);

		if (temp.t < hit.t)
			hit = temp;
	}

	//2. find box
	for (int i = 0; i < 2; i++)
	{
		RayHit temp = getIntersectionBox(ray, box + i);

		if (temp.t < hit.t)
			hit = temp;
	}


	return hit;
}

void Scene::WriteFrameBuffer()
{
	int i, j, point = 0;
	vec3 Q = vec3(0,0,0);

	vec3 E;
	//in camera space
	float h = 2 * camera.near_plane * tan(radians(camera.fovy / 2));
	float w = h * width / height;
	Ray ray;
	ray.beginPoint = Q;
	for (i = 0; i < height; i++)
	{
		for (j = 0; j < width; j++)
		{
			vec3 s = vec3((j + 0.5) / width * w - w/2, (i + 0.5) / height * h - h/2, -camera.near_plane);
		
			E = s - Q;
			E = normalize(E);
			ray.direction = E;
			vec3 color = RayTracing(ray, 0);
			buffer[point] = color;
			point++;
		}
	}
}

vec3 Scene::RayTracing(Ray & ray, int depth)
{
	RayHit hit = findNearestObject(ray);
	
	if (hit.obj)
	{
		vec3 s = normalize(light.pos - hit.point);

		float s_dot_n = dot(s, hit.normal);
		float r_dot_v = dot(normalize(reflect(s, hit.normal)), -ray.direction);

		float Ia = hit.obj->ambient;
		float Id = light.intensity * hit.obj->diffuse * s_dot_n;
		float Is = s_dot_n <= 0 ? 0 : light.intensity * hit.obj->phong * pow(r_dot_v, 50);


		return (Ia + Id) * light.color * hit.obj->color + Is * light.color;
	}

	return vec3(0, 0, 0);
}

void Scene::displayScene()
{
	if (isSceneChanged) {
		WriteFrameBuffer();
		isSceneChanged = false;
	}
	
	glDrawPixels(width, height, GL_RGB, GL_FLOAT, &buffer[0]);
}
