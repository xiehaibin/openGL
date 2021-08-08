#include "Lesson04_6_12.h"
#include <iostream>

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
Vertex verCude12[] = {
	/*
	ǰ
	0------2
	|      |
	|      |
	1------3
	*/
	{-0.5,  0.5,   0.5,  0.0, 1.0},
	{-0.5,  -0.5,  0.5,  0.0, 0.0},
	{0.5,   0.5,   0.5,  1.0, 1.0},
	{0.5,   -0.5,  0.5,  1.0, 0.0},
	/*
	��
	4------6
	|      |
	|      |
	5------7
	*/
	{-0.5,  0.5,   -0.5,  0.0, 1.0},
	{-0.5,  -0.5,  -0.5,  0.0, 0.0},
	{0.5,   0.5,   -0.5,  1.0, 1.0},
	{0.5,   -0.5,  -0.5,  1.0, 0.0},
};

unsigned char indexVer12[] = {
	0,1,2,3,
	4,5,6,7,
	4,0,5,1,
	6,2,7,3,
	4,0,6,2,
	5,1,7,3
};

void Lesson04_6_12::onDisable()
{
	// ɾ������(����)
	glDeleteTextures(2, &m_unTextureId);

	glDeleteBuffers(1, &m_unVbo);

	int err = glGetError(); //0û�д��� 
}

void Lesson04_6_12::initGL()
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	m_unTextureId = createTextureFromImage("res/floor.bmp");
	createVbo();
	createIbo();

	//�����С�����fboһ�������򴴽�ʧ�� ������
	m_dynamicTexId = createTexture(unWidth, unHeight, 0);
}

unsigned int Lesson04_6_12::createTexture(int w, int h, const void* data)
{
	unsigned texId;
	glGenTextures(1, &texId);
	glBindTexture(GL_TEXTURE_2D, texId);
	// �����˲�
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

	return texId;
}
unsigned int Lesson04_6_12::createTextureFromImage(const char* fileName)
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

void Lesson04_6_12::createVbo()
{
	// ����һ��buff
	glGenBuffers(1, &m_unVbo);
	// ��buff ����openGL m_unVbo�Ƕ��㻺����
	glBindBuffer(GL_ARRAY_BUFFER, m_unVbo);
	//glBufferData:�����Կ��ڴ� glBufferDataARB:arb����չ������arb����ֲ��ʱ�򷽱�һ�㣬��glES���Գɱ�׼��arbȥ����
	//����Դ治��������glBufferData���·����Դ棬���ԭ���Ļ�������ַɾ��
	glBufferData(GL_ARRAY_BUFFER, sizeof(verCude12), verCude12, GL_STATIC_DRAW);
	//glBufferSubData(); //��̬��������
	// �ָ�״̬����Ȼ��Ժ������Ʋ���Ӱ��
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Lesson04_6_12::createIbo()
{
	glGenBuffers(1, &m_unIbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_unIbo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexVer12), indexVer12, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Lesson04_6_12::drawCude(bool isRotate, float offsetX)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, double(unWidth) / double(unHeight), 0.1f, 1000.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(offsetX, 0, -3);
	static float angle = 0.0f;
	if (isRotate)
	{
		glRotated(angle, 0, 1, 0);
		angle = (angle += 0.2);
	}

	glBindBuffer(GL_ARRAY_BUFFER, m_unVbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_unIbo);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glVertexPointer(3, GL_FLOAT, sizeof(Vertex), 0);
	glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), (void*)12);

	glDrawElements(GL_TRIANGLE_STRIP, 24, GL_UNSIGNED_BYTE, 0);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

void Lesson04_6_12::render()
{
	glBindTexture(GL_TEXTURE_2D, m_unTextureId);
	drawCude(true, -1);

#if 0
	//��Ч�汾
	char* data = new char[unWidth * unHeight * 4];
	glReadPixels(0, 0, unWidth, unHeight, GL_RGBA, GL_UNSIGNED_BYTE, (void*)data);
	glBindTexture(GL_TEXTURE_2D, m_dynamicTexId);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, unWidth, unHeight, GL_RGBA, GL_UNSIGNED_BYTE, (void*)data);
	drawCude(false, 0);
	saveImage(unWidth, unHeight, data);
	delete[] data;
#endif

	//��Ч 
	glBindTexture(GL_TEXTURE_2D, m_dynamicTexId);
	glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, unWidth, unHeight);
	drawCude(false, 0);
}

bool Lesson04_6_12::saveImage(int w, int h, char* data)
{
	FIBITMAP*   bitmap = FreeImage_Allocate(w, h, 32, 0, 0, 0);
	BYTE*       pixels = (BYTE*)FreeImage_GetBits(bitmap);

	for (int i = 0; i < w*h * 4; i += 4)
	{
		BYTE temp = data[i];
		data[i] = data[i + 2];
		data[i + 2] = temp;
	}
	memcpy(pixels, data, w * h * 4);
	bool    bSuccess = FreeImage_Save(FIF_PNG, bitmap, "e:/12.png", PNG_DEFAULT);
	FreeImage_Unload(bitmap);
	return  bSuccess;
}
