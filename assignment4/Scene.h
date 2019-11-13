#pragma once

#include "Ray.h"
#include "Box.h"
#include "Sphere.h"
#include "Camera.h"
#include "Light.h"
#include <vector>

#include "IMaterial.h"

using namespace glm;
using namespace std;

struct RayHit
{
	IMaterial* obj;
	float t;
	vec3 point;
	vec3 normal;

	RayHit() : obj(nullptr), t(std::numeric_limits<float>().max()), point({ 0, 0, 0 }), normal({0, 0, 0}) {}
};

class Scene {
public:
	Scene();
	void initScene(Box* b, Sphere* s, Camera camera, Light l);
	void displayScene();
	vec3 localLighting();
	vec3 crossRayPlane(Ray& ray, Box& box);
	RayHit getIntersectionSphere(Ray& r, Sphere* s);
	RayHit getIntersectionBox(Ray& r, Box* s);
	RayHit findNearestObject(Ray& ray, IMaterial* objToIgnore = nullptr);
	void WriteFrameBuffer();
	vec3 RayTracing(Ray& ray, int depth);
	void setViewPort(int w, int h);
	

public:
	Box* box;
	Sphere* sphere;
	Camera camera;
	Light light;
	vec3 eye;
	
	int width;
	int height;
	vector<vec3> buffer;
	bool isSceneChanged;
};