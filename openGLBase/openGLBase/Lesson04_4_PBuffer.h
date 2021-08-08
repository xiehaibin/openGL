#pragma once
#include "GLContext.h"

/*
PBuffer:opengl��չ�������Կ���֧�֣�es��SufaceBuffer��
������һ��Ӧ�ó����н������openGL����
������Ⱦ
*/
class Lesson04_4_PBuffer
{
public:
	void initGL(GLContext glContext, HWND hWnd, HDC hdc, HGLRC hrc, int width, int height);
	void onDisable();
	void render();

private:
	unsigned int  createTextureFromImage(const char* fileName);
	unsigned int  createTexture(int w, int h, const void* data);
	void          createVbo();
	void          createIbo();
	void          drawCude(bool isRotate);

public:
	GLuint  m_dynamicTexId;
	GLuint  m_unTextureId;
	GLuint  m_unVbo;
	GLuint  m_unIbo;
	float   m_fAngle;

	GLContext glContext;
};

