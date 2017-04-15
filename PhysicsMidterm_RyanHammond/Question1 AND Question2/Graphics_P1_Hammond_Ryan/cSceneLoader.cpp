#include "cSceneLoader.h"
#include <fstream>
#include <glm/gtc/matrix_transform.hpp>

cSceneLoader::cSceneLoader()
{

}

cSceneLoader::~cSceneLoader()
{

}

extern unsigned int g_yWingID;

bool cSceneLoader::loadScene(std::string fileName, cMeshTypeManager &meshManager, unsigned int program, std::vector<cSphere*> &vec_pSpheres, std::vector<cTriangleMesh*> &vec_pTriangles, glm::mat4 &targetTransform)
{
	std::ifstream theFile(fileName.c_str());
	if (!theFile.is_open())
	{
		return -1;
	}

	std::string test;
	theFile >> test;	//read in first element in line
	//load sphere
	if (!meshManager.LoadPlyFileIntoGLBuffer(program, "Sphere_N.ply", true))//load the mesh by name from text file
	{
		//if mesh isnt loaded 
		return -1;
	}

	if (test == "Camera")
	{
		
		float tempFloat;
		theFile >> tempFloat;
		targetTransform[3].x = tempFloat;
		theFile >> tempFloat;
		targetTransform[3].y = tempFloat;
		theFile >> tempFloat;
		targetTransform[3].z = tempFloat;
		theFile >> test;
		
	}

	while (test != "Environment")
	{
		theFile >> test;
	}

	std::string meshName;
	theFile >> meshName;
	while (meshName != "endOfEnvironment")
	{

		if (!meshManager.LoadPlyFileIntoGLBuffer(program, meshName, true))//load the mesh by name from text file
		{
			//if mesh isnt loaded 
			return -1;
		}
		cTriangleMesh* pGameObject = new cTriangleMesh();
		int temp;
		theFile >> temp;	//read in each value and assign them to the appropriate variable in cGameObject
		pGameObject->bIsWireframe = temp;
		theFile >> temp;
		pGameObject->bIsUpdatedByPhysics = temp;

		float tempFloat;
		theFile >> tempFloat;
		pGameObject->position.x = tempFloat;
		theFile >> tempFloat;
		pGameObject->position.y = tempFloat;
		theFile >> tempFloat;
		pGameObject->position.z = tempFloat;

		theFile >> tempFloat;
		pGameObject->postRotation.x = tempFloat;
		theFile >> tempFloat;
		pGameObject->postRotation.y = tempFloat;
		theFile >> tempFloat;
		pGameObject->postRotation.z = tempFloat;

		theFile >> tempFloat;
		pGameObject->scale = tempFloat;

		theFile >> tempFloat;
		pGameObject->solidColour.r = tempFloat;
		theFile >> tempFloat;
		pGameObject->solidColour.g = tempFloat;
		theFile >> tempFloat;
		pGameObject->solidColour.b = tempFloat;

		pGameObject->meshName = meshName;
		vec_pTriangles.push_back(pGameObject);	//add the gameobject to the global variable from the main

		theFile >> meshName;
	}

	theFile >> test;
	while (test != "Turrets")
	{
		theFile >> test;
	}

	theFile >> meshName;
	if (!meshManager.LoadPlyFileIntoGLBuffer(program, meshName, true))//load the mesh by name from text file
	{
		//if mesh isnt loaded 
		return -1;
	}
	int numberToCreate;
	theFile >> numberToCreate;
	float scale;
	theFile >> scale;
	float r, g, b;
	theFile >> r >> g >> b;
	for (int index = 0; index != numberToCreate; index++)
	{
		cTriangleMesh* pGameObject = new cTriangleMesh();
		pGameObject->bIsWireframe = false;
		pGameObject->bIsUpdatedByPhysics = false;
		//less than 2 or greater than -2
		float randX = 0;
		while (randX < 2  && randX > -2)
		{
			randX = getRandFloat(-200.f, 200.f);
		}
		pGameObject->position.x = randX;// getRandFloat(-250.f, 250.f);
		pGameObject->position.y = 0.f;
		pGameObject->position.z = getRandFloat(-200.f, 200.f);
		pGameObject->scale = scale;
		pGameObject->solidColour.r = r;
		pGameObject->solidColour.g = g;
		pGameObject->solidColour.b = b;

		pGameObject->meshName = meshName;
		vec_pTriangles.push_back(pGameObject);
	}

	theFile >> test;
	while (test != "yWing")
	{
		theFile >> test;
	}
	theFile >> meshName;
	if (!meshManager.LoadPlyFileIntoGLBuffer(program, meshName, true))//load the mesh by name from text file
	{
		//if mesh isnt loaded 
		return -1;
	}
	theFile >> scale;
	float radius;
	theFile >> radius;
	float x, y, z;
	theFile >> x >> y >> z >> r >> g >> b;

	cSphere* pGameObject = new cSphere();
	pGameObject->bIsWireframe = false;
	pGameObject->bIsUpdatedByPhysics = false;
	
	pGameObject->position.x = x;
	pGameObject->position.y = y;
	pGameObject->position.z = z;
	pGameObject->scale = scale;
	pGameObject->radius = radius;
	pGameObject->solidColour.r = r;
	pGameObject->solidColour.g = g;
	pGameObject->solidColour.b = b;
	pGameObject->postRotation.y = 0.78;

	pGameObject->meshName = meshName;
	vec_pSpheres.push_back(pGameObject);
	g_yWingID = pGameObject->getID();

	theFile >> test;
	if (test == "eof")
	{
		return 1;
	}

		




}