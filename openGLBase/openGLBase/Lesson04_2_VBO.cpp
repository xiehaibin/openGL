#include "Lesson04_2_VBO.h"

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
Vertex verCude2[] = {
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

void Lesson04_2_VBO::onDisable()
{
	// 删除纹理(数组)
	glDeleteTextures(2, &m_unTextureId);

	// 
	glDeleteBuffers(1, &m_unVbo);

	int err = glGetError(); //0没有错误 
}

void Lesson04_2_VBO::initGL()
{
	// 启动纹理
	glEnable(GL_TEXTURE_2D);
	// 启动深度测试
	glEnable(GL_DEPTH_TEST);

	m_unTextureId = createTextureFromImage("res/1009.png");
	createVbo();
	m_fAngle = 0;
}

unsigned int Lesson04_2_VBO::createTexture(int w, int h, const void* data)
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
unsigned int Lesson04_2_VBO::createTextureFromImage(const char* fileName)
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

void Lesson04_2_VBO::createVbo()
{
	// 产生一个buff
	glGenBuffers(1, &m_unVbo);
	// 绑定buff 告诉openGL m_unVbo是顶点数组
	glBindBuffer(GL_ARRAY_BUFFER, m_unVbo);
	//glBufferData:申请显卡内存 glBufferDataARB:arb的扩展，不加arb在移植的时候方便一点，在glES下以成标准把arb去掉了
	//如果显存不够，调用glBufferData重新分配显存，会把原来的缓冲区地址删掉
	glBufferData(GL_ARRAY_BUFFER, sizeof(verCude2), verCude2, GL_STATIC_DRAW);
	//glBufferSubData(); //动态更新数据
	// 恢复状态，不然会对后续绘制产生影响
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Lesson04_2_VBO::drawCude()
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	// 每一帧都把数据打包传到我们显卡中
	//glVertexPointer(3, GL_FLOAT, sizeof(Vertex), verCude2);
	//glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), &verCude2[0].u);
	// 优化 减少内存到显存的传递
	glBindBuffer(GL_ARRAY_BUFFER, m_unVbo);
	// 绑定以后告诉glVertexPointer，glTexCoordPointer到显存里取，不会到内存里取
	glVertexPointer(3, GL_FLOAT, sizeof(Vertex), 0);
	glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), (void*)12);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(-1, 0, -5);
	glRotated(m_fAngle, 0, 1, 0);
	glBindTexture(GL_TEXTURE_2D, m_unTextureId);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 24);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	m_fAngle = (m_fAngle += 0.2);
}

void Lesson04_2_VBO::render()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, double(unWidth) / double(unHeight), 0.1f, 1000.0f);

	drawCude();
}