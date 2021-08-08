#pragma once
#include "GLContext.h"

/*
OVB:���㻺��������
���ã�ֱ�ӵ��ú��������Դ棬��Ŷ���������
*/
class Lesson04_2_VBO
{
public:
	void initGL();
	void onDisable();
	void render();

private:
	unsigned int  createTextureFromImage(const char* fileName);
	unsigned int  createTexture(int w, int h, const void* data);
	void          createVbo();
	void          drawCude();

public:
	GLuint m_unTextureId;
	GLuint m_unVbo;
	float  m_fAngle;
};

