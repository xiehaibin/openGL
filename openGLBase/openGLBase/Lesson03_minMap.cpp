#include "Lesson03_minMap.h"
#include <stdlib.h>
//#include <gl/GLU.h> //����minmap 

extern unsigned int unWidth;
extern unsigned int unHeight;

struct Vertex3
{
	float x;
	float y;
	float z;

	float u;
	float v;
};

Vertex3 verCude[] = {
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
����»��Դ��������
����ָ�Ĳ������ݴ�ϵͳ�ڴ洫�䵽�Դ���Ҫ�Ĵ�����
�Դ��㹻������� ����������һֱ�������Դ��
����ָ������Ⱦʱ�������Դ棨ʵ����Ҳ��һ�����Դ� ����GPU������֧��DMA���ʹ���Ԫ֮�䴫������Ҫ�Ĵ�����
*/

/*
ʹ�� Mipmap ��������Ҫ���ŵ㣺
1. ͨ��������������Ч�����������, �ر�����ǿ��С������¡�
2.ͨ��������ʹ�� mipmapping �������������ɵĻ�����Ӷ����ͼ��������
mipmapping �ĵ�һ������ÿ��ͼ����Ҫ��Լ����֮һ�������ڴ档�������������Ⱦ�ٶȺ�ͼ����������ĺô���ȣ�����ɱ����ܺ�С��
��ĳЩ����£���Ӧʹ��Mipmap��������˵���ڲ��ܺ���Ӧ�ù��˵�����£���Ӧʹ��mipmapping�����������ͼ�����ݣ����������������������
���ڴӲ���С������ҲӦ�ñ��⣬���磬UIԪ���е�����Ԫ������������һһӳ��
*/

/*
Ϊ�˼ӿ���Ⱦ�ٶȺͼ���ͼ���ݣ���ͼ���������һϵ�б�Ԥ�ȼ�����Ż�����ͼƬ��ɵ��ļ�,��������ͼ����Ϊ MIP map ���� mipmap
�༶������������ܹ���Ч�����ͼ����Ⱦ�ٶȣ���������ͶӰƽ���Զʱ��Direct3D��ѡ��һ�ųߴ��С�����������Ⱦ��
�����辭�����ӵ������������������ˣ�����������ʱ������Ҫ���Դ�Ȳ�ʹ�ö༶��������ʱС���������Ч�ؼ������������Դ��ʱ�䡣
ȱ��:�Ƕ��ڴ��Ҫ��Ƚϸ�
*/
Lesson03_minMap::Lesson03_minMap()
{

}
Lesson03_minMap::~Lesson03_minMap()
{
}

void Lesson03_minMap::onDisable()
{
	// ɾ������(����)
	glDeleteTextures(1, &m_unTexId1);

	int err = glGetError(); //0û�д��� 
}

unsigned int Lesson03_minMap::createTextureFromImage(const char* fileName)
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

unsigned int Lesson03_minMap::createTexture(int w, int h, const void* data)
{
	unsigned texId;
	glGenTextures(1, &texId);
	glBindTexture(GL_TEXTURE_2D, texId);
	// �����˲�
	/*
	GL_LINEAR:  ��mip������ʹ�����Թ��� 
	GL_NEAREST: ��mip������ʹ�����ڽ����� 

	GL_NEAREST_MIPMAP_NEAREST	ѡ�����ڽ���mip�㣬��ʹ�����ڽ�����
	GL_NEAREST_MIPMAP_LINEAR	��mip��֮��ʹ�����Բ�ֵ�����ڽ�����

	GL_LINEAR_MIPMAP_NEAREST	ѡ�����ڽ���mip�㣬ʹ�����Թ���
	GL_LINEAR_MIPMAP_LINEAR	    ��mip��֮��ʹ�����Բ�ֵ��ʹ�����Թ��ˣ��ֳ�������mipmap  �������  Ч�����
	*/
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	// �����װ
	/*
	GL_REPEAT:          �ظ���Ĭ�ϣ�
	GL_CLAMP��          ��Ե����
	GL_MIRRORED_REPEAT: ����
	*/
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	/*
	�����Դ����Դ��д������� ���ԭ����ɾ��
	*/
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);


	/*
	mipMap:
	��������һ������ �����ɶ���ͼ��ɣ�0��ʾ����
	һ��ͼƬ�Ŵ���С�Կ�Ҫ����������һ���µ�ͼƬ��������̱ȽϺ�ʱ
	������������ǽ���ʱ���ô�����������Զ��ʱ���ý�С������ ��һ����������һ��������1/2
	mipMap�����߱������ĵı���  ��� 256*256 �ɲ���9������
	*/
	//gluBuild2DMipmaps �ڲ�forѭ���������� ѭ��������߳�2 ����1Ϊֹ �ڴ�ռ������ 
	//for()glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, w, h, GL_RGBA, GL_UNSIGNED_BYTE, data); // ����һ��mipmap����
	// ��������
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL,  10);

	return texId;
}

void Lesson03_minMap::initGL()
{
	// ��������
	glEnable(GL_TEXTURE_2D);
	// ������Ȳ���
	glEnable(GL_DEPTH_TEST);

	//init();

	m_unTexId1 = createTextureFromImage("res/1009.png");
}

void Lesson03_minMap::render()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, (double)unWidth/(double)unHeight, 0.1f, 1000.0f);

	drawCude();
}

void Lesson03_minMap::drawCude()
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glVertexPointer(3, GL_FLOAT, sizeof(Vertex3), verCude);
	glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex3), &verCude[0].u);

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();
	glTranslatef(-2, 0, m_fAngle);
	//glRotated(-10, 0, 1, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 24);

	glLoadIdentity();
	glTranslatef(2, 0, m_fAngle);
	//glRotated(10, 0, 1, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 24);
	
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	m_fAngle = m_fAngle - 0.05f;
}
