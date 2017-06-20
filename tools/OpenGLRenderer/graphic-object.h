#pragma once

#include <string>
#include "actor.h"
#include "bounding-box.h"
class CMesh;
class CMaterial;
class XMLElement;
class BoundingCylinder;
using namespace std;
#include <vector>

class CGraphicObject: public CActor
{
protected:
	string m_name;
	vector<CMesh*> m_meshes;
	BoundingBox3D m_bb;

	void updateBoundingBox();
public:
	CGraphicObject(string name);
	CGraphicObject(tinyxml2::XMLElement* pNode);
	virtual ~CGraphicObject();

	string name() { return m_name; }

	static CGraphicObject* getInstance(tinyxml2::XMLElement* pNode);

	void addMesh(CMesh* pMesh) { m_meshes.push_back(pMesh); }

	void draw();

	BoundingBox3D boundingBox() const;
	void drawBoundingBox();
	void fitToBoundingBox(BoundingBox3D& newBB);
	void fitToBoundingCylinder(BoundingCylinder& newBC);
};

