#pragma once
#include "GLContext.h"

class Lesson03_TexUpdate
{
public:
	void onDisable();
	void initGL();
	FIBITMAP* readImage(const char* fileName);
	unsigned int  createTexture();

	void render();
	void drawCude();

private:
	FIBITMAP* m_unTextures[2];
	float  m_fAngle;
	unsigned texId;
};
