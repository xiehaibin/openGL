#pragma once
#include "GLContext.h"

class Lesson03_EnvironmentTex
{
public:
	void onDisable();
	void initGL();
	unsigned int createTextureFromImage(const char* fileName);
	unsigned int  createTexture(int w, int h, const void* data);

	void render();

public :
	GLuint m_unTexId;
	GLUquadricObj* quadric; // ÇòÌå
	float  m_angle;
};

