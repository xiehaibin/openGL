#include "Lesson03.h"
#include <stdlib.h>

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

Vertex vertex[4] = {
	{-0.5, 0.5,  0.0,  0.0, 3.0},
	{-0.5, -0.5, 0.0,  0.0, 0.0},
	{0.5,  0.5,  0.0,  3.0, 3.0},
	{0.5, -0.5,  0.0,  3.0, 0.0},
};

Vertex verCude[] = {
	// ǰ
	{-0.5,  0.5,   0.5,  0.0, 1.0},
	{-0.5,  -0.5,  0.5,  0.0, 0.0},
	{0.5,   0.5,   0.5,  1.0, 1.0},
	{0.5,   -0.5,  0.5,  1.0, 0.0},
	// ��
	{-0.5,  0.5,   -0.5,  1.0, 0.0},
	{-0.5,  -0.5,  -0.5,  0.0, 0.0},
	{0.5,   0.5,   -0.5,  1.0, 1.0},
	{0.5,   -0.5,  -0.5,  1.0, 0.0},

	// ��
	{-0.5,  0.5,   -0.5,  1.0, 0.0},
	{-0.5,  0.5,   0.5,   0.0, 0.0},
	{-0.5,  -0.5,  -0.5,  1.0, 1.0},
	{-0.5,  -0.5,  0.5,   1.0, 0.0},
	// ��
	{0.5,  0.5,   -0.5, 1.0, 0.0},
	{0.5,  0.5,   0.5,  0.0, 0.0},
	{0.5,  -0.5,  -0.5, 1.0, 1.0},
	{0.5,  -0.5,  0.5,  1.0, 0.0},

	// ��
	{-0.5,  0.5,  -0.5,  1.0, 0.0},
	{ 0.5,  0.5,  -0.5,  0.0, 0.0},
	{-0.5,  0.5,  0.5,   1.0, 1.0},
	{ 0.5,  0.5,  0.5,   1.0, 0.0},
	// ��
	{-0.5,  -0.5,  -0.5,  1.0, 0.0},
	{ 0.5,  -0.5,  -0.5,  0.0, 0.0},
	{-0.5,  -0.5,  0.5,   1.0, 1.0},
	{ 0.5,  -0.5,  0.5,   1.0, 0.0},
};

/*
��������
0,1  1,1
0,0  1,0
*/
Lesson03::Lesson03()
{

}
Lesson03::~Lesson03()
{
}

void Lesson03::onDisable()
{
	// ɾ������(����)
	glDeleteTextures(1, &m_unTexId1);
	glDeleteTextures(1, &m_unTexId2);
	glDeleteTextures(1, &m_unTextureId);

	int err = glGetError(); //0û�д��� 
}

void Lesson03::initGL()
{
	// ��������
	glEnable(GL_TEXTURE_2D);
	// ������Ȳ���
	glEnable(GL_DEPTH_TEST);

	//init();

	m_unTexId1 = createTextureFromImage("res/1009.png");
	m_unTexId2 = createTextureFromImage("res/9010.jpg");
}

void Lesson03::init()
{
	const int len = 128 * 128 * 4;
	unsigned char* data = new unsigned char[len];
	for (int i = 0; i < len; ++i)
	{
		data[i] = rand() % 255;
	}

	// ����������Ҫ����������������������û�з���������ڴ�
	// 1:�������
	// 2���洢���������ĵ�һ��Ԫ��ָ��
	glGenTextures(1, &m_unTextureId);

	glBindTexture(GL_TEXTURE_2D, m_unTextureId);
	//OpenGL����������˺���glTexParameteri()
	//ͼ�������ͼ��ռ�ӳ�䵽֡����ͼ��ռ�(ӳ����Ҫ���¹�������ͼ��, �����ͻ����Ӧ�õ�������ϵ�ͼ��ʧ��), 
	//��ʱ�Ϳ���glTexParmeteri()������ȷ����ΰ���������ӳ�������.
	glTexParameteri(GL_TEXTURE_2D, 
		GL_TEXTURE_MAG_FILTER,     // �Ŵ����
		GL_LINEAR);                // ���Թ���, ʹ�þ��뵱ǰ��Ⱦ�������������4�����ؼ�Ȩƽ��ֵ.
	glTexParameteri(GL_TEXTURE_2D, 
		GL_TEXTURE_MIN_FILTER,     // ��С����
		GL_LINEAR);  

	// ����һ��2D����
	glTexImage2D(
		GL_TEXTURE_2D, 
		0,
		GL_RGBA,          //�ڲ���ʽ
		/*
		������������߲���2^n
		�����Կ���֧�ֲ��������������Կ���֧��
		*/
		128,              //����ͼ��Ŀ�� 
		128,
		0,                //ָ���߿�Ŀ��
		GL_RGBA,          //�������ݵ���ɫ��ʽ, �ⲿ��ʽ
		GL_UNSIGNED_BYTE, //�������ݵ���������
		data              //ָ���ڴ���ָ��ͼ�����ݵ�ָ��
		);
}

unsigned int Lesson03::createTexture(int w, int h, const void* data)
{
	unsigned texId;
	glGenTextures(1, &texId);
	glBindTexture(GL_TEXTURE_2D, texId);
	// �����˲�
	/*
	GL_LINEAR:  ���Թ��� Ч���� ���ܲ�
	GL_NEAREST: �ڽ����� Ч���� ���ܺ�
	*/
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	// �����װ
	/*
	GL_REPEAT:          �ظ�
	GL_CLAMP��          ��Ե����
	GL_MIRRORED_REPEAT: ����
	*/
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	/*
	mipMap:
	��������һ������ �����ɶ���ͼ��ɣ�0��ʾ����
	һ��ͼƬ�Ŵ���С�Կ�Ҫ����������һ���µ�ͼƬ��������̱ȽϺ�ʱ
	������������ǽ���ʱ���ô�����������Զ��ʱ���ý�С������ ��һ����������һ��������1/2
	mipMap�����߱������ĵı���
	*/
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

	return texId;
}
unsigned int Lesson03::createTextureFromImage(const char* fileName)
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

void Lesson03::drawTexture2()
{
	unsigned int texId = createTextureFromImage("res/1008.jpg");

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glVertexPointer(3, GL_FLOAT, sizeof(Vertex), vertex);
	glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), &vertex[0].u);

	glBindTexture(GL_TEXTURE_2D, texId);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0, 0, -5);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

void Lesson03::drawCudeTex()
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glVertexPointer(3, GL_FLOAT, sizeof(Vertex), verCude);
	glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), &verCude[0].u);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(1, 0, -5);
	glRotated(m_fAngle, 1, 1, 1);
	glBindTexture(GL_TEXTURE_2D, m_unTexId1);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 24);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(-1, 0, -5);
	glRotated(m_fAngle, 1, 1, 1);
	glBindTexture(GL_TEXTURE_2D, m_unTexId2);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 24);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	m_fAngle = (m_fAngle += 0.1);
}

void Lesson03::drawCudeTex2()
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glVertexPointer(3, GL_FLOAT, sizeof(Vertex), verCude);
	glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), &verCude[0].u);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0, 0, m_fAngle);
	//glRotated(m_fAngle, 1, 1, 1);
	for (int i = 0; i < 6; i++)
	{
		if (i%2 == 0) glBindTexture(GL_TEXTURE_2D, m_unTexId1);
		else glBindTexture(GL_TEXTURE_2D, m_unTexId2);
		glDrawArrays(GL_TRIANGLE_STRIP, i*4, 4);
	}
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	m_fAngle -= 0.05;
}

void Lesson03::render()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, (double)unWidth/(double)unHeight, 0.1f, 1000.0f);

	//drawTexture();
	//drawTexture2();
	//drawCudeTex();
	drawCudeTex2();
}

void Lesson03::drawTexture()
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glVertexPointer(3, GL_FLOAT, sizeof(Vertex), vertex);
	glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), &vertex[0].u);

	glBindTexture(GL_TEXTURE_2D, m_unTextureId);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0, 0, -10);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}
