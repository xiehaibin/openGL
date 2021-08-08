#pragma once
#include "GLContext.h"

/*
在openGL低版本上高效实现 画中画
*/
class Lesson04_6_12
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
	void          drawCude(bool isRotate, float offsetX);
	bool          saveImage(int w, int h, char* data);

public:
	GLuint  m_dynamicTexId;
	GLuint  m_unTextureId;
	GLuint  m_unVbo;
	GLuint  m_unIbo;
};

