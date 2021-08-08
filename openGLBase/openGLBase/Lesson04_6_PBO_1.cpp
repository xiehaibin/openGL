#include "Lesson04_6_PBO_1.h"
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
Vertex verCude7[] = {
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

unsigned char indexVer7[] = {
	0,1,2,3,
	4,5,6,7,
	4,0,5,1,
	6,2,7,3,
	4,0,6,2,
	5,1,7,3
};

void Lesson04_6_PBO_1::onDisable()
{
	// 删除纹理(数组)
	glDeleteTextures(2, &m_unTextureId);

	glDeleteBuffers(1, &m_unVbo);

	glDeleteBuffers(2, m_pbo);

	int err = glGetError(); //0没有错误 
}

void Lesson04_6_PBO_1::initGL()
{
	//glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	m_unTextureId = createTextureFromImage("res/floor.bmp");
	m_unTextureId1 = createTextureFromImage("res/9010.jpg");
	createVbo();
	createIbo();

	DMA = 0;
	READ = 0;

	glGenBuffers(2, m_pbo);
	glBindBuffer(GL_PIXEL_PACK_BUFFER, m_pbo[0]); //pack从显卡读取数据
	glBufferData(GL_PIXEL_PACK_BUFFER, unWidth*unHeight*4, NULL, GL_STREAM_READ); //分配显卡内存空间 GL_STREAM_READ用于读取

	glBindBuffer(GL_PIXEL_PACK_BUFFER, m_pbo[1]); //pack从显卡读取数据
	glBufferData(GL_PIXEL_PACK_BUFFER, unWidth*unHeight * 4, NULL, GL_STREAM_READ); //分配显卡内存空间 GL_STREAM_READ用于读取

	glBindBuffer(GL_PIXEL_PACK_BUFFER, 0); // 恢复状态

	//纹理大小必须和fbo一样，否则创建失败 ？？？
	m_dynamicTexId = createTexture(unWidth, unHeight, 0);
}

unsigned int Lesson04_6_PBO_1::createTexture(int w, int h, const void* data)
{
	unsigned texId;
	glGenTextures(1, &texId);
	glBindTexture(GL_TEXTURE_2D, texId);
	// 纹理滤波
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

	return texId;
}
unsigned int Lesson04_6_PBO_1::createTextureFromImage(const char* fileName)
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

void Lesson04_6_PBO_1::createVbo()
{
	// 产生一个buff
	glGenBuffers(1, &m_unVbo);
	// 绑定buff 告诉openGL m_unVbo是顶点缓冲区
	glBindBuffer(GL_ARRAY_BUFFER, m_unVbo);
	//glBufferData:申请显卡内存 glBufferDataARB:arb的扩展，不加arb在移植的时候方便一点，在glES下以成标准把arb去掉了
	//如果显存不够，调用glBufferData重新分配显存，会把原来的缓冲区地址删掉
	glBufferData(GL_ARRAY_BUFFER, sizeof(verCude7), verCude7, GL_STATIC_DRAW);
	//glBufferSubData(); //动态更新数据
	// 恢复状态，不然会对后续绘制产生影响
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Lesson04_6_PBO_1::createIbo()
{
	glGenBuffers(1, &m_unIbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_unIbo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexVer7), indexVer7, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Lesson04_6_PBO_1::drawCude(bool isRotate)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, double(unWidth) / double(unHeight), 0.1f, 100.0f);

	glEnable(GL_DEPTH_TEST);

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

	glBindTexture(GL_TEXTURE_2D, m_unTextureId1);
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_ONE, GL_ONE);
	glDrawElements(GL_TRIANGLE_STRIP, 24, GL_UNSIGNED_BYTE, 0);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);



	//!!!!!! 必須关闭buffer  后面的绘制无法显示,  pbo保存窗口有残影
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Lesson04_6_PBO_1::drawImg()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, unWidth, unHeight, 0, -100, 1000);

	glDisable(GL_DEPTH_TEST);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0, 0, 0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0, 0, 0);

	Vertex rect[] =
	{
		{0,       unHeight, 0,   0, 1},
		{0,       0,        0,   0, 0},
		{unWidth, unHeight, 0,   1, 1},
		{unWidth, 0,        0,   1, 0},
	};

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glVertexPointer(3, GL_FLOAT, sizeof(Vertex), rect);
	glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), &rect[0].u);

	glBindTexture(GL_TEXTURE_2D, m_unTextureId);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

void Lesson04_6_PBO_1::render()
{
	drawImg();
	drawCude(true);


	//----------------------------------------------------------------
	//!!以下由DMA显存控制器完成 不耗费CPU
	glBindBuffer(GL_PIXEL_PACK_BUFFER, m_pbo[DMA]); //pbo在显存和内存各有一份
	//将显存数据读到pbo里
	glReadPixels(0, 0, unWidth, unHeight, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	//----------------------------------------------------------------

	glBindBuffer(GL_PIXEL_PACK_BUFFER, m_pbo[READ]); 
	//用来将一个缓冲区对象中的数据映射为客户端中的地址空间  加锁 等待dma完成
	void* data = glMapBuffer(GL_PIXEL_PACK_BUFFER, GL_READ_WRITE);
	if (data)
	{
		saveImage(unWidth, unHeight, (char*)data);
	}
	//取消映射  解锁
	glUnmapBuffer(GL_PIXEL_PACK_BUFFER);
	std::swap(DMA,READ);


	/*
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, m_pbo);
	// 绑定纹理
	glBindTexture(GL_TEXTURE_2D, m_dynamicTexId);
	// 这个函数会判断 GL_PIXEL_UNPACK_BUFFER 这个地方有没有绑定一个缓冲区
	//   如果有，就从这个缓冲区读取数据，而不是data参数指定的那个内存
	// 前面参数很简单就不解释了，最后一个参数和上面glReadPixels同理，传NULL就行
	// 这样glTexSubImage2D就会从我们的缓冲区中读取数据了
	// 这里为什么要用glTexSubImage2D呢，因为如果用glTexImage2D，glTexImage2D会销毁纹理内存重新申请，glTexSubImage2D就仅仅只是更新纹理中的数据
	//  这就提高了速度，并且优化了显存的利用率
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, unWidth, unHeight, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.5, 0, -2);
	renderCube(false);

	// 完事了把GL_PIXEL_UNPACK_BUFFER这个地方的缓冲区解绑掉，以免别的函数误操作
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
	*/
}

bool Lesson04_6_PBO_1::saveImage(int w, int h, char* data)
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
