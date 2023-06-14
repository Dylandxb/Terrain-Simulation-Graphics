#pragma once
#include "stdafx.h"
#include "app.h"
#include "RenderLib.h"
#include <random>
#include "Physics.h"


class Week6App : public SOF::App
{
	// geometry
	TexturedLit* floorQuad;
	TexturedLit* cuboid;

	// camera
	glm::vec3 eye;    
	glm::vec3 lookAt; 
	glm::vec3 up;     
	FlyCam* camera;

	// scene graph
	Node* sceneGraphRoot;
	Node* treeNode;
	Node* tentNode;
	TransformNode* treeParentObj;
	Terrain* terrCoord ;
	MemTexture* memTextureTerr;
	Cone* coneEmissive;
	Physics* physics;
	CRSpline* cameraSpline;
	MeshObject* meshObject;
	Emissive* lightSphere;
	Emissive* colorObj;
	//data
	GLint treeCount = 0;
	GLint treeSize = 7;
	float splineParameter = 0.0f;
	float speed = 0.5f;
	glm::vec3 lightPos[4];
	glm::vec4 lightColors[4];
	float lightRadius[4];
	//Nodes for collision
	GroupNode* dynamicObjectNode;
	TransformNode* projectileNode;
	void Shoot();


	float Random();
public:
	void setTerrain(Terrain* t)
	{
		terrCoord = t;				//Pointer to setting terrain in scene
	};
	virtual void Init();
	virtual void Draw();
	virtual void SpawnTrees(float timeSpawn);
	virtual void SpawnTents(float timeSpawn);
	virtual void DeleteTrees();
	virtual void Update(float deltaT);
};