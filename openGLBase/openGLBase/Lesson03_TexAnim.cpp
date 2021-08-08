#include "Lesson03_TexAnim.h"
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

Vertex vertex2[4] = {
	{-0.5, 0.125,  0.0,  0.0, 0.25},
	{-0.5, -0.125, 0.0,  0.0, 0.0},
	{0.5,  0.125,  0.0,  1.0, 0.25},
	{0.5, -0.125,  0.0,  1.0, 0.0},
};

void Lesson03_TexAnim::onDisable()
{
	// ɾ������(����)
	glDeleteTextures(2, m_unTextureId);

	int err = glGetError(); //0û�д��� 
}

void Lesson03_TexAnim::initGL()
{
	// ��������
	glEnable(GL_TEXTURE_2D);
	// ������Ȳ���
	glEnable(GL_DEPTH_TEST);

	m_unTextureId[0] = createTextureFromImage("res/1.png");
	m_unTextureId[1] = createTextureFromImage("res/1008.jpg");
	m_unTextureId[2] = createTextureFromImage("res/1009.png");
}

unsigned int Lesson03_TexAnim::createTexture(int w, int h, const void* data)
{
	unsigned texId;
	glGenTextures(1, &texId);
	glBindTexture(GL_TEXTURE_2D, texId);
	// �����˲�
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	// �����װ
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

	return texId;
}
unsigned int Lesson03_TexAnim::createTextureFromImage(const char* fileName)
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

// �������
void Lesson03_TexAnim::playTexAnim()
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	// 1.ͨ��uvʵ��������
	//for (int i = 0; i < 4; ++i) vertex2[i].v -= 0.001;

	// 2. ͨ���������ʵ�������� ��ֻ��pc�Ͽ��ã�
	glMatrixMode(GL_TEXTURE);
	//glLoadIdentity();
	//glTranslatef(0, 0.5f, 0);
	//glScalef(4,4,4);
	glRotatef(1, 0, 0, 1);
	

	glVertexPointer(3, GL_FLOAT, sizeof(Vertex), vertex2);
	glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), &vertex2[0].u);

	glBindTexture(GL_TEXTURE_2D, m_unTextureId[0]);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0, 0, -3);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}
// ������
// ����ǰopenGLĬ��һ�������������ٿ�����8��ͼ�� ��arb��չ�Ķ�����������һ��������������32����������2~3�Ź��ˣ�
/*
����OpenGL�ı�׼���²��Ǻ�Ƶ������ˣ���ĳ�ּ���Ӧ����������ʱ���Կ�����Ϊ��֧�ָü�������ʹ���Լ�����չ��ʵ�ָù��ܡ�
���ǲ�ͬ��������в�ͬ��ʵ�֣���ô�����д�����쳣��������˶�����̹�ͬЭ��ʹ��һ�µ���չ�������EXT��չ��
��������չ�����೧��Э�̣����ҵõ�OpenGL��ϵ�ṹ���ίԱ�ᣨ��ARB����ȷ�ϣ���ô����չ���ΪARB��չ��
��������׼�ƶ�����Ϊ�ù����б�Ҫ��ӵ��°汾�ı�׼�У������չ��Ϊ��׼��һ���֣���������չ��
������չ��������OpenGL�Ͱ汾Ҳ��ʵ�ִ󲿷ָ߰汾�Ĺ��ܡ�ʹ����չʱ��Ҫע�����ȼ�����׼>ARB>EXT�����б�׼���ܱ㲻����չ����ARB��չʵ�ֱ㲻��EXT��չ��
*/
void Lesson03_TexAnim::manyTexture1()
{
	/*opengl��չ������1.5�汾����У� ���ڵ��Կ�100%֧��1.5����������*/
	// ���Ԫ����������8����Ĭ�ϴӵ�0����Ԫȡ��
	glActiveTextureARB(GL_TEXTURE0_ARB);
	glEnable(GL_TEXTURE_2D);
	// ��ָ��������ŵ���0����Ԫ��
	glBindTexture(GL_TEXTURE_2D, m_unTextureId[0]);

	glActiveTextureARB(GL_TEXTURE1_ARB);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, m_unTextureId[1]);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0, 0, -3);

	glBegin(GL_TRIANGLE_STRIP);
		// ��������
		glMultiTexCoord2dARB(GL_TEXTURE0_ARB, 0.0f, 1.0f);
		glMultiTexCoord2dARB(GL_TEXTURE1_ARB, 0.0f, 1.0f);
		// ��������
		glVertex3f(-0.5, 0.5, 0.0);

		glMultiTexCoord2dARB(GL_TEXTURE0_ARB, 0.0f, 0.0f);
		glMultiTexCoord2dARB(GL_TEXTURE1_ARB, 0.0f, 0.0f);
		glVertex3f(-0.5, -0.5, 0.0);

		glMultiTexCoord2dARB(GL_TEXTURE0_ARB, 1.0f, 1.0f);
		glMultiTexCoord2dARB(GL_TEXTURE1_ARB, 1.0f, 1.0f);
		glVertex3f(0.5, 0.5, 0.0);

		glMultiTexCoord2dARB(GL_TEXTURE0_ARB, 1.0f, 0.0f);
		glMultiTexCoord2dARB(GL_TEXTURE1_ARB, 1.0f, 0.0f);
		glVertex3f(0.5, -0.5, 0.0);
	glEnd();
}

void Lesson03_TexAnim::manyTexture2()
{
	glActiveTextureARB(GL_TEXTURE0_ARB);            // ѡ��GL_TEXTURE0Ϊ����Ŀ��
	//glEnable(GL_TEXTURE_2D);                      // ����GL_TEXTURE0��Ԫ GL_TEXTURE0���Բ�Ҫ
	glBindTexture(GL_TEXTURE_2D, m_unTextureId[0]); // ΪGL_TEXTURE0��Ԫ��texture����ͼ��
	//glDisable(GL_TEXTURE_2D);                     // �ر�����

	glActiveTextureARB(GL_TEXTURE1_ARB);
	glEnable(GL_TEXTURE_2D);  // ���뿪��
	glBindTexture(GL_TEXTURE_2D, m_unTextureId[1]);
	//glDisable(GL_TEXTURE_2D); // �رն������� GL_TEXTURE1_ARB������,

	glActiveTextureARB(GL_TEXTURE2_ARB);
	glEnable(GL_TEXTURE_2D);    // ���뿪��
	glBindTexture(GL_TEXTURE_2D, m_unTextureId[2]);
	//glDisable(GL_TEXTURE_2D);   // �رն������� GL_TEXTURE2_ARB������,


	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0, 0, -3);


	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, sizeof(Vertex), &vertex2);


	glClientActiveTextureARB(GL_TEXTURE0_ARB); // ��������Ԫ������������
	glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), &vertex2[0].u);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glClientActiveTextureARB(GL_TEXTURE1_ARB);
	glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), &vertex2[0].u);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glClientActiveTextureARB(GL_TEXTURE2_ARB);
	glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), &vertex2[0].u);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void Lesson03_TexAnim::playManyTextureAnim()
{
	glMatrixMode(GL_TEXTURE); // ��������GL_TEXTURE1 ���һ�ΰ�
	glTranslatef(0.01, 0, 0);

	glActiveTextureARB(GL_TEXTURE0_ARB);            // ѡ��GL_TEXTURE0Ϊ����Ŀ��
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, m_unTextureId[0]); // ΪGL_TEXTURE0��Ԫ��texture����ͼ��
	//glMatrixMode(GL_TEXTURE); // ��������GL_TEXTURE0
	//glTranslatef(0.01, 0, 0);

	glActiveTextureARB(GL_TEXTURE1_ARB);
	glEnable(GL_TEXTURE_2D); 
	glBindTexture(GL_TEXTURE_2D, m_unTextureId[1]);
	//glMatrixMode(GL_TEXTURE); // ��������GL_TEXTURE1
	//glTranslatef(0.01, 0, 0);
	

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, sizeof(Vertex), &vertex2);

	glClientActiveTextureARB(GL_TEXTURE0_ARB); // ��������Ԫ������������
	glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), &vertex2[0].u);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glClientActiveTextureARB(GL_TEXTURE1_ARB);
	glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), &vertex2[0].u);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0, 0, -2);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void Lesson03_TexAnim::render()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, (double)unWidth/(double)unHeight, 0.1f, 1000.0f);

	//playTexAnim();
	//manyTexture1();
	//manyTexture2();
	playManyTextureAnim();
}
