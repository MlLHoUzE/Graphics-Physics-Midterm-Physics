#include "cMeshTypeManager.h"

#include <vector>
#include <fstream>

#include <glad/glad.h>			// OpenGL defines...

struct vert_XYZ_RGB_N
{
	float x, y, z;				// 4 + 4 + 4
	float r, g, b;			// 4 + 4 + 4  = 24 bytes
	float nX, nY, nZ;
};

std::map<std::string, std::vector<cPlyTriangle> > m_mapMeshNameToTriInfo;


cMeshTypeManager::cMeshTypeManager()
{
	return;
}

cMeshTypeManager::~cMeshTypeManager()
{
	return;
}

// Loads file from disk, AND
// Copies inforamtion into VAO (Vertex Array Object)
bool cMeshTypeManager::LoadPlyFileIntoGLBuffer(unsigned int programID, std::string plyFile, bool bKeepTriangleInfoAround)
{
	// ********************************************************************
	// *** START OF: Copying object data into VAO (vertex array object)

	// Here is where the model is loaded into the vertex buffer
	std::vector< cPlyVertexXYZ_N > vec_PlyVerts;
	std::vector< cPlyTriFace > vec_PlyIndices;

	if (!this->m_loadPlyModel(plyFile, vec_PlyVerts, vec_PlyIndices))
	{
		return false;
	}

	// Calculate the maximum extent (max-min) for x,y,z
	float minX, minY, minZ;
	float maxX, maxY, maxZ;
	// Set the initial values to the 1st vertex.
	minX = maxX = vec_PlyVerts[0].x;
	minY = maxY = vec_PlyVerts[0].y;
	minZ = maxZ = vec_PlyVerts[0].z;

	for (int index = 0; index != vec_PlyVerts.size(); index++)
	{
		if (vec_PlyVerts[index].x < minX) { minX = vec_PlyVerts[index].x; }
		if (vec_PlyVerts[index].y < minY) { minY = vec_PlyVerts[index].y; }
		if (vec_PlyVerts[index].z < minZ) { minZ = vec_PlyVerts[index].z; }

		if (vec_PlyVerts[index].x > maxX) { maxX = vec_PlyVerts[index].x; }
		if (vec_PlyVerts[index].y > maxY) { maxY = vec_PlyVerts[index].y; }
		if (vec_PlyVerts[index].z > maxZ) { maxZ = vec_PlyVerts[index].z; }
	}
	// What the max extent
	float extentX = maxX - minX;
	float extentY = maxY - minY;
	float extentZ = maxZ - minZ;

	float maxExtent = extentX;
	if (extentY > maxExtent) { maxExtent = extentY; }
	if (extentZ > maxExtent) { maxExtent = extentZ; }


	// vert_XYZ_RGB for my Model
	int numberofVerts = vec_PlyVerts.size();
	vert_XYZ_RGB_N* p_vertArray = new vert_XYZ_RGB_N[numberofVerts];

	for (int index = 0; index != numberofVerts; index++)
	{
		p_vertArray[index].x = vec_PlyVerts[index].x;
		p_vertArray[index].y = vec_PlyVerts[index].y;
		p_vertArray[index].z = vec_PlyVerts[index].z;

		p_vertArray[index].nX = vec_PlyVerts[index].nX;
		p_vertArray[index].nY = vec_PlyVerts[index].nY;
		p_vertArray[index].nZ = vec_PlyVerts[index].nZ;
	}


	// Copy data in... 

	// Create a vertex array object...
	cMeshTypeManager::VAOInfo tempMeshVAOInfo;
	tempMeshVAOInfo.meshName = plyFile;
	// Save the unit scale value
	tempMeshVAOInfo.unitScaleValue = 1.0f / maxExtent;  //0.01f


	glGenVertexArrays(1, &(tempMeshVAOInfo.VAO_ID));
	// Refer to specific vertex array buffer
	glBindVertexArray(tempMeshVAOInfo.VAO_ID);

	GLuint vertex_buffer = 0;
	GLuint index_buffer = 0;

	// Create a vertex buffer
	glGenBuffers(1, &vertex_buffer);
	// Brings the particular vertex buffer into context
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);

	int sizeInBytes = sizeof(vert_XYZ_RGB_N) * numberofVerts;

	glBufferData(GL_ARRAY_BUFFER,
		sizeInBytes,
		p_vertArray,			// vertices
		GL_STATIC_DRAW);

	// Generate an "index" buffer
	glGenBuffers(1, &index_buffer);
	// Brings the particular vertex buffer into context
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);

	// Copy the index information into the index buffer
	int numberOfTriangles = vec_PlyIndices.size();
	//	int numberOfIndices = numberOfTriangles * 3;
	tempMeshVAOInfo.numberOfIndices = numberOfTriangles * 3;
	int* pIndexArray = new int[tempMeshVAOInfo.numberOfIndices];


	int indexBufferIndex = 0;
	for (int triIndex = 0;
	triIndex != vec_PlyIndices.size();
		triIndex++, indexBufferIndex += 3)
	{
		pIndexArray[indexBufferIndex + 0] = vec_PlyIndices[triIndex].v1;		// Index 0
		pIndexArray[indexBufferIndex + 1] = vec_PlyIndices[triIndex].v2;		// Index 0
		pIndexArray[indexBufferIndex + 2] = vec_PlyIndices[triIndex].v3;		// Index 0
	}

	int indexBufferArraySizeInBytes = tempMeshVAOInfo.numberOfIndices * sizeof(int);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
		indexBufferArraySizeInBytes,
		pIndexArray,			// index array
		GL_STATIC_DRAW);



	// Bind the vertex attributes to the shader
	int sizeOfVertInBytes = sizeof(vert_XYZ_RGB_N);

	// Telling the shader where the vertex info is...
	GLuint vpos_location = glGetAttribLocation(programID, "vPosition");	// 8
	//GLuint vcol_location = glGetAttribLocation(programID, "vColour");	// 24
	GLuint vnorm_location = glGetAttribLocation(programID, "vNormal");	// 24

	int offsetInBytesToPosition = offsetof(vert_XYZ_RGB_N, x);
	glEnableVertexAttribArray(vpos_location);
	glVertexAttribPointer(vpos_location, 3,
		GL_FLOAT, GL_FALSE,
		sizeOfVertInBytes,	// sizeof(float) * 6,		// 
		(void*)offsetInBytesToPosition);

	//int offsetInBytesToColour = offsetof(vert_XYZ_RGB_N, r);
	//glEnableVertexAttribArray(vcol_location);
	//glVertexAttribPointer(vcol_location, 3, GL_FLOAT, GL_FALSE,
	//	sizeOfVertInBytes,	// sizeof(float) * 5, 
	//	(void*)offsetInBytesToColour);

	int offsetInBytesToNorm = offsetof(vert_XYZ_RGB_N, nX);
	glEnableVertexAttribArray(vnorm_location);
	glVertexAttribPointer(vnorm_location, 3, GL_FLOAT, GL_FALSE,
		sizeOfVertInBytes,	// sizeof(float) * 5, 
		(void*)offsetInBytesToNorm);


	// Data is copied into the vertex (and index) buffers, so delete array(s)
	delete[] p_vertArray;
	delete[] pIndexArray;

	// Save that VAO info for later

	this->m_MapMeshNameToVAOInfo[plyFile] = tempMeshVAOInfo;


	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// ********************************************************************
	// *** END OF: Copying object data into VAO (vertex array object)
	// ********************************************************************

	if (bKeepTriangleInfoAround)
	{
		this->m_GenerateTriangleInfoFromIndexedVectors(plyFile, vec_PlyVerts, vec_PlyIndices);
	}

	return true;
}

bool cMeshTypeManager::LookUpMeshInfo(std::string meshName,
	unsigned int &VAO_ID,	    // GLuint by ref
	int &numberOfIndices,
	float &unitScale)
{
	// Perform a lookup into the map
	std::map< std::string, cMeshTypeManager::VAOInfo >::iterator itVAO
		= this->m_MapMeshNameToVAOInfo.find(meshName);

	// If the iterator ISN'T set to end(), then it found it
	if (itVAO == this->m_MapMeshNameToVAOInfo.end())
	{	// Didn't find
		return false;
	}

	VAO_ID = itVAO->second.VAO_ID;
	numberOfIndices = itVAO->second.numberOfIndices;
	unitScale = itVAO->second.unitScaleValue;

	return true;
}

bool cMeshTypeManager::m_loadPlyModel(std::string filename,
	std::vector< cPlyVertexXYZ_N > &vecPlyVerts,
	std::vector< cPlyTriFace > &vecPlyIndices)
{
	std::ifstream theFile(filename.c_str());
	if (!theFile.is_open())
	{
		return false;
	}

	// Assume formant is only xyz with no other vertex attribs

	// Read until we get to the text "vertex" 
	std::string temp;
	bool bKeepReading = true;
	while (bKeepReading)
	{
		theFile >> temp;
		// "vertex"?
		if (temp == "vertex")
		{
			bKeepReading = false;
		}
	}
	// Next value is the number of vertices
	int numVertices = 0;
	theFile >> numVertices;

	// Read until we get to the text "face"
	bKeepReading = true;
	while (bKeepReading)
	{
		theFile >> temp;
		// "face"?
		if (temp == "face")
		{
			bKeepReading = false;
		}
	}
	// Next value is the number of faces (aka "triangles")
	int numberOfTriangles = 0;
	theFile >> numberOfTriangles;

	// Read until we get to "end_header"
	bKeepReading = true;
	while (bKeepReading)
	{
		theFile >> temp;
		// "end_header"?
		if (temp == "end_header")
		{
			bKeepReading = false;
		}
	}

	// Now read all the vertices
	for (int index = 0; index != numVertices; index++)
	{
		cPlyVertexXYZ_N tempVert;
		theFile >> tempVert.x >> tempVert.y >> tempVert.z;
		theFile >> tempVert.nX >> tempVert.nY >> tempVert.nZ;
		// add to the temporary vector
		vecPlyVerts.push_back(tempVert);
	}

	// Now the indices...
	for (int index = 0; index != numberOfTriangles; index++)
	{
		cPlyTriFace tempTri;
		int numIndicesDiscard = 0;		// 3
		theFile >> numIndicesDiscard >> tempTri.v1 >> tempTri.v2 >> tempTri.v3;
		vecPlyIndices.push_back(tempTri);
	}

	// All done.

	return true;
}

bool cMeshTypeManager::m_GenerateTriangleInfoFromIndexedVectors(std::string fileName, std::vector<cPlyVertexXYZ_N> &vecPlyVerts, std::vector<cPlyTriFace> &vecPlyIndices)
{
	//create a vector of triangles for us to load
	std::vector<cPlyTriangle> vecTriangles;
	
	//Allocate space for the entire set of triangles
	//faster due to not having to re-allocate for each created triangle
	vecTriangles.reserve(vecPlyIndices.size());

	//Now place a bunch of "empty" triangles
	int numberOfTriangles = vecPlyIndices.size();
	cPlyTriangle tempTri;
	for (int count = 0; count != numberOfTriangles; count++)
	{
		vecTriangles.push_back(tempTri);
	}

	//treating the vector as an array we assign the triangles by index
	for (int index = 0; index != numberOfTriangles; index++)
	{
		//look up vertices from the triangle info (verbose way)
		//int triVert1_X_index = vecPlyIndices[index].v1;				//get the vertex index from the triangle
		//float vertex1_X_value = vecPlyVerts[triVert1_X_index].x;	//get vertex value using the vertex index
		//vecTriangles[index].v1.x = vertex1_X_value;					//assign it

		//one step way
		vecTriangles[index].v1.x = vecPlyVerts[vecPlyIndices[index].v1].x;
		vecTriangles[index].v1.y = vecPlyVerts[vecPlyIndices[index].v1].y;
		vecTriangles[index].v1.z = vecPlyVerts[vecPlyIndices[index].v1].z;

		vecTriangles[index].v2.x = vecPlyVerts[vecPlyIndices[index].v2].x;
		vecTriangles[index].v2.y = vecPlyVerts[vecPlyIndices[index].v2].y;
		vecTriangles[index].v2.z = vecPlyVerts[vecPlyIndices[index].v2].z;

		vecTriangles[index].v3.x = vecPlyVerts[vecPlyIndices[index].v3].x;
		vecTriangles[index].v3.y = vecPlyVerts[vecPlyIndices[index].v3].y;
		vecTriangles[index].v3.z = vecPlyVerts[vecPlyIndices[index].v3].z;
	}//for(int index = 0.....

	//save it in the map, looked up by mesh info

	this->m_mapMeshNameToTriInfo[fileName] = vecTriangles;
	

	return true;
}

bool cMeshTypeManager::getTriangleVertexInformation(std::string meshName, std::vector<cPlyTriangle> &vecTris)
{
	//look for it...
	std::map<std::string, std::vector<cPlyTriangle> >::iterator itMeshTri = this->m_mapMeshNameToTriInfo.find(meshName);

	//...found it
	if (itMeshTri == this->m_mapMeshNameToTriInfo.end())
	{
		//nope 
		return false;
	}

	//found it, so return a reference
	//first is the mesh name
	//second if the thing the map stores, so the vector in this case.
	vecTris = itMeshTri->second;

	return true;
}

void cMeshTypeManager::getTriangleNormal(std::vector<cPlyTriangle> &vecTris, int triIndex, glm::vec3 &normal)
{
	
	glm::vec3 triangleSide1 = glm::vec3(vecTris[triIndex].v2.x - vecTris[triIndex].v1.x, vecTris[triIndex].v2.y - vecTris[triIndex].v1.y, vecTris[triIndex].v2.z - vecTris[triIndex].v1.z);
	glm::vec3 triangleSide2 = glm::vec3(vecTris[triIndex].v3.x - vecTris[triIndex].v1.x, vecTris[triIndex].v3.y - vecTris[triIndex].v1.y, vecTris[triIndex].v3.z - vecTris[triIndex].v1.z);

	normal = glm::cross(triangleSide1, triangleSide2);
	normal = glm::normalize(normal);
}