#pragma once
#include "GLContext.h"

class Lesson03_minMap
{
public:
	Lesson03_minMap();
	~Lesson03_minMap();
	void onDisable();
	void initGL();
	unsigned int createTextureFromImage(const char* fileName);
	unsigned int  createTexture(int w, int h, const void* data);

	void render();
	void drawCude();

private:
	GLuint m_unTexId1;
	float  m_fAngle;
};
