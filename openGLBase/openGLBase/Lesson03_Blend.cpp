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
	// ɾ������(����)
	glDeleteTextures(1, &m_unTexId0);
	glDeleteTextures(1, &m_unTexId1);
	glDeleteTextures(1, &m_unTexId2);
}

unsigned int Lesson03_Blend::createTextureFromImage(const char* fileName)
{
	// ��ȡͼƬ��ʽ
	FREE_IMAGE_FORMAT fifmt = FreeImage_GetFileType(fileName, 0);
	if (fifmt == FIF_UNKNOWN)
	{
		return 0;
	}
	// ����ͼƬ
	FIBITMAP* dib = FreeImage_Load(fifmt, fileName, 0);
	// ��ȡͼƬ��ɫ��ʽ
	FREE_IMAGE_COLOR_TYPE type = FreeImage_GetColorType(dib);
	// ��ȡ����ָ��
	FIBITMAP* temp = dib;
	// ת����32λͼƬ
	dib = FreeImage_ConvertTo32Bits(dib);
	// �ͷ�
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
	// �����˲�
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	// �����װ
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
	// ��������
	glEnable(GL_TEXTURE_2D);
	// ������Ȳ���
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

	glDisable(GL_DEPTH_TEST); // �������

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glVertexPointer(3, GL_FLOAT, sizeof(Vertex), mask);
	glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), &mask[0].u);

	// ָ�������ɫ
	//if (0) glColor4f(0, 1, 0, 1);
	//else glColor4f(1, 1, 1, 1);

	/*
	GL_ZERO��				��ʾʹ��0.0��Ϊ���ӣ�ʵ�����൱�ڲ�ʹ��������ɫ���������㡣
	GL_ONE��				��ʾʹ��1.0��Ϊ���ӣ�ʵ�����൱����ȫ��ʹ����������ɫ���������㡣
	GL_SRC_ALPHA��			��ʾʹ��Դ��ɫ��alphaֵ����Ϊ���ӡ�
	GL_DST_ALPHA��			��ʾʹ��Ŀ����ɫ��alphaֵ����Ϊ���ӡ�
	GL_ONE_MINUS_SRC_ALPHA����ʾ��1.0��ȥԴ��ɫ��alphaֵ����Ϊ���ӡ�
	GL_ONE_MINUS_DST_ALPHA����ʾ��1.0��ȥĿ����ɫ��alphaֵ����Ϊ���ӡ�
	
	GL_SRC_COLOR����Դ��ɫ���ĸ������ֱ���Ϊ���ӵ��ĸ���������
	GL_ONE_MINUS_SRC_COLOR��
	GL_DST_COLOR��
	GL_ONE_MINUS_DST_COLOR�ȣ�
	ǰ������OpenGL�ɰ汾��ֻ����������Ŀ�����ӣ���������OpenGL �ɰ汾��ֻ����������Դ���ӡ��°汾��OpenGL��û��������ƣ�
	����֧���µ�GL_CONST_COLOR���趨һ�ֳ�����ɫ�������ĸ������ֱ���Ϊ ���ӵ��ĸ���������

	GL_ONE_MINUS_CONST_COLOR��
	GL_CONST_ALPHA�� 
	GL_ONE_MINUS_CONST_ALPHA��
	GL_SRC_ALPHA_SATURATE��

	�°汾��OpenGL��������ɫ��alpha ֵ��RGBֵ���ò�ͬ�Ļ�����ӡ�����Щ������������������Ҫ�˽�ġ��Ͼ��⻹�����Ž̲ģ�����Ҫ����̫����~
	*/
	
	/*
	glBlendFunc(GL_ONE, GL_ZERO); ���ʾ��ȫʹ��Դ��ɫ����ȫ��ʹ��Ŀ����ɫ����˻���Ч���Ͳ�ʹ�û�ϵ�ʱ��һ�£���ȻЧ�ʿ��ܻ��һ��㣩��
	                              ���û������Դ���Ӻ�Ŀ�����ӣ���Ĭ������������������á�
	glBlendFunc(GL_ZERO, GL_ONE); ���ʾ��ȫ��ʹ��Դ��ɫ������������뻭ʲô����󶼲��ᱻ����ȥ�ˡ������Ⲣ����˵�������þ�û���ã���Щʱ�������������;��
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); ���ʾԴ��ɫ���������alpha ֵ��Ŀ����ɫ����1.0��ȥԴ��ɫ��alphaֵ������һ����
	                              Դ��ɫ��alphaֵԽ�������������ɫ��Դ��ɫ��ռ������Խ�󣬶�Ŀ����ɫ��ռ������� С����������£�
								  ���ǿ��Լ򵥵Ľ�Դ��ɫ��alphaֵ���Ϊ����͸���ȡ�����Ҳ�ǻ��ʱ��õķ�ʽ��
	glBlendFunc(GL_ONE, GL_ONE);  ���ʾ��ȫʹ��Դ��ɫ��Ŀ����ɫ�����յ���ɫʵ���Ͼ���������ɫ�ļ���ӡ�
	                              �����ɫ(1, 0, 0)����ɫ(0, 1, 0)��ӵõ�(1, 1, 0)�����Ϊ��ɫ��

	*/

	//ע�⣺ ��νԴ��ɫ��Ŀ����ɫ���Ǹ����Ƶ�˳���йصġ������Ȼ�����һ����ɫ�����壬�������ϻ�����ɫ�����塣����ɫ��Դ��ɫ����ɫ��Ŀ����ɫ��
	//glBlendFunc �ú����Ĺ����ǽ������ֵ����ɫ���������е�ֵ���

	//Դ����: GL_DST_COLOR��Ļ����  Ŀ������: GL_ZEROĿ����������Ҫ����
	//glBlendFunc(GL_ONE, GL_ZERO); //Ĭ��
	//glBlendFunc(GL_DST_COLOR, GL_ZERO);


	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0, 0, 0);
	
	glEnable(GL_BLEND);

	//glBlendFunc(GL_DST_COLOR, GL_ZERO);  //�൱�� Ŀ�� * Դ
	//glBindTexture(GL_TEXTURE_2D, m_unTexId2);
	//glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(50, 50, 0);

	glBlendFunc(GL_ONE, GL_ONE);  //�൱�� Ŀ�� + Դ
	glBindTexture(GL_TEXTURE_2D, m_unTexId1);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	
	

	// �뱳�����
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
	// ��alphaͨ�����
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

	glDisable(GL_DEPTH_TEST); // �������


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
	glBlendFunc(GL_DST_COLOR, GL_ZERO);   //�൱�� Ŀ�� * Դ
	glBindTexture(GL_TEXTURE_2D, m_unTexId6);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(20, 20, 0);

	glBlendFunc(GL_ONE, GL_ONE);         //�൱�� Ŀ�� + Դ
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
