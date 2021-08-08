#pragma once
#include "GLContext.h"

/*
Pixel Buffer Object(opengl2.0以后的扩展)
UNPACK:内存到显卡
*/
class Lesson04_6_PBO_UNPACK
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
	bool          saveImage(int w, int h, char* data);
	void          updateData(unsigned char* data);

public:
	GLuint  m_dynamicTexId;
	GLuint  m_unTextureId;
	GLuint  m_unVbo;
	GLuint  m_unIbo;

	GLuint  m_pbo[2];

	int DMA;
	int READ;
};

