#ifndef _cGameObject_HG_
#define _cGameObject_HG_

#include <glm/vec3.hpp>
#include <string>
#include <vector>
#include "Utilities.h"
#include <glm/mat4x4.hpp>



class cSphere
{
public:
	cSphere();
	~cSphere();
	int getID(void);
	glm::mat4 getModelMatrix(float unitScale);
	//physics info
	glm::vec3 position;
	glm::vec3 lastPosition;
	glm::vec3 preRotation;
	glm::vec3 postRotation;
	glm::vec3 acceleration;
	glm::vec3 velocity;
	float scale;
	float radius;
	bool bIsUpdatedByPhysics;

	int mUniqueID;
	static int m_nextID;

	//rendering data
	int meshID;
	std::string meshName;
	bool bIsWireframe;
	glm::vec3 solidColour;
	float groundCollisionFadeCounter;


};

class cTriangleMesh
{
public:
	cTriangleMesh();
	~cTriangleMesh();
	int getID(void);
	glm::mat4 getModelMatrix(float unitScale);
	void setTriangles(std::vector <cPlyTriangle> trianglesIn, float unitScale);

	//physics info
	glm::vec3 position;
	glm::vec3 lastPosition;
	glm::vec3 preRotation;
	glm::vec3 postRotation;
	glm::vec3 acceleration;
	glm::vec3 velocity;
	float scale;
	std::vector<cPlyTriangle> physicsTris;
	bool bIsUpdatedByPhysics;

	int mUniqueID;
	static int m_nextID;

	//rendering info
	int meshID;
	std::string meshName;
	bool bIsWireframe;
	glm::vec3 solidColour;

};

#endif 
