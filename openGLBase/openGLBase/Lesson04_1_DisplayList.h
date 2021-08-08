#pragma once
#include "GLContext.h"
/*
��ʾ�б�openGL1.2��
��openGL1.2֮ǰ������Ż��ֶ�
openGL ES/����2.0�� �Ѿ����ṩ��

��openglָ����ݴ��һ�£��ŵ��Կ���ȥִ��
��Щ������ʧЧ ״̬��ѯ��

ȱ�㣺
1.���ݱ����Ҫɾ��ԭ�����ݣ��ٴ���
2.���Ĵ����Դ棬��Ϊ�����״̬�ܶ�
�ŵ㣺
1.�ٶȿ�
*/
class Lesson04_1_DisplayList
{
public :
	void initGL();
	void onDisable();
	unsigned int createTextureFromImage(const char* fileName);
	unsigned int  createTexture(int w, int h, const void* data);

	void drawCube();
	void renderCube();
	void render();

public :
	GLuint m_unTextureId;
	GLuint m_rectDisplay1;
	GLuint m_rectDisplay2;
};

