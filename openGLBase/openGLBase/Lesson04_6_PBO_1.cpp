#include "Lesson04_6_PBO_1.h"
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
Vertex verCude7[] = {
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

unsigned char indexVer7[] = {
	0,1,2,3,
	4,5,6,7,
	4,0,5,1,
	6,2,7,3,
	4,0,6,2,
	5,1,7,3
};

void Lesson04_6_PBO_1::onDisable()
{
	// ɾ������(����)
	glDeleteTextures(2, &m_unTextureId);

	glDeleteBuffers(1, &m_unVbo);

	glDeleteBuffers(2, m_pbo);

	int err = glGetError(); //0û�д��� 
}

void Lesson04_6_PBO_1::initGL()
{
	//glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	m_unTextureId = createTextureFromImage("res/floor.bmp");
	m_unTextureId1 = createTextureFromImage("res/9010.jpg");
	createVbo();
	createIbo();

	DMA = 0;
	READ = 0;

	glGenBuffers(2, m_pbo);
	glBindBuffer(GL_PIXEL_PACK_BUFFER, m_pbo[0]); //pack���Կ���ȡ����
	glBufferData(GL_PIXEL_PACK_BUFFER, unWidth*unHeight*4, NULL, GL_STREAM_READ); //�����Կ��ڴ�ռ� GL_STREAM_READ���ڶ�ȡ

	glBindBuffer(GL_PIXEL_PACK_BUFFER, m_pbo[1]); //pack���Կ���ȡ����
	glBufferData(GL_PIXEL_PACK_BUFFER, unWidth*unHeight * 4, NULL, GL_STREAM_READ); //�����Կ��ڴ�ռ� GL_STREAM_READ���ڶ�ȡ

	glBindBuffer(GL_PIXEL_PACK_BUFFER, 0); // �ָ�״̬

	//�����С�����fboһ�������򴴽�ʧ�� ������
	m_dynamicTexId = createTexture(unWidth, unHeight, 0);
}

unsigned int Lesson04_6_PBO_1::createTexture(int w, int h, const void* data)
{
	unsigned texId;
	glGenTextures(1, &texId);
	glBindTexture(GL_TEXTURE_2D, texId);
	// �����˲�
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

	return texId;
}
unsigned int Lesson04_6_PBO_1::createTextureFromImage(const char* fileName)
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

void Lesson04_6_PBO_1::createVbo()
{
	// ����һ��buff
	glGenBuffers(1, &m_unVbo);
	// ��buff ����openGL m_unVbo�Ƕ��㻺����
	glBindBuffer(GL_ARRAY_BUFFER, m_unVbo);
	//glBufferData:�����Կ��ڴ� glBufferDataARB:arb����չ������arb����ֲ��ʱ�򷽱�һ�㣬��glES���Գɱ�׼��arbȥ����
	//����Դ治��������glBufferData���·����Դ棬���ԭ���Ļ�������ַɾ��
	glBufferData(GL_ARRAY_BUFFER, sizeof(verCude7), verCude7, GL_STATIC_DRAW);
	//glBufferSubData(); //��̬��������
	// �ָ�״̬����Ȼ��Ժ������Ʋ���Ӱ��
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Lesson04_6_PBO_1::createIbo()
{
	glGenBuffers(1, &m_unIbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_unIbo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexVer7), indexVer7, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Lesson04_6_PBO_1::drawCude(bool isRotate)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, double(unWidth) / double(unHeight), 0.1f, 100.0f);

	glEnable(GL_DEPTH_TEST);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0, 0, -2);


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

	glBindTexture(GL_TEXTURE_2D, m_unTextureId1);
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_ONE, GL_ONE);
	glDrawElements(GL_TRIANGLE_STRIP, 24, GL_UNSIGNED_BYTE, 0);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);



	//!!!!!! ��횹ر�buffer  ����Ļ����޷���ʾ,  pbo���洰���в�Ӱ
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Lesson04_6_PBO_1::drawImg()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, unWidth, unHeight, 0, -100, 1000);

	glDisable(GL_DEPTH_TEST);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0, 0, 0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0, 0, 0);

	Vertex rect[] =
	{
		{0,       unHeight, 0,   0, 1},
		{0,       0,        0,   0, 0},
		{unWidth, unHeight, 0,   1, 1},
		{unWidth, 0,        0,   1, 0},
	};

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glVertexPointer(3, GL_FLOAT, sizeof(Vertex), rect);
	glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), &rect[0].u);

	glBindTexture(GL_TEXTURE_2D, m_unTextureId);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

void Lesson04_6_PBO_1::render()
{
	drawImg();
	drawCude(true);


	//----------------------------------------------------------------
	//!!������DMA�Դ��������� ���ķ�CPU
	glBindBuffer(GL_PIXEL_PACK_BUFFER, m_pbo[DMA]); //pbo���Դ���ڴ����һ��
	//���Դ����ݶ���pbo��
	glReadPixels(0, 0, unWidth, unHeight, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	//----------------------------------------------------------------

	glBindBuffer(GL_PIXEL_PACK_BUFFER, m_pbo[READ]); 
	//������һ�������������е�����ӳ��Ϊ�ͻ����еĵ�ַ�ռ�  ���� �ȴ�dma���
	void* data = glMapBuffer(GL_PIXEL_PACK_BUFFER, GL_READ_WRITE);
	if (data)
	{
		saveImage(unWidth, unHeight, (char*)data);
	}
	//ȡ��ӳ��  ����
	glUnmapBuffer(GL_PIXEL_PACK_BUFFER);
	std::swap(DMA,READ);


	/*
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, m_pbo);
	// ������
	glBindTexture(GL_TEXTURE_2D, m_dynamicTexId);
	// ����������ж� GL_PIXEL_UNPACK_BUFFER ����ط���û�а�һ��������
	//   ����У��ʹ������������ȡ���ݣ�������data����ָ�����Ǹ��ڴ�
	// ǰ������ܼ򵥾Ͳ������ˣ����һ������������glReadPixelsͬ����NULL����
	// ����glTexSubImage2D�ͻ�����ǵĻ������ж�ȡ������
	// ����ΪʲôҪ��glTexSubImage2D�أ���Ϊ�����glTexImage2D��glTexImage2D�����������ڴ��������룬glTexSubImage2D�ͽ���ֻ�Ǹ��������е�����
	//  ���������ٶȣ������Ż����Դ��������
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, unWidth, unHeight, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.5, 0, -2);
	renderCube(false);

	// �����˰�GL_PIXEL_UNPACK_BUFFER����ط��Ļ����������������ĺ��������
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
	*/
}

bool Lesson04_6_PBO_1::saveImage(int w, int h, char* data)
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
	bool    bSuccess = FreeImage_Save(FIF_PNG, bitmap, "e:/xx.png", PNG_DEFAULT);
	FreeImage_Unload(bitmap);
	return  bSuccess;
}
