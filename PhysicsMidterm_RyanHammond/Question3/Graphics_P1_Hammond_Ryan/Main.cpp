#include <glad/glad.h>		
#include <GLFW/glfw3.h>		
#include <iostream>
#include "global.h"

#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr

#include <stdlib.h>
#include <stdio.h>

#include <vector>
#include "cGameObject.h"
#include "cShaderManager.h"
#include "Input.h"
#include <cstdlib>
#include <ctime>


#include "cMeshTypeManager.h"	
#include "cSceneLoader.h"
#include "cContact.h"

std::vector< cSphere* > g_vec_pSpheres;
std::vector <cTriangleMesh*> g_vec_pTriangleMeshes;
std::vector< cContact > g_vec_Contacts;
std::vector<cTriangleMesh*> g_vec_hitMarkers;

void PhysicsStep(float deltaTime);
void CollisionStep(float deltaTime);
void highlightTurrets();
void bombAway();
void createHitMarker(glm::vec3 contactPoint);
void deleteSphereByID(unsigned int ID);

cMeshTypeManager* g_pMeshTypeManager = 0;
cShaderManager* g_pShaderManager = 0;
cSceneLoader* g_pSceneLoader = 0;

unsigned int g_yWingID = 0;

float fireRate = 1.0f;
float lastShot = 0;

glm::vec3 g_offset = glm::vec3(-10.0f, 0.0f, 0.f);

glm::mat4 g_targetTransform;// = glm::translate(g_targetTransform, glm::vec3(-50, 0, -50));

glm::vec3 lightPos(0.0f, 15.0f, 0.0f);




static void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error: %s\n", description);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);

}

cSphere* findObjectByID(unsigned int ID)
{
	for (int index = 0; index != g_vec_pSpheres.size(); index++)
	{
		if (::g_vec_pSpheres[index]->getID() == ID)
		{
			return ::g_vec_pSpheres[index];
		}
	}
	return 0;
}


cTriangleMesh* findTurretByID(unsigned int ID)
{
	for (int index = 0; index != g_vec_pTriangleMeshes.size(); index++)
	{
		if (::g_vec_pTriangleMeshes[index]->getID() == ID)
		{
			return ::g_vec_pTriangleMeshes[index];
		}
	}
	return 0;
}

glm::vec3 ClosestPtPointTriangle(glm::vec3 p, glm::vec3 a, glm::vec3 b, glm::vec3 c)
{
	// Check if P in vertex region outside A
	glm::vec3 ab = b - a;
	glm::vec3 ac = c - a;
	glm::vec3 ap = p - a;
	float d1 = glm::dot(ab, ap);		// glm::dot( ab, ap );
	float d2 = glm::dot(ac, ap);
	if (d1 <= 0.0f && d2 <= 0.0f) return a; // barycentric coordinates (1,0,0)

											// Check if P in vertex region outside B
	glm::vec3 bp = p - b;
	float d3 = glm::dot(ab, bp);
	float d4 = glm::dot(ac, bp);
	if (d3 >= 0.0f && d4 <= d3) return b; // barycentric coordinates (0,1,0)

										  // Check if P in edge region of AB, if so return projection of P onto AB
	float vc = d1*d4 - d3*d2;
	if (vc <= 0.0f && d1 >= 0.0f && d3 <= 0.0f) {
		float v = d1 / (d1 - d3);
		return a + v * ab; // barycentric coordinates (1-v,v,0)
	}

	// Check if P in vertex region outside C
	glm::vec3 cp = p - c;
	float d5 = glm::dot(ab, cp);
	float d6 = glm::dot(ac, cp);
	if (d6 >= 0.0f && d5 <= d6) return c; // barycentric coordinates (0,0,1)

										  // Check if P in edge region of AC, if so return projection of P onto AC
	float vb = d5*d2 - d1*d6;
	if (vb <= 0.0f && d2 >= 0.0f && d6 <= 0.0f) {
		float w = d2 / (d2 - d6);
		return a + w * ac; // barycentric coordinates (1-w,0,w)
	}

	// Check if P in edge region of BC, if so return projection of P onto BC
	float va = d3*d6 - d5*d4;
	if (va <= 0.0f && (d4 - d3) >= 0.0f && (d5 - d6) >= 0.0f) {
		float w = (d4 - d3) / ((d4 - d3) + (d5 - d6));
		return b + w * (c - b); // barycentric coordinates (0,1-w,w)
	}

	// P inside face region. Compute Q through its barycentric coordinates (u,v,w)
	float denom = 1.0f / (va + vb + vc);
	float v = vb * denom;
	float w = vc * denom;
	return a + ab * v + ac * w; // = u*a + v*b + w*c, u = va * denom = 1.0f - v - w
}

int main(void)
{
	
	GLint locID_matModel = -1;		// 
	GLint locID_matView = -1;
	GLint locID_matProj = -1;

	GLint locID_myLightPosition = -1;
	GLint locID_myLightDiffuse = -1;
	GLint locID_myLightAmbient = -1;

	GLuint UniformLoc_ID_objectColour = -1;
	GLuint UniformLoc_ID_isWireframe = -1;

	glfwSetErrorCallback(error_callback);

	std::srand(std::time(NULL));

	if (!glfwInit())
		exit(EXIT_FAILURE);


	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	gWindow = glfwCreateWindow(1200, 800, "Graphics Project1 : By Ryan Hammond", NULL, NULL);
	if (!gWindow)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	

	glfwSetKeyCallback(gWindow, key_callback);
	glfwMakeContextCurrent(gWindow);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	glfwSwapInterval(1);
	glEnable(GL_DEPTH_TEST);

	::g_pShaderManager = new cShaderManager();

	cShaderManager::cShader verShader;
	cShaderManager::cShader fragShader;

	verShader.fileName = "simpleVert.glsl";
	fragShader.fileName = "simpleFrag.glsl";

	if (!::g_pShaderManager->createProgramFromFile("simple", verShader, fragShader))
	{
		std::cout << ::g_pShaderManager->getLastError() << std::endl;
		std::cout.flush();
		return -1;
	}
	
	// Entire loading the model and placing ito the VAOs 
	// is now in the cMeshTypeManager which is called from the scene loader
	::g_pMeshTypeManager = new cMeshTypeManager();
	::g_pSceneLoader = new cSceneLoader();

	GLuint shadProgID = ::g_pShaderManager->getIDFromFriendlyName("simple");

	::g_pSceneLoader->loadScene("scene.txt", *g_pMeshTypeManager, shadProgID, g_vec_pSpheres, g_vec_pTriangleMeshes, g_targetTransform);

	
	for (int index = 0; index != g_vec_pTriangleMeshes.size(); index++)
	{
		GLuint vaoID;
		float unitScale;
		int numberOfIndices;
		::g_pMeshTypeManager->LookUpMeshInfo(g_vec_pTriangleMeshes[index]->meshName,
			vaoID,
			numberOfIndices,
			unitScale);

		std::vector <cPlyTriangle> triangleInfo;
		::g_pMeshTypeManager->getTriangleVertexInformation(g_vec_pTriangleMeshes[index]->meshName, triangleInfo);
		g_vec_pTriangleMeshes[index]->setTriangles(triangleInfo, unitScale);
	}

	locID_matModel = glGetUniformLocation(shadProgID, "matModel");
	locID_matView = glGetUniformLocation(shadProgID, "matView");
	locID_matProj = glGetUniformLocation(shadProgID, "matProj");

	locID_myLightPosition = glGetUniformLocation(shadProgID, "myLightPosition");
	locID_myLightDiffuse = glGetUniformLocation(shadProgID, "myLightDiffuse");
	locID_myLightAmbient = glGetUniformLocation(shadProgID, "myLightAmbient");

	

	UniformLoc_ID_objectColour = glGetUniformLocation(shadProgID, "objectColour");
	UniformLoc_ID_isWireframe = glGetUniformLocation(shadProgID, "isWireframe");


	while (!glfwWindowShouldClose(gWindow))
	{
		float ratio;
		int width, height;

		glm::mat4x4 matProjection;

		glfwGetFramebufferSize(gWindow, &width, &height);
		ratio = width / (float)height;

		glViewport(0, 0, width, height);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		bool wPressed = nInput::IsKeyPressed::W();
		bool sPressed = nInput::IsKeyPressed::S();
		bool dPressed = nInput::IsKeyPressed::D();
		bool aPressed = nInput::IsKeyPressed::A();
		bool qPressed = nInput::IsKeyPressed::Q();
		bool ePressed = nInput::IsKeyPressed::E();
		bool fPressed = nInput::IsKeyPressed::F();
		bool shift = nInput::IsKeyPressed::LEFTSHIFT();
		float cameraSpeed = 1.0f;
		float turnSpeed = 1.0f;
		float upSpeed = 1.0f;

		cTriangleMesh* p_yWing = findTurretByID(g_yWingID);
		if (wPressed != sPressed)
		{
			if (sPressed) cameraSpeed *= -1;
			g_targetTransform = glm::translate(g_targetTransform, glm::vec3(cameraSpeed, 0.0, 0.f));
		}
		if (aPressed != dPressed)
		{
			if (dPressed) turnSpeed *= -1;
			g_targetTransform = glm::rotate(g_targetTransform, 0.1f, glm::vec3(0.0, turnSpeed, 0.0));
			
		}
		

		
		if (qPressed != ePressed && shift)
		{
			if (qPressed) upSpeed *= -1;
			p_yWing->position.y += upSpeed;
		}
		if (qPressed != ePressed && !shift)
		{
			if (qPressed) upSpeed *= -1;
			g_targetTransform = glm::translate(g_targetTransform, glm::vec3(0.0, upSpeed, 0.0));
		}

		if (fPressed)
		{
			//drop a bomb
			if (lastShot > fireRate)
			{
				bombAway();
				lastShot = 0;
			}
		}


		glm::mat4x4 matView(1.0f);	// "View" (or camera) matrix

		matProjection = glm::perspective(0.6f, ratio, 0.01f, 10000.0f);

		//set up the view matrix
		glm::vec4 eye4(g_targetTransform[3].x, g_targetTransform[3].y, g_targetTransform[3].z, 1.0f);
		glm::vec3 target(g_targetTransform[3].x, g_targetTransform[3].y, g_targetTransform[3].z);
		glm::vec4 offset(g_offset.x, g_offset.y, g_offset.z, 0.f);
		offset = g_targetTransform * offset;
		offset = glm::normalize(offset) * 1.f;
		glm::vec3 eye(target.x + offset.x, target.y + offset.y, target.z + offset.z);

		matView = glm::lookAtRH(eye, target, glm::vec3(0.f, 1.f, 0.f));

		PhysicsStep(0.01f);
		CollisionStep(0.01f);
		highlightTurrets();

		glUniformMatrix4fv(locID_matProj, 1, GL_FALSE,
			(const GLfloat*)glm::value_ptr(matProjection));

		// This is set once at the start of the "scene" draw.
		glUniformMatrix4fv(locID_matView, 1, GL_FALSE,
			(const GLfloat*)glm::value_ptr(matView));


		glUniform3f(locID_myLightPosition, lightPos.x, lightPos.y, lightPos.z);
		glUniform3f(locID_myLightDiffuse, 1.0, 1.0f, 1.0f);
		glUniform3f(locID_myLightAmbient, 1.0f, 1.0f, 1.0f);



		// Start of Draw Scene

		for (int index = 0; index != ::g_vec_pSpheres.size(); index++)
		{
			cSphere* pCurGameObject = ::g_vec_pSpheres[index];

			std::string meshModelName = pCurGameObject->meshName;

			GLuint VAO_ID = 0;
			int numberOfIndices = 0;
			float unitScale = 1.0f;
			if (!::g_pMeshTypeManager->LookUpMeshInfo(meshModelName,
				VAO_ID,
				numberOfIndices,
				unitScale))
			{	// Skip the rest of the for loop, but continue
				continue;
			}


			glm::mat4x4 matModel = glm::mat4x4(1.0f);


			// Pre-rotation
			// Translation
			// Post-rotation
			// Scale


			matModel = glm::translate(matModel, pCurGameObject->position);

			matModel = glm::rotate(matModel, pCurGameObject->postRotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
			matModel = glm::rotate(matModel, pCurGameObject->postRotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
			matModel = glm::rotate(matModel, pCurGameObject->postRotation.z, glm::vec3(0.0f, 0.0f, 1.0f));

			//
			float actualScale = pCurGameObject->scale * unitScale;

			matModel = glm::scale(matModel, glm::vec3(actualScale, actualScale, actualScale));


			if (pCurGameObject->bIsWireframe)
			{	// Turn off backface culling
				// Enable "wireframe" polygon mode
				glPolygonMode(GL_FRONT_AND_BACK,	// GL_FRONT_AND_BACK is the only thing you can pass here
					GL_LINE);			// GL_POINT, GL_LINE, or GL_FILL
				glDisable(GL_CULL_FACE);
			}
			else
			{	// "Regular" rendering: 
				glCullFace( GL_BACK );		// GL_FRONT, GL_BACK, or GL_FRONT_AND_BACK
				glEnable( GL_CULL_FACE );
				glPolygonMode(GL_FRONT_AND_BACK,	// GL_FRONT_AND_BACK is the only thing you can pass here
					GL_FILL);			// GL_POINT, GL_LINE, or GL_FILL
			}
			

			::g_pShaderManager->useShaderProgram("simple");

			glUniformMatrix4fv(locID_matModel, 1, GL_FALSE,
				(const GLfloat*)glm::value_ptr(matModel));

			// Setting the uniform colours
			glUniform3f(UniformLoc_ID_objectColour,
				pCurGameObject->solidColour.r, pCurGameObject->solidColour.g, pCurGameObject->solidColour.b);
			if (pCurGameObject->bIsWireframe)
			{
				glUniform1i(UniformLoc_ID_isWireframe, TRUE);		// 1
			}
			else
			{
				glUniform1i(UniformLoc_ID_isWireframe, FALSE);	// 0
			}

			// Drawing indirectly from the index buffer

			glBindVertexArray(VAO_ID);
			glDrawElements(GL_TRIANGLES,
				numberOfIndices,
				GL_UNSIGNED_INT,	// Each index is how big?? 
				(GLvoid*)0);		// Starting point in buffer
			glBindVertexArray(0);

		}// for ( int index = 0;.... (bottom of "render scene" loop)

		//loop through all triangle meshes
		for (int index = 0; index != ::g_vec_pTriangleMeshes.size(); index++)
		{
			cTriangleMesh* pCurGameObject = ::g_vec_pTriangleMeshes[index];

			std::string meshModelName = pCurGameObject->meshName;

			GLuint VAO_ID = 0;
			int numberOfIndices = 0;
			float unitScale = 1.0f;
			if (!::g_pMeshTypeManager->LookUpMeshInfo(meshModelName,
				VAO_ID,
				numberOfIndices,
				unitScale))
			{	// Skip the rest of the for loop, but continue
				continue;
			}
			glm::mat4x4 matModel = glm::mat4x4(1.0f);

			matModel = glm::translate(matModel, pCurGameObject->position);

			matModel = glm::rotate(matModel, pCurGameObject->postRotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
			matModel = glm::rotate(matModel, pCurGameObject->postRotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
			matModel = glm::rotate(matModel, pCurGameObject->postRotation.z, glm::vec3(0.0f, 0.0f, 1.0f));

			float actualScale = pCurGameObject->scale * unitScale;

			matModel = glm::scale(matModel, glm::vec3(actualScale, actualScale, actualScale));


			if (pCurGameObject->bIsWireframe)
			{	// Turn off backface culling
				// Enable "wireframe" polygon mode
				glPolygonMode(GL_FRONT_AND_BACK,	// GL_FRONT_AND_BACK is the only thing you can pass here
					GL_LINE);			// GL_POINT, GL_LINE, or GL_FILL
				glDisable(GL_CULL_FACE);
			}
			else
			{	// "Regular" rendering: 
				glCullFace( GL_BACK );		// GL_FRONT, GL_BACK, or GL_FRONT_AND_BACK
				glEnable( GL_CULL_FACE );
				glPolygonMode(GL_FRONT_AND_BACK,	// GL_FRONT_AND_BACK is the only thing you can pass here
					GL_FILL);			// GL_POINT, GL_LINE, or GL_FILL
			}
			

			::g_pShaderManager->useShaderProgram("simple");

			glUniformMatrix4fv(locID_matModel, 1, GL_FALSE,
				(const GLfloat*)glm::value_ptr(matModel));

			// Setting the uniform colours
			glUniform3f(UniformLoc_ID_objectColour,
				pCurGameObject->solidColour.r, pCurGameObject->solidColour.g, pCurGameObject->solidColour.b);
			if (pCurGameObject->bIsWireframe)
			{
				glUniform1i(UniformLoc_ID_isWireframe, TRUE);		// 1
			}
			else
			{
				glUniform1i(UniformLoc_ID_isWireframe, FALSE);	// 0
			}

			// Drawing indirectly from the index buffer

			glBindVertexArray(VAO_ID);
			glDrawElements(GL_TRIANGLES,
				numberOfIndices,
				GL_UNSIGNED_INT,	// Each index is how big?? 
				(GLvoid*)0);		// Starting point in buffer
			glBindVertexArray(0);

		}// for ( int index = 0;.... (bottom of "render scene" loop)

		for (int index = 0; index != ::g_vec_hitMarkers.size(); index++)
		{
			cTriangleMesh* pCurGameObject = ::g_vec_hitMarkers[index];

			std::string meshModelName = pCurGameObject->meshName;

			GLuint VAO_ID = 0;
			int numberOfIndices = 0;
			float unitScale = 1.0f;
			if (!::g_pMeshTypeManager->LookUpMeshInfo(meshModelName,
				VAO_ID,
				numberOfIndices,
				unitScale))
			{	// Skip the rest of the for loop, but continue
				continue;
			}
			glm::mat4x4 matModel = glm::mat4x4(1.0f);
			
			matModel = glm::translate(matModel, pCurGameObject->position);

			matModel = glm::rotate(matModel, pCurGameObject->postRotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
			matModel = glm::rotate(matModel, pCurGameObject->postRotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
			matModel = glm::rotate(matModel, pCurGameObject->postRotation.z, glm::vec3(0.0f, 0.0f, 1.0f));

			float actualScale = pCurGameObject->scale * unitScale;

			matModel = glm::scale(matModel, glm::vec3(actualScale, actualScale, actualScale));


			if (pCurGameObject->bIsWireframe)
			{	// Turn off backface culling
				// Enable "wireframe" polygon mode
				glPolygonMode(GL_FRONT_AND_BACK,	// GL_FRONT_AND_BACK is the only thing you can pass here
					GL_LINE);			// GL_POINT, GL_LINE, or GL_FILL
				glDisable(GL_CULL_FACE);
			}
			else
			{	// "Regular" rendering: 
				glCullFace(GL_BACK);		// GL_FRONT, GL_BACK, or GL_FRONT_AND_BACK
				glEnable(GL_CULL_FACE);
				glPolygonMode(GL_FRONT_AND_BACK,	// GL_FRONT_AND_BACK is the only thing you can pass here
					GL_FILL);			// GL_POINT, GL_LINE, or GL_FILL
			}


			::g_pShaderManager->useShaderProgram("simple");

			glUniformMatrix4fv(locID_matModel, 1, GL_FALSE,
				(const GLfloat*)glm::value_ptr(matModel));

			// Setting the uniform colours
			glUniform3f(UniformLoc_ID_objectColour,
				pCurGameObject->solidColour.r, pCurGameObject->solidColour.g, pCurGameObject->solidColour.b);
			if (pCurGameObject->bIsWireframe)
			{
				glUniform1i(UniformLoc_ID_isWireframe, TRUE);		// 1
			}
			else
			{
				glUniform1i(UniformLoc_ID_isWireframe, FALSE);	// 0
			}

			// Drawing indirectly from the index buffer

			glBindVertexArray(VAO_ID);
			glDrawElements(GL_TRIANGLES,
				numberOfIndices,
				GL_UNSIGNED_INT,	// Each index is how big?? 
				(GLvoid*)0);		// Starting point in buffer
			glBindVertexArray(0);

		}
		 // End of Draw Scene

		 // Show or "present" what we drew...
		glfwSwapBuffers(gWindow);

		glfwPollEvents();
	}

	// Bye bye...
	delete ::g_pMeshTypeManager;
	delete ::g_pSceneLoader;
	delete ::g_pShaderManager;

	glfwDestroyWindow(gWindow);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}

float distanceBetweenPoints(glm::vec3 A, glm::vec3 B)
{
	float deltaX = A.x - B.x;
	float deltaY = A.y - B.y;
	float deltaZ = A.z - B.z;
	
	return sqrt(deltaX*deltaX + deltaY*deltaY + deltaZ*deltaZ);

	// ***************************************************
	// Note that glm already has a function for this:
	//return glm::distance( A, B );
	// ***************************************************
}

float distanceBetweenSpheres(cSphere* pBallA, cSphere* pBallB)
{
	float deltaX = pBallA->position.x - pBallB->position.x;
	float deltaY = pBallA->position.y - pBallB->position.y;
	float deltaZ = pBallA->position.z - pBallB->position.z;

	return sqrt(deltaX*deltaX + deltaY*deltaY + deltaZ*deltaZ);

}

// Narrow phase 
bool testSphereSphereCollision(cSphere* pBallA, cSphere* pBallB)
{
	float totalRadius = pBallA->radius + pBallB->radius;
	if (distanceBetweenSpheres(pBallA, pBallB) <= totalRadius)
	{
		return true;
	}
	return false;
}

void CollisionStep(float deltaTime)
{
	g_vec_Contacts.clear();

	// Brute force narrow phase detection

	// 1. For each ball, check for collision with other balls
	for (int indexOut = 0; indexOut != ::g_vec_pSpheres.size(); indexOut++)
	{
		cSphere* pOutBall = ::g_vec_pSpheres[indexOut];

		for (int indexIn = 0; indexIn != ::g_vec_pSpheres.size(); indexIn++)
		{
			cSphere* pInBall = ::g_vec_pSpheres[indexIn];

			// Is this the same object
			if (pOutBall != pInBall && pOutBall->radius != 0.0f && pInBall->radius != 0.0f)
			{	// No, so we're good to check

				pOutBall->solidColour.b = pOutBall->solidColour.g = pOutBall->solidColour.r = 1.0f;
				pInBall->solidColour.b = pInBall->solidColour.g = pInBall->solidColour.r = 1.0f;

				if (testSphereSphereCollision(pOutBall, pInBall))
				{
					glm::vec3 direction = pOutBall->position - pInBall->position;
					glm::vec3 contactPoint = pOutBall->position + direction * pOutBall->radius;
					glm::vec3 normal = glm::cross(direction, pOutBall->velocity);
					normal = glm::normalize(normal);

					glm::vec3 temp;					

					// Add to a vector of contact points...
					cContact curContact;
					curContact.normal = normal;
					curContact.contactXYZ = contactPoint;
					curContact.otherVelocity = pInBall->velocity;
					curContact.objectID = pOutBall->getID();
					curContact.collisionType = cContact::WITH_SPHERE;


					::g_vec_Contacts.push_back(curContact);

				}//if ( testSphereSphereCollision(pOutBall, pInBall) )
			}
		}//for ( int indexIn = 0; 
	}//for ( int indexOut 


	for (int index = 0; index != ::g_vec_pSpheres.size(); index++)
	{
		cSphere* pCurGO = ::g_vec_pSpheres[index];
		// Maybe check for zero...?
		if (pCurGO->groundCollisionFadeCounter <= 0.0f)
		{
			pCurGO->solidColour.r = 1.0f;
			pCurGO->solidColour.g = 1.0;
			pCurGO->solidColour.b = 1.0f;
		}
		else
		{	// Set it to the ground collision colour
			pCurGO->solidColour.r = 0.0f;
			pCurGO->solidColour.b = pCurGO->groundCollisionFadeCounter;
			pCurGO->solidColour.g = 0.0f;
			// Decrease this a little ("fade" it)
			pCurGO->groundCollisionFadeCounter -= 0.05f;
		}
	}//for ( int index...


	for (int ballIndex = 0; ballIndex != ::g_vec_pSpheres.capacity(); ballIndex++)
	{
		if (g_vec_pSpheres.size() == 0)
		{
			break;
		}
		cSphere* pBall = ::g_vec_pSpheres[ballIndex];

		for (int triMeshIndex = 0; triMeshIndex != g_vec_pTriangleMeshes.size(); triMeshIndex++)
		{
			std::vector< cPlyTriangle > meshTriInfo;
			meshTriInfo = g_vec_pTriangleMeshes[triMeshIndex]->physicsTris;	

		// Go through all the triangles, checking for collisions
			for (int triIndex = 0; triIndex != meshTriInfo.size(); triIndex++)
			{
				glm::vec3 ballCentre(pBall->position.x, pBall->position.y, pBall->position.z);

				glm::vec3 closestPoint = ClosestPtPointTriangle(ballCentre,
					meshTriInfo[triIndex].v1,
					meshTriInfo[triIndex].v2,
					meshTriInfo[triIndex].v3);
				// Is this closer than the radius of the ball?? 
				if (distanceBetweenPoints(ballCentre, closestPoint) < pBall->radius)
				{	
					glm::vec3 normal;
					g_pMeshTypeManager->getTriangleNormal(meshTriInfo, triIndex, normal);
					cContact curContact;
					curContact.contactXYZ = closestPoint;
					curContact.normal = normal;
					curContact.objectID = g_vec_pTriangleMeshes[triMeshIndex]->getID();
					curContact.sphereID = pBall->getID();
					curContact.collisionType = cContact::WITH_TRIANGLE;
					createHitMarker(closestPoint);
					deleteSphereByID(pBall->getID());

					::g_vec_Contacts.push_back(curContact);
					 
					// Set the ground collision fade to something
					pBall->groundCollisionFadeCounter = 1.0f;
				}


			}// for ( int triIndex = 0...
		}
	}// for ( int ballIndex = 0...

	
	return;
}
//
void PhysicsStep(float deltaTime)		
{
	glm::vec3 gravityForce(-5.0f, -15.f, -5.0f);

	lastShot += deltaTime;

	for (int index = 0; index != ::g_vec_pSpheres.size(); index++)
	{
		// each second 
		cSphere* pCurGO = ::g_vec_pSpheres[index];

		if (pCurGO->bIsUpdatedByPhysics)
		{

			pCurGO->velocity.x += (pCurGO->acceleration.x + gravityForce.x) * deltaTime;
			pCurGO->velocity.y += (pCurGO->acceleration.y + gravityForce.y) * deltaTime;
			pCurGO->velocity.z += (pCurGO->acceleration.z + gravityForce.z) * deltaTime;

			pCurGO->lastPosition.x = pCurGO->position.x;
			pCurGO->lastPosition.y = pCurGO->position.y;
			pCurGO->lastPosition.z = pCurGO->position.z;

			// Update position based on velocity
			pCurGO->position.x += pCurGO->velocity.x * deltaTime;
			pCurGO->position.y += pCurGO->velocity.y * deltaTime;
			pCurGO->position.z += pCurGO->velocity.z * deltaTime;

		}//if ( pCurShip->bIsUpdatedByPhysics )
	}

	for (int index = 0; index != ::g_vec_pTriangleMeshes.size(); index++)
	{
		// each second 
		cTriangleMesh* pCurGO = ::g_vec_pTriangleMeshes[index];

		if (pCurGO->bIsUpdatedByPhysics)
		{
			pCurGO->velocity.x += (pCurGO->acceleration.x ) * deltaTime;
			pCurGO->velocity.y += (pCurGO->acceleration.y ) * deltaTime;
			pCurGO->velocity.z += (pCurGO->acceleration.z ) * deltaTime;

			pCurGO->lastPosition.x = pCurGO->position.x;
			pCurGO->lastPosition.y = pCurGO->position.y;
			pCurGO->lastPosition.z = pCurGO->position.z;

			// Update position based on velocity
			pCurGO->position.x += pCurGO->velocity.x * deltaTime;
			pCurGO->position.y += pCurGO->velocity.y * deltaTime;
			pCurGO->position.z += pCurGO->velocity.z * deltaTime;

			if (pCurGO->position.x > 480)
			{
				pCurGO->position = glm::vec3(-480, pCurGO->position.y, -420);
			}
			
		}//if ( pCurShip->bIsUpdatedByPhysics )
	}

	return;
}

void highlightTurrets()
{
	cTriangleMesh* p_yWing = findTurretByID(g_yWingID);

	if (!p_yWing)
	{
		return;
	}

	//loop through all turrets
	//check distance from ywing
	//keep vector of closest 10
	//change the colour of closest 10
	std::vector<int>closestIDs;
	if(g_vec_pTriangleMeshes.size() > 102)
	g_vec_pTriangleMeshes.erase(g_vec_pTriangleMeshes.begin() + 102, g_vec_pTriangleMeshes.begin() + 112);
	float closestDistance = 100000;
	float tempDistance = 0;
	bool alreadyFound = false;
	for (int outIndex = 0; outIndex != 10; outIndex++)
	{
		closestIDs.push_back(-1);
		float closestDistance = 100000;
		for (int index = 2; index != 102; index++)
		{
			alreadyFound = false;
			cTriangleMesh* curTurret = g_vec_pTriangleMeshes[index];
			for (int checkIndex = 0; checkIndex != closestIDs.size(); checkIndex++)
			{
				if (curTurret->getID() == closestIDs[checkIndex])
				{
					alreadyFound = true;
				}
			}
			if (alreadyFound)
			{
				continue;
			}
			//calculate distance
			tempDistance = glm::distance(p_yWing->position, curTurret->position);
			if (tempDistance < closestDistance)
			{
				closestDistance = tempDistance;
				closestIDs[outIndex] = curTurret->getID();
			}
		}
	}

	for (int index = 0; index != closestIDs.size(); index++)
	{
		cTriangleMesh* highlightSphere = new cTriangleMesh();
		cTriangleMesh* curTurret = findTurretByID(closestIDs[index]);
		highlightSphere->position = curTurret->position;
		highlightSphere->meshName = "Sphere_N.ply";
		if (index == 0)
		{
			highlightSphere->solidColour.r = 0.1f;
			highlightSphere->solidColour.g = 1.0f;
			highlightSphere->solidColour.b = 0.1f;
		}
		else
		{
			highlightSphere->solidColour.r = 1.0f;
			highlightSphere->solidColour.g = 0.1f;
			highlightSphere->solidColour.b = 1.0f;
		}
		highlightSphere->scale = 10.f;

		g_vec_pTriangleMeshes.push_back(highlightSphere);
	}
	return;
}

void bombAway()
{

	cTriangleMesh* p_yWing = findTurretByID(g_yWingID);
	cSphere* bomb = new cSphere();
	bomb->meshName = "Sphere_N.ply";
	bomb->scale = 2;
	bomb->radius = 1;
	bomb->solidColour = glm::vec3(1.0, 1.0, 1.0);
	bomb->position = glm::vec3(p_yWing->position.x, p_yWing->position.y - 2.5, p_yWing->position.z);
	bomb->velocity = p_yWing->velocity;
	bomb->bIsUpdatedByPhysics = true;
	bomb->bIsWireframe = false;
	g_vec_pSpheres.push_back(bomb);
}

void createHitMarker(glm::vec3 contactPoint)
{
	cTriangleMesh* hitSphere = new cTriangleMesh();
	hitSphere->position = contactPoint;
	hitSphere->meshName = "Sphere_N.ply";
	hitSphere->solidColour.r = 1.0f;
	hitSphere->solidColour.g = 1.0f;
	hitSphere->solidColour.b = 1.0f;
	hitSphere->scale = 1.f;

	g_vec_hitMarkers.push_back(hitSphere);
	return;
}

void deleteSphereByID(unsigned int ID)
{
	for (int index = 0; index != g_vec_pSpheres.capacity(); index++)
	{
		if (g_vec_pSpheres[index]->getID() == ID)
		{
			g_vec_pSpheres.erase(g_vec_pSpheres.begin() + index);
			
		}
	}
	g_vec_pSpheres.shrink_to_fit();
	return;
}