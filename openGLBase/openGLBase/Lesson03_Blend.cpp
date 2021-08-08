#include "Lesson03_Blend.h"

extern unsigned int unWidth;
extern unsigned int unHeight;

struct Vertex
{
	float x, y, z;
	float u, v;
};

Vertex walls[4] = {
	{-1, 1, 0.0,  0.0, 1.0},
	{-1,-1, 0.0,  0.0, 0.0},
	{1,  1, 0.0,  1.0, 1.0},
	{1, -1, 0.0,  1.0, 0.0},
};

Vertex mask[4] = {
	{0,         0,            0.0,  0.0, 1.0},
	{0,         unHeight/2,   0.0,  0.0, 0.0},
	{unWidth/2, 0,            0.0,  1.0, 1.0},
	{unWidth/2, unHeight/2,   0.0,  1.0, 0.0},
};

void Lesson03_Blend::onDisable()
{
	// 删除纹理(数组)
	glDeleteTextures(1, &m_unTexId0);
	glDeleteTextures(1, &m_unTexId1);
	glDeleteTextures(1, &m_unTexId2);
}

unsigned int Lesson03_Blend::createTextureFromImage(const char* fileName)
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

	for (int i = 0; i < width * height * 4; i += 4)
	{
		BYTE temp = pixels[i];
		pixels[i] = pixels[i + 2];
		pixels[i + 2] = temp;
	}

	unsigned int res = createTexture(width, height, pixels);
	FreeImage_Unload(dib);

	return res;
}

unsigned int Lesson03_Blend::createTexture(int w, int h, const void* data)
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
	
	//gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, w, h, GL_RGBA, GL_UNSIGNED_BYTE, data);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 10);
	

	return texId;
}

void Lesson03_Blend::initGL()
{
	// 启动纹理
	glEnable(GL_TEXTURE_2D);
	// 启动深度测试
	glEnable(GL_DEPTH_TEST);

	m_unTexId0 = createTextureFromImage("res/floor.bmp");
	m_unTexId1 = createTextureFromImage("res/CrossHair.bmp");
	m_unTexId2 = createTextureFromImage("res/CrossHairMask.bmp");

	m_unTexId3 = createTextureFromImage("res/9010.jpg");
	m_unTexId4 = createTextureFromImage("res/btn02.png");
	m_unTexId5 = createTextureFromImage("res/1008.jpg");
	m_unTexId6 = createTextureFromImage("res/particle.bmp");

	m_unTexId1000 = createTextureFromImage("res/1000.png");
	m_unTexId1001 = createTextureFromImage("res/1001.png");
	m_unTexId1002 = createTextureFromImage("res/1002.png");
}

void Lesson03_Blend::renderWill()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, (double)unWidth / (double)unHeight, 0.1f, 100.0f);

	glEnable(GL_DEPTH_TEST);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glVertexPointer(3, GL_FLOAT, sizeof(Vertex), walls);
	glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), &walls[0].u);

	glBindTexture(GL_TEXTURE_2D, m_unTexId0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.0f, 0.0f, -1.5f);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

void Lesson03_Blend::randerOrtho()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, unWidth, unHeight, 0, -100, 100);

	glDisable(GL_DEPTH_TEST); // 禁用深度

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glVertexPointer(3, GL_FLOAT, sizeof(Vertex), mask);
	glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), &mask[0].u);

	// 指定混合颜色
	//if (0) glColor4f(0, 1, 0, 1);
	//else glColor4f(1, 1, 1, 1);

	/*
	GL_ZERO：				表示使用0.0作为因子，实际上相当于不使用这种颜色参与混合运算。
	GL_ONE：				表示使用1.0作为因子，实际上相当于完全的使用了这种颜色参与混合运算。
	GL_SRC_ALPHA：			表示使用源颜色的alpha值来作为因子。
	GL_DST_ALPHA：			表示使用目标颜色的alpha值来作为因子。
	GL_ONE_MINUS_SRC_ALPHA：表示用1.0减去源颜色的alpha值来作为因子。
	GL_ONE_MINUS_DST_ALPHA：表示用1.0减去目标颜色的alpha值来作为因子。
	
	GL_SRC_COLOR（把源颜色的四个分量分别作为因子的四个分量）、
	GL_ONE_MINUS_SRC_COLOR、
	GL_DST_COLOR、
	GL_ONE_MINUS_DST_COLOR等，
	前两个在OpenGL旧版本中只能用于设置目标因子，后两个在OpenGL 旧版本中只能用于设置源因子。新版本的OpenGL则没有这个限制，
	并且支持新的GL_CONST_COLOR（设定一种常数颜色，将其四个分量分别作为 因子的四个分量）、

	GL_ONE_MINUS_CONST_COLOR、
	GL_CONST_ALPHA、 
	GL_ONE_MINUS_CONST_ALPHA。
	GL_SRC_ALPHA_SATURATE。

	新版本的OpenGL还允许颜色的alpha 值和RGB值采用不同的混合因子。但这些都不是我们现在所需要了解的。毕竟这还是入门教材，不需要整得太复杂~
	*/
	
	/*
	glBlendFunc(GL_ONE, GL_ZERO); 则表示完全使用源颜色，完全不使用目标颜色，因此画面效果和不使用混合的时候一致（当然效率可能会低一点点）。
	                              如果没有设置源因子和目标因子，则默认情况就是这样的设置。
	glBlendFunc(GL_ZERO, GL_ONE); 则表示完全不使用源颜色，因此无论你想画什么，最后都不会被画上去了。（但这并不是说这样设置就没有用，有些时候可能有特殊用途）
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 则表示源颜色乘以自身的alpha 值，目标颜色乘以1.0减去源颜色的alpha值，这样一来，
	                              源颜色的alpha值越大，则产生的新颜色中源颜色所占比例就越大，而目标颜色所占比例则减 小。这种情况下，
								  我们可以简单的将源颜色的alpha值理解为“不透明度”。这也是混合时最常用的方式。
	glBlendFunc(GL_ONE, GL_ONE);  则表示完全使用源颜色和目标颜色，最终的颜色实际上就是两种颜色的简单相加。
	                              例如红色(1, 0, 0)和绿色(0, 1, 0)相加得到(1, 1, 0)，结果为黄色。

	*/

	//注意： 所谓源颜色和目标颜色，是跟绘制的顺序有关的。假如先绘制了一个红色的物体，再在其上绘制绿色的物体。则绿色是源颜色，红色是目标颜色。
	//glBlendFunc 该函数的功能是将引入的值与颜色缓冲中已有的值混合

	//源因子: GL_DST_COLOR屏幕数据  目标因子: GL_ZERO目标因子我们要画的
	//glBlendFunc(GL_ONE, GL_ZERO); //默认
	//glBlendFunc(GL_DST_COLOR, GL_ZERO);


	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0, 0, 0);
	
	glEnable(GL_BLEND);

	//glBlendFunc(GL_DST_COLOR, GL_ZERO);  //相当于 目标 * 源
	//glBindTexture(GL_TEXTURE_2D, m_unTexId2);
	//glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(50, 50, 0);

	glBlendFunc(GL_ONE, GL_ONE);  //相当于 目标 + 源
	glBindTexture(GL_TEXTURE_2D, m_unTexId1);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	
	

	// 与背景混合
	/*
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_COLOR, GL_DST_COLOR);
	glBindTexture(GL_TEXTURE_2D, m_unTexId5);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_COLOR, GL_ZERO);
	glBindTexture(GL_TEXTURE_2D, m_unTexId6);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	*/


	/*
	// 以alpha通道混合
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBindTexture(GL_TEXTURE_2D, m_unTexId4);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	*/

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

void Lesson03_Blend::randerOrthoTest()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, unWidth, unHeight, 0, -100, 100);

	glDisable(GL_DEPTH_TEST); // 禁用深度


	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glVertexPointer(3, GL_FLOAT, sizeof(Vertex), mask);
	glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), &mask[0].u);

	/*
	// rgb + rgb
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ZERO);
	glBindTexture(GL_TEXTURE_2D, m_unTexId1000);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	// + 
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);
	glBindTexture(GL_TEXTURE_2D, m_unTexId1001);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	*/

	/*
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0, 0, 0);

	glEnable(GL_BLEND);
	glBlendFunc(GL_DST_COLOR, GL_ZERO);   //相当于 目标 * 源
	glBindTexture(GL_TEXTURE_2D, m_unTexId6);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(20, 20, 0);

	glBlendFunc(GL_ONE, GL_ONE);         //相当于 目标 + 源
	glBindTexture(GL_TEXTURE_2D, m_unTexId1002);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	*/

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_DST_ALPHA);
	glBindTexture(GL_TEXTURE_2D, m_unTexId1001);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);


	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

void Lesson03_Blend::render()
{
	renderWill();
	randerOrtho();
	//randerOrthoTest();
}
