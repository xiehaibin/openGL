#pragma once
#include "GLContext.h"

class Lesson03_FrameAnim
{
public:
	void onDisable();
	void initGL();
	unsigned int createTextureFromImage(const char* fileName);
	unsigned int  createTexture(int w, int h, const void* data);

	void render();

public:
	GLuint           m_unTexId;
	unsigned int     m_unTexWidth;
	unsigned int     m_unTexHeight;
	unsigned int     m_unFrameWidth;
	unsigned int     m_unFrameHeight;

	unsigned int     m_unFrame;
};

