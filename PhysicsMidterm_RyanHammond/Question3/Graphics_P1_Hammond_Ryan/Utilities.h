#ifndef _Utilities_HG_
#define _Utilities_HG_

#include <glm/vec3.hpp>

struct cPlyVertexXYZ_N
{
	cPlyVertexXYZ_N() : x(0.0f), y(0.0f), z(0.0f), nX(0.0f), nY(0.0f), nZ(0.0f) {}
	float x;
	float y;
	float z;
	float nX;
	float nY;
	float nZ;
};

struct cPlyTriFace
{
	cPlyTriFace() : v1(-1), v2(-1), v3(-1) {}	// can never be -1 in an actual model
	int v1;
	int v2;
	int v3;
};

struct cPlyTriangle
{
	glm::vec3 v1;
	glm::vec3 v2;
	glm::vec3 v3;
};

float getRandFloat(float LO, float HI);

#endif