#ifndef BOX_H
#define BOX_H

#include <GL/glew.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/constants.hpp>
#include <glm/gtx/string_cast.hpp>

#include <iostream>

#include "IMaterial.h"

using namespace std;
using namespace glm;

class Box : public IMaterial
{    
public: // it would be better to have some kind of protection on members...
	unsigned int num;
    
	vec3 minPos;
	vec3 maxPos;
	vec3 normal[6];
    
    // rotation matrices
    mat4 rotMat;
    mat4 invRotMat;

	vec3 dimension;
	mat4 projMat;
	mat4 invProjMat;

    
public:
    Box(){

	};
    ~Box(){};
	void SetNormal() {
		//top
		vec3 line1 = vec3(maxPos.x, maxPos.y, minPos.z) - vec3(minPos.x, maxPos.y, minPos.z);
		vec3 line2 = vec3(minPos.x, maxPos.y, maxPos.z) - vec3(minPos.x, maxPos.y, minPos.z);
		normal[0] = cross(line1, line2);
		//bottom
		line1 = vec3(maxPos.x, minPos.y, minPos.z) - vec3(minPos.x, minPos.y, minPos.z);
		line2 = vec3(minPos.x, minPos.y, maxPos.z) - vec3(minPos.x, minPos.y, minPos.z);
		normal[1] = cross(line2, line1);
		//front
		line1 = vec3(maxPos.x, maxPos.y, maxPos.z) - vec3(maxPos.x, minPos.y, maxPos.z);
		line2 = vec3(minPos.x, minPos.y, maxPos.z) - vec3(maxPos.x, minPos.y, maxPos.z);
		normal[2] = cross(line2, line1);

		//back
		line1 = vec3(maxPos.x, minPos.y, minPos.z) - vec3(minPos.x, minPos.y, minPos.z);
		line2 = vec3(minPos.x, maxPos.y, minPos.z) - vec3(minPos.x, minPos.y, minPos.z);
		normal[3] = cross(line1, line2);

		//left
		line1 = vec3(minPos.x, maxPos.y, minPos.z) - vec3(minPos.x, minPos.y, minPos.z);
		line2 = vec3(minPos.x, minPos.y, maxPos.z) - vec3(minPos.x, minPos.y, minPos.z);
		normal[4] = cross(line1, line2);

		//right
		line1 = vec3(maxPos.x, maxPos.y, maxPos.z) - vec3(maxPos.x, minPos.y, minPos.z);
		line2 = vec3(maxPos.x, maxPos.y, minPos.z) - vec3(maxPos.x, minPos.y, minPos.z);
		normal[5] = cross(line1, line2);


	}
    void Draw()
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glEnable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);
        glDisable(GL_LIGHTING);
        
        
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glMultMatrixf((const float*)glm::value_ptr(rotMat));
        
        glColor3f(color.x, color.y, color.z);
		
        
        glBegin(GL_QUADS);
        
        // Top face (y = 1.0f)
        // Counter-clockwise (CCW) with normal pointing out
        glVertex3f(maxPos.x, maxPos.y, minPos.z);
        glVertex3f(minPos.x, maxPos.y, minPos.z);
        glVertex3f(minPos.x, maxPos.y, maxPos.z);
        glVertex3f(maxPos.x, maxPos.y, maxPos.z);
        
        // Bottom face (y = -1.0f)
        glVertex3f(maxPos.x, minPos.y, maxPos.z);
        glVertex3f(minPos.x, minPos.y, maxPos.z);
        glVertex3f(minPos.x, minPos.y, minPos.z);
        glVertex3f(maxPos.x, minPos.y, minPos.z);
        
        // Front face  (z = 1.0f)
        glVertex3f(maxPos.x, maxPos.y, maxPos.z);
        glVertex3f(minPos.x, maxPos.y, maxPos.z);
        glVertex3f(minPos.x, minPos.y, maxPos.z);
        glVertex3f(maxPos.x, minPos.y, maxPos.z);
        
        // Back face (z = -1.0f)
        glVertex3f(maxPos.x, minPos.y, minPos.z);
        glVertex3f(minPos.x, minPos.y, minPos.z);
        glVertex3f(minPos.x, maxPos.y, minPos.z);
        glVertex3f(maxPos.x, maxPos.y, minPos.z);
        
        // Left face (x = -1.0f)
        glVertex3f(minPos.x, maxPos.y, maxPos.z);
        glVertex3f(minPos.x, maxPos.y, minPos.z);
        glVertex3f(minPos.x, minPos.y, minPos.z);
        glVertex3f(minPos.x, minPos.y, maxPos.z);
        
        // Right face (x = 1.0f)
        glVertex3f(maxPos.x, maxPos.y, minPos.z);
        glVertex3f(maxPos.x, maxPos.y, maxPos.z);
        glVertex3f(maxPos.x, minPos.y, maxPos.z);
        glVertex3f(maxPos.x, minPos.y, minPos.z);
        
        glEnd();
        glPopMatrix();
    };
    
	bool Contains(vec3 p)
	{
		return p.x >= -1 && p.x <= 1 &&
			p.y >= -1 && p.y <= 1 &&
			p.z >= -1 && p.z <= 1;
	}
};

#endif