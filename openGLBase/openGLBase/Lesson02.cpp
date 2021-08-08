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
	// ǰ
	{-0.5,  0.5,   0.5,  255, 0, 0},
	{-0.5,  -0.5,  0.5,  255, 0, 0},
	{0.5,   0.5,   0.5,  255, 0, 0},
	{0.5,   -0.5,  0.5,  255, 0, 0},
	// ��
	{-0.5,  0.5,   -0.5,  0, 255, 0},
	{-0.5,  -0.5,  -0.5,  0, 255, 0},
	{0.5,   0.5,   -0.5,  0, 255, 0},
	{0.5,   -0.5,  -0.5,  0, 255, 0},

	// ��
	{-0.5,  0.5,   -0.5,  0, 0, 255},
	{-0.5,  0.5,   0.5,  0, 0, 255},
	{-0.5,  -0.5,  -0.5,  0, 0, 255},
	{-0.5,  -0.5,  0.5,  0, 0, 255},
	// ��
	{0.5,  0.5,   -0.5,  0, 255, 255},
	{0.5,  0.5,   0.5,  0, 255, 255},
	{0.5,  -0.5,  -0.5,  0, 255, 255},
	{0.5,  -0.5,  0.5,  0, 255, 255},

	// ��
	{-0.5,  0.5,  -0.5,  255, 255, 0},
	{ 0.5,  0.5,  -0.5,  255, 255, 0},
	{-0.5,  0.5,  0.5,  255, 255, 0},
	{ 0.5,  0.5,  0.5,  255, 255, 0},
	// ��
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
		// ǰ
		{-0.5,  0.5,   -1.0},
		{-0.5,  -0.5,  -1.0},
		{0.5,   0.5,   -1.0},
		{0.5,   -0.5,  -1.0},
		// ��
		{-0.5,  0.5,   -2.0},
		{-0.5,  -0.5,  -2.0},
		{0.5,   0.5,   -2.0},
		{0.5,   -0.5,  -2.0},

		// ��
		{-0.5,  0.5,   -2.0},
		{-0.5,  0.5,   -1.0},
		{-0.5,  -0.5,  -2.0},
		{-0.5,  -0.5,  -1.0},
		// ��
		{0.5,  0.5,   -2.0},
		{0.5,  0.5,   -1.0},
		{0.5,  -0.5,  -2.0},
		{0.5,  -0.5,  -1.0},
		
		// ��
		{-0.5,  0.5,  -2.0},
		{ 0.5,  0.5,  -2.0},
		{-0.5,  0.5,  -1.0},
		{ 0.5,  0.5,  -1.0},
		// ��
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
	// ƽ��1
	/*
	for (int i = 0; i < sizeof(vertexs) / sizeof(Vertex); ++i)
	{
		vertexs[i].z -= 4.0f;
	}
	*/
	//=======================================================
	glMatrixMode(GL_MODELVIEW); // ָ����ǰ��������ģ�;���
	/*
	��������������Ȼ������Ĺ��ܣ�Ҳ���ǣ����ͨ���ȽϺ����ֵ�����仯�ˣ�����и�����Ȼ������Ĳ�������������OpenGL�Ϳ��Ը�����Z���ϵ����أ�
	��������ֻ�����Ǹ�����ǰ��û�ж���ʱ���Ż�滭������ء�
	*/
	glEnable(GL_DEPTH_TEST);

	/**
	���
	*/
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glVertexPointer(3, GL_FLOAT,         sizeof(Vertex), vertexs);
	glColorPointer(3,  GL_UNSIGNED_BYTE, sizeof(Vertex), &vertexs[0].r);

	angle = (++angle) % 360;

	
	glLoadIdentity();           // �ѵ�ǰ��������ģ�;�����ɵ�λ����
	glTranslatef(2, 0, -5);     // ����һ�����󣬻�͵�ǰʹ�õľ������˷�
	glRotatef(angle, 0, 1, 0);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, sizeof(vertexs)/sizeof(Vertex));

	glLoadIdentity();           
	glTranslatef(-2, 0, -5);     
	glRotatef(angle, 0, 0, 1);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, sizeof(vertexs) / sizeof(Vertex));
	
	// ����һ�����ž��󣬺͵�ǰʹ�õľ������˷�  ��ǰ����*ƽ��*����*��ת ����ǰ����*����*ƽ�� û��Ч��
	glLoadIdentity();
	glTranslatef(0, 0, -5);
	glScalef(0.6, 0.6, 0.6);
	glRotatef(angle, 1, 1, 1);

	//!!!!!!!!!!!!!!!!!�������ĵ� ��-5Ϊ�뾶��ת
	//glRotatef(angle, 1, 0, 0);
	//glTranslatef(0, 0, -5);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, sizeof(vertexs)/sizeof(Vertex));

	/**
	Ч�ʸ� �������
	*/
	// c3:3��color ub:unsiged char  3v:�������� f
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
	// ̫��
	glTranslatef(0, 0, -50);
	glRotatef(angle, 0, 1, 0);
	glScalef(2,2,2);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, sizeof(vertexs)/ sizeof(Vertex));

	//����
	glRotatef(angle, 0, 1, 0);
	glTranslatef(0, 0, -20);
	glScalef(0.7, 0.7, 0.7);
	glRotatef(angle, 0, 1, 0);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, sizeof(vertexs) / sizeof(Vertex));

	//����
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
	opengl32.lib��DLL)��MSΪopenGL�ܹ���window�����¹�����Ƶĺ����⣬�����Ĺ����ܼ򵥡������Ļ����ϴ���Ӳ�����ٵ�GL���������ø�����������������ķ���ʵ�֡�
	����gl.h�ļ���ʹ��������ĺ������뽫opengl32.lib������Ĺ��̡�
	
	glu������gl�����ϵ���չ����������˵����ʵ�����Ѿ���OpenGL��һ�����ˡ����ĺ���������glu��ͷ�ģ�������gl�����壩����ʹ�õ�gluPerspective���������ĺ�����
	Ҫʹ����Щ���������뽫glu32.lib���ӵ���Ĺ����С�
	*/
	gluPerspective(45.0, (double)unWidth/(double)unHeight, 0.1, 1000.0);

	//DrawTrianglse();
	//DrawCube();
	//DrawColorCube();
	DrawSolarSystem();
}