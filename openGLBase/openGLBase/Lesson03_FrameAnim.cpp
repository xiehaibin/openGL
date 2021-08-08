#include "Lesson03_FrameAnim.h"
#include <stdio.h>
extern unsigned int unWidth;
extern unsigned int unHeight;

struct Vertex
{
	float x;
	float y;
	float z;

	float u;
	float v;
};

Vertex rectangle[4] = {
	{0,   108, 0,  0, 1},
	{0,   0,   0,  0, 0},
	{108, 108, 0,  1, 1},
	{108, 0,   0,  1, 0},
};

/*
帧动画
*/
void Lesson03_FrameAnim::onDisable()
{
	// 删除纹理(数组)
	glDeleteTextures(1, &m_unTexId);
}

unsigned int Lesson03_FrameAnim::createTextureFromImage(const char* fileName)
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

	m_unTexWidth      = width;
	m_unTexHeight     = height;
	m_unFrameWidth    = 108;
	m_unFrameHeight   = 108;

	//bgra => rgba
	/*
	for (int i = 0; i < width*height * 4; i += 4)
	{
		BYTE temp = pixels[i];
		pixels[i] = pixels[i + 2];
		pixels[i + 2] = temp;
	}
	*/

	unsigned int res = createTexture(width, height, pixels);
	FreeImage_Unload(dib);

	return res;
}

unsigned int Lesson03_FrameAnim::createTexture(int w, int h, const void* data)
{
	unsigned texId;
	glGenTextures(1, &texId);
	glBindTexture(GL_TEXTURE_2D, texId);
	// 纹理滤波
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	// 纹理包装
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	/*
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, w, h, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 10);
	*/

	return texId;
}

void Lesson03_FrameAnim::initGL()
{
	// 启动纹理
	glEnable(GL_TEXTURE_2D);
	// 启动深度测试
	glEnable(GL_DEPTH_TEST);

	m_unFrame = 0;
	m_unTexId = createTextureFromImage("res/xx.png");
}

void Lesson03_FrameAnim::render()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//gluPerspective(45.0f, (double)unWidth / (double)unHeight, 0.1f, 1000.0f);
	glOrtho(0, unWidth, unHeight, 0, -100, 100);

	unsigned int row = m_unFrame / 9;
	unsigned int col = m_unFrame % 9;

	float u   = float(col * m_unFrameWidth)        / float(m_unTexWidth);
	float v   = float(row * m_unFrameHeight)       / float(m_unTexHeight);
	float u1  = float((col + 1) * m_unFrameWidth)  / float(m_unTexWidth);
	float v1  = float((row + 1) * m_unFrameHeight) / float(m_unTexHeight);

	rectangle[0].u = u;
	rectangle[0].v = v1;

	rectangle[1].u = u;
	rectangle[1].v = v;

	rectangle[2].u = u1;
	rectangle[2].v = v1;

	rectangle[3].u = u1;
	rectangle[3].v = v;

	glBindTexture(GL_TEXTURE_2D, m_unTexId);

	//glMatrixMode(GL_MODELVIEW);
	///glLoadIdentity();
	//glTranslatef(0, 0, 10);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glVertexPointer(3, GL_FLOAT, sizeof(Vertex), rectangle);
	glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), &rectangle[0].u);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);


	m_unFrame = ++m_unFrame%64;
}