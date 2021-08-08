#include "Lesson04_1_DisplayList.h"

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

Vertex rect[4] = {
	{-0.5, 0.5,  0.0,  0.0, 1.0},
	{-0.5, -0.5, 0.0,  0.0, 0.0},
	{0.5,  0.5,  0.0,  1.0, 1.0},
	{0.5, -0.5,  0.0,  1.0, 0.0},
};

void Lesson04_1_DisplayList::onDisable()
{
	// 删除纹理(数组)
	glDeleteTextures(2, &m_unTextureId);

	glDeleteLists(m_rectDisplay1, 1);
	glDeleteLists(m_rectDisplay2, 1);

	int err = glGetError(); //0没有错误 
}

void Lesson04_1_DisplayList::initGL()
{
	// 启动纹理
	glEnable(GL_TEXTURE_2D);
	// 启动深度测试
	glEnable(GL_DEPTH_TEST);

	m_unTextureId = createTextureFromImage("res/1009.png");

	/*
		GL_COMPILE  编译
		把中间的opengl指令及数据打包一下，放到显卡端去执行
		有些函数会失效 状态查询的
	*/
	m_rectDisplay1 = 1;
	glNewList(m_rectDisplay1, GL_COMPILE); 
		renderCube();
	glEndList();

	m_rectDisplay2 = 2;
	glNewList(m_rectDisplay2, GL_COMPILE);
		drawCube();
	glEndList();
}

unsigned int Lesson04_1_DisplayList::createTexture(int w, int h, const void* data)
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
unsigned int Lesson04_1_DisplayList::createTextureFromImage(const char* fileName)
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

void  Lesson04_1_DisplayList::drawCube()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, double(unWidth)/double(unHeight), 0.01f, 1000.0f);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glVertexPointer(3, GL_FLOAT, sizeof(Vertex), rect);
	glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), &rect[0].u);

	glBindTexture(GL_TEXTURE_2D, m_unTextureId);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0, 0.5, -3);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

void Lesson04_1_DisplayList::renderCube()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, double(unWidth) / double(unHeight), 0.1f, 1000.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.0f, 0.0f, -5);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	
	glBindTexture(GL_TEXTURE_2D, m_unTextureId);
	glBegin(GL_QUADS);
		////////////////////// BACK WALL ///////////////////////
		glTexCoord2f(0.0f, 2.0f);	glVertex3f(-15, 1, -15);
		glTexCoord2f(0.0f, 0.0f);	glVertex3f(-15, -1, -15);
		glTexCoord2f(12.0f, 0.0f);	glVertex3f(15, -1, -15);
		glTexCoord2f(12.0f, 2.0f);	glVertex3f(15, 1, -15);

		////////////////////// FRONT WALL ///////////////////////
		glTexCoord2f(0.0f, 2.0f);	glVertex3f(-15, 1, 15);
		glTexCoord2f(0.0f, 0.0f);	glVertex3f(-15, -1, 15);
		glTexCoord2f(12.0f, 0.0f);	glVertex3f(15, -1, 15);
		glTexCoord2f(12.0f, 2.0f);	glVertex3f(15, 1, 15);

		////////////////////// LEFT WALL ///////////////////////
		glTexCoord2f(0.0f, 2.0f);	glVertex3f(-15, 1, -15);
		glTexCoord2f(0.0f, 0.0f);	glVertex3f(-15, -1, -15);
		glTexCoord2f(12.0f, 0.0f);	glVertex3f(-15, -1, 15);
		glTexCoord2f(12.0f, 2.0f);	glVertex3f(-15, 1, 15);

		////////////////////// RIGHT WALL ///////////////////////
		glTexCoord2f(0.0f, 2.0f);	glVertex3f(15, 1, -15);
		glTexCoord2f(0.0f, 0.0f);	glVertex3f(15, -1, -15);
		glTexCoord2f(12.0f, 0.0f);	glVertex3f(15, -1, 15);
		glTexCoord2f(12.0f, 2.0f);	glVertex3f(15, 1, 15);
		// Stop drawing the walls
	glEnd();
	
	
	glBindTexture(GL_TEXTURE_2D, m_unTextureId);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 16.0f);	glVertex3f(-15, -1, -15);
		glTexCoord2f(0.0f, 0.0f);	glVertex3f(-15, -1, 15);
		glTexCoord2f(16.0f, 0.0f);	glVertex3f(15, -1, 15);
		glTexCoord2f(16.0f, 16.0f);	glVertex3f(15, -1, -15);
	glEnd();
}

void Lesson04_1_DisplayList::render()
{
	//glCallList(m_rectDisplay1);
	GLuint list[] = { m_rectDisplay1, m_rectDisplay2};
	glListBase(0); // 开始点
	glCallLists(2, GL_UNSIGNED_INT, list);
}