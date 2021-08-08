#pragma once
#include "GLContext.h"

/*
Pixel Buffer Object(opengl2.0以后的扩展)
双缓冲
*/
class Lesson04_6_PBO_2
{
public:
	void initGL();
	void onDisable();
	void render();

private:
	unsigned int  createTextureFromImage(const char* fileName);
	unsigned int  createTexture(int w, int h, const void* data);
	bool          saveImage(int w, int h, char* data);
	void          renderCube(GLuint tex, bool rot);

public:
	GLuint              _texture;
	GLuint              _pbo[2];
	int                 _DMA;
	int                 _READ;
};

