#pragma once
#include "GLContext.h"
#include "FrameBufferObject.h"

/*
FBO(FrameBuffer0bject ֡�������) opengl��չ��openGL2.0���ϣ� ������Ⱦ
����Ļ��ȡ����
1.����Ҫ���������openGL������
2.�����Դ�����Щ����������������Щ����������ռ���ڴ���
3.������״̬�����������Զ�ָ̬�� pbuffer�������ܸ�
*/
class Lesson04_5_FBO
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

public:
	GLuint  m_dynamicTexId;
	GLuint  m_unTextureId;
	GLuint  m_unVbo;
	GLuint  m_unIbo;

	FrameBufferObject m_fbo;
};

