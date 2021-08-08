#include "Lesson04_6_PBO_2.h"
#include <iostream>

extern unsigned int unWidth;
extern unsigned int unHeight;

struct Vertex
{
	float x, y, z;
	float u, v;
	float r, g, b;
};

void Lesson04_6_PBO_2::onDisable()
{
}

void Lesson04_6_PBO_2::initGL()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glEnable(GL_TEXTURE_2D);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, double(unWidth) / double(unHeight), 0.1f, 100.0f);
	_texture = createTextureFromImage("floor.bmp");

	glGenBuffers(2, _pbo);
	glBindBuffer(GL_PIXEL_PACK_BUFFER, _pbo[0]);
	glBufferData(GL_PIXEL_PACK_BUFFER, unWidth * unHeight * 4, 0, GL_STREAM_READ);

	glBindBuffer(GL_PIXEL_PACK_BUFFER, _pbo[1]);
	glBufferData(GL_PIXEL_PACK_BUFFER, unWidth * unHeight * 4, 0, GL_STREAM_READ);

	glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);

}

unsigned int Lesson04_6_PBO_2::createTexture(int w, int h, const void* data)
{
	unsigned    texId;
	glGenTextures(1, &texId);
	glBindTexture(GL_TEXTURE_2D, texId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

	return  texId;
}
unsigned int Lesson04_6_PBO_2::createTextureFromImage(const char* fileName)
{
	//1 获取图片格式
	FREE_IMAGE_FORMAT fifmt = FreeImage_GetFileType(fileName, 0);
	if (fifmt == FIF_UNKNOWN)
	{
		return  0;
	}
	//2 加载图片
	FIBITMAP    *dib = FreeImage_Load(fifmt, fileName, 0);

	FREE_IMAGE_COLOR_TYPE type = FreeImage_GetColorType(dib);

	//! 获取数据指针
	FIBITMAP*   temp = dib;
	dib = FreeImage_ConvertTo32Bits(dib);
	FreeImage_Unload(temp);

	BYTE*   pixels = (BYTE*)FreeImage_GetBits(dib);
	int     width = FreeImage_GetWidth(dib);
	int     height = FreeImage_GetHeight(dib);

	for (int i = 0; i < width * height * 4; i += 4)
	{
		BYTE temp = pixels[i];
		pixels[i] = pixels[i + 2];
		pixels[i + 2] = temp;
	}

	unsigned    res = createTexture(width, height, pixels);
	FreeImage_Unload(dib);
	return      res;
}

void Lesson04_6_PBO_2::renderCube(GLuint tex, bool rot)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, double(unWidth) / double(unHeight), 0.1f, 100.0f);

	//glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();
	static  float   angle = 0;
	if (rot)
	{
		glRotated(angle, 1, 1, 1);
		angle += 1;
	}

	glBindTexture(GL_TEXTURE_2D, tex);

	Vertex g_cubeVertices[] =
	{
		{ -1.0f,-1.0f, 1.0f,0.0f, 0.0f,1.0f, 0.0f, 0.0f },
		{ 1.0f,-1.0f, 1.0f,1.0f, 0.0f,1.0f, 0.0f, 0.0f },
		{ 1.0f, 1.0f, 1.0f,1.0f, 1.0f,1.0f, 0.0f, 0.0f },
		{ -1.0f, 1.0f, 1.0f,0.0f, 1.0f,1.0f, 0.0f, 0.0f },

		{ -1.0f,-1.0f,-1.0f,1.0f, 0.0f,0.0f, 1.0f, 0.0f },
		{ -1.0f, 1.0f,-1.0f,1.0f, 1.0f,0.0f, 1.0f, 0.0f },
		{ 1.0f, 1.0f,-1.0f,0.0f, 1.0f,0.0f, 1.0f, 0.0f },
		{ 1.0f,-1.0f,-1.0f,0.0f, 0.0f,0.0f, 1.0f, 0.0f },

		{ -1.0f, 1.0f,-1.0f,0.0f, 1.0f,0.0f, 0.0f, 1.0f },
		{ -1.0f, 1.0f, 1.0f,0.0f, 0.0f,0.0f, 0.0f, 1.0f },
		{ 1.0f, 1.0f, 1.0f,1.0f, 0.0f,0.0f, 0.0f, 1.0f },
		{ 1.0f, 1.0f,-1.0f,1.0f, 1.0f,0.0f, 0.0f, 1.0f },

		{ -1.0f,-1.0f,-1.0f,1.0f, 1.0f,1.0f, 1.0f, 0.0f },
		{ 1.0f,-1.0f,-1.0f,0.0f, 1.0f,1.0f, 1.0f, 0.0f },
		{ 1.0f,-1.0f, 1.0f,0.0f, 0.0f,1.0f, 1.0f, 0.0f },
		{ -1.0f,-1.0f, 1.0f,1.0f, 0.0f,1.0f, 1.0f, 0.0f },

		{ 1.0f,-1.0f,-1.0f,1.0f, 0.0f,1.0f, 0.0f, 1.0f },
		{ 1.0f, 1.0f,-1.0f,1.0f, 1.0f,1.0f, 0.0f, 1.0f },
		{ 1.0f, 1.0f, 1.0f,0.0f, 1.0f,1.0f, 0.0f, 1.0f },
		{ 1.0f,-1.0f, 1.0f,0.0f, 0.0f,1.0f, 0.0f, 1.0f },

		{ -1.0f,-1.0f,-1.0f,0.0f, 0.0f,0.0f, 1.0f, 1.0f },
		{ -1.0f,-1.0f, 1.0f,1.0f, 0.0f,0.0f, 1.0f, 1.0f },
		{ -1.0f, 1.0f, 1.0f,1.0f, 1.0f,0.0f, 1.0f, 1.0f },
		{ -1.0f, 1.0f,-1.0f,0.0f, 1.0f,0.0f, 1.0f, 1.0f }
	};

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);


	/**
	* 这里大家可以慢慢体会
	*/
	float* addrVertex = (float*)g_cubeVertices;
	float* uvAddress = (float*)&g_cubeVertices[0].u;

	float* colorAddress = (float*)&g_cubeVertices[0].r;

	//--------------元素个数---元素类型---元素之间的内存偏移---数据地址
	//OpenGL根据元素之间的内存偏移来计算下一个元素的位置。
	glVertexPointer(3, GL_FLOAT, sizeof(Vertex), addrVertex);
	glColorPointer(3, GL_FLOAT, sizeof(Vertex), colorAddress);
	glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), uvAddress);

	glDrawArrays(GL_QUADS, 0, 24);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
}

void Lesson04_6_PBO_2::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.0f, 0.0f, -5);
	renderCube(_texture, true);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(3.0f, 0.0f, -5);
	renderCube(_texture, false);

	//glReadBuffer(GL_FRONT);
	//glReadBuffer(GL_BACK);

	//! DMA
	glBindBuffer(GL_PIXEL_PACK_BUFFER, _pbo[_DMA]);
	glReadPixels(0, 0, unWidth, unHeight, GL_RGBA, GL_UNSIGNED_BYTE, 0);


	glBindBuffer(GL_PIXEL_PACK_BUFFER, _pbo[_READ]);
	void*   data = glMapBuffer(GL_PIXEL_PACK_BUFFER, GL_READ_ONLY);
	if (data)
	{
		saveImage(unWidth, unHeight,(char*)data);
	}
	glUnmapBuffer(GL_PIXEL_PACK_BUFFER);

	std::swap(_DMA, _READ);
}

bool Lesson04_6_PBO_2::saveImage(int w, int h, char* data)
{
	FIBITMAP*   bitmap = FreeImage_Allocate(w, h, 32, 0, 0, 0);

	BYTE*       pixels = (BYTE*)FreeImage_GetBits(bitmap);

	memcpy(pixels, data, w * h * 4);
	bool    bSuccess = FreeImage_Save(FIF_PNG, bitmap, "e:/xx.png", PNG_DEFAULT);
	FreeImage_Unload(bitmap);
	return  bSuccess;
}
