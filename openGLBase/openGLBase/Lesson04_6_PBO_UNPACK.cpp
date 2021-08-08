#include "Lesson04_6_PBO_UNPACK.h"
#include <iostream>

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
Vertex verCude8[] = {
	/*
	前
	0------2
	|      |
	|      |
	1------3
	*/
	{-0.5,  0.5,   0.5,  0.0, 1.0},
	{-0.5,  -0.5,  0.5,  0.0, 0.0},
	{0.5,   0.5,   0.5,  1.0, 1.0},
	{0.5,   -0.5,  0.5,  1.0, 0.0},
	/*
	后
	4------6
	|      |
	|      |
	5------7
	*/
	{-0.5,  0.5,   -0.5,  0.0, 1.0},
	{-0.5,  -0.5,  -0.5,  0.0, 0.0},
	{0.5,   0.5,   -0.5,  1.0, 1.0},
	{0.5,   -0.5,  -0.5,  1.0, 0.0},
};

unsigned char indexVer8[] = {
	0,1,2,3,
	4,5,6,7,
	4,0,5,1,
	6,2,7,3,
	4,0,6,2,
	5,1,7,3
};

void Lesson04_6_PBO_UNPACK::onDisable()
{
	// 删除纹理(数组)
	glDeleteTextures(2, &m_unTextureId);

	glDeleteBuffers(1, &m_unVbo);

	glDeleteBuffers(2, m_pbo);

	int err = glGetError(); //0没有错误 
}

void Lesson04_6_PBO_UNPACK::initGL()
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	m_unTextureId = createTextureFromImage("res/floor.bmp");
	createVbo();
	createIbo();

	DMA = 0;
	READ = 0;

	glGenBuffers(2, m_pbo);
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, m_pbo[0]); //pack从显卡读取数据
	glBufferData(GL_PIXEL_UNPACK_BUFFER, unWidth*unHeight*4, NULL, GL_STREAM_DRAW); //分配显卡内存空间 GL_STREAM_READ用于读取

	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, m_pbo[1]); //pack从显卡读取数据
	glBufferData(GL_PIXEL_UNPACK_BUFFER, unWidth*unHeight * 4, NULL, GL_STREAM_DRAW); //分配显卡内存空间 GL_STREAM_READ用于读取

	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0); // 恢复状态

	//纹理大小必须和fbo一样，否则创建失败 ？？？
	m_dynamicTexId = createTexture(unWidth, unHeight, 0);
}

unsigned int Lesson04_6_PBO_UNPACK::createTexture(int w, int h, const void* data)
{
	unsigned texId;
	glGenTextures(1, &texId);
	glBindTexture(GL_TEXTURE_2D, texId);
	// 纹理滤波
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

	return texId;
}
unsigned int Lesson04_6_PBO_UNPACK::createTextureFromImage(const char* fileName)
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

void Lesson04_6_PBO_UNPACK::createVbo()
{
	// 产生一个buff
	glGenBuffers(1, &m_unVbo);
	// 绑定buff 告诉openGL m_unVbo是顶点缓冲区
	glBindBuffer(GL_ARRAY_BUFFER, m_unVbo);
	//glBufferData:申请显卡内存 glBufferDataARB:arb的扩展，不加arb在移植的时候方便一点，在glES下以成标准把arb去掉了
	//如果显存不够，调用glBufferData重新分配显存，会把原来的缓冲区地址删掉
	glBufferData(GL_ARRAY_BUFFER, sizeof(verCude8), verCude8, GL_STATIC_DRAW);
	//glBufferSubData(); //动态更新数据
	// 恢复状态，不然会对后续绘制产生影响
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Lesson04_6_PBO_UNPACK::createIbo()
{
	glGenBuffers(1, &m_unIbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_unIbo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexVer8), indexVer8, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Lesson04_6_PBO_UNPACK::drawCude(bool isRotate)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, double(unWidth) / double(unHeight), 0.1f, 1000.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0, 0, -2);

	static float angle = 0.0f;
	if (isRotate)
	{
		glRotated(angle, 0, 1, 0);
		angle = (angle += 0.2);
	}

	glBindBuffer(GL_ARRAY_BUFFER, m_unVbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_unIbo);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glVertexPointer(3, GL_FLOAT, sizeof(Vertex), 0);
	glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), (void*)12);

	glDrawElements(GL_TRIANGLE_STRIP, 24, GL_UNSIGNED_BYTE, 0);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

void Lesson04_6_PBO_UNPACK::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	glBindTexture(GL_TEXTURE_2D, m_dynamicTexId);
	//----------------------------------------------------------------
	//纹理数据 from pbo 通过 DMA to texture
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, m_pbo[DMA]); 
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, unWidth, unHeight, GL_RGBA, GL_UNSIGNED_BYTE, 0);

	//----------------------------------------------------------------
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, m_pbo[READ]);
	void* data = glMapBuffer(GL_PIXEL_UNPACK_BUFFER, GL_READ_WRITE);
	if (data)
	{
		updateData((unsigned char*)data);
	}
	glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER);
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
	std::swap(DMA,READ);

	drawCude(true);
}

void Lesson04_6_PBO_UNPACK::updateData(unsigned char* data)
{
	//unsigned int* pData = (unsigned int*)data;
	int size = unWidth * unHeight * 4;
	for (int i = 0; i < size; i += 8)
	{
		data[i + 0] = rand();
		data[i + 1] = rand();
		data[i + 2] = rand();
		data[i + 3] = 255;

		data[i + 4] = rand();
		data[i + 5] = rand();
		data[i + 6] = rand();
		data[i + 7] = 255;
	}
}

bool Lesson04_6_PBO_UNPACK::saveImage(int w, int h, char* data)
{
	FIBITMAP*   bitmap = FreeImage_Allocate(w, h, 32, 0, 0, 0);
	BYTE*       pixels = (BYTE*)FreeImage_GetBits(bitmap);

	for (int i = 0; i < w*h * 4; i += 4)
	{
		BYTE temp = data[i];
		data[i] = data[i + 2];
		data[i + 2] = temp;
	}
	memcpy(pixels, data, w * h * 4);
	bool    bSuccess = FreeImage_Save(FIF_PNG, bitmap, "e:/xx.png", PNG_DEFAULT);
	FreeImage_Unload(bitmap);
	return  bSuccess;
}
