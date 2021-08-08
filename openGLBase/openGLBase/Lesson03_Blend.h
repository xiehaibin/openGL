#pragma once
#include "GLContext.h"

class Lesson03_Blend
{
public:
	void onDisable();
	void initGL();
	unsigned int createTextureFromImage(const char* fileName);
	unsigned int createTexture(int w, int h, const void* data);

	void render();

	void renderWill();
	void randerOrtho();
	void randerOrthoTest();

public:
	GLuint           m_unTexId0;
	GLuint           m_unTexId1;
	GLuint           m_unTexId2;
	GLuint           m_unTexId3;
	GLuint           m_unTexId4;
	GLuint           m_unTexId5;
	GLuint           m_unTexId6;

	GLuint           m_unTexId1000;
	GLuint           m_unTexId1001;
	GLuint           m_unTexId1002;
};

