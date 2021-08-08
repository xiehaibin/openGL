#pragma once
#include "GLContext.h"

class Lesson03
{
public:
	Lesson03();
	~Lesson03();
	void initGL();
	void init();
	void render();
	void onDisable();
	unsigned int createTextureFromImage(const char* fileName);
	unsigned int  createTexture(int w, int h, const void* data);

	void drawTexture();
	void drawTexture2();
	void drawCudeTex();
	void drawCudeTex2();

private:
	GLuint m_unTextureId;
	GLuint m_unTexId1;
	GLuint m_unTexId2;
	float  m_fAngle;
};
