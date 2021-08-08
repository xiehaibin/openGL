#pragma once
#include "GLContext.h"

class Lesson03_TexAnim
{
public:
	void initGL();
	void onDisable();
	unsigned int createTextureFromImage(const char* fileName);
	unsigned int  createTexture(int w, int h, const void* data);

	void render();

	void playTexAnim();
	void manyTexture1();
	void manyTexture2();
	void playManyTextureAnim();

private:
	GLuint m_unTextureId[3];
};
