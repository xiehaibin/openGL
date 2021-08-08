#pragma once
#include "GLContext.h"

/*
OVB:顶点缓冲区对象
作用：直接调用函数分配显存，存放顶点速数据
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

