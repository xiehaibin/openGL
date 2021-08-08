#include "Lesson03_EnvironmentTex.h"
extern unsigned int unWidth;
extern unsigned int unHeight;

/*
��������
���ӱȽϾã����ڶ���������ͼ����պ��ӣ�
*/
void Lesson03_EnvironmentTex::onDisable()
{
	// ɾ������(����)
	glDeleteTextures(1, &m_unTexId);
}

unsigned int Lesson03_EnvironmentTex::createTextureFromImage(const char* fileName)
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

unsigned int Lesson03_EnvironmentTex::createTexture(int w, int h, const void* data)
{
	unsigned texId;
	glGenTextures(1, &texId);
	glBindTexture(GL_TEXTURE_2D, texId);
	// �����˲�
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	// �����װ
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, w, h, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 10);

	return texId;
}

void Lesson03_EnvironmentTex::initGL()
{
	// ��������
	glEnable(GL_TEXTURE_2D);
	// ������Ȳ���
	glEnable(GL_DEPTH_TEST);

	m_angle = 0;
	m_unTexId = createTextureFromImage("res/1009.png");


	quadric = gluNewQuadric();
	gluQuadricNormals(quadric, GLU_SMOOTH);  // ��������
	gluQuadricTexture(quadric, GL_TRUE);     // ������������
	///*
	glEnable(GL_TEXTURE_GEN_S); // ����������S����
	glEnable(GL_TEXTURE_GEN_T);
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP); // ָ�������������Ĳ���
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
	//*/
}

void Lesson03_EnvironmentTex::render()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, (double)unWidth / (double)unHeight, 0.1f, 1000.0f);

	glBindTexture(GL_TEXTURE_2D, m_unTexId);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0, 0, -18);
	glRotated(m_angle, 0, 1, 0);

	gluSphere(quadric, 1.3f, 32, 32); // �ȽϾ�

	m_angle += 0.1f;
}
