#include "lesson01.h"
#include <windows.h>
#include <math.h>
#include <corecrt_math_defines.h>
#include "GLContext.h"

extern unsigned int unWidth;
extern unsigned int unHeight;

struct Vertex
{
	float x;
	float y;
	float z;

public:
	Vertex()
	{
		x = 0.0f;
		y = 0.0f;
		z = 0.0f;
	}
};

struct float3
{
	float x;
	float y;
	float z;
};

struct VertexC
{
	float x;
	float y;
	float z;

	float r;
	float g;
	float b;
};

unsigned char _Pixel[100 * 100 * 4] = {};
Lesson01::Lesson01()
{
	int size = sizeof(_Pixel) / sizeof(unsigned char);
	for (int i = 0; i < size; i++)
	{
		_Pixel[i] = rand()%255;
	}
}

void Lesson01::DrawPoint()
{
	glPointSize(10);
	//启用抗锯齿
	glEnable(GL_POINT_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glBegin(GL_POINTS);
	glVertex2f(250, 50);
	glEnd();
	glDisable(GL_POINT_SMOOTH);
	glDisable(GL_BLEND);

	glBegin(GL_POINTS);
	glVertex2f(230, 50);
	glEnd();
}

void Lesson01::DrawLine()
{
	glLineWidth(5);
	glColor3f(0.0, 1.0, 0.0); // 全局颜色

	//-------------------------------------------------
	// 启用抗锯齿 默认是关闭
	glEnable(GL_LINE_SMOOTH);
	// 指定抗锯齿算法
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

	// 启动混合模式
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// 启动样式
	glEnable(GL_LINE_STIPPLE);
	glLineStipple(1, 0xFF);  // 线的样式

	// 指定绘制模式“画线
	glBegin(GL_LINES);
	glVertex3f(0, 0, 0);
	glVertex3f(100, 100, 0);
	glEnd(); // 结束画线

	glDisable(GL_LINE_SMOOTH);
	glDisable(GL_BLEND);
	glDisable(GL_LINE_STIPPLE);
	//--------------------------------------------------

	// 画线段
	VertexC lines[4] = {
		{100.0f,   10.0f, 0,  1, 0, 0},
		{200.0f, 10.0f, 0,  1, 1, 0},

		{100.0f,   80.0f, 0,  1, 0, 1},
		{200.0f, 80.0f, 0,  0, 1, 1},
	};
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	glVertexPointer(3, GL_FLOAT, sizeof(VertexC), &lines);
	glColorPointer(3, GL_FLOAT, sizeof(VertexC), &lines[0].r);

	//glDrawArrays(GL_LINES, 0, 4);       //= 画线段
	//glDrawArrays(GL_LINE_STRIP, 0, 4);  //Z 线带
	glDrawArrays(GL_LINE_LOOP, 0, 4);     //X 线带首尾相连

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);

	//画圆
	int cx = 500;
	int cy = 100;
	int cz = 0;
	int r = 80;
	double radian = M_PI / 180.0f;
	float3 circular[361 * 2] = {};
	for (int i = 0; i < 361; i++)
	{
		circular[i * 2 + 0].x = cx + r * cos(i*radian);
		circular[i * 2 + 0].y = cy + r * sin(i*radian);
		circular[i * 2 + 0].z = 0;

		circular[i * 2 + 1].x = cx + r * cos((i + 1)*radian);
		circular[i * 2 + 1].y = cy + r * sin((i + 1)*radian);
		circular[i * 2 + 1].z = 0;
	}
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, sizeof(float3), &circular);
	glDrawArrays(GL_LINES, 0, 361 * 2);
	//glDrawArrays(GL_LINE_STRIP, 0, 361 * 2 -1);
	//glDrawArrays(GL_LINE_LOOP, 0, 361 * 2);  
	glDisableClientState(GL_VERTEX_ARRAY);

	// 优化1
	cx = 510;
	cy = 110;
	float3 circular2[361] = {};
	for (int i = 0; i < 361; i++)
	{
		circular2[i].x = cx + r * cos(i*radian);
		circular2[i].y = cy + r * sin(i*radian);
		circular2[i].z = 0;
	}
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, sizeof(float3), &circular2);
	glDrawArrays(GL_LINE_STRIP, 0, 361);
	glDisableClientState(GL_VERTEX_ARRAY);

	// 优化2
	cx = 490;
	cy = 90;
	float3 circular3[360] = {};
	for (int i = 0; i < 360; i++)
	{
		circular3[i].x = cx + r * cos(i*radian);
		circular3[i].y = cy + r * sin(i*radian);
		circular3[i].z = 0;
	}
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, sizeof(float3), &circular3);
	glDrawArrays(GL_LINE_LOOP, 0, 360);
	glDisableClientState(GL_VERTEX_ARRAY);
}
void Lesson01::DrawTriangle()
{
	// 启用抗锯齿 默认是关闭
	glEnable(GL_POLYGON_SMOOTH);
	// 指定抗锯齿算法
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);

	// 启动混合模式
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_POLYGON);
		glVertex3f(unWidth*0.5, 0, 0);
		glVertex3f(unWidth*0.5 - 50, 100, 0);
		glVertex3f(unWidth*0.5 + 50, 100, 0);
	glEnd();

	glDisable(GL_POLYGON_SMOOTH);
	glDisable(GL_BLEND);


	// 三角形带 四边形
	float3 circle2[4] = {
		{0.0f,   100.0f, 0.0f},
		{100.0f, 100.0f, 0.0f},
		{0.0f,   200.0f, 0.0f},
		{100.0f, 200.0f, 0.0f}
	};
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, sizeof(float3), circle2);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);  // 减少顶点传递量

	// 三角形颜色 四边形
	VertexC v[4] = {
		{0.0f,   200.0f, 0.0f,  1.0f, 0.0f, 0.0f},
		{100.0f, 200.0f, 0.0f,  0.0f, 1.0f, 0.0f},
		{0.0f,   300.0f, 0.0f,  0.0f, 0.0f, 1.0f},
		{100.0f, 300.0f, 0.0f,  0.0f, 1.0f, 1.0f},
	};
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glVertexPointer(3, GL_FLOAT, sizeof(VertexC), v);
	glColorPointer(3, GL_FLOAT, sizeof(VertexC), &v[0].r);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);

	// 圆
	glColor3f(1.0, 0, 0);
	float nCenterPointX = unWidth * 0.5f;
	float nCenterPointY = unHeight * 0.5f;
	float radius = 50.0f;
	const int step = 10;
	//glBegin glEnd 比较原始旧 效率低  1.0版本 高版本已抛弃
	glBegin(GL_TRIANGLES);
	for (int i = 0; i < 360; i += step)
	{
		float   rad = (double)i*(M_PI / 180.0f); // pi的精度要够
		float x = radius * cos(rad) + nCenterPointX;
		float y = radius * sin(rad) + nCenterPointY;

		rad = ((double)i + step)*(M_PI / 180.0f);
		float x1 = radius * cos(rad) + nCenterPointX;
		float y1 = radius * sin(rad) + nCenterPointY;

		glVertex3f(nCenterPointX, nCenterPointY, 0);
		glVertex3f(x, y, 0);
		glVertex3f(x1, y1, 0);
	}
	glEnd();

	//优化1
	glColor3f(0, 0, 1.0);
	Vertex circle[36 * 3] = {};
	nCenterPointX = unWidth * 0.5f + 100;
	nCenterPointY = unHeight * 0.5f;
	int nIndex = 0;
	for (int i = 0; i < 360; i += step)
	{
		circle[nIndex].x = nCenterPointX;
		circle[nIndex].y = nCenterPointY;
		circle[nIndex].z = 0;

		float   rad = (double)i*(M_PI / 180.0f);
		circle[nIndex + 1].x = radius * cos(rad) + nCenterPointX;
		circle[nIndex + 1].y = radius * sin(rad) + nCenterPointY;
		circle[nIndex + 1].z = 0;

		rad = ((double)i + step)*(M_PI / 180.0f);
		circle[nIndex + 2].x = radius * cos(rad) + nCenterPointX;
		circle[nIndex + 2].y = radius * sin(rad) + nCenterPointY;
		circle[nIndex + 2].z = 0;

		nIndex += 3;
	}
	// 启用顶点数组模式
	glEnableClientState(GL_VERTEX_ARRAY);
	//0为偏移  3*GL_FLOAT/sizeof(Vertex)
	glVertexPointer(3, GL_FLOAT, sizeof(Vertex)/*0*/, circle); // 顶点一批传递给显卡或openGL,减小压栈
	glDrawArrays(GL_TRIANGLES, 0, 36 * 3);     // 绘制

	//优化2
	glColor3f(0, 1.0, 1.0);
	Vertex circle1[38] = {};
	nCenterPointX = unWidth * 0.5f - 100;
	nCenterPointY = unHeight * 0.5f;
	nIndex = 0;
	circle1[nIndex].x = nCenterPointX;
	circle1[nIndex].y = nCenterPointY;
	circle1[nIndex].z = 0;
	for (int i = 0; i <= 360; i += step)
	{
		nIndex += 1;
		float   rad = (double)i*(M_PI / 180.0f);
		circle1[nIndex].x = radius * cos(rad) + nCenterPointX;
		circle1[nIndex].y = radius * sin(rad) + nCenterPointY;
		circle1[nIndex].z = 0;
	}
	// 启用顶点数组模式
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, sizeof(Vertex), circle1);
	//三角形扇
	glDrawArrays(GL_TRIANGLE_FAN, 0, 38);  // 减少顶点传递量 2/3
}

//贝塞尔曲线
void Lesson01::DrawBesselCurve()
{
	float3 vertex[500] = {};

	float3 p0 = { 100.0f, 300.0f, 0.0f };
	float3 p1 = { 150.0f, 450.0f, 0.0f };
	float3 p2 = { 200.0f, 300.0f, 0.0f };
	float3 p3 = { 250.0f, 400.0f, 0.0f };

	int index = 0;
	for (float t = 0; t < 1.0f; t += 0.01f, ++index)
	{
		vertex[index].x = ((1.0f - t) * (1.0f - t) * (1.0f - t) * p0.x) + (3.0f * p1.x * t * (1.0 - t) * (1.0 - t)) + (3.0 * p2.x * t * t * (1.0f - t)) + (p3.x * t * t * t);
		vertex[index].y = ((1.0f - t) * (1.0f - t) * (1.0f - t) * p0.y) + (3.0f * p1.y * t * (1.0 - t) * (1.0 - t)) + (3.0 * p2.y * t * t * (1.0f - t)) + (p3.y * t * t * t);
		vertex[index].z = 0;
	}

	glEnableClientState(GL_VERTEX_ARRAY);

	glVertexPointer(3, GL_FLOAT, sizeof(float3), &vertex);
	glDrawArrays(GL_LINE_STRIP, 0, index - 1);

	glDisableClientState(GL_VERTEX_ARRAY);
}

// 自由曲线
void Lesson01::DrawSpline()
{

}

void Lesson01::DrawImage()
{
	// 效率比较低 没有走显卡（没有把数据给显卡完成） 由cpu执行绘制。 高版本OpenGL或ES也没有这个函数
	glRasterPos2i(100, 100);
	glDrawPixels(100, 100, GL_RGBA, GL_UNSIGNED_BYTE, _Pixel);
}

// openGL扩展使用/获取
void Lesson01::OpenGLEx()
{
	// openGL windows已经不更新了
	// 返回的是当前显卡所支持的所有扩展
	char* ext = (char*)glGetString(GL_EXTENSIONS);
	// 以函数指针的形式使用扩展。//也可以直接使用扩展库 下载 glew http://glew.sourceforge.net/
	PFNGLARRAYELEMENTEXTPROC glArrayElementEXT = (PFNGLARRAYELEMENTEXTPROC)wglGetProcAddress("glArrayElementEXT");
	if (glArrayElementEXT)
	{
		glArrayElementEXT(1);
	}
}

void Lesson01::Render()
{
	// 指定以下操作针对投影矩阵
	glMatrixMode(GL_PROJECTION);
	// 将投影矩阵清空成单位矩阵
	glLoadIdentity();
	// 正交矩阵
	glOrtho(0, unWidth, unHeight, 0, -100, 100);

	DrawPoint();
	DrawLine();
	DrawTriangle();
	DrawBesselCurve();
	DrawImage();
	OpenGLEx();

	//===================================================
	//矩形 GL_QUADS GL_QUAD_STRIP    新版openGL已近没有  也是用三角形画
	//===================================================
	float3 rect[4] = {
		{400.0f, 300.0f, 0.0f},
		{500.0f, 300.0f, 0.0f},
		{500.0f, 400.0f, 0.0f},
		{400.0f, 400.0f, 0.0f}
	};
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, sizeof(float3), rect);
	glDrawArrays(GL_QUADS, 0, 4);  // 减少顶点传递量
	//===================================================

	//===================================================
	//GL_POLYGON 已过时 多变形是基于三角形扇 画凹多变形是无法绘制的 要用glu库（openGL的附属库 辅助提供一些基本图形功能和算法）
	//===================================================
}