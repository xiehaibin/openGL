#include "Lesson03_Sprite.h"
#include <cmath>
#include<time.h>

extern unsigned int unWidth;
extern unsigned int unHeight;

#define PI                      3.14159265358979323

float unitRandom()
{
	return float(rand()) / float(RAND_MAX);
}

float rangeRandom(float fLow, float fHigh)
{
	return (fHigh - fLow)*unitRandom() + fLow;
}

unsigned long timeGetTime()
{
	return (unsigned long)GetTickCount();
}

/*
sprite
*/
void Lesson03_Sprite::onDisable()
{
	// 删除纹理(数组)
	glDeleteTextures(1, &m_unTexId);
}

unsigned int Lesson03_Sprite::createTextureFromImage(const char* fileName)
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

unsigned int Lesson03_Sprite::createTexture(int w, int h, const void* data)
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

void Lesson03_Sprite::initGL()
{
	srand((int)time(0));
	// 启动纹理
	glEnable(GL_TEXTURE_2D);
	// 启动深度测试
	glEnable(GL_DEPTH_TEST);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, (double)unWidth / (double)unHeight, 0.1f, 100.0f);

	m_unFrame = 0;
	m_unTexId = createTextureFromImage("res/particle.bmp");
	initPointSprites();
}

float3 Lesson03_Sprite::getRandomVector(void)
{
	float3 vVector;
	vVector.z = rangeRandom(-1.0f, 1.0f);

	float radius = (float)sqrt(1 - vVector.z * vVector.z);
	float t = rangeRandom(-PI, PI);
	vVector.x = (float)cosf(t) * radius;
	vVector.y = (float)sinf(t) * radius;

	return vVector;
}

void Lesson03_Sprite::initPointSprites(void)
{
	for (int i = 0; i < c_ParticlesMax; i++)
	{
		m_aParticle[i].curPos = { 0.0f, 0.0f, 0.0f };

		float3 v = getRandomVector();
		v.x *= rangeRandom(0.5f, 5.0f);
		v.y *= rangeRandom(0.5f, 5.0f);
		v.z *= rangeRandom(0.5f, 5.0f);
		m_aParticle[i].curVel = v;

		m_aParticle[i].r = rand() % 255;
		m_aParticle[i].g = rand() % 255;
		m_aParticle[i].b = rand() % 255;
		m_aParticle[i].a = 255;
	}
}

void Lesson03_Sprite::updateSprite(void)
{
	static double dStartAppTime = timeGetTime();
	float fElpasedAppTime = (float)((timeGetTime() - dStartAppTime) * 0.001);

	static double dLastFrameTime = timeGetTime();
	double dCurrenFrameTime = timeGetTime();
	double dElpasedFrameTime = (float)((dCurrenFrameTime - dLastFrameTime) * 0.001);
	dLastFrameTime = dCurrenFrameTime;

	if (fElpasedAppTime >= 5.0f)
	{
		for (int i = 0; i < c_ParticlesMax; ++i)
		{
			m_aParticle[i].curPos = { 0.0f, 0.0f, 0.0f };
		}

		dStartAppTime = timeGetTime();
	}

	for (int i = 0; i < c_ParticlesMax; ++i)
	{
		float vx = m_aParticle[i].curVel.x * (float)dElpasedFrameTime;
		float vy = m_aParticle[i].curVel.y * (float)dElpasedFrameTime;
		float vz = m_aParticle[i].curVel.z * (float)dElpasedFrameTime;

		m_aParticle[i].curPos.x += vx;
		m_aParticle[i].curPos.y += vy;
		m_aParticle[i].curPos.z += vz;
	}
}
void Lesson03_Sprite::renderSprite(void)
{
	//启动混合
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);

	// This is how will our point sprite's size will be modified by 
	// distance from the viewer
	// float quadratic[] = { 1.0f, 0.0f, 0.01f };
	// glPointParameterfvARB(GL_POINT_DISTANCE_ATTENUATION_ARB, quadratic);

	float maxSize = 0.0f;
	glGetFloatv(GL_POINT_SIZE_MAX_ARB, &maxSize);
	// size of 1024.0f!
	if (maxSize > 100.0f)
		maxSize = 100.0f;

	glPointSize(maxSize);

	// 指定点的阀值
	// 如果精灵超过60 自动消隐
	glPointParameterfARB(GL_POINT_FADE_THRESHOLD_SIZE_ARB, 60.0f);
	// 指定精灵点的最小值
	glPointParameterfARB(GL_POINT_SIZE_MIN_ARB, 1.0f);
	// 指定精灵点的最大值
	glPointParameterfARB(GL_POINT_SIZE_MAX_ARB, maxSize);
	// 纹理环境 用点精灵的坐标替换当前坐标
	glTexEnvf(GL_POINT_SPRITE_ARB, GL_COORD_REPLACE_ARB, GL_TRUE);
	// 启用顶点精灵
	glEnable(GL_POINT_SPRITE_ARB);

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, sizeof(Particle), &m_aParticle[0].curPos.x);

	glEnableClientState(GL_COLOR_ARRAY);
	glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(Particle), &m_aParticle[0].r);


	glDrawArrays(GL_POINTS, 0, c_ParticlesMax);


	glDisable(GL_POINT_SPRITE_ARB);
}

void Lesson03_Sprite::render()
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.0f, 0.0f, -10.0f);
	
	updateSprite();

	glBindTexture(GL_TEXTURE_2D, m_unTexId);

	renderSprite();
}