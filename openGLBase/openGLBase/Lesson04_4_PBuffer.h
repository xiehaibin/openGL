#pragma once
#include "GLContext.h"

/*
PBuffer:opengl扩展，现在显卡都支持（es叫SufaceBuffer）
可以在一个应用程序中建立多个openGL对象
离屏渲染
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

