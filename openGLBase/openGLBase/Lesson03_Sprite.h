#pragma once
#include "GLContext.h"

struct float3
{
	float x, y, z;
};

struct Particle
{
	float3  curPos;
	float3  curVel;
	unsigned char   r;
	unsigned char   g;
	unsigned char   b;
	unsigned char   a;
};

const unsigned int c_ParticlesMax = 100;

class Lesson03_Sprite
{
public:
	void onDisable();
	void initGL();
	unsigned int createTextureFromImage(const char* fileName);
	unsigned int createTexture(int w, int h, const void* data);

	float3       getRandomVector(void);
	void         updateSprite(void);
	void         renderSprite(void);
	void         initPointSprites(void);

	void render();

public:
	GLuint           m_unTexId;
	Particle         m_aParticle[c_ParticlesMax];
	unsigned int     m_unFrame;
};

