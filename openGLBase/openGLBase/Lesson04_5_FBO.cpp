#include "Lesson04_5_FBO.h"

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
Vertex verCude5[] = {
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

unsigned char indexVer5[] = {
	0,1,2,3,
	4,5,6,7,
	4,0,5,1,
	6,2,7,3,
	4,0,6,2,
	5,1,7,3
};

void Lesson04_5_FBO::onDisable()
{
	// ɾ������(����)
	glDeleteTextures(2, &m_unTextureId);

	glDeleteBuffers(1, &m_unVbo);

	int err = glGetError(); //0û�д��� 
}

void Lesson04_5_FBO::initGL()
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	m_unTextureId = createTextureFromImage("res/9010.jpg");
	createVbo();
	createIbo();

	m_fbo.init(unWidth, unHeight);
	//�����С�����fboһ�������򴴽�ʧ�� ������
	m_dynamicTexId = createTexture(unWidth, unHeight+33, 0);
}

unsigned int Lesson04_5_FBO::createTexture(int w, int h, const void* data)
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
unsigned int Lesson04_5_FBO::createTextureFromImage(const char* fileName)
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

void Lesson04_5_FBO::createVbo()
{
	// ����һ��buff
	glGenBuffers(1, &m_unVbo);
	// ��buff ����openGL m_unVbo�Ƕ��㻺����
	glBindBuffer(GL_ARRAY_BUFFER, m_unVbo);
	//glBufferData:�����Կ��ڴ� glBufferDataARB:arb����չ������arb����ֲ��ʱ�򷽱�һ�㣬��glES���Գɱ�׼��arbȥ����
	//����Դ治��������glBufferData���·����Դ棬���ԭ���Ļ�������ַɾ��
	glBufferData(GL_ARRAY_BUFFER, sizeof(verCude5), verCude5, GL_STATIC_DRAW);
	//glBufferSubData(); //��̬��������
	// �ָ�״̬����Ȼ��Ժ������Ʋ���Ӱ��
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Lesson04_5_FBO::createIbo()
{
	glGenBuffers(1, &m_unIbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_unIbo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexVer5), indexVer5, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Lesson04_5_FBO::drawCude(bool isRotate)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, double(unWidth) / double(unHeight), 0.1f, 1000.0f);

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

void Lesson04_5_FBO::render()
{
	m_fbo.begin(m_dynamicTexId);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glBindTexture(GL_TEXTURE_2D, m_unTextureId);
		drawCude(true);

		unsigned char* data = new unsigned char[m_fbo.m_width * m_fbo.m_heigth * 4];
		memset(data, 0, m_fbo.m_width * m_fbo.m_heigth * 4);
		glReadPixels(0, 0, m_fbo.m_width, m_fbo.m_heigth, GL_RGBA, GL_UNSIGNED_BYTE, data);
		saveImage(m_fbo.m_width, m_fbo.m_heigth, (char*)data);
		delete[] data;

	m_fbo.end();


	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBindTexture(GL_TEXTURE_2D, m_dynamicTexId);
	drawCude(false);
	/*
	��pbuffer fbo������
	GL_RGBA, GL_UNSIGNED_BYTE  esֻ����������
	������������棨�����������ݣ������ڱ���ס����С���Ƕ���������
	*/
	/*
	unsigned char* data = new unsigned char[unWidth * unHeight * 4];
	memset(data, 0, unWidth * unHeight * 4);
	glReadPixels(0, 0, unWidth, unHeight, GL_RGBA, GL_UNSIGNED_BYTE, data);
	saveImage(unWidth, unHeight, (char*)data);
	delete[] data;
	*/
}

bool Lesson04_5_FBO::saveImage(int w, int h, char* data)
{
	FIBITMAP*   bitmap = FreeImage_Allocate(w, h, 32, 0, 0, 0);
	BYTE*       pixels = (BYTE*)FreeImage_GetBits(bitmap);

	for (int i = 0; i < w*h*4; i += 4)
	{
		BYTE temp = data[i];
		data[i] = data[i + 2];
		data[i + 2] = temp;
	}
	memcpy(pixels, data, w * h * 4);
	bool    bSuccess = FreeImage_Save(FIF_PNG, bitmap, "d:/xx.png", PNG_INTERLACED);
	FreeImage_Unload(bitmap);
	return  bSuccess;
}