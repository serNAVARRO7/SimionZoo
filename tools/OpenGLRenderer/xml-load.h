#pragma once
namespace tinyxml2 { class XMLElement; }

#define XML_TAG_SCENE "Scene"
#define XML_TAG_IMPORT_SCENE "Import-Scene"
#define XML_TAG_OBJECTS "Objects"
#define XML_TAG_COLLADA_MODEL "Collada-Model"
#define XML_TAG_SPHERE "Sphere"
#define XML_TAG_CILINDER "Cilinder"
#define XML_TAG_BOX "Box"
#define XML_TAG_BOX_DRAW_TOP "DrawTop"
#define XML_TAG_TEX_COORD_MULT_FACTOR "TexCoordMultFactor"
#define XML_TAG_POLYLINE "Polyline"
#define XML_TAG_POINT "Point"
#define XML_TAG_WIDTH "Width"
#define XML_TAG_NAME_ATTR "Name"
#define XML_TAG_PATH_ATTR "Path"
#define XML_TAG_TRANSFORM "Transform"
#define XML_TAG_TRANSLATION "Translation"
#define XML_TAG_ROTATION "Rotation"
#define XML_TAG_SCALE "Scale"
#define XML_TAG_DEPTH "Depth"
#define XML_TAG_X "X"
#define XML_TAG_Y "Y"
#define XML_TAG_Z "Z"
#define XML_TAG_W "W"
#define XML_TAG_YAW "Yaw"
#define XML_TAG_PITCH "Pitch"
#define XML_TAG_ROLL "Roll"
#define XML_TAG_MATERIAL "Material"
#define XML_TAG_SIMPLE_TL_MATERIAL "Simple-TL-Material"
#define XML_TAG_TRANSLUCENT_MATERIAL "Translucent-Material"
#define XML_TAG_R "R"
#define XML_TAG_G "G"
#define XML_TAG_B "B"
#define XML_TAG_A "A"
#define XML_TAG_FIT_BOUNDING_BOX "Fit-BoundingBox"
#define XML_TAG_FIT_BOUNDING_CYLINDER "Fit-BoundingCylinder"
#define XML_TAG_MIN "Min"
#define XML_TAG_MAX "Max"
#define XML_TAG_RADIUS "Radius"
#define XML_TAG_DIFFUSE "Diffuse"
#define XML_TAG_AMBIENT "Ambient"
#define XML_TAG_SPECULAR "Specular"
#define XML_TAG_EMISSION "Emission"
#define XML_TAG_SHININESS "Shininess"
#define XML_TAG_TEXTURE "Texture"
#define XML_TAG_PATH "Path"
#define XML_TAG_LIGHT "Light"
#define XML_TAG_DIR_LIGHT "Directional-Light"
#define XML_TAG_DIRECTION "Direction"
#define XML_TAG_COLOR "Color"

#define XML_ATTR_BINDING "Bind"

#define XML_TAG_CAMERA "Camera"
#define XML_TAG_SIMPLE_CAMERA "Simple-Camera"

//COLLADA TAGS
#define XML_TAG_COLLADA_ROOT "COLLADA"
#define XML_TAG_COLLADA_LIB_GEOM "library_geometries"
#define XML_TAG_COLLADA_GEOMETRY "geometry"
#define XML_TAG_COLLADA_MESH "mesh"
#define XML_TAG_COLLADA_SOURCE "source"
#define XML_TAG_COLLADA_FLOAT_ARRAY "float_array"
#define XML_TAG_COLLADA_NAME "name"
#define XML_TAG_COLLADA_ID_ATTR "id"
#define XML_TAG_COLLADA_COUNT_ATTR "count"
#define XML_TAG_COLLADA_TECHNIQUE_COMMON "technique_common"
#define XML_TAG_COLLADA_ACCESOR "accessor"
#define XML_TAG_COLLADA_PARAM "param"
#define XML_TAG_COLLADA_NAME "name"
#define XML_TAG_COLLADA_VERTICES "vertices"
#define XML_TAG_COLLADA_INPUT "input"
#define XML_TAG_COLLADA_SEMANTIC_ATTR "semantic"
#define XML_TAG_COLLADA_TRIANGLES "triangles"
#define XML_TAG_COLLADA_POLYLIST "polylist"
#define XML_TAG_COLLADA_POLYGONS "polygons"
#define XML_TAG_COLLADA_MATERIAL_ATTR "material"
#define XML_TAG_COLLADA_OFFSET_ATTR "offset"
#define XML_TAG_COLLADA_PRIMITIVE "p"
#define XML_TAG_COLLADA_POSITION_SEMANTIC "POSITION"
#define XML_TAG_COLLADA_NORMAL_SEMANTIC "NORMAL"
#define XML_TAG_COLLADA_TEXCOORD_SEMANTIC "TEXCOORD"
#define XML_TAG_COLLADA_VERTEX_SEMANTIC "VERTEX"
#define XML_TAG_COLLADA_MATERIAL_LIB "library_materials"
#define XML_TAG_COLLADA_MATERIAL "material"
#define XML_TAG_COLLADA_INSTANCE_FX "instance_effect"
#define XML_TAG_COLLADA_URL "url"
#define XML_TAG_COLLADA_FX_LIB "library_effects"
#define XML_TAG_COLLADA_FX "effect"
#define XML_TAG_COLLADA_PROFILE_FX "profile_COMMON"
#define XML_TAG_COLLADA_TECHNIQUE "technique"
#define XML_TAG_COLLADA_DIFFUSE "diffuse"
#define XML_TAG_COLLADA_AMBIENT "ambient"
#define XML_TAG_COLLADA_SPECULAR "specular"
#define XML_TAG_COLLADA_EMISSION "emission"
#define XML_TAG_COLLADA_SHININESS "shininess"
#define XML_TAG_COLLADA_COLOR "color"
#define XML_TAG_COLLADA_FLOAT "float"
#define XML_TAG_COLLADA_TEXTURE "texture"
#define XML_TAG_COLLADA_TEXTURE_ATTR "texture"
#define XML_TAG_COLLADA_IMAGE_LIB "library_images"
#define XML_TAG_COLLADA_IMAGE "image"
#define XML_TAG_COLLADA_INIT "init_from"
#define XML_TAG_COLLADA_IMAGE_FILE_PREFIX "file:///"
#define XML_TAG_COLLADA_INSTANCE_MATERIAL "instance_material"
#define XML_TAG_COLLADA_SYMBOL_ATTR "symbol"
#define XML_TAG_COLLADA_TARGET "target"
#define XML_TAG_COLLADA_NEWPARAM "newparam"
#define XML_TAG_COLLADA_SID "sid"
#define XML_TAG_COLLADA_UP_AXIS "up_axis"
#define XML_TAG_COLLADA_Z_UP "Z_UP"
#define XML_TAG_COLLADA_Y_UP "Y_UP"
#define XML_TAG_COLLADA_ASSET "asset"
#define XML_TAG_COLLADA_VISUAL_SCENE_LIB "library_visual_scenes"
#define XML_TAG_COLLADA_VISUAL_SCENE "visual_scene"
#define XML_TAG_COLLADA_NODE "node"
#define XML_TAG_COLLADA_MATRIX "matrix"
#define XML_TAG_COLLADA_TRANSLATE "translate"
#define XML_TAG_COLLADA_GEOMETRY_INSTANCE "instance_geometry"
#define XML_TAG_COLLADA_CONTROLLER_LIB "library_controllers"
#define XML_TAG_COLLADA_CONTROLLER "controller"
#define XML_TAG_COLLADA_SKIN "skin"
#define XML_TAG_COLLADA_BIND_SHAPE_MATRIX "bind_shape_matrix"

//loading functions
#include "bindings.h"
class Color;
class Matrix44;
class Transform3D;
class Transform2D;
class BoundingBox3D;
class BoundingBox2D;
class BoundingCylinder;

namespace XML
{
	template <typename T>
	double loadBindableValue(tinyxml2::XMLElement* pNode, const char* xmlTag, T& obj)
	{
		double value= 0.0;
		const char* pBindingName;
		tinyxml2::XMLElement* pChild = pNode->FirstChildElement(xmlTag);
		if (pChild)
		{
			if (pChild->GetText())
				value = atof(pChild->GetText());
			pBindingName = pChild->Attribute(XML_ATTR_BINDING);
			if (pBindingName)
				CRenderer::get()->registerBinding<T>(pBindingName, obj, xmlTag);
			return value;
		}
		return value;
	}
	void loadColladaMatrix(const char* pText, Matrix44& outMatrix);
	void loadVector3D(const char* pText, Vector3D& outVec);

	void load(tinyxml2::XMLElement* pNode, Vector3D& outVec);
	void load(tinyxml2::XMLElement* pNode, Point3D& outVec);
	void load(tinyxml2::XMLElement* pNode, Quaternion& outVec);
	void load(tinyxml2::XMLElement* pNode, Vector2D& outVec);
	void load(tinyxml2::XMLElement* pNode, Point2D& outVec);
	void load(tinyxml2::XMLElement* pNode, Transform3D& transform);
	void load(tinyxml2::XMLElement* pNode, Transform2D& transform);
	void load(tinyxml2::XMLElement* pNode, Color& color);
	void load(tinyxml2::XMLElement* pNode, BoundingBox3D& box);
	void load(tinyxml2::XMLElement* pNode, BoundingBox2D& box);
	void load(tinyxml2::XMLElement* pNode, BoundingCylinder& cylinder);
}
#include <vector>


template <typename T>
void loadChildren(tinyxml2::XMLElement* pParentNode, const char* xmlTag, std::vector<T*> &outVec)
{
	T* pObj;
	int x = 0;
	tinyxml2::XMLElement *pNode = pParentNode->FirstChildElement(xmlTag);
	while (pNode)
	{
		pObj = T::getInstance(pNode);
		if (pObj)
			outVec.push_back(pObj);
		else
			++x;
		pNode = pNode->NextSiblingElement(xmlTag);
	}
}

unsigned int countChildren(tinyxml2::XMLElement* pParentNode, const char* xmlTag);