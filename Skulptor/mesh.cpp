	#include "mesh.h"
	#include <iostream>
#include <fstream>
#include <sstream>
#include <map>

	HalfEdge* Mesh::addVertex(HalfEdge* HE0) {

		Vertex* VN = new Vertex(0, 0, 0, new QString(QString::number(verts->size())));
		HE0 = HE0->getPrev();
		HalfEdge* HE1 = HE0->next;
		HalfEdge* HE2 = HE1->sym;
		if (HE1 == NULL || HE2 == NULL) {
			return NULL;
		}

		HalfEdge* HENL = new HalfEdge();
		HalfEdge* HENR = new HalfEdge();

		HENL->next = HE1;
		HE0->next = HENL;
		HENR->next = HE2->next;
		HE2->next = HENR;

		HENL->sym = HENR;
		HENR->sym = HENL;

		HENL->f = HE0->f;
		HENR->f = HE2->f;

		HENL->v = VN;
		HENR->v = HE2->v;
		HE2->v = VN;

		*VN->xyz = (*HE1->v->xyz + *HE0->v->xyz)/2.0f;

		halfedges->push_back(HENR);
		return HENL;

	}

	HalfEdge* Mesh::addEdge(Face* FA, Face* FB, HalfEdge* HEA0, HalfEdge* HEB0) {

		if (HEB0->v != HEA0->v) { 
			std::cout << "addEdge unsuccessful" << std::endl;
			return HEA0;
		}

		//What is this supposed to be?
		Vertex* V1 = new Vertex(HEA0->v->xyz->x, HEA0->v->xyz->y, HEA0->v->xyz->z, new QString("56095"));
		verts->push_back(V1);

		HalfEdge* HENL = new HalfEdge();
		HalfEdge* HENR = new HalfEdge();

		HENL->next = HEA0->next;
		HEA0->next = HENL;
		HENL->v = V1;
		HENL->f = FA;
		HENL->sym = HENR;

		HENR->next = HEB0->next;
		HEB0->next = HENR;
		HENR->v = HEA0->v;
		HEB0->v = V1;
		HENR->f = FB;
		HENR->sym = HENL;
		halfedges->push_back(HENR);
		halfedges->push_back(HENL);

		return HENL;
	}

	void Mesh::moveVertex(Vertex* vert, float newX, float newY, float newZ) {
		vert->xyz = new glm::vec3(newX, newY, newZ);
		fixPlanarity(vert);
	}

	void Mesh::fixPlanarity(Vertex* vert) {
		vector<HalfEdge*> edgesToCheck;
		Face* f;
		HalfEdge* he;
		for (int i = 0; i < faces->size(); i++) {
			f = faces->at(i);
			he = f->first;
			while (true) {
				if (he->v == vert) {
					edgesToCheck.push_back(he);
					break;
				}
				if (he->next == f->first) {
					break;
				}
				he = he->next;
			}
		}
		for (int j = 0; j < edgesToCheck.size(); j++) {
			HalfEdge* toCheck = edgesToCheck.at(j);
			if (!toCheck->f->isPlanar()) {
				splitQuad(toCheck->next);
			}
		}
	}

	Face* Mesh::splitQuad(HalfEdge* HE0) {

		Face* FACE1 = HE0->f;
		FACE1->deselect();
		HE0 = FACE1->first;
		if (FACE1->numEdges() < 4) {
			return FACE1;
		}

		QString* q = new QString(QString::number(faces->size() + 1));
		Face* FACE2 = new Face(NULL, FACE1->color->x, FACE1->color->y, FACE1->color->z, q);

		HalfEdge* HEL = new HalfEdge();
		HalfEdge* HER = new HalfEdge();

		HEL->f = FACE1;
		HER->f = FACE2;
		FACE2->first = HER;

		HEL->sym = HER;
		HER->sym = HEL;

		HEL->v = HE0->next->next->v;
		HEL->next = HE0->next->next->next;
		HER->next = HE0->next;
		HER->next->next->next = HER;
		HE0->next = HEL;
		HER->v = HE0->v;

		halfedges->push_back(HEL);
		halfedges->push_back(HER);
		faces->push_back(FACE2);

		if (!FACE2->isPlanar()) {
			splitQuad(FACE2->first);
		}
		if (!FACE1->isPlanar()) {
			splitQuad(FACE1->first);
		}
		return FACE2;
	}

	Mesh::Mesh() {
		faces = new vector<Face*>();
		verts = new vector<Vertex*>();
		halfedges = new vector<HalfEdge*>();
		vertices = NULL;
		colors = NULL;
		indices = NULL;
		normals = NULL;
		filename = NULL;
	}

	void Mesh::connectFaces(HalfEdge* he1, HalfEdge* he2) {
		if (he1 == NULL || he2 == NULL) {
			return;
		}
		if (he1->v != he2->getPrev()->v) {
			std::cout << "Faces not connected" << std::endl;
			return;
		}
		he1->sym = he2;
		he2->sym = he1;
	}

	Mesh* Mesh::unitCube() {
		Mesh* cube = new Mesh();

		Vertex* v1 = new Vertex(1, 1, 1, new QString("0"));
		Vertex* v2 = new Vertex(-1, 1, 1, new QString("1"));
		Vertex* v3 = new Vertex(-1, -1, 1, new QString("2"));
		Vertex* v4 = new Vertex(1, -1, 1, new QString("3"));
		Vertex* v5 = new Vertex(1, -1, -1, new QString("4"));
		Vertex* v6 = new Vertex(1, 1, -1, new QString("5"));
		Vertex* v7 = new Vertex(-1, 1, -1, new QString("6"));
		Vertex* v8 = new Vertex(-1, -1, -1, new QString("7"));

		cube->verts->push_back(v8);
		cube->verts->push_back(v1);
		cube->verts->push_back(v2);
		cube->verts->push_back(v3);
		cube->verts->push_back(v4);
		cube->verts->push_back(v5);
		cube->verts->push_back(v6);
		cube->verts->push_back(v7);

		Face* f1 = cube->makeFace(v1, v2, v3, v4, 0, 0.4f, 0, new QString("1"));
		Face* f2 = cube->makeFace(v1, v4, v5, v6, 0.6f, 0.4f, 0, new QString("2"));
		Face* f3 = cube->makeFace(v1, v6, v7, v2, 0, 0.4f, 0.6f, new QString("3"));
		Face* f4 = cube->makeFace(v2, v7, v8, v3, 0, 0.8f, 0, new QString("4"));
		Face* f5 = cube->makeFace(v8, v5, v4, v3, 0.5f, 0.0f, 0.76f, new QString("5"));
		Face* f6 = cube->makeFace(v5, v8, v7, v6, 0.6f, 0.4f, 0.2f, new QString("6"));

		connectFaces(f1->first, f2->first->next);
		connectFaces(f1->first->next, f3->first);
		connectFaces(f1->first->next->next, f4->first);
		connectFaces(f1->first->getPrev(), f5->first->getPrev());
		connectFaces(f2->first, f3->first->next);
		connectFaces(f2->first->next->next, f5->first->next->next);
		connectFaces(f2->first->getPrev(), f6->first);
		connectFaces(f3->first->getPrev(), f4->first->next);
		connectFaces(f3->first->next->next, f6->first->getPrev());
		connectFaces(f4->first->getPrev(), f5->first);
		connectFaces(f4->first->next->next, f6->first->next->next);
		connectFaces(f5->first->next, f6->first->next);

		cube->faces->push_back(f1);
		cube->faces->push_back(f2);
		cube->faces->push_back(f3);
		cube->faces->push_back(f4);
		cube->faces->push_back(f5);
		cube->faces->push_back(f6);

		cube->generateVertArray();
		cube->generateNormArray();
		cube->generateColorArray();
		cube->generateIndArray();

		return cube;

	}

	Face* Mesh::findIntersectedFace(glm::vec3 point, glm::vec3 ray) {
			Face* shortest = NULL;
			for (int i = 0; i < faces->size(); i++) {
				if (faces->at(i)->intersectsWith(point, ray) != -1) {
					if (shortest == NULL) {
						shortest = faces->at(i);
						continue;
					}
					if (faces->at(i)->intersectsWith(point, ray) > shortest->intersectsWith(point, ray)) {
						shortest = faces->at(i);
					}
				}
			}
			return shortest;
	}

	void Mesh::generateVertArray() {
		if (vertices != NULL) {
			delete [] vertices;
			vertices = 0;
		}
		vertices = new float[getTotalVerts() * 4];
		int j = 0;
		HalfEdge* he = NULL;
		float* x;
		float* y;
		float* z;
		float* h;
		for (int i = 0; i < faces->size(); i++) {
			he = faces->at(i)->first;
			while (true) {
				x = new float(he->v->getX());
				vertices[j] = *x;
				delete x;
				j++;
				y = new float(he->v->getY());
				vertices[j] = *y;
				delete y;
				j++;
				z = new float(he->v->getZ());
				vertices[j] = *z;
				delete z;
				j++;
				h = new float(1.0f);
				vertices[j] = *h;
				delete h;
				j++;
				he = he->next;
				if (he == faces->at(i)->first) {
					break;
				}
			}
		}
	}

	int Mesh::getTotalVerts() {
		int total = 0;
		for (int n = 0; n < faces->size(); n++) {
			total += faces->at(n)->numEdges();
		}
		return total;
	}

	void Mesh::generateNormArray() {
		if (normals != NULL) {
			delete normals;
			normals = 0;
		}
		normals = new float[getTotalVerts() * 4];
		int j = 0;
		HalfEdge* he = NULL;
		float* normX;
		float* normY;
		float* normZ;
		float* normH;
		Vertex* v;
		glm::vec3 norm;
		for (int i = 0; i < faces->size(); i++) {
			he = faces->at(i)->first;
			v = he->v;
			while (true) {
				norm = he->getNormAtVert();
				normX = new float(norm.x);
				normals[j] = *normX;
				delete normX;
				j++;
				normY = new float(norm.y);
				normals[j] = *normY;
				delete normY;
				j++;
				normZ = new float(norm.z);
				normals[j] = *normZ;
				delete normZ;
				j++;
				normH = new float(0.0f);
				normals[j] = *normH;
				delete normH;
				j++;
				he = he->next;
				if (he == faces->at(i)->first) {
					break;
				}
			}
		}
	}

	void Mesh::generateColorArray() {
		if (colors != NULL) {
			delete [] colors;
			colors = 0;
		}
		colors = new float[getTotalVerts() * 4];
		int j = 0;
		float *x;
		float *y;
		float* z;
		float* a;
		for (int i = 0; i < faces->size(); i++) {
			for (int k = 0; k < faces->at(i)->numEdges(); k++) {
				x = new float(faces->at(i)->color->x);
				colors[j] = *x;
				delete x;
				j++;
				y = new float(faces->at(i)->color->y);
				colors[j] = *y;
				delete y;
				j++;
				z = new float(faces->at(i)->color->z);
				colors[j] = *z;
				delete z;
				j++;
				a = new float(1.0f);
				colors[j] = *a;
				delete a;
				j++;
			}
		}
	}

	int Mesh::getTotalInds() {
		int total = 0;
		for (int i = 0; i < faces->size(); i++) {
			total += ((faces->at(i)->numEdges() - 2) * 3);
		}
		return total;
	}

	void Mesh::generateIndArray() {
		if (indices != NULL) {
			delete [] indices;
			indices = 0;
		}
		indices = new unsigned short[getTotalInds()];
		int k = 0;
		int totalVerts = 0;
		unsigned short* x;
		unsigned short* y;
		unsigned short* z;
		HalfEdge* he;
		HalfEdge* he2;
		for (int i = 0; i < faces->size(); i++) {
			he = faces->at(i)->first;
			he2 = he->next;
			int n = 1;
			while(he2->next != he) {
				x = new unsigned short(totalVerts);
				indices[k] = *x;
				delete x;
				k++;
				y = new unsigned short(n + totalVerts);
				indices[k] = *y;
				delete y;
				k++;
				z = new unsigned short(n + 1 + totalVerts);
				indices[k] = *z;
				delete z;
				k++;
				n++;
				he2 = he2->next;
			}
			totalVerts += faces->at(i)->numEdges();
		}
	}

	void Mesh::deleteVertex(HalfEdge* he) {
		if (he == NULL) {
			return;
		}
		Vertex* ve = he->v;
		vector<int> vec;
		int x = faces->size();
		for (int i = 0; i < x; i++) {
			if (faces->at(i)->containsVert(he->v)) {
				vec.push_back(i);
			}
		}
		for (int k = 0; k < vec.size(); k++) {
			int x = vec.at(k) - k;
			if (x < 0 || x >= faces->size()) {
				continue;
			}
			faces->erase(faces->begin() + x);
		}

	}

	Face* Mesh::makeFace(Vertex* v1, Vertex* v2, Vertex* v3, Vertex* v4, float r, float g, float b, QString* ID) {

		Face *f1 = new Face(NULL, r, g, b, ID);
		HalfEdge* h1 = new HalfEdge(f1, v1, NULL, NULL);
		halfedges->push_back(h1);
		f1->first = h1;
		HalfEdge* h4 = new HalfEdge(f1, v4, h1, NULL);
		halfedges->push_back(h4);
		HalfEdge* h3 = new HalfEdge(f1, v3, h4, NULL);
		halfedges->push_back(h3);
		HalfEdge* h2 = new HalfEdge(f1, v2, h3, NULL);
		halfedges->push_back(h2);
		h1->next = h2;
		return f1;
	
	}

	vector<int> Mesh::getVertAdjFaces(Vertex* ver, vector<Face*>* faceVec) {
		vector<int> ret;
		for (int i = 0; i < faceVec->size(); i++) {
			if (faceVec->at(i)->containsVert(ver)) {
				ret.push_back(i);
			}
		}
		return ret;
	}

	vector<HalfEdge*> Mesh::getVertAdjEdges(Vertex* ver, vector<HalfEdge*> halfVec) {
		vector<HalfEdge*> ret;
		for (int i = 0; i < halfVec.size(); i++) {
			if (halfVec.at(i)->v == ver) {
				ret.push_back(halfVec.at(i)->getPrev());
			}
		}
		return ret;
	}

	bool Mesh::containsOrSym(HalfEdge* he, vector<HalfEdge*> vec) {
		for (int i = 0; i < vec.size(); i++) {
			if (vec.at(i)->sym == he) {
				return true;
			}
		}
		return false;
	}

	void Mesh::subdivideMesh() {

		//vectors storing all new vertices and faces to update mesh later
		vector<Vertex*> allNewVerts;
		vector<Face*> allNewFaces;

		//compute centroids because they'll be corrupted after the edge vertex addition
		vector<glm::vec3> centroids;
		for (int c = 0; c < faces->size(); c++) {
			faces->at(c)->centroid = faces->at(c)->getCentroid();
			centroids.push_back(faces->at(c)->getCentroid());
		}

		//add edge vertices
		vector<HalfEdge*> hes;
		for (int i = 0; i < halfedges->size(); i++) {
			if (!containsOrSym(halfedges->at(i), hes)) {
				hes.push_back(halfedges->at(i));
			}
		}

		//move edge vertices according to Catmull-Clark
		vector<HalfEdge*> newhes;
		vector<glm::vec3> newpos;
		HalfEdge* he;
		HalfEdge* hv;
		for (int k = 0; k < hes.size(); k++) {
			hv = hes.at(k);
			glm::vec3 cent1 = hv->f->centroid;
			glm::vec3 cent2 = hv->sym->f->centroid;
			he = addVertex(hes.at(k));
			allNewVerts.push_back(he->v);
			glm::vec3 oldpos = (cent1 + cent2 + *he->next->v->xyz + *he->getPrev()->v->xyz)/4.0f;
			newpos.push_back(oldpos);
			newhes.push_back(he);
		}

		//For each face, create centroid vertex, create splitting halfedges and set next pointers for the next pass.
		Face* fa;
		Vertex* vn;
		HalfEdge* nh;
		HalfEdge* newhtocent = 0;
		HalfEdge* newhtomid = 0;
		Face* newF;
		glm::vec3 centr;
		vector<Face*> newfaces;
		for (int n = 0; n < faces->size(); n++) {
			newfaces.clear(); //clear previous face's vector
			fa = faces->at(n); 

			//initialize and position centroid vertex
			vn = new Vertex(0, 0, 0, new QString("545"));
			*vn->xyz = fa->centroid;
			allNewVerts.push_back(vn);
			//find a pointer to a newly added midpoint on this face
			for (int x = 0; x < newhes.size(); x++) {
				nh = newhes.at(x);
				if (nh->f == fa) {
					break;
				}
				if (nh->sym->f == fa) {
					nh = nh->sym->next;
					break;
				}
			}

			//Traverse the face, fixing pointers and creating split faces, all of which include the centroid
			HalfEdge* first = nh;
			while (true) {
				newF = new Face(NULL, fa->color->x, fa->color->y, fa->color->z, new QString("1010"));
				newhtomid = new HalfEdge(newF, nh->v, nh->next, NULL);
				newhtocent = new HalfEdge(newF, vn, newhtomid, NULL);
				newF->first = newhtocent;
				allNewFaces.push_back(newF);
				newfaces.push_back(newF);
				newhes.push_back(newhtomid);
				newhes.push_back(newhtocent);
				nh->next->f = newF;
				nh = nh->next->next;
				nh->f = newF;
				if (nh == first) {
					break;
				}
			}
			
			Face* fa2;
			//Now that the pointers are appropriately set, go back and fix each of the pointers that were left before
			for (int h = 0; h < newfaces.size(); h++) {
				nh = newfaces.at(h)->first;
				nh->next->next->next->next = nh;
				fa = newfaces.at(h);
				int q;
				if (h + 1 == newfaces.size()) {
					q = 0;
				} else {
					q = h + 1;
				}
				nh = fa->first;
				nh->sym = NULL;
				fa2 = newfaces.at(q);
				Mesh::connectFaces(fa2->first->next, nh);
				if (nh->sym == NULL) {
					cout << "Failed to connect face " << h << " with face " << q << endl;
				}
			}
			//cout << "Finished setting all remaining next pointers! " << endl;

			//Connect new aligned faces' sym pointers
			//for (int b = 0; b < newfaces.size(); b++) {
				/*fa = newfaces.at(b);
				int q;
				if (b + 1 == newfaces.size()) {
1					q = 0;
				} else {
					q = b + 1;
				}
				nh = fa->first;
				nh->sym = NULL;
				fa2 = newfaces.at(q);
				Mesh::connectFaces(nh, fa2->first->next);
				if (nh->sym == NULL) {
					cout << "Failed to connect face " << b << " with face " << q << endl;
				}*/
			//}
			
			//cout << "Connected all split faces!" << endl;
			//cout << "Completed face " << n << " !" << endl;
		}

		for (int g = 0; g < newhes.size(); g++) {
			HalfEdge* pos = newhes.at(g);
			if (g < newpos.size()) {
				*pos->v->xyz = newpos.at(g);
			}
			halfedges->push_back(pos);
		}

		//reposition vertices
		for (int t = 0; t < verts->size(); t++) {

			vn = verts->at(t);

			glm::vec3 facesum = glm::vec3(0.0f);
			vector<HalfEdge*> adjEs = getVertAdjEdges(vn, *halfedges);
			glm::vec3 edgesum = glm::vec3(0.0f);
			for (int o = 0; o < adjEs.size(); o++) {
				edgesum += *adjEs.at(o)->v->xyz;
				facesum += *adjEs.at(o)->f->first->getPrev()->v->xyz;
			}

			float n = static_cast<float>(adjEs.size());
			*vn->xyz = (*vn->xyz * ((n-2.0f)/n)) + (facesum/(n*n)) + (edgesum/(n*n));
		}

		for (int y = 0; y < allNewVerts.size(); y++) {
			verts->push_back(allNewVerts.at(y));
		}

		*faces = allNewFaces;
		
		generateVertArray();
		generateColorArray();
		generateIndArray();
		generateNormArray();
		cout << "Subdivision complete!" << endl;
		
}	
void Mesh::objFileStructure2(const char* mtlfilename){
	cout<<mtlfilename<<endl;
	ifstream mtl(mtlfilename,ios::in);
	if(!mtl){
			cerr<<"File could not be found."<<endl;
			exit(1);
	}
	string materialLine="";
	string materialToken="";
	while(!mtl.eof()){
	
		if(materialToken!="newmtl"){
			getline(mtl, materialLine);
			std::stringstream os1(materialLine);
			os1>>materialToken;
		}
		
		if(materialToken =="newmtl"){
			std::stringstream os2(materialLine);
			os2>>materialToken;//new mtl
			
			os2>>materialToken;//material
			
			string thematerial = materialToken;
			float red = 0.0776f;
			float green = 0.2571f;
			float blue = 0.2041f;
			do{
				if(mtl.eof()){
					break;
				}
				if(!mtl.eof()){
					getline(mtl, materialLine);
				//	cout<<"lines before newmtl ..."<<materialLine<<endl;
					std::stringstream os3(materialLine);
					os3>>materialToken;
					
				if(materialToken=="Kd"){
					
						os3>>materialToken;
						//cout<<"red "<<materialToken<<endl;
						 red = atof(materialToken.c_str());
						
						os3>>materialToken;
						//cout<<"green "<<materialToken<<endl;
						 green = atof(materialToken.c_str());
						
						os3>>materialToken;
					//	cout<<"blue "<<materialToken<<endl;
						 blue = atof(materialToken.c_str());
						

				}
				
				}
						
			}while(materialToken!="newmtl");
				namesOfMaterials.push_back(thematerial);
				materialRed.push_back(red);
				materialGreen.push_back(green);
				materialBlue.push_back(blue);
			
		}
	}

	mtl.close();

}

Mesh* Mesh::objFileStructure3(const char* filename){
	Mesh* obj = new Mesh();

		char type;
		ifstream file(filename,ios::in);

		if(!file){
			cerr<<"File could not be found."<<endl;
			exit(1);
		}
		int numvert = 0;
		int numface = 0;
		float r = 0.0776f;
		float g = 0.2571f;
		float b = 0.2041f;

		while(!file.eof()){
			
			string fileLine;
			string token;
			getline(file, fileLine);
			std::stringstream os(fileLine); 
			os>>token;

			if(token=="#"){
				continue;
			}
			else if(token =="mtllib"){
				os>>token;
				objFileStructure2(token.c_str());
				
			}

			else if(token =="v"){
				numvert++;
				os>>token;
				float x = atof(token.c_str());
				os>>token;
				float y = atof(token.c_str());
				os>>token;
				float z = atof(token.c_str());
				Vertex* v = new Vertex(x, y, z, new QString(numvert));
				
				obj->verts->push_back(v);
			}
			else if(token =="usemtl"){
				os>>token;
				for(int a = 0; a <namesOfMaterials.size();a++){
					if(token==namesOfMaterials[a]){
						r = materialRed[a];
						g = materialGreen[a];
						b = materialBlue[a];
						
						break;
						
					}
				}
			}
			//face
			else if(token=="f"){
				std::vector<HalfEdge*> faceHalfEdges; //half edges for each face
				numface++;
				Face* f = new Face();
				f->id = new int(numface);
				f->oldcolor = new glm::vec3(r, g, b);
				f->color = new glm::vec3(r, g, b);
				//cout<<"face";
				std::vector<int> vertOfFace;  //use vector to store vertices' id of which the current face consists
				while(os>>token){
					int vi = atoi(token.c_str());
					vertOfFace.push_back(vi);
				//	cout<<vi<<" ";
				//create a vector of the indices	
				}
			//	cout<<"r: "<<r<<"g: "<<g<<"b: "<<b<<endl;
				int faceVertSize = vertOfFace.size();
				//create halfedges for each vertex of the face
				for(int j = 0; j<faceVertSize;j++){
					HalfEdge* edge = new HalfEdge();
					obj->halfedges->push_back(edge); //add halfedges the the mesh
					faceHalfEdges.push_back(edge); //add haldedges to the face vector
					edge->v = obj->verts->at(vertOfFace[j]-1); //set the vertex
					edge->f = f; //set the face
				}
				//set next EDGE
				for(int k=0; k<faceVertSize; k++){
					if(k < faceVertSize-1){
						faceHalfEdges[k]->next = faceHalfEdges[k+1];
					} else {
						faceHalfEdges[k]->next = faceHalfEdges[0];
					}
				}
				//set faces first halfedge
				f->first = faceHalfEdges[0];
				obj->faces->push_back(f);
				faceHalfEdges.clear();

			}
		}
	for(int a = 0; a<obj->halfedges->size()-1; a++){
		for(int b = a+1; b<obj->halfedges->size(); b++){
				glm::vec3* v1id = obj->halfedges->at(a)->v->xyz;
				glm::vec3* v2id = obj->halfedges->at(b)->v->xyz;
				glm::vec3* v1nextid = obj->halfedges->at(a)->getPrev()->v->xyz;
				glm::vec3* v2nextid = obj->halfedges->at(b)->getPrev()->v->xyz;
				if(/*(v1id==v2id && v1nextid==v2nextid)||*/(v1id==v2nextid && v1nextid==v2id)){
					//Mesh::connectFaces(obj->halfedges->at(a), obj->halfedges->at(b));
					//cout << 
					HalfEdge* e1 = obj->halfedges->at(a);
					HalfEdge* e2 = obj->halfedges->at(b);
					//cout<<"face: "<<e1->getFace()->getID()<<"half edge 1: "<<e1->getVert()->getID()<<"->"<<e1->getNext()->getVert()->getID()<<endl;
					//cout<<"face: "<<e2->getFace()->getID()<<" half edge 2: "<<e2->getVert()->getID()<<"->"<<e2->getNext()->getVert()->getID()<<endl;
					e1->sym = e2;
					e2->sym = e1;
				}
				
			}
		}
		return obj;

}

	vector<Vertex*> Mesh::createBoundingBox(int l, int m, int n){

	//find min and max vertices

	float minX, minY, minZ;
	float maxX, maxY, maxZ;

	minX = verts->at(0)->xyz->x;
	minY = verts->at(0)->xyz->y;
	minZ = verts->at(0)->xyz->z;
	maxX = verts->at(0)->xyz->x;
	maxY = verts->at(0)->xyz->y;
	maxZ = verts->at(0)->xyz->z;
	for(int c=0; c<verts->size(); c++){
		float x = verts->at(c)->xyz->x;
		float y = verts->at(c)->xyz->y;
		float z = verts->at(c)->xyz->z;
		//check minx
		if(x < minX){
			minX = x;
		}
		//check miny
		if(y < minY){
			minY = y;
		}
		//check minz
		if(z < minZ){
			minZ = z;
		}

		if(x > maxX){
			maxX = x;
		}
		
		if(y > maxY){
			maxY = y;
		}
		
		if(z > maxZ){
			maxZ = z;
		}
	}
	
	

	//Create Bounding Box
	vector<Vertex*> boundingBoxPoints;
	//v7
	P0 = new Vertex(minX, minY, minZ, new QString("0"));
	Vertex* Pmax = new Vertex(maxX, maxY, maxZ, new QString("7"));
	//the corners of the bounding box
	Vertex* P1 = new Vertex(minX, maxY, maxZ, new QString("1"));
	Vertex* P2 = new Vertex(minX, minY, maxZ, new QString("2"));
	Vertex* P3 = new Vertex(maxX, minY, maxZ, new QString("3"));
	Vertex* P4 = new Vertex(maxX, minY, minZ, new QString("4"));
	Vertex* P5 = new Vertex(maxX, maxY, minZ, new QString("5"));
	Vertex* P6 = new Vertex(minX, maxY, minZ, new QString("6"));
	/*float Slength = P4->xyz->x - P0->xyz->x;
	float SDivisionLength = Slength/(float)l;
	float Tlength = P5->xyz->y - P4->xyz->y;
	float TDivisionLength = Tlength/(float)m;
	float Ulength = P3->xyz->z - P4->xyz->z;
	float UDivisionLength = Ulength/(float)n;*/
		minXValue = minX;
	minYValue = minY;
	minZValue = minZ;
	maxXValue = maxX;
	maxYValue = maxY;
	maxZValue = maxZ;
	float Slength = maxX - minX;
	float SDivisionLength = Slength/(float)l;
	float Tlength = maxY - minY;
	float TDivisionLength = Tlength/(float)m;
	float Ulength = maxZ - minZ;
	float UDivisionLength = Ulength/(float)n;


	for(int i = 0; i<=l; i++){
		for(int j=0; j<=m; j++){
			for(int k = 0; k<=n; k++){
				float x = minX + ((float)i)*SDivisionLength;
				float y = minY + ((float)j)*TDivisionLength;
				float z = minZ + ((float)k)*UDivisionLength;
				Vertex* subpart = new Vertex(x, y, z, new QString("10"));
				boundingBoxPoints.push_back(subpart);

			}
		}
	}
		

	for(int b=0; b<boundingBoxPoints.size();b++){
		char buffer [33];
		int x = boundingBoxPoints[b]->getID();
		/*cout<<itoa(x, buffer, 10)<<" x:"<<boundingBoxPoints[b]->xyz->x<<" y:"<<boundingBoxPoints[b]->xyz->y<<" z:"<<boundingBoxPoints[b]->xyz->z<<endl;*/
	}
	
	return boundingBoxPoints;
}

void Mesh::saveObjFile(const char* outputFilename){
ofstream outFile; // object for writing to a file

outFile.open(outputFilename, ios::out);

if (!outFile) {
  cerr << "Can't open output file " << outputFilename << endl;
  exit(1);
}

for(int i=0; i<verts->size(); i++){
	float x = verts->at(i)->xyz->x;
	float y = verts->at(i)->xyz->y;
	float z = verts->at(i)->xyz->z;
	float w = 1.0f;
	ostringstream bufferx;
	ostringstream buffery;
	ostringstream bufferz;
	ostringstream bufferw;
	bufferx << x;
	buffery << y;
	bufferz << z;
	bufferw << w;
	
	outFile << "v " << bufferx.str()<<" "<<buffery.str()<<" "<<bufferz.str()<<" "<<bufferw.str()<<"\n";
}

for(int i =0; i<faces->size(); i++){
	outFile << "f";
	vector<Vertex*> faceVertices;
	HalfEdge* e = faces->at(i)->getFirst();
	HalfEdge* loop = e;
	//faceVertices.push_back(e->getVert());
	do{
		faceVertices.push_back(loop->getVert());
		loop = loop->getNext();
		
		
	}while(loop!=e);

	for(int k = 0; k <faceVertices.size();k++){
		Vertex* fv = faceVertices[k];
		for(int j = 0; j<verts->size(); j++){
			if(fv == verts->at(j)){
				outFile<<" "<<j+1;
			}

		}
	}

	outFile<<"\n";
}
outFile.close();
}