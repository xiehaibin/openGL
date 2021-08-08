#pragma once
#include "GLContext.h"

/*
IBO:���㻺��������
���ã�ֱ�ӵ��ú��������Դ棬��Ŷ���������
*/
class Lesson04_3_IBO
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
	void          drawCude();

public:
	GLuint m_unTextureId;
	GLuint m_unVbo;
	GLuint m_unIbo;
	float  m_fAngle;
};

