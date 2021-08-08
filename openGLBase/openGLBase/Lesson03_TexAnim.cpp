#include "Lesson03_TexAnim.h"
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

Vertex vertex2[4] = {
	{-0.5, 0.125,  0.0,  0.0, 0.25},
	{-0.5, -0.125, 0.0,  0.0, 0.0},
	{0.5,  0.125,  0.0,  1.0, 0.25},
	{0.5, -0.125,  0.0,  1.0, 0.0},
};

void Lesson03_TexAnim::onDisable()
{
	// 删除纹理(数组)
	glDeleteTextures(2, m_unTextureId);

	int err = glGetError(); //0没有错误 
}

void Lesson03_TexAnim::initGL()
{
	// 启动纹理
	glEnable(GL_TEXTURE_2D);
	// 启动深度测试
	glEnable(GL_DEPTH_TEST);

	m_unTextureId[0] = createTextureFromImage("res/1.png");
	m_unTextureId[1] = createTextureFromImage("res/1008.jpg");
	m_unTextureId[2] = createTextureFromImage("res/1009.png");
}

unsigned int Lesson03_TexAnim::createTexture(int w, int h, const void* data)
{
	unsigned texId;
	glGenTextures(1, &texId);
	glBindTexture(GL_TEXTURE_2D, texId);
	// 纹理滤波
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	// 纹理包装
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

	return texId;
}
unsigned int Lesson03_TexAnim::createTextureFromImage(const char* fileName)
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

// 纹理矩阵
void Lesson03_TexAnim::playTexAnim()
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	// 1.通过uv实现纹理动画
	//for (int i = 0; i < 4; ++i) vertex2[i].v -= 0.001;

	// 2. 通过纹理矩阵实现纹理动画 （只在pc上可用）
	glMatrixMode(GL_TEXTURE);
	//glLoadIdentity();
	//glTranslatef(0, 0.5f, 0);
	//glScalef(4,4,4);
	glRotatef(1, 0, 0, 1);
	

	glVertexPointer(3, GL_FLOAT, sizeof(Vertex), vertex2);
	glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), &vertex2[0].u);

	glBindTexture(GL_TEXTURE_2D, m_unTextureId[0]);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0, 0, -3);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}
// 多纹理
// 在以前openGL默认一个三角面上最少可以贴8张图， 在arb扩展的多纹理里面在一个表面最多可以贴32张纹理（基本2~3张够了）
/*
由于OpenGL的标准更新不是很频繁，因此，当某种技术应用流行起来时，显卡厂商为了支持该技术，会使用自己的扩展来实现该功能。
但是不同厂商如果有不同的实现，那么程序编写将会异常繁琐。因此多个厂商共同协商使用一致的扩展，这就是EXT扩展。
如果这个扩展不仅多厂商协商，而且得到OpenGL体系结构审核委员会（即ARB）的确认，那么该扩展便成为ARB扩展。
最后如果标准制定者认为该功能有必要添加到新版本的标准中，则该扩展成为标准的一部分，不再是扩展。
利用扩展，即便是OpenGL低版本也能实现大部分高版本的功能。使用扩展时，要注意优先级，标准>ARB>EXT，即有标准功能便不用扩展，有ARB扩展实现便不用EXT扩展。
*/
void Lesson03_TexAnim::manyTexture1()
{
	/*opengl扩展（至少1.5版本后才有） 现在的显卡100%支持1.5版甚至更高*/
	// 激活单元纹理（至少有8个，默认从第0个单元取）
	glActiveTextureARB(GL_TEXTURE0_ARB);
	glEnable(GL_TEXTURE_2D);
	// 将指定的纹理放到第0个单元上
	glBindTexture(GL_TEXTURE_2D, m_unTextureId[0]);

	glActiveTextureARB(GL_TEXTURE1_ARB);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, m_unTextureId[1]);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0, 0, -3);

	glBegin(GL_TRIANGLE_STRIP);
		// 纹理坐标
		glMultiTexCoord2dARB(GL_TEXTURE0_ARB, 0.0f, 1.0f);
		glMultiTexCoord2dARB(GL_TEXTURE1_ARB, 0.0f, 1.0f);
		// 顶点坐标
		glVertex3f(-0.5, 0.5, 0.0);

		glMultiTexCoord2dARB(GL_TEXTURE0_ARB, 0.0f, 0.0f);
		glMultiTexCoord2dARB(GL_TEXTURE1_ARB, 0.0f, 0.0f);
		glVertex3f(-0.5, -0.5, 0.0);

		glMultiTexCoord2dARB(GL_TEXTURE0_ARB, 1.0f, 1.0f);
		glMultiTexCoord2dARB(GL_TEXTURE1_ARB, 1.0f, 1.0f);
		glVertex3f(0.5, 0.5, 0.0);

		glMultiTexCoord2dARB(GL_TEXTURE0_ARB, 1.0f, 0.0f);
		glMultiTexCoord2dARB(GL_TEXTURE1_ARB, 1.0f, 0.0f);
		glVertex3f(0.5, -0.5, 0.0);
	glEnd();
}

void Lesson03_TexAnim::manyTexture2()
{
	glActiveTextureARB(GL_TEXTURE0_ARB);            // 选择GL_TEXTURE0为设置目标
	//glEnable(GL_TEXTURE_2D);                      // 激活GL_TEXTURE0单元 GL_TEXTURE0可以不要
	glBindTexture(GL_TEXTURE_2D, m_unTextureId[0]); // 为GL_TEXTURE0单元绑定texture纹理图像
	//glDisable(GL_TEXTURE_2D);                     // 关闭纹理

	glActiveTextureARB(GL_TEXTURE1_ARB);
	glEnable(GL_TEXTURE_2D);  // 必须开启
	glBindTexture(GL_TEXTURE_2D, m_unTextureId[1]);
	//glDisable(GL_TEXTURE_2D); // 关闭多重纹理 GL_TEXTURE1_ARB层纹理,

	glActiveTextureARB(GL_TEXTURE2_ARB);
	glEnable(GL_TEXTURE_2D);    // 必须开启
	glBindTexture(GL_TEXTURE_2D, m_unTextureId[2]);
	//glDisable(GL_TEXTURE_2D);   // 关闭多重纹理 GL_TEXTURE2_ARB层纹理,


	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0, 0, -3);


	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, sizeof(Vertex), &vertex2);


	glClientActiveTextureARB(GL_TEXTURE0_ARB); // 设置纹理单元的纹理顶点数组
	glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), &vertex2[0].u);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glClientActiveTextureARB(GL_TEXTURE1_ARB);
	glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), &vertex2[0].u);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glClientActiveTextureARB(GL_TEXTURE2_ARB);
	glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), &vertex2[0].u);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void Lesson03_TexAnim::playManyTextureAnim()
{
	glMatrixMode(GL_TEXTURE); // 滚动的是GL_TEXTURE1 最后一次绑定
	glTranslatef(0.01, 0, 0);

	glActiveTextureARB(GL_TEXTURE0_ARB);            // 选择GL_TEXTURE0为设置目标
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, m_unTextureId[0]); // 为GL_TEXTURE0单元绑定texture纹理图像
	//glMatrixMode(GL_TEXTURE); // 滚动的是GL_TEXTURE0
	//glTranslatef(0.01, 0, 0);

	glActiveTextureARB(GL_TEXTURE1_ARB);
	glEnable(GL_TEXTURE_2D); 
	glBindTexture(GL_TEXTURE_2D, m_unTextureId[1]);
	//glMatrixMode(GL_TEXTURE); // 滚动的是GL_TEXTURE1
	//glTranslatef(0.01, 0, 0);
	

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, sizeof(Vertex), &vertex2);

	glClientActiveTextureARB(GL_TEXTURE0_ARB); // 设置纹理单元的纹理顶点数组
	glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), &vertex2[0].u);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glClientActiveTextureARB(GL_TEXTURE1_ARB);
	glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), &vertex2[0].u);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0, 0, -2);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void Lesson03_TexAnim::render()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, (double)unWidth/(double)unHeight, 0.1f, 1000.0f);

	//playTexAnim();
	//manyTexture1();
	//manyTexture2();
	playManyTextureAnim();
}
