#pragma once
#include "GLContext.h"

/*
Pixel Buffer Object(opengl2.0以后的扩展)
双缓冲
*/
class Lesson04_6_PBO_1
{
public:
	void initGL();
	void onDisable();
	void render();

private:
	unsigned int  createTextureFromImage(const char* fileName);
	unsigned int  createTexture(int w, int h, const void* data);
	void          createVbo();
	void          createIbo();
	void          drawCude(bool isRotate);
	void          drawImg();
	bool          saveImage(int w, int h, char* data);
	void          randerOrtho();

public:
	GLuint  m_dynamicTexId;
	GLuint  m_unTextureId;
	GLuint  m_unTextureId1;
	GLuint  m_unVbo;
	GLuint  m_unIbo;

	GLuint  m_pbo[2];

	int DMA;
	int READ;
};

