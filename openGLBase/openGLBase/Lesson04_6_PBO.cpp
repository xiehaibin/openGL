#include "Lesson04_6_PBO.h"

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
Vertex verCude6[] = {
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

unsigned char indexVer6[] = {
	0,1,2,3,
	4,5,6,7,
	4,0,5,1,
	6,2,7,3,
	4,0,6,2,
	5,1,7,3
};

struct Vertex33
{
	float x, y, z;
	float u, v;
	float r, g, b;
};
void Lesson04_6_PBO::onDisable()
{
	// ɾ������(����)
	glDeleteTextures(2, &m_unTextureId);

	glDeleteBuffers(1, &m_unVbo);

	glDeleteBuffers(1, &m_pbo);

	int err = glGetError(); //0û�д��� 
}

void Lesson04_6_PBO::initGL()
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	m_unTextureId = createTextureFromImage("res/1009.png");
	createVbo();
	createIbo();

	glGenBuffers(1, &m_pbo);
	glBindBuffer(GL_PIXEL_PACK_BUFFER, m_pbo); //pack���Կ���ȡ����
	glBufferData(GL_PIXEL_PACK_BUFFER, unWidth*unHeight*4, NULL, GL_STREAM_READ); //�����Կ��ڴ�ռ� GL_STREAM_READ���ڶ�ȡ
	glBindBuffer(GL_PIXEL_PACK_BUFFER, 0); // �ָ�״̬

	//�����С�����fboһ�������򴴽�ʧ�� ������
	m_dynamicTexId = createTexture(unWidth, unHeight, 0);
}

unsigned int Lesson04_6_PBO::createTexture(int w, int h, const void* data)
{
	unsigned texId;
	glGenTextures(1, &texId);
	glBindTexture(GL_TEXTURE_2D, texId);
	// �����˲�
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	// �����װ
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

	return texId;
}
unsigned int Lesson04_6_PBO::createTextureFromImage(const char* fileName)
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

void Lesson04_6_PBO::createVbo()
{
	// ����һ��buff
	glGenBuffers(1, &m_unVbo);
	// ��buff ����openGL m_unVbo�Ƕ��㻺����
	glBindBuffer(GL_ARRAY_BUFFER, m_unVbo);
	//glBufferData:�����Կ��ڴ� glBufferDataARB:arb����չ������arb����ֲ��ʱ�򷽱�һ�㣬��glES���Գɱ�׼��arbȥ����
	//����Դ治��������glBufferData���·����Դ棬���ԭ���Ļ�������ַɾ��
	glBufferData(GL_ARRAY_BUFFER, sizeof(verCude6), verCude6, GL_STATIC_DRAW);
	//glBufferSubData(); //��̬��������
	// �ָ�״̬����Ȼ��Ժ������Ʋ���Ӱ��
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Lesson04_6_PBO::createIbo()
{
	glGenBuffers(1, &m_unIbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_unIbo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexVer6), indexVer6, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Lesson04_6_PBO::drawCude(bool isRotate)
{
	//glMatrixMode(GL_PROJECTION);
	//glLoadIdentity();
	//gluPerspective(45.0f, double(unWidth) / double(unHeight), 0.1f, 1000.0f);

	//glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();
	//glTranslatef(0, 0, -2);

	static float angle = 60.0f;
	if (isRotate)
	{
		glRotated(angle, 0, 1, 0);
		angle = (angle += 0.2);
	}

	glBindBuffer(GL_ARRAY_BUFFER, m_unVbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_unIbo);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	// ���Ժ����glVertexPointer��glTexCoordPointer���Դ���ȡ�����ᵽ�ڴ���ȡ
	glVertexPointer(3, GL_FLOAT, sizeof(Vertex), 0);
	glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), (void*)12);

	//glBindTexture(GL_TEXTURE_2D, m_unTextureId);

	//glDrawArrays(GL_TRIANGLE_STRIP, 0, 24);
	//glDrawElements(GL_TRIANGLE_STRIP, 24, GL_UNSIGNED_BYTE, indexVer);
	glDrawElements(GL_TRIANGLE_STRIP, 24, GL_UNSIGNED_BYTE, 0);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}



void Lesson04_6_PBO::renderCube(bool rot)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, double(unWidth) / double(unHeight), 0.1f, 100.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.0f, 0.0f, -5);

	static  float   angle = 0;
	if (rot)
	{
		glRotated(angle, 1, 1, 1);
		angle += 1;
	}

	glBindTexture(GL_TEXTURE_2D, m_unTextureId);

	Vertex33 g_cubeVertices[] =
	{
		{ -1.0f,-1.0f, 1.0f,0.0f, 0.0f,1.0f, 0.0f, 0.0f },
		{ 1.0f,-1.0f, 1.0f,1.0f, 0.0f,1.0f, 0.0f, 0.0f },
		{ 1.0f, 1.0f, 1.0f,1.0f, 1.0f,1.0f, 0.0f, 0.0f },
		{ -1.0f, 1.0f, 1.0f,0.0f, 1.0f,1.0f, 0.0f, 0.0f },

		{ -1.0f,-1.0f,-1.0f,1.0f, 0.0f,0.0f, 1.0f, 0.0f },
		{ -1.0f, 1.0f,-1.0f,1.0f, 1.0f,0.0f, 1.0f, 0.0f },
		{ 1.0f, 1.0f,-1.0f,0.0f, 1.0f,0.0f, 1.0f, 0.0f },
		{ 1.0f,-1.0f,-1.0f,0.0f, 0.0f,0.0f, 1.0f, 0.0f },

		{ -1.0f, 1.0f,-1.0f,0.0f, 1.0f,0.0f, 0.0f, 1.0f },
		{ -1.0f, 1.0f, 1.0f,0.0f, 0.0f,0.0f, 0.0f, 1.0f },
		{ 1.0f, 1.0f, 1.0f,1.0f, 0.0f,0.0f, 0.0f, 1.0f },
		{ 1.0f, 1.0f,-1.0f,1.0f, 1.0f,0.0f, 0.0f, 1.0f },

		{ -1.0f,-1.0f,-1.0f,1.0f, 1.0f,1.0f, 1.0f, 0.0f },
		{ 1.0f,-1.0f,-1.0f,0.0f, 1.0f,1.0f, 1.0f, 0.0f },
		{ 1.0f,-1.0f, 1.0f,0.0f, 0.0f,1.0f, 1.0f, 0.0f },
		{ -1.0f,-1.0f, 1.0f,1.0f, 0.0f,1.0f, 1.0f, 0.0f },

		{ 1.0f,-1.0f,-1.0f,1.0f, 0.0f,1.0f, 0.0f, 1.0f },
		{ 1.0f, 1.0f,-1.0f,1.0f, 1.0f,1.0f, 0.0f, 1.0f },
		{ 1.0f, 1.0f, 1.0f,0.0f, 1.0f,1.0f, 0.0f, 1.0f },
		{ 1.0f,-1.0f, 1.0f,0.0f, 0.0f,1.0f, 0.0f, 1.0f },

		{ -1.0f,-1.0f,-1.0f,0.0f, 0.0f,0.0f, 1.0f, 1.0f },
		{ -1.0f,-1.0f, 1.0f,1.0f, 0.0f,0.0f, 1.0f, 1.0f },
		{ -1.0f, 1.0f, 1.0f,1.0f, 1.0f,0.0f, 1.0f, 1.0f },
		{ -1.0f, 1.0f,-1.0f,0.0f, 1.0f,0.0f, 1.0f, 1.0f }
	};

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);


	/**
	* �����ҿ����������
	*/
	float* addrVertex = (float*)g_cubeVertices;
	float* uvAddress = (float*)&g_cubeVertices[0].u;

	float* colorAddress = (float*)&g_cubeVertices[0].r;

	//--------------Ԫ�ظ���---Ԫ������---Ԫ��֮����ڴ�ƫ��---���ݵ�ַ
	//OpenGL����Ԫ��֮����ڴ�ƫ����������һ��Ԫ�ص�λ�á�
	glVertexPointer(3, GL_FLOAT, sizeof(Vertex), addrVertex);
	glColorPointer(3, GL_FLOAT, sizeof(Vertex), colorAddress);
	glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), uvAddress);

	glDrawArrays(GL_QUADS, 0, 24);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
}

int count = 0;
void Lesson04_6_PBO::render()
{
	/*
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, double(unWidth) / double(unHeight), 0.1f, 1000.0f);


	glBindTexture(GL_TEXTURE_2D, m_unTextureId);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(-2, 0, -5);
	drawCude(true);

	//----------------------------------------------------------------
	//!!������DMA�Դ��������� ���ķ�CPU
	glBindBuffer(GL_PIXEL_PACK_BUFFER, m_pbo); //pbo���Դ���ڴ����һ��
	//����̨�Դ����ݶ���pbo��
	glReadPixels(0, 0, unWidth, unHeight, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

	//----------------------------------------------------------------
	//������һ�������������е�����ӳ��Ϊ�ͻ����еĵ�ַ�ռ�  
	//glMapBufferӳ������ ���dma���þ͵�dma���  Ч�ʵ�
	void* data = glMapBuffer(GL_PIXEL_PACK_BUFFER, GL_READ_WRITE);
	if (data)
	{
		//saveImage(unWidth, unHeight, (char*)data);
	}
	//ȡ��ӳ��  ����
	glUnmapBuffer(GL_PIXEL_PACK_BUFFER);
	glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
	*/


	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	renderCube(true);

	count++;
	if (count == 50)
	{
		// ָ��������
		//glReadBuffer(GL_BACK);  // �󻺳��� ����
		glReadBuffer(GL_FRONT); // ǰ������ ��ʾ
		//! DMA�Դ������ ������cpuʱ��
		glBindBuffer(GL_PIXEL_PACK_BUFFER, m_pbo);
		// ���Դ����pbuff
		glReadPixels(0, 0, unWidth, unHeight, GL_RGBA, GL_UNSIGNED_BYTE, 0);
		void*   data = glMapBuffer(GL_PIXEL_PACK_BUFFER, GL_READ_ONLY);
		if (data)
		{
			saveImage(unWidth, unHeight, (char*)data);
		}
		glUnmapBuffer(GL_PIXEL_PACK_BUFFER);
	}


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

bool Lesson04_6_PBO::saveImage(int w, int h, char* data)
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
