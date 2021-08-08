#include "Lesson03.h"
#include <stdlib.h>

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

Vertex vertex[4] = {
	{-0.5, 0.5,  0.0,  0.0, 3.0},
	{-0.5, -0.5, 0.0,  0.0, 0.0},
	{0.5,  0.5,  0.0,  3.0, 3.0},
	{0.5, -0.5,  0.0,  3.0, 0.0},
};

Vertex verCude[] = {
	// 前
	{-0.5,  0.5,   0.5,  0.0, 1.0},
	{-0.5,  -0.5,  0.5,  0.0, 0.0},
	{0.5,   0.5,   0.5,  1.0, 1.0},
	{0.5,   -0.5,  0.5,  1.0, 0.0},
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
};

/*
纹理坐标
0,1  1,1
0,0  1,0
*/
Lesson03::Lesson03()
{

}
Lesson03::~Lesson03()
{
}

void Lesson03::onDisable()
{
	// 删除纹理(数组)
	glDeleteTextures(1, &m_unTexId1);
	glDeleteTextures(1, &m_unTexId2);
	glDeleteTextures(1, &m_unTextureId);

	int err = glGetError(); //0没有错误 
}

void Lesson03::initGL()
{
	// 启动纹理
	glEnable(GL_TEXTURE_2D);
	// 启动深度测试
	glEnable(GL_DEPTH_TEST);

	//init();

	m_unTexId1 = createTextureFromImage("res/1009.png");
	m_unTexId2 = createTextureFromImage("res/9010.jpg");
}

void Lesson03::init()
{
	const int len = 128 * 128 * 4;
	unsigned char* data = new unsigned char[len];
	for (int i = 0; i < len; ++i)
	{
		data[i] = rand() % 255;
	}

	// 用来产生你要操作的纹理对象的索引，并没有分配纹理的内存
	// 1:纹理个数
	// 2：存储纹理索引的第一个元素指针
	glGenTextures(1, &m_unTextureId);

	glBindTexture(GL_TEXTURE_2D, m_unTextureId);
	//OpenGL——纹理过滤函数glTexParameteri()
	//图象从纹理图象空间映射到帧缓冲图象空间(映射需要重新构造纹理图像, 这样就会造成应用到多边形上的图像失真), 
	//这时就可用glTexParmeteri()函数来确定如何把纹理象素映射成像素.
	glTexParameteri(GL_TEXTURE_2D, 
		GL_TEXTURE_MAG_FILTER,     // 放大过滤
		GL_LINEAR);                // 线性过滤, 使用距离当前渲染像素中心最近的4个纹素加权平均值.
	glTexParameteri(GL_TEXTURE_2D, 
		GL_TEXTURE_MIN_FILTER,     // 缩小过滤
		GL_LINEAR);  

	// 生成一个2D纹理
	glTexImage2D(
		GL_TEXTURE_2D, 
		0,
		GL_RGBA,          //内部格式
		/*
		不规则纹理：宽高不是2^n
		早期显卡不支持不规则纹理，现在显卡都支持
		*/
		128,              //纹理图像的宽度 
		128,
		0,                //指定边框的宽度
		GL_RGBA,          //像素数据的颜色格式, 外部格式
		GL_UNSIGNED_BYTE, //像素数据的数据类型
		data              //指定内存中指向图像数据的指针
		);
}

unsigned int Lesson03::createTexture(int w, int h, const void* data)
{
	unsigned texId;
	glGenTextures(1, &texId);
	glBindTexture(GL_TEXTURE_2D, texId);
	// 纹理滤波
	/*
	GL_LINEAR:  线性过滤 效果好 性能差
	GL_NEAREST: 邻近过滤 效果差 性能好
	*/
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	// 纹理包装
	/*
	GL_REPEAT:          重复
	GL_CLAMP：          边缘拉伸
	GL_MIRRORED_REPEAT: 镜像
	*/
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	/*
	mipMap:
	纹理并不是一张纹理， 纹理由多张图组成，0表示最大的
	一张图片放大缩小显卡要纹理计算产生一张新的图片，这个过程比较耗时
	当摄像机离我们近的时候用大纹理，离我们远的时候用较小的纹理， 下一级纹理是上一级纹理宽高1/2
	mipMap纹理宽高必须是四的倍数
	*/
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

	return texId;
}
unsigned int Lesson03::createTextureFromImage(const char* fileName)
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

	unsigned int res = createTexture(width, height, pixels);
	FreeImage_Unload(dib);

	return res;
}

void Lesson03::drawTexture2()
{
	unsigned int texId = createTextureFromImage("res/1008.jpg");

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glVertexPointer(3, GL_FLOAT, sizeof(Vertex), vertex);
	glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), &vertex[0].u);

	glBindTexture(GL_TEXTURE_2D, texId);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0, 0, -5);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

void Lesson03::drawCudeTex()
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glVertexPointer(3, GL_FLOAT, sizeof(Vertex), verCude);
	glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), &verCude[0].u);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(1, 0, -5);
	glRotated(m_fAngle, 1, 1, 1);
	glBindTexture(GL_TEXTURE_2D, m_unTexId1);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 24);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(-1, 0, -5);
	glRotated(m_fAngle, 1, 1, 1);
	glBindTexture(GL_TEXTURE_2D, m_unTexId2);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 24);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	m_fAngle = (m_fAngle += 0.1);
}

void Lesson03::drawCudeTex2()
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glVertexPointer(3, GL_FLOAT, sizeof(Vertex), verCude);
	glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), &verCude[0].u);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0, 0, m_fAngle);
	//glRotated(m_fAngle, 1, 1, 1);
	for (int i = 0; i < 6; i++)
	{
		if (i%2 == 0) glBindTexture(GL_TEXTURE_2D, m_unTexId1);
		else glBindTexture(GL_TEXTURE_2D, m_unTexId2);
		glDrawArrays(GL_TRIANGLE_STRIP, i*4, 4);
	}
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	m_fAngle -= 0.05;
}

void Lesson03::render()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, (double)unWidth/(double)unHeight, 0.1f, 1000.0f);

	//drawTexture();
	//drawTexture2();
	//drawCudeTex();
	drawCudeTex2();
}

void Lesson03::drawTexture()
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glVertexPointer(3, GL_FLOAT, sizeof(Vertex), vertex);
	glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), &vertex[0].u);

	glBindTexture(GL_TEXTURE_2D, m_unTextureId);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0, 0, -10);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}
