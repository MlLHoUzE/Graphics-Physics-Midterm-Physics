#include "cGameObject.h"
#include <glm/gtc/matrix_transform.hpp>

int cTriangleMesh::m_nextID = 1;

cTriangleMesh::cTriangleMesh()
{
	this->mUniqueID = cTriangleMesh::m_nextID;
	cTriangleMesh::m_nextID++;

	// C'tor to init all values to 0.0f;

	this->position.x = this->position.y = this->lastPosition.z = 0.0f;
	this->velocity.x = 0.0f;
	this->velocity.y = 0.0f;
	this->velocity.z = 0.0f;
	this->acceleration.x = this->acceleration.y = this->acceleration.z = 0.0f;

	this->preRotation.x = this->preRotation.y = this->preRotation.z = 0.0f;
	this->postRotation.x = this->postRotation.y = this->postRotation.z = 0.0f;
	this->scale = 1.0f;

	this->bIsWireframe = false;	// Solid 
	this->solidColour.b = 1.0f;
	this->solidColour.g = 1.0f;
	this->solidColour.r = 1.0f;	// default white

	this->bIsUpdatedByPhysics = false;

	this->meshID = 0;

	return;
}
cTriangleMesh::~cTriangleMesh()
{

}
int cTriangleMesh::getID(void)
{
	return this->mUniqueID;
}
glm::mat4 cTriangleMesh::getModelMatrix(float unitScale)
{
	glm::mat4 m = glm::mat4x4(1.0f);
	m = glm::translate(m, this->position);
	m = glm::rotate(m, this->postRotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
	m = glm::rotate(m, this->postRotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
	m = glm::rotate(m, this->postRotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
	float actualScale = scale * unitScale;
	m = glm::scale(m, glm::vec3(actualScale, actualScale, actualScale));
	return m;
}

void cTriangleMesh::setTriangles(std::vector <cPlyTriangle> trianglesIn, float unitScale)
{
	glm::mat4 m = getModelMatrix(unitScale);
	for (int index = 0; index != trianglesIn.size(); index++)
	{
		cPlyTriangle temp;
		temp.v1 =  m * glm::vec4(trianglesIn[index].v1, 1.0f);
		temp.v2 = m * glm::vec4(trianglesIn[index].v2, 1.0f);
		temp.v3 = m * glm::vec4(trianglesIn[index].v3, 1.0f);
		this->physicsTris.push_back(temp);
	}
}