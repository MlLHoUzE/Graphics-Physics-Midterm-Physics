#ifndef _cMeshTypeManager_HG_
#define _cMeshTypeManager_HG_

#include <string>
#include <vector>
#include <map>
#include <glm/glm.hpp>
#include "Utilities.h"

class cMeshTypeManager
{
public:
	cMeshTypeManager();
	~cMeshTypeManager();

	// Loads file from disk, AND
	// Copies inforamtion into VAO (Vertex Array Object)
	bool LoadPlyFileIntoGLBuffer(unsigned int programID, std::string plyFile, bool bKeepTriangleInfoAround);

	bool LookUpMeshInfo(std::string meshName,
		unsigned int &VAO_ID,	    // GLuint by ref
		int &numberOfIndices,
		float &unitScale);		// by ref

	bool getTriangleVertexInformation(std::string meshName, std::vector<cPlyTriangle> &vecTris);

	void getTriangleNormal(std::vector<cPlyTriangle> &vecTris, int triIndex, glm::vec3 &normal);

private:
	bool m_loadPlyModel(std::string filename,
		std::vector< cPlyVertexXYZ_N > &vecPlyVerts,
		std::vector< cPlyTriFace > &vecPlyIndices);

	struct VAOInfo
	{
		VAOInfo() : VAO_ID(0), numberOfIndices(0), unitScaleValue(1.0f) {}
		std::string meshName;
		unsigned int VAO_ID;
		unsigned int numberOfIndices;
		float unitScaleValue;		// Multiply by this scale and object 1.0f
	};
	// Map... aka "dictionay" "look up table"
	std::map< std::string, VAOInfo > m_MapMeshNameToVAOInfo;

	bool m_GenerateTriangleInfoFromIndexedVectors(std::string fileName, std::vector<cPlyVertexXYZ_N> &vecPlyVerts, std::vector<cPlyTriFace> &vecPlyIndices);


	
	std::map<std::string, std::vector<cPlyTriangle> > m_mapMeshNameToTriInfo;
};

#endif