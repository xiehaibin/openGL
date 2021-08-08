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
	//���ÿ����
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
	glColor3f(0.0, 1.0, 0.0); // ȫ����ɫ

	//-------------------------------------------------
	// ���ÿ���� Ĭ���ǹر�
	glEnable(GL_LINE_SMOOTH);
	// ָ��������㷨
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

	// �������ģʽ
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// ������ʽ
	glEnable(GL_LINE_STIPPLE);
	glLineStipple(1, 0xFF);  // �ߵ���ʽ

	// ָ������ģʽ������
	glBegin(GL_LINES);
	glVertex3f(0, 0, 0);
	glVertex3f(100, 100, 0);
	glEnd(); // ��������

	glDisable(GL_LINE_SMOOTH);
	glDisable(GL_BLEND);
	glDisable(GL_LINE_STIPPLE);
	//--------------------------------------------------

	// ���߶�
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

	//glDrawArrays(GL_LINES, 0, 4);       //= ���߶�
	//glDrawArrays(GL_LINE_STRIP, 0, 4);  //Z �ߴ�
	glDrawArrays(GL_LINE_LOOP, 0, 4);     //X �ߴ���β����

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);

	//��Բ
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

	// �Ż�1
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

	// �Ż�2
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
	// ���ÿ���� Ĭ���ǹر�
	glEnable(GL_POLYGON_SMOOTH);
	// ָ��������㷨
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);

	// �������ģʽ
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


	// �����δ� �ı���
	float3 circle2[4] = {
		{0.0f,   100.0f, 0.0f},
		{100.0f, 100.0f, 0.0f},
		{0.0f,   200.0f, 0.0f},
		{100.0f, 200.0f, 0.0f}
	};
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, sizeof(float3), circle2);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);  // ���ٶ��㴫����

	// ��������ɫ �ı���
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

	// Բ
	glColor3f(1.0, 0, 0);
	float nCenterPointX = unWidth * 0.5f;
	float nCenterPointY = unHeight * 0.5f;
	float radius = 50.0f;
	const int step = 10;
	//glBegin glEnd �Ƚ�ԭʼ�� Ч�ʵ�  1.0�汾 �߰汾������
	glBegin(GL_TRIANGLES);
	for (int i = 0; i < 360; i += step)
	{
		float   rad = (double)i*(M_PI / 180.0f); // pi�ľ���Ҫ��
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

	//�Ż�1
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
	// ���ö�������ģʽ
	glEnableClientState(GL_VERTEX_ARRAY);
	//0Ϊƫ��  3*GL_FLOAT/sizeof(Vertex)
	glVertexPointer(3, GL_FLOAT, sizeof(Vertex)/*0*/, circle); // ����һ�����ݸ��Կ���openGL,��Сѹջ
	glDrawArrays(GL_TRIANGLES, 0, 36 * 3);     // ����

	//�Ż�2
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
	// ���ö�������ģʽ
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, sizeof(Vertex), circle1);
	//��������
	glDrawArrays(GL_TRIANGLE_FAN, 0, 38);  // ���ٶ��㴫���� 2/3
}

//����������
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

// ��������
void Lesson01::DrawSpline()
{

}

void Lesson01::DrawImage()
{
	// Ч�ʱȽϵ� û�����Կ���û�а����ݸ��Կ���ɣ� ��cpuִ�л��ơ� �߰汾OpenGL��ESҲû���������
	glRasterPos2i(100, 100);
	glDrawPixels(100, 100, GL_RGBA, GL_UNSIGNED_BYTE, _Pixel);
}

// openGL��չʹ��/��ȡ
void Lesson01::OpenGLEx()
{
	// openGL windows�Ѿ���������
	// ���ص��ǵ�ǰ�Կ���֧�ֵ�������չ
	char* ext = (char*)glGetString(GL_EXTENSIONS);
	// �Ժ���ָ�����ʽʹ����չ��//Ҳ����ֱ��ʹ����չ�� ���� glew http://glew.sourceforge.net/
	PFNGLARRAYELEMENTEXTPROC glArrayElementEXT = (PFNGLARRAYELEMENTEXTPROC)wglGetProcAddress("glArrayElementEXT");
	if (glArrayElementEXT)
	{
		glArrayElementEXT(1);
	}
}

void Lesson01::Render()
{
	// ָ�����²������ͶӰ����
	glMatrixMode(GL_PROJECTION);
	// ��ͶӰ������ճɵ�λ����
	glLoadIdentity();
	// ��������
	glOrtho(0, unWidth, unHeight, 0, -100, 100);

	DrawPoint();
	DrawLine();
	DrawTriangle();
	DrawBesselCurve();
	DrawImage();
	OpenGLEx();

	//===================================================
	//���� GL_QUADS GL_QUAD_STRIP    �°�openGL�ѽ�û��  Ҳ���������λ�
	//===================================================
	float3 rect[4] = {
		{400.0f, 300.0f, 0.0f},
		{500.0f, 300.0f, 0.0f},
		{500.0f, 400.0f, 0.0f},
		{400.0f, 400.0f, 0.0f}
	};
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, sizeof(float3), rect);
	glDrawArrays(GL_QUADS, 0, 4);  // ���ٶ��㴫����
	//===================================================

	//===================================================
	//GL_POLYGON �ѹ�ʱ ������ǻ����������� ������������޷����Ƶ� Ҫ��glu�⣨openGL�ĸ����� �����ṩһЩ����ͼ�ι��ܺ��㷨��
	//===================================================
}