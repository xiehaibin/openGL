#include "Lesson02.h"
#include "GLContext.h"

extern unsigned int unWidth;
extern unsigned int unHeight;

struct float3 
{
	float x;
	float y;
	float z;
};

struct Vertex
{
	float x;
	float y;
	float z;

	unsigned char r;
	unsigned char g;
	unsigned char b;
};

Vertex vertexs[] = {
	// 前
	{-0.5,  0.5,   0.5,  255, 0, 0},
	{-0.5,  -0.5,  0.5,  255, 0, 0},
	{0.5,   0.5,   0.5,  255, 0, 0},
	{0.5,   -0.5,  0.5,  255, 0, 0},
	// 后
	{-0.5,  0.5,   -0.5,  0, 255, 0},
	{-0.5,  -0.5,  -0.5,  0, 255, 0},
	{0.5,   0.5,   -0.5,  0, 255, 0},
	{0.5,   -0.5,  -0.5,  0, 255, 0},

	// 左
	{-0.5,  0.5,   -0.5,  0, 0, 255},
	{-0.5,  0.5,   0.5,  0, 0, 255},
	{-0.5,  -0.5,  -0.5,  0, 0, 255},
	{-0.5,  -0.5,  0.5,  0, 0, 255},
	// 右
	{0.5,  0.5,   -0.5,  0, 255, 255},
	{0.5,  0.5,   0.5,  0, 255, 255},
	{0.5,  -0.5,  -0.5,  0, 255, 255},
	{0.5,  -0.5,  0.5,  0, 255, 255},

	// 上
	{-0.5,  0.5,  -0.5,  255, 255, 0},
	{ 0.5,  0.5,  -0.5,  255, 255, 0},
	{-0.5,  0.5,  0.5,  255, 255, 0},
	{ 0.5,  0.5,  0.5,  255, 255, 0},
	// 下
	{-0.5,  -0.5,  -0.5,  255, 0, 255},
	{ 0.5,  -0.5,  -0.5,  255, 0, 255},
	{-0.5,  -0.5,  0.5,  255, 0, 255},
	{ 0.5,  -0.5,  0.5,  255, 0, 255},
};

Lesson02::Lesson02()
{
	angle = 0;
	angle2 = 0;
}

void Lesson02::DrawTrianglse()
{
	float z = -1;
	float3 vert[3] = {
		{0,    0.1,  z},
		{-0.1, -0.1, z},
		{0.1,  -0.1, z},
	};
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, sizeof(float3), vert);

	glDrawArrays(GL_TRIANGLES, 0, 3);

	glDisableClientState(GL_VERTEX_ARRAY);
}
void Lesson02::DrawCube()
{
	float3 vertexs[] = {
		// 前
		{-0.5,  0.5,   -1.0},
		{-0.5,  -0.5,  -1.0},
		{0.5,   0.5,   -1.0},
		{0.5,   -0.5,  -1.0},
		// 后
		{-0.5,  0.5,   -2.0},
		{-0.5,  -0.5,  -2.0},
		{0.5,   0.5,   -2.0},
		{0.5,   -0.5,  -2.0},

		// 左
		{-0.5,  0.5,   -2.0},
		{-0.5,  0.5,   -1.0},
		{-0.5,  -0.5,  -2.0},
		{-0.5,  -0.5,  -1.0},
		// 右
		{0.5,  0.5,   -2.0},
		{0.5,  0.5,   -1.0},
		{0.5,  -0.5,  -2.0},
		{0.5,  -0.5,  -1.0},
		
		// 上
		{-0.5,  0.5,  -2.0},
		{ 0.5,  0.5,  -2.0},
		{-0.5,  0.5,  -1.0},
		{ 0.5,  0.5,  -1.0},
		// 下
		{-0.5,  -0.5,  -2.0},
		{ 0.5,  -0.5,  -2.0},
		{-0.5,  -0.5,  -1.0},
		{ 0.5,  -0.5,  -1.0},
	};

	for (int i = 0; i < sizeof(vertexs)/sizeof(float3); ++i)
	{
		vertexs[i].z -= 2.0f;
	}

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, sizeof(float3), vertexs);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, sizeof(vertexs) / sizeof(float3));

	glDisableClientState(GL_VERTEX_ARRAY);
}

void Lesson02::DrawColorCube()
{
	//=======================================================
	// 平移1
	/*
	for (int i = 0; i < sizeof(vertexs) / sizeof(Vertex); ++i)
	{
		vertexs[i].z -= 4.0f;
	}
	*/
	//=======================================================
	glMatrixMode(GL_MODELVIEW); // 指明当前操作的是模型矩阵
	/*
	用来开启更新深度缓冲区的功能，也就是，如果通过比较后深度值发生变化了，会进行更新深度缓冲区的操作。启动它，OpenGL就可以跟踪再Z轴上的像素，
	这样，它只会再那个像素前方没有东西时，才会绘画这个像素。
	*/
	glEnable(GL_DEPTH_TEST);

	/**
	灵活
	*/
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glVertexPointer(3, GL_FLOAT,         sizeof(Vertex), vertexs);
	glColorPointer(3,  GL_UNSIGNED_BYTE, sizeof(Vertex), &vertexs[0].r);

	angle = (++angle) % 360;

	
	glLoadIdentity();           // 把当前操作的是模型矩阵，清成单位矩阵
	glTranslatef(2, 0, -5);     // 产生一个矩阵，会和当前使用的矩阵做乘法
	glRotatef(angle, 0, 1, 0);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, sizeof(vertexs)/sizeof(Vertex));

	glLoadIdentity();           
	glTranslatef(-2, 0, -5);     
	glRotatef(angle, 0, 0, 1);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, sizeof(vertexs) / sizeof(Vertex));
	
	// 产生一个缩放矩阵，和当前使用的矩阵做乘法  当前矩阵*平移*缩放*旋转 ，当前矩阵*缩放*平移 没有效果
	glLoadIdentity();
	glTranslatef(0, 0, -5);
	glScalef(0.6, 0.6, 0.6);
	glRotatef(angle, 1, 1, 1);

	//!!!!!!!!!!!!!!!!!沿着中心点 以-5为半径旋转
	//glRotatef(angle, 1, 0, 0);
	//glTranslatef(0, 0, -5);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, sizeof(vertexs)/sizeof(Vertex));

	/**
	效率高 但不灵活
	*/
	// c3:3个color ub:unsiged char  3v:三个顶点 f
	//glInterleavedArrays(GL_C3F_V3F, 0, vertexs);
	//glDrawArrays(GL_TRIANGLE_STRIP, 0, sizeof(vertexs) / sizeof(Vertex));
}

void Lesson02::DrawSolarSystem()
{
	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_MODELVIEW);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	glVertexPointer(3, GL_FLOAT, sizeof(Vertex), vertexs);
	glColorPointer(3, GL_UNSIGNED_BYTE, sizeof(Vertex), &vertexs[0].r);

	glLoadIdentity();
	// 太阳
	glTranslatef(0, 0, -50);
	glRotatef(angle, 0, 1, 0);
	glScalef(2,2,2);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, sizeof(vertexs)/ sizeof(Vertex));

	//地球
	glRotatef(angle, 0, 1, 0);
	glTranslatef(0, 0, -20);
	glScalef(0.7, 0.7, 0.7);
	glRotatef(angle, 0, 1, 0);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, sizeof(vertexs) / sizeof(Vertex));

	//月球
	glRotatef(angle2, 0, 1, 0);
	glTranslatef(0, 0, -5);
	glScalef(0.5, 0.5, 0.5);
	glRotatef(angle, 0, 1, 0);
	
	glDrawArrays(GL_TRIANGLE_STRIP, 0, sizeof(vertexs) / sizeof(Vertex));

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);

	angle = (angle += 1) % 360;
	angle2 = (angle2 += 10) % 360;
}

void Lesson02::Render()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	/*
	opengl32.lib（DLL)是MS为openGL能够在window环境下工作设计的函数库，这个库的工作很简单。如果你的机器上存在硬件加速的GL驱动，调用该驱动，否则用软件的方法实现。
	包含gl.h文件并使用了里面的函数必须将opengl32.lib加入你的工程。
	
	glu则是在gl基础上的扩展，如上面所说，他实际上已经是OpenGL的一部分了。他的函数都是以glu开头的（区别于gl函数族），你使用的gluPerspective就是这样的函数。
	要使用这些函数，必须将glu32.lib链接到你的工程中。
	*/
	gluPerspective(45.0, (double)unWidth/(double)unHeight, 0.1, 1000.0);

	//DrawTrianglse();
	//DrawCube();
	//DrawColorCube();
	DrawSolarSystem();
}