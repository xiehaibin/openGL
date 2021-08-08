#pragma once
#include "GLContext.h"

/*
Pixel Buffer Object(opengl2.0�Ժ����չ)
���ٵ����Կ�����/��ȡ����
��FBOҪ��
*/
class Lesson04_6_PBO
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
	void          renderCube(bool rot);
	bool          saveImage(int w, int h, char* data);

public:
	GLuint  m_dynamicTexId;
	GLuint  m_unTextureId;
	GLuint  m_unVbo;
	GLuint  m_unIbo;

	GLuint  m_pbo;
};

