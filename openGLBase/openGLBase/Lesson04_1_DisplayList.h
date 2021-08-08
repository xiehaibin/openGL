#pragma once
#include "GLContext.h"
/*
显示列表（openGL1.2）
对openGL1.2之前提出的优化手段
openGL ES/尤其2.0后 已经不提供了

把opengl指令及数据打包一下，放到显卡端去执行
有些函数会失效 状态查询的

缺点：
1.数据变更，要删除原来数据，再创建
2.消耗大量显存，认为保存的状态很多
优点：
1.速度快
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

