#include "vertex.h"
#include "face.h"
#include "halfedge.h"
#include <vector>
using namespace std;

class Mesh {

private:


public:

	//Mansha<----
	string* filename;
	vector<Vertex*> createBoundingBox(int i, int j, int k);
	Vertex* P0; //min point in the bounding box

	float minXValue;
	float minYValue;
	float minZValue;

	float maxXValue;
	float maxYValue;
	float maxZValue;
	
	//---->
	vector<Vertex*>* verts;
	float* vertices;
	vector<Face*>* faces;
	vector<HalfEdge*>* halfedges;
	float* normals;
	float* colors;
	unsigned short* indices;
	int getTotalVerts();
	int getTotalInds();
	vector<int> getVertAdjFaces(Vertex* ver, vector<Face*>* faceVec);
	vector<HalfEdge*> getVertAdjEdges(Vertex* ver, vector<HalfEdge*> halfVec);
	void fixPlanarity(Vertex* vert);
	Mesh();
	vector<string> namesOfMaterials;
	vector<float> materialRed;
	vector<float> materialGreen;
	vector<float> materialBlue;
	vector<float> materialAlpha;
	void objFileStructure2(const char*);
	Mesh* objFileStructure3(const char* filename);
	void saveObjFile(const char*);
	static Mesh* Mesh::objFileStructure();
	Face* makeFace(Vertex* v1, Vertex* v2, Vertex* v3, Vertex* v4, float r, float g, float b, QString* ID);
	static Mesh* unitCube();
	bool containsOrSym(HalfEdge*, vector<HalfEdge*>);
	void moveVertex(Vertex* vert, float newX, float newY, float newZ);
	HalfEdge* addVertex(HalfEdge* HE0);
	HalfEdge* addEdge(Face* FA, Face* FB, HalfEdge* HEA0, HalfEdge* HEB0);
	Face* splitQuad(HalfEdge* HE0);
	void deleteVertex(HalfEdge* he);
	void subdivideMesh();
	static void connectFaces(HalfEdge* he1, HalfEdge* he2);
	void generateVertArray();
	void generateColorArray();
	void generateIndArray();
	void generateNormArray();
	Face* findIntersectedFace(glm::vec3 point, glm::vec3 ray);
};