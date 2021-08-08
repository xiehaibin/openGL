#include "Lesson03_minMap.h"
#include <stdlib.h>
//#include <gl/GLU.h> //制作minmap 

extern unsigned int unWidth;
extern unsigned int unHeight;

struct Vertex3
{
	float x;
	float y;
	float z;

	float u;
	float v;
};

Vertex3 verCude[] = {
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
你恐怕还对带宽有误解
带宽指的不是数据从系统内存传输到显存需要的传输量
显存足够的情况下 纹理数据是一直存在于显存的
带宽指的是渲染时数据在显存（实际上也不一定是显存 现在GPU基本都支持DMA）和处理单元之间传输所需要的传输量
*/

/*
使用 Mipmap 有两个重要的优点：
1. 通过大幅提高纹理缓存效率来提高性能, 特别是在强缩小的情况下。
2.通过消除不使用 mipmapping 的纹理采样而造成的混叠，从而提高图像质量。
mipmapping 的单一限制是每个图像需要大约三分之一的纹理内存。根据情况，与渲染速度和图像质量方面的好处相比，这个成本可能很小。
在某些情况下，不应使用Mipmap。具体来说，在不能合理应用过滤的情况下，不应使用mipmapping，例如包含非图像数据（如索引或深度纹理）的纹理。
对于从不缩小的纹理也应该避免，例如，UI元素中的纹理元素总是与像素一一映射
*/

/*
为了加快渲染速度和减少图像锯齿，贴图被处理成由一系列被预先计算和优化过的图片组成的文件,这样的贴图被称为 MIP map 或者 mipmap
多级渐进纹理过滤能够有效地提高图形渲染速度，当物体离投影平面较远时，Direct3D会选择一张尺寸较小的纹理进行渲染，
而无需经过复杂的诸如各项异性纹理过滤，并且由于这时纹理需要的显存比不使用多级渐进纹理时小，因此能有效地减少纹理载入显存的时间。
缺点:是对内存的要求比较高
*/
Lesson03_minMap::Lesson03_minMap()
{

}
Lesson03_minMap::~Lesson03_minMap()
{
}

void Lesson03_minMap::onDisable()
{
	// 删除纹理(数组)
	glDeleteTextures(1, &m_unTexId1);

	int err = glGetError(); //0没有错误 
}

unsigned int Lesson03_minMap::createTextureFromImage(const char* fileName)
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

unsigned int Lesson03_minMap::createTexture(int w, int h, const void* data)
{
	unsigned texId;
	glGenTextures(1, &texId);
	glBindTexture(GL_TEXTURE_2D, texId);
	// 纹理滤波
	/*
	GL_LINEAR:  在mip基层上使用线性过滤 
	GL_NEAREST: 在mip基层上使用最邻近过滤 

	GL_NEAREST_MIPMAP_NEAREST	选择最邻近的mip层，并使用最邻近过滤
	GL_NEAREST_MIPMAP_LINEAR	在mip层之间使用线性插值和最邻近过滤

	GL_LINEAR_MIPMAP_NEAREST	选择最邻近的mip层，使用线性过滤
	GL_LINEAR_MIPMAP_LINEAR	    在mip层之间使用线性插值和使用线性过滤，又称三线性mipmap  性能最差  效果最好
	*/
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	// 纹理包装
	/*
	GL_REPEAT:          重复（默认）
	GL_CLAMP：          边缘拉伸
	GL_MIRRORED_REPEAT: 镜像
	*/
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	/*
	分配显存在显存中创建纹理， 会把原来的删掉
	*/
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);


	/*
	mipMap:
	纹理并不是一张纹理， 纹理由多张图组成，0表示最大的
	一张图片放大缩小显卡要纹理计算产生一张新的图片，这个过程比较耗时
	当摄像机离我们近的时候用大纹理，离我们远的时候用较小的纹理， 下一级纹理是上一级纹理宽高1/2
	mipMap纹理宽高必须是四的倍数  宽高 256*256 可产生9级纹理
	*/
	//gluBuild2DMipmaps 内部for循环创建纹理 循环次数宽高除2 除到1为止 内存占用增加 
	//for()glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, w, h, GL_RGBA, GL_UNSIGNED_BYTE, data); // 产生一个mipmap纹理
	// 顶级限制
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL,  10);

	return texId;
}

void Lesson03_minMap::initGL()
{
	// 启动纹理
	glEnable(GL_TEXTURE_2D);
	// 启动深度测试
	glEnable(GL_DEPTH_TEST);

	//init();

	m_unTexId1 = createTextureFromImage("res/1009.png");
}

void Lesson03_minMap::render()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, (double)unWidth/(double)unHeight, 0.1f, 1000.0f);

	drawCude();
}

void Lesson03_minMap::drawCude()
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glVertexPointer(3, GL_FLOAT, sizeof(Vertex3), verCude);
	glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex3), &verCude[0].u);

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();
	glTranslatef(-2, 0, m_fAngle);
	//glRotated(-10, 0, 1, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 24);

	glLoadIdentity();
	glTranslatef(2, 0, m_fAngle);
	//glRotated(10, 0, 1, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 24);
	
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	m_fAngle = m_fAngle - 0.05f;
}
