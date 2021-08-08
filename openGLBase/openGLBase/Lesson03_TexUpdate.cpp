#include "Lesson03_TexUpdate.h"
#include <stdlib.h>
//#include <gl/GLU.h> //制作minmap 

extern unsigned int unWidth;
extern unsigned int unHeight;

struct Vertex4
{
	float x;
	float y;
	float z;

	float u;
	float v;
};

Vertex4 verCude2[] = {
	// 前
	{-0.5,  0.5,   0.5,  0.0, 1.0},
	{-0.5,  -0.5,  0.5,  0.0, 0.0},
	{0.5,   0.5,   0.5,  1.0, 1.0},
	{0.5,   -0.5,  0.5,  1.0, 0.0},
	/*
	// 后
	{-0.5,  0.5,   -0.5,  1.0, 0.0},
	{-0.5,  -0.5,  -0.5,  0.0, 0.0},
	{0.5,   0.5,   -0.5,  1.0, 1.0},
	{0.5,   -0.5,  -0.5,  1.0, 0.0},

	// 左
	{-0.5,  0.5,   -0.5,  1.0, 0.0},
	{-0.5,  0.5,   0.5,   0.0, 0.0},
	{-0.5,  -0.5,  -0.5,  1.0, 1.0},
	{-0.5,  -0.5,  0.5,   1.0, 0.0},
	// 右
	{0.5,  0.5,   -0.5, 1.0, 0.0},
	{0.5,  0.5,   0.5,  0.0, 0.0},
	{0.5,  -0.5,  -0.5, 1.0, 1.0},
	{0.5,  -0.5,  0.5,  1.0, 0.0},

	// 上
	{-0.5,  0.5,  -0.5,  1.0, 0.0},
	{ 0.5,  0.5,  -0.5,  0.0, 0.0},
	{-0.5,  0.5,  0.5,   1.0, 1.0},
	{ 0.5,  0.5,  0.5,   1.0, 0.0},
	// 下
	{-0.5,  -0.5,  -0.5,  1.0, 0.0},
	{ 0.5,  -0.5,  -0.5,  0.0, 0.0},
	{-0.5,  -0.5,  0.5,   1.0, 1.0},
	{ 0.5,  -0.5,  0.5,   1.0, 0.0},
	*/
};

void Lesson03_TexUpdate::onDisable()
{
	// 删除纹理(数组)
	glDeleteTextures(1, &texId);

	int err = glGetError(); //0没有错误 
}

FIBITMAP* Lesson03_TexUpdate::readImage(const char* fileName)
{
	// 获取图片格式
	FREE_IMAGE_FORMAT fifmt = FreeImage_GetFileType(fileName, 0);
	if (fifmt == FIF_UNKNOWN)
	{
		return 0;
	}
	// 加载图片
	FIBITMAP* dib = FreeImage_Load(fifmt, fileName, 0);
	// 获取图片颜色样式
	FREE_IMAGE_COLOR_TYPE type = FreeImage_GetColorType(dib);
	// 获取数据指针
	FIBITMAP* temp = dib;
	// 转化成32位图片
	dib = FreeImage_ConvertTo32Bits(dib);
	// 释放
	FreeImage_Unload(temp);

	// rgba
	BYTE* pixels = (BYTE*)FreeImage_GetBits(dib);
	int   width = FreeImage_GetWidth(dib);
	int   height = FreeImage_GetHeight(dib);

	//bgra => rgba
	for (int i = 0; i < width*height * 4; i += 4)
	{
		BYTE temp = pixels[i];
		pixels[i] = pixels[i + 2];
		pixels[i + 2] = temp;
	}

	return dib;
	//FreeImage_Unload(dib);
}

unsigned int Lesson03_TexUpdate::createTexture()
{
	m_unTextures[0] = readImage("res/1008.jpg");
	m_unTextures[1] = readImage("res/1009.png");

	glGenTextures(1, &texId);
	glBindTexture(GL_TEXTURE_2D, texId);
	// 纹理滤波
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	// 纹理包装
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	BYTE*   pixels = (BYTE*)FreeImage_GetBits(m_unTextures[0]);
	int     width = FreeImage_GetWidth(m_unTextures[0]);
	int     height = FreeImage_GetHeight(m_unTextures[0]);

	/*
	如果用glTexImage2D更新纹理。 会在显存中重新创建纹理，会删除原来的显存，重新分配。速度慢
	*/
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
	FreeImage_Unload(m_unTextures[0]);

	
	{
		BYTE* pixels = (BYTE*)FreeImage_GetBits(m_unTextures[1]);
		int   width = FreeImage_GetWidth(m_unTextures[1]);
		int   height = FreeImage_GetHeight(m_unTextures[1]);
		// 不会删除显存  在上面跟新  效率高
		glTexSubImage2D(GL_TEXTURE_2D, 0, 100, 100, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
		FreeImage_Unload(m_unTextures[1]);
	}

	return texId;
}

void Lesson03_TexUpdate::initGL()
{
	// 启动纹理
	glEnable(GL_TEXTURE_2D);
	// 启动深度测试
	glEnable(GL_DEPTH_TEST);

	createTexture();
}

void Lesson03_TexUpdate::render()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, (double)unWidth/(double)unHeight, 0.1f, 1000.0f);

	drawCude();
}

void Lesson03_TexUpdate::drawCude()
{
	glBindTexture(GL_TEXTURE_2D, texId);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glVertexPointer(3, GL_FLOAT, sizeof(Vertex4), verCude2);
	glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex4), &verCude2[0].u);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0, 0, -2.0f);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 24);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	//m_fAngle = m_fAngle - 0.05f;
}
