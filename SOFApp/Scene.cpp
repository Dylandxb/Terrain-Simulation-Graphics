#include "stdafx.h"
#include "RenderLib.h"
#include "Week6App.h"
#include <random>

extern Week6App* app;
Node* treeGroup;
Node* treeSubGraph;  //Point to Nodes
std::vector<glm::vec3> splinePoints;

//Cone Nodes
Node* coneGroup;
Node* coneSubGraph;

//callback for animating a rotation about the y axis 
class YRotatorCallback : public UpdateCallback
{

	TransformNode* trans;
	float rate;
	float t;
public:
	YRotatorCallback(TransformNode* trans, float rate) : trans(trans), rate(rate), t(0.0f) {}				//Rotate transform about Y axis at a fixed rate
	virtual void operator()(float dt)
	{
		if (SOF::Window::IsKeyDown(GLFW_KEY_R))
		{
			t += dt;
			trans->SetTransform(glm::rotate(glm::mat4(), t * rate, glm::vec3(0, 1, 0)));						//Updates the angle by the timestep and then sets the transform of the model
		}
	}
};

class MoveSphereWithKey : public UpdateCallback
{
	TransformNode* trans;
	glm::vec3 pos;
	glm::vec3 scale;
	float t;
public:
	MoveSphereWithKey(TransformNode* trans, glm::vec3& pos, glm::vec3& scale) : trans(trans), pos(pos), scale(scale), t(0.0f) {} //Update call
	virtual void operator()(float dt)
	{
		if (SOF::Window::IsKeyDown(GLFW_KEY_K))
		{
			t += dt;
			pos.y += 0.1f * t;									//Key press to change the y scale of vec3 in the up direction
			if (pos.y >= 20.0f)
			{
				pos.y = 20.0f;									//Set max height cap, bound y to 20.0f
			}
		}
		if (SOF::Window::IsKeyDown(GLFW_KEY_J))
		{
			t += dt;
			pos.y -= 0.1f * t;									//J key to move the y scale of vec3 pos down by 0.1 by time
			if (pos.y <= 4.0f)
			{
				pos.y = 4.0f;									//Set min height cap, bound y to 5.0f
			}
		}
		trans->SetTransform(glm::translate(glm::mat4(), pos));
	}
};

class ChangeEmissive : public UpdateCallback
{
	Emissive* emissive;
public:
	ChangeEmissive(Emissive* emissive) : emissive(emissive) {}
	virtual void operator ()(float dt) override
	{
		if (SOF::Window::IsKeyPressed(GLFW_KEY_8))
		{
			float cycleRate = emissive->GetTime();		//Upon pressing 8 the cycle of emissive starts
			emissive->SetTime(cycleRate + dt);
		}
	}
};

glm::vec4 randomColor()
{
	glm::vec4 ret;
	ret.r = rand() / (float)RAND_MAX;
	ret.g = rand() / (float)RAND_MAX;
	ret.b = rand() / (float)RAND_MAX;
	ret.a = rand() / (float)(1.0f);
	return ret;
	//Random colour function to make code cleaner
}

Node* CreateTent(Physics* physics)
{

	GroupNode* tentParent = new GroupNode();						//Creating the groupNode to spawn multiple nodes
	Cone con(2.0f, 16);
	glm::vec4 colour = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	Emissive* coneEmissive = new Emissive(con, colour);
	TransformNode* cone = new TransformNode(glm::mat4());
	TransformNode* conScale = new TransformNode(glm::scale(glm::mat4(), glm::vec3(1.0f, 1.0f, 1.0f)));
	cone->AddChild(conScale);
	conScale->AddChild(new GeometryNode(coneEmissive));
	tentParent->AddChild(conScale);
	tentParent->SetUpdateCallback(new ChangeEmissive(coneEmissive));

	return tentParent;
}

Node* CreateTree(Physics* physics)
{
	//Tree Parent Object, made from faceted cylinders and spheres
	GroupNode* treeParentObj = new GroupNode(); 
	TransformNode* treeRotateParent = new TransformNode(glm::mat4());
	CylinderGeom cyl(1.0f, 16, true);									//Use cyl geom, true means the cylinder is faceted
	TexturedLit* cylRender = new TexturedLit(cyl, "textures/Tree Textures/Bark/Bark001_1K_Color.jpg", "textures/Tree Textures/Bark/Bark001_1K_NormalGL.jpg");
	TransformNode* cylinder = new TransformNode(glm::mat4());
	TransformNode* cylTrans = new TransformNode(glm::translate(glm::mat4(), glm::vec3(19.0f, 3.7f, 19.0f)));
	TransformNode* cylScale = new TransformNode(glm::scale(glm::mat4(), glm::vec3(0.4f, 1.0f, 0.4f)));
	cylinder->AddChild(cylTrans);
	cylTrans->AddChild(cylScale);
	cylTrans->AddChild(treeRotateParent);
	treeRotateParent->AddChild(cylScale);
	treeRotateParent->SetUpdateCallback(new YRotatorCallback(treeRotateParent, 0.5f));
	cylScale->AddChild(new GeometryNode(cylRender));
	treeParentObj->AddChild(cylinder);
	//TRUNK IS ROTATING

	//Log texture made from a Capsule geom
	Capsule cap(1.0f, 16);
	glm::mat4 rotateTrans;
	rotateTrans = glm::rotate(rotateTrans, glm::radians(90.0f), glm::vec3(0, 0, 1));   	//Rotate textured Lit log object 
	TexturedLit* capRender = new TexturedLit(cap, "textures/LogTextures/Bark007_1K_Color.jpg", "textures/LogTextures/Bark007_1K_NormalGL.jpg");
	TransformNode* capsule = new TransformNode(glm::mat4());
	TransformNode* capTrans = new TransformNode(glm::translate(glm::mat4(), glm::vec3(20.0f, 3.2f, 20.0f)) * rotateTrans );

	TransformNode* capsule1 = new TransformNode(glm::mat4());
	TransformNode* cap1Trans = new TransformNode(glm::translate(glm::mat4(), glm::vec3(20.0f, 3.2f, 19.75f)) * rotateTrans);

	TransformNode* capsule2 = new TransformNode(glm::mat4());
	TransformNode* cap2Trans = new TransformNode(glm::translate(glm::mat4(), glm::vec3(20.0f, 3.2f, 20.25f)) * rotateTrans);

	TransformNode* capsule3 = new TransformNode(glm::mat4());
	TransformNode* cap3Trans = new TransformNode(glm::translate(glm::mat4(), glm::vec3(20.0f, 3.4f, 20.15f)) * rotateTrans);

	TransformNode* capsule4 = new TransformNode(glm::mat4());
	TransformNode* cap4Trans = new TransformNode(glm::translate(glm::mat4(), glm::vec3(20.0f, 3.4f, 19.85f)) * rotateTrans);

	TransformNode* capsule5 = new TransformNode(glm::mat4());
	TransformNode* cap5Trans = new TransformNode(glm::translate(glm::mat4(), glm::vec3(20.0f, 3.6f, 20.0f)) * rotateTrans);

	TransformNode* capScale = new TransformNode(glm::scale(glm::mat4(), glm::vec3(0.2f, 0.5f, 0.2f)));

	TransformNode* capsuleRotator = new TransformNode(glm::mat4());

	//multiple logs on ground at different transforms around tree i.e stack of logs in a pile
	capsule->AddChild(capTrans);
	capsule->AddChild(cap1Trans);
	capsule->AddChild(cap2Trans);
	capsule->AddChild(cap3Trans);
	capsule->AddChild(cap4Trans);
	capsule->AddChild(cap5Trans);
	capTrans->AddChild(capsuleRotator);
	cap1Trans->AddChild(capsuleRotator);
	cap2Trans->AddChild(capsuleRotator);
	cap3Trans->AddChild(capsuleRotator);
	cap4Trans->AddChild(capsuleRotator);
	cap5Trans->AddChild(capsuleRotator);


	capTrans->AddChild(capScale);
	cap1Trans->AddChild(capScale);
	cap2Trans->AddChild(capScale);
	cap3Trans->AddChild(capScale);
	cap4Trans->AddChild(capScale);
	cap5Trans->AddChild(capScale);
	capsuleRotator->AddChild(capScale);
	capsuleRotator->SetUpdateCallback(new YRotatorCallback(capsuleRotator, 0.5f));
	capScale->AddChild(new GeometryNode(capRender));

	treeParentObj->AddChild(capsule);               //make capsule child of tree parent

	SphereGeometry leaves(20);
	TexturedLit* leavesRender = new TexturedLit(leaves, "textures/Tree Textures/Moss/Moss002_1K_Color.jpg", "textures/Tree Textures/Moss/Moss002_1K_NormalGL.jpg");
	//Leaf transforms
	//y = 5
	TransformNode* leafTrans = new TransformNode(glm::translate(glm::mat4(), glm::vec3(19.0f, 5.0f, 19.0f)));		//Center leaf
	TransformNode* leaf1Trans = new TransformNode(glm::translate(glm::mat4(), glm::vec3(19.25f,5.0f, 19.95f)));	
	TransformNode* leaf2Trans = new TransformNode(glm::translate(glm::mat4(), glm::vec3(19.85f,5.0f, 19.10f)));	
	TransformNode* leaf3Trans = new TransformNode(glm::translate(glm::mat4(), glm::vec3(18.7f,5.0f, 19.3f))); 
	TransformNode* leaf4Trans = new TransformNode(glm::translate(glm::mat4(), glm::vec3(19.2f,5.0f, 19.3f))); 
	TransformNode* leaf5Trans = new TransformNode(glm::translate(glm::mat4(), glm::vec3(18.45f,5.0f, 18.8f))); 
	TransformNode* leaf6Trans = new TransformNode(glm::translate(glm::mat4(), glm::vec3(19.2f,5.0f, 18.8f))); 
	// y = 4.5
	TransformNode* leaf7Trans = new TransformNode(glm::translate(glm::mat4(), glm::vec3(19.22f,4.5f, 18.82f)));		//Bottom leaves 
	TransformNode* leaf8Trans = new TransformNode(glm::translate(glm::mat4(), glm::vec3(19.45f,4.5f, 18.84f))); 
	TransformNode* leaf9Trans = new TransformNode(glm::translate(glm::mat4(), glm::vec3(19.0f, 4.5f, 19.2f))); 
	TransformNode* leaf10Trans = new TransformNode(glm::translate(glm::mat4(), glm::vec3(18.8f,4.5f, 19.0f))); 
	TransformNode* leaf11Trans = new TransformNode(glm::translate(glm::mat4(), glm::vec3(18.7f,4.5f, 18.8f)));  
	TransformNode* leaf12Trans = new TransformNode(glm::translate(glm::mat4(), glm::vec3(19.22f,4.5f, 18.84f))); 
	//y = 5
	TransformNode* leaf13Trans = new TransformNode(glm::translate(glm::mat4(), glm::vec3(18.3f, 5.0f, 18.60f)));	
	TransformNode* leaf14Trans = new TransformNode(glm::translate(glm::mat4(), glm::vec3(18.2f, 5.0f, 18.5f))); 
	//y = 5.5
	TransformNode* leaf15Trans = new TransformNode(glm::translate(glm::mat4(), glm::vec3(18.0f, 5.5f, 19.33f)));	
	TransformNode* leaf16Trans = new TransformNode(glm::translate(glm::mat4(), glm::vec3(18.33f,5.5f, 18.79f))); 
	TransformNode* leaf17Trans = new TransformNode(glm::translate(glm::mat4(), glm::vec3(18.2f, 5.5f, 19.53f))); 
	TransformNode* leaf18Trans = new TransformNode(glm::translate(glm::mat4(), glm::vec3(18.45f,5.5f, 19.64f))); 
	TransformNode* leaf19Trans = new TransformNode(glm::translate(glm::mat4(), glm::vec3(19.48f,5.5f, 18.38f))); 
	TransformNode* leaf20Trans = new TransformNode(glm::translate(glm::mat4(), glm::vec3(19.02f,5.5f, 18.36f))); 
	TransformNode* leaf21Trans = new TransformNode(glm::translate(glm::mat4(), glm::vec3(19.08f,5.5f, 18.72f))); 
	// y= 5
	TransformNode* leaf22Trans = new TransformNode(glm::translate(glm::mat4(), glm::vec3(19.33f,5.0f, 18.96f))); 
	TransformNode* leaf23Trans = new TransformNode(glm::translate(glm::mat4(), glm::vec3(18.14f,5.0f, 19.32f))); 
	TransformNode* leaf24Trans = new TransformNode(glm::translate(glm::mat4(), glm::vec3(18.36f,5.0f, 19.58f))); 
	TransformNode* leaf25Trans = new TransformNode(glm::translate(glm::mat4(), glm::vec3(18.08f,5.0f, 19.5f))); 
	// y=  6
	TransformNode* leaf26Trans = new TransformNode(glm::translate(glm::mat4(), glm::vec3(18.71f, 6.0f, 18.44f))); 
	TransformNode* leaf27Trans = new TransformNode(glm::translate(glm::mat4(), glm::vec3(19.44f, 6.0f, 19.04f))); 
	TransformNode* leaf28Trans = new TransformNode(glm::translate(glm::mat4(), glm::vec3(18.54f, 6.0f, 19.18f))); 
	TransformNode* leaf29Trans = new TransformNode(glm::translate(glm::mat4(), glm::vec3(18.92f, 6.0f, 19.68f))); 
	// y = 6.5
	TransformNode* leaf30Trans = new TransformNode(glm::translate(glm::mat4(), glm::vec3(18.8f, 6.5f, 18.9f))); 
	// y = 5.5
	TransformNode* leaf31Trans = new TransformNode(glm::translate(glm::mat4(), glm::vec3(18.86f,5.5f, 18.9f)));
	TransformNode* leaf32Trans = new TransformNode(glm::translate(glm::mat4(), glm::vec3(18.8f, 5.5f, 18.9f)));
	TransformNode* leaf33Trans = new TransformNode(glm::translate(glm::mat4(), glm::vec3(19.44f,5.5f, 19.04f)));
	TransformNode* leaf34Trans = new TransformNode(glm::translate(glm::mat4(), glm::vec3(18.71f,5.5f, 18.44f)));
	TransformNode* leaf35Trans = new TransformNode(glm::translate(glm::mat4(), glm::vec3(18.54f,5.5f, 19.18f)));

	//Leaf scales
	TransformNode* leafSize = new TransformNode(glm::scale(glm::mat4(), glm::vec3(0.4f, 0.4f, 0.4f)));

	TransformNode* leafScale = leafSize;
	TransformNode* leaf1Scale = leafSize;
	TransformNode* leaf2Scale = leafSize;
	TransformNode* leaf3Scale = leafSize;
	TransformNode* leaf4Scale = leafSize;
	TransformNode* leaf5Scale = leafSize;
	TransformNode* leaf6Scale = leafSize;
	TransformNode* leaf7Scale = leafSize;
	TransformNode* leaf8Scale = leafSize;
	TransformNode* leaf9Scale = leafSize;
	TransformNode* leaf10Scale =leafSize;
	TransformNode* leaf11Scale =leafSize;
	TransformNode* leaf12Scale =leafSize;
	TransformNode* leaf13Scale =leafSize;
	TransformNode* leaf14Scale =leafSize;
	TransformNode* leaf15Scale =leafSize;
	TransformNode* leaf16Scale =leafSize;
	TransformNode* leaf17Scale =leafSize;
	TransformNode* leaf18Scale =leafSize;
	TransformNode* leaf19Scale =leafSize;
	TransformNode* leaf20Scale =leafSize;
	TransformNode* leaf21Scale =leafSize;
	TransformNode* leaf22Scale =leafSize;
	TransformNode* leaf23Scale =leafSize;
	TransformNode* leaf24Scale =leafSize;
	TransformNode* leaf25Scale =leafSize;
	TransformNode* leaf26Scale =leafSize;
	TransformNode* leaf27Scale =leafSize;
	TransformNode* leaf28Scale =leafSize;
	TransformNode* leaf29Scale =leafSize;
	TransformNode* leaf30Scale =leafSize;
	TransformNode* leaf31Scale = leafSize;
	TransformNode* leaf32Scale = leafSize;
	TransformNode* leaf33Scale = leafSize;
	TransformNode* leaf34Scale = leafSize;
	TransformNode* leaf35Scale = leafSize;

	//Constructing the tree using scales as a child of transforms, and then creating a new Geometry Node

	leafTrans->AddChild(leafScale);
	leaf1Trans->AddChild(leaf1Scale);
	leaf2Trans->AddChild(leaf2Scale);
	leaf3Trans->AddChild(leaf3Scale);
	leaf4Trans->AddChild(leaf4Scale);
	leaf5Trans->AddChild(leaf5Scale);
	leaf6Trans->AddChild(leaf6Scale);
	leaf7Trans->AddChild(leaf7Scale);
	leaf8Trans->AddChild(leaf8Scale);
	leaf9Trans->AddChild(leaf9Scale);
	leaf10Trans->AddChild(leaf10Scale);
	leaf11Trans->AddChild(leaf11Scale);
	leaf12Trans->AddChild(leaf12Scale);
	leaf13Trans->AddChild(leaf13Scale);
	leaf14Trans->AddChild(leaf14Scale);
	leaf15Trans->AddChild(leaf15Scale);
	leaf16Trans->AddChild(leaf16Scale);
	leaf17Trans->AddChild(leaf17Scale);
	leaf18Trans->AddChild(leaf18Scale);
	leaf19Trans->AddChild(leaf19Scale);
	leaf20Trans->AddChild(leaf20Scale);
	leaf21Trans->AddChild(leaf21Scale);
	leaf22Trans->AddChild(leaf22Scale);
	leaf23Trans->AddChild(leaf23Scale);
	leaf24Trans->AddChild(leaf24Scale);
	leaf25Trans->AddChild(leaf25Scale);
	leaf26Trans->AddChild(leaf26Scale);
	leaf27Trans->AddChild(leaf27Scale);
	leaf28Trans->AddChild(leaf28Scale);
	leaf29Trans->AddChild(leaf29Scale);
	leaf30Trans->AddChild(leaf30Scale);
	leaf31Trans->AddChild(leaf31Scale);
	leaf31Trans->AddChild(leaf32Scale);
	leaf31Trans->AddChild(leaf33Scale);
	leaf31Trans->AddChild(leaf34Scale);
	leaf31Trans->AddChild(leaf35Scale);

	GeometryNode* leafGeom = new GeometryNode(leavesRender);

	leafScale->AddChild(leafGeom);
	leaf1Scale->AddChild(leafGeom);
	leaf2Scale->AddChild(leafGeom);
	leaf3Scale->AddChild(leafGeom);
	leaf4Scale->AddChild(leafGeom);
	leaf5Scale->AddChild(leafGeom);
	leaf6Scale->AddChild(leafGeom);
	leaf7Scale->AddChild(leafGeom);
	leaf8Scale->AddChild(leafGeom);
	leaf9Scale->AddChild(leafGeom);
	leaf10Scale->AddChild(leafGeom);
	leaf11Scale->AddChild(leafGeom);
	leaf12Scale->AddChild(leafGeom);
	leaf13Scale->AddChild(leafGeom);
	leaf14Scale->AddChild(leafGeom);
	leaf15Scale->AddChild(leafGeom);
	leaf16Scale->AddChild(leafGeom);
	leaf17Scale->AddChild(leafGeom);
	leaf18Scale->AddChild(leafGeom);
	leaf19Scale->AddChild(leafGeom);
	leaf20Scale->AddChild(leafGeom);
	leaf21Scale->AddChild(leafGeom);
	leaf22Scale->AddChild(leafGeom);
	leaf23Scale->AddChild(leafGeom);
	leaf24Scale->AddChild(leafGeom);
	leaf25Scale->AddChild(leafGeom);
	leaf26Scale->AddChild(leafGeom);
	leaf27Scale->AddChild(leafGeom);
	leaf28Scale->AddChild(leafGeom);
	leaf29Scale->AddChild(leafGeom);
	leaf30Scale->AddChild(leafGeom);
	leaf31Scale->AddChild(leafGeom);
	leaf32Scale->AddChild(leafGeom);
	leaf33Scale->AddChild(leafGeom);
	leaf34Scale->AddChild(leafGeom);
	leaf35Scale->AddChild(leafGeom);

	treeParentObj->AddChild(leafTrans);
	treeParentObj->AddChild(leaf1Trans);
	treeParentObj->AddChild(leaf2Trans);
	treeParentObj->AddChild(leaf3Trans);
	treeParentObj->AddChild(leaf4Trans);
	treeParentObj->AddChild(leaf5Trans);
	treeParentObj->AddChild(leaf6Trans);
	treeParentObj->AddChild(leaf7Trans);
	treeParentObj->AddChild(leaf8Trans);
	treeParentObj->AddChild(leaf9Trans);
	treeParentObj->AddChild(leaf10Trans);
	treeParentObj->AddChild(leaf11Trans);
	treeParentObj->AddChild(leaf12Trans);
	treeParentObj->AddChild(leaf13Trans);
	treeParentObj->AddChild(leaf14Trans);
	treeParentObj->AddChild(leaf15Trans);
	treeParentObj->AddChild(leaf16Trans);
	treeParentObj->AddChild(leaf17Trans);
	treeParentObj->AddChild(leaf18Trans);
	treeParentObj->AddChild(leaf19Trans);
	treeParentObj->AddChild(leaf20Trans);
	treeParentObj->AddChild(leaf21Trans);
	treeParentObj->AddChild(leaf22Trans);
	treeParentObj->AddChild(leaf23Trans);
	treeParentObj->AddChild(leaf24Trans);
	treeParentObj->AddChild(leaf25Trans);
	treeParentObj->AddChild(leaf26Trans);
	treeParentObj->AddChild(leaf27Trans);
	treeParentObj->AddChild(leaf28Trans);
	treeParentObj->AddChild(leaf29Trans);
	treeParentObj->AddChild(leaf30Trans);
	treeParentObj->AddChild(leaf31Trans);
	treeParentObj->AddChild(leaf32Trans);
	treeParentObj->AddChild(leaf33Trans);
	treeParentObj->AddChild(leaf34Trans);
	treeParentObj->AddChild(leaf35Trans);
	treeRotateParent->AddChild(capScale);
	treeRotateParent->AddChild(leafSize);


	return treeParentObj;
}



// callback for animating on a spline
class SplineAnimationCallback : public UpdateCallback
{
	Spline* spline;
	TransformNode* trans;
	float rate;
	float t = 0;
	FlyCam* camera;
	
public:
	SplineAnimationCallback(TransformNode* trans, Spline* spline, float rate) : trans(trans), spline(spline), rate(rate), t(0.0f) {}
	virtual void operator()(float dt)
	{
		t += dt;
		if (t > spline->GetNumPoints())
			t -= spline->GetNumPoints();
		// work out the new transform here
		glm::vec3 position = spline->GetPoint(t);
		glm::vec3 tangent = spline->GetTangent(t);
		glm::mat4 lookAt = glm::lookAt(position, position + tangent, glm::vec3(0, 1, 0));
		trans->SetTransform(glm::inverse(lookAt));

	}
};

class MoveLightCallback : public UpdateCallback
{
	LightNode* node;
	float t;
public:
	MoveLightCallback(LightNode* node) : node(node), t(0.0f) {}
	virtual void operator()(float dt)
	{
		t += dt*0.25f;  //Slows the rate of light
		float x, y, z;
		x = 50.0f * cos(t);
		y = 20.0f;
		z = 50.0f * sin(t);
		node->light.position = glm::vec3(x, y, z);
		//Key L to change pointLight direction animation
		if (SOF::Window::IsKeyDown(GLFW_KEY_L))
		{
			node->light.position = glm::vec3(-x, y, -z);
		}
	}
};

Node* Scene(Physics* physics) //Adds physics to scene
{
	TransformNode* rootNode = new TransformNode(glm::mat4());
	LightNode* light = new LightNode(glm::vec3(0, 30,0), glm::vec4(1, 1, 1, 1), 0.1, 0.9, 0.4, 25, 400); //white light
	rootNode->AddChild(light);
	light->SetUpdateCallback(new MoveLightCallback(light));													//Update callback moves the lightNode around a position

	
	//try load mesh here
	//Change draw to accept lightNode
	Drawable* drawMesh;




	glm::vec3 pos = glm::vec3(0.0f, 3.0f, 0.0f);
	glm::vec3 scaleSB = glm::vec3(2.0f, 2.0f, 2.0f);
	glm::mat4 mat = glm::scale(glm::translate(glm::mat4(), pos), glm::vec3(2.0f, 2.0f, 2.0f));
	TransformNode* t2 = new TransformNode(mat);
	t2->SetUpdateCallback(new MoveSphereWithKey(t2, pos,scaleSB));
	light->AddChild(t2);
	//t2->AddChild(sphereGeom);
	// create a cubemap
	std::vector<std::string> cubeMapFiles;
	cubeMapFiles.push_back("textures/cubemaps/miramar_up.png");
	cubeMapFiles.push_back("textures/cubemaps/miramar_dn.png");
	cubeMapFiles.push_back("textures/cubemaps/miramar_ft.png");
	cubeMapFiles.push_back("textures/cubemaps/miramar_bk.png");
	cubeMapFiles.push_back("textures/cubemaps/miramar_rt.png");
	cubeMapFiles.push_back("textures/cubemaps/miramar_lf.png");
	SOF::Cubemap* cubeMap = new SOF::Cubemap(cubeMapFiles);
	Skybox* sb = new Skybox(cubeMap);
	GeometryNode* sbNode = new GeometryNode(sb);

	light->AddChild(sbNode);

	//create terrain add it to sceneGraph as a Node below the skyBox
	std::string heightmapFile = "textures/terrainTextures/mountains512.png";
	std::string splatmapFile= "textures/terrainTextures/splatmap.png";
	std::string texFile0 = "textures/terrainTextures/road.jpg";
	std::string texFile1 = "textures/terrainTextures/dirt.jpg";
	std::string texFile2 = "textures/terrainTextures/grass.jpg";
	glm::mat4 terrMat = glm::mat4();
	glm::vec3 terrScale = glm::vec3(100.0f, 20.f, 0.0f);
	Terrain* terr = new Terrain(heightmapFile, splatmapFile, texFile0, texFile1, texFile2,100.0f, 20.0f);
	app->setTerrain(terr);																							//Connects terrain to the app class
	GeometryNode* terrNode = new GeometryNode(terr);
	//Make a new transformNode for the terrain
	TransformNode* terrTrans = new TransformNode(glm::translate(glm::mat4(), glm::vec3(-50.0f, 0.001f, -50.0f)));


	//Collide with the terrain, COMMENTING LINES ALLOWS CAM TO MOVE
	//StaticCollider* terrColl = new TerrainCollider(heightmapFile, terrScale);
	//physics->AddCollider(terrColl);


	light->AddChild(terrTrans);
	terrTrans->AddChild(terrNode);

	//Create floor for scene
	QuadGeometry quad(10);
	TexturedLit* floor = new TexturedLit(quad, "textures/Extras/Ground054_4K_Color.jpg", "textures/Extras/Ground054_4K_NormalGL.jpg");
	GeometryNode* geomNode = new GeometryNode(floor);
	// scale the floor and add 
	glm::mat4 floorMat = glm::scale(glm::mat4(), glm::vec3(150, 100, 150));
	TransformNode* floorScale = new TransformNode(floorMat);
	light->AddChild(floorScale);
	floorScale->AddChild(geomNode);
	StaticCollider* coll = new QuadCollider(floorMat);								//Creates a new mat4 to match the floorMat and add a collider to it
	physics->AddCollider(coll);
	
	//Sphere with CubeMap
	SphereGeometry sphere(20);
	CylinderGeom cyl1(5.0f,16.0f,false);
	TransformNode* cyl1Trans = new TransformNode(glm::translate(glm::mat4(), glm::vec3(0.0f, 20.0f, 0.0f)));
	TransformNode* cyl1Scale = new TransformNode(glm::scale(glm::mat4(), glm::vec3(0.75f, 4.0f, 0.75f)));
	TexturedLit* cyl1Render = new TexturedLit(cyl1, "textures/marble.png");

	TexturedLit* sphereRender = new TexturedLit(sphere, "textures/stone.png", "default", cubeMap, 0.3f);
	TransformNode* sphereTrans = new TransformNode(glm::translate(glm::mat4(), glm::vec3(0.0f, 40.0f, 0.0f)));
	TransformNode* sphereScale = new TransformNode(glm::scale(glm::mat4(), glm::vec3(2.5f, 2.5f, 2.5f)));

	TransformNode* sphereOrbitRotator = new TransformNode(glm::mat4());
	light->AddChild(cyl1Trans);
	light->AddChild(sphereTrans);
	cyl1Trans->AddChild(cyl1Scale);
	sphereTrans->AddChild(sphereScale);
	sphereTrans->AddChild(sphereOrbitRotator);
	sphereOrbitRotator->AddChild(sphereScale);
	sphereOrbitRotator->SetUpdateCallback(new YRotatorCallback(sphereOrbitRotator, 1.5f));							//Adds rotation to the sphere
	sphereScale->AddChild(new GeometryNode(sphereRender));
	cyl1Scale->AddChild(new GeometryNode(cyl1Render));

	// add emissive spheres at each corner of the map
	Emissive* lightSph = new Emissive(sphere, glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
	glm::vec3 scaleSph = glm::vec3(2.0f, 2.0f, 2.0f);
	glm::vec3 pos2 = glm::vec3(77.0f, 4.0f, 77.0f);
	glm::mat4 mat2 = glm::scale(glm::translate(glm::mat4(), pos2), glm::vec3(2.0f, 2.0f, 2.0f));

	glm::vec3 pos3 = glm::vec3(-77.0f, 4.0f, -77.0f);
	glm::mat4 mat3 = glm::scale(glm::translate(glm::mat4(), pos3), glm::vec3(2.0f, 2.0f, 2.0f));

	glm::vec3 pos4 = glm::vec3(77.0f, 4.0f, -77.0f);
	glm::mat4 mat4 = glm::scale(glm::translate(glm::mat4(), pos4), glm::vec3(2.0f, 2.0f, 2.0f));

	glm::vec3 pos5 = glm::vec3(-77.0f, 4.0f, 77.0f);
	glm::mat4 mat5 = glm::scale(glm::translate(glm::mat4(), pos5), glm::vec3(2.0f, 2.0f, 2.0f));

	TransformNode* t = new TransformNode(mat2);
	TransformNode* t1 = new TransformNode(mat3);
	TransformNode* t3 = new TransformNode(mat4);
	TransformNode* t4 = new TransformNode(mat5);

	//Update callback to move each emissive sphere in Y dir on the boundary corners
	t->SetUpdateCallback(new MoveSphereWithKey(t, pos2,scaleSph));
	t1->SetUpdateCallback(new MoveSphereWithKey(t1, pos3,scaleSph));
	t3->SetUpdateCallback(new MoveSphereWithKey(t3, pos4,scaleSph));
	t4->SetUpdateCallback(new MoveSphereWithKey(t4, pos5,scaleSph));

	//Add sphere collision *************************************
	//StaticCollider* emSphColl = new SphereCollider(mat2, mat3, mat4, mat5);
	//physics->AddCollider(emSphColl);

	light->AddChild(t);
	light->AddChild(t1);
	light->AddChild(t3);
	light->AddChild(t4);

	GeometryNode* SphereGn = new GeometryNode(lightSph);
	SphereGn->SetUpdateCallback(new ChangeEmissive(lightSph));					//Call the change emissive updatecallback to cycle through colours on the spheres
	t->AddChild(SphereGn);
	t1->AddChild(SphereGn);
	t3->AddChild(SphereGn);
	t4->AddChild(SphereGn);

	//********Terrain Boundry*******// //Add Collision
	//Back wall
	CuboidGeometry cuboid(5.0f, 5.0f, 5.0f);
	glm::mat4 cuboidParent = glm::mat4();
	glm::mat4 rotateCuboidTrans;
	rotateCuboidTrans = glm::rotate(rotateCuboidTrans, glm::radians(90.0f), glm::vec3(0, 1, 0)); //Used to rotate the object by 90 deg on y plane
	TexturedLit* cuboidRender = new TexturedLit(cuboid, "textures/Extras/Ground054_4K_Color.jpg", "textures/Extras/Ground054_4K_NormalGL.jpg");
	TransformNode* cuboidTrans = new TransformNode(glm::translate(glm::mat4(), glm::vec3(77.0f, 1.0f, 0.0f)));
	TransformNode* cuboidScale = new TransformNode(glm::scale(glm::mat4(), glm::vec3(2.0f, 2.0f, 19.75f)));
	TransformNode* cuboidRScale = new TransformNode(glm::scale(glm::mat4(), glm::vec3(2.0f, 2.0f, 18.75f)));
	light->AddChild(cuboidTrans);
	cuboidTrans->AddChild(cuboidScale);
	//Front perspective wall
	TransformNode* cuboidTrans1 = new TransformNode(glm::translate(glm::mat4(), glm::vec3(-77.0f, 1.0f, 0.0f)));
	light->AddChild(cuboidTrans1);
	cuboidTrans1->AddChild(cuboidScale);
	//Left perspective wall
	TransformNode* cuboidTrans2 = new TransformNode(glm::translate(glm::mat4(), glm::vec3(0.0f, 1.0f, -77.0f)) * rotateCuboidTrans);

	light->AddChild(cuboidTrans2);
	cuboidTrans2->AddChild(cuboidRScale);
	//Right perspective wall
	TransformNode* cuboidTrans3 = new TransformNode(glm::translate(glm::mat4(), glm::vec3(0.0f, 1.0f, 77.0f)) * rotateCuboidTrans);
	light->AddChild(cuboidTrans3);
	cuboidTrans3->AddChild(cuboidRScale);

	cuboidScale->AddChild(new GeometryNode(cuboidRender));
	cuboidRScale->AddChild(new GeometryNode(cuboidRender));
	//Add cuboid Collision using AABB
	StaticCollider* cuboidColl = new QuadCollider(cuboidParent);
	physics->AddCollider(cuboidColl);

	coneSubGraph = CreateTent(physics);								//Create a new set of group nodes in order to spawn a tent
	coneGroup = new GroupNode();

	TransformNode* cone1 = new TransformNode(glm::translate(glm::mat4(), glm::vec3(0.0f, 0.0f, 0.0f)));

	light->AddChild(coneGroup);
	coneGroup->AddChild(cone1);
	cone1->AddChild(coneSubGraph);

	treeSubGraph = CreateTree(physics);								//Create a subgraph of trees with physics in the scene
	treeGroup = new GroupNode();

	TransformNode* tree1 = new TransformNode(glm::translate(glm::mat4(), glm::vec3(0.5f, 0.0f, 5.0f)));  //Base tree in scene before any others are spawned
	TransformNode* tree2 = new TransformNode(glm::translate(glm::mat4(), glm::vec3(-50.5f, -2.0f, 5.0f)));
	TransformNode* tree3 = new TransformNode(glm::translate(glm::mat4(), glm::vec3(-50.5f, 0.0f, -40.0f)));
	TransformNode* tree4 = new TransformNode(glm::translate(glm::mat4(), glm::vec3(-10.5f, -2.4f, -60.0f)));
	TransformNode* tree5 = new TransformNode(glm::translate(glm::mat4(), glm::vec3(25.0f, -2.6f, -42.0f)));
	TransformNode* tree6 = new TransformNode(glm::translate(glm::mat4(), glm::vec3(0.0f, 0.0f, -22.0f)));


	light->AddChild(treeGroup);
	treeGroup->AddChild(tree1);
	treeGroup->AddChild(tree2);
	treeGroup->AddChild(tree3);
	treeGroup->AddChild(tree4);
	treeGroup->AddChild(tree5);
	treeGroup->AddChild(tree6);

	tree1->AddChild(treeSubGraph);
	tree2->AddChild(treeSubGraph);
	tree3->AddChild(treeSubGraph);
	tree4->AddChild(treeSubGraph);
	tree5->AddChild(treeSubGraph);
	tree6->AddChild(treeSubGraph);
	
	

	//Creating a spline to move camera through the scene. Moves through order of spline points pushed onto vector
	std::vector<glm::vec3> lineVerts;
	//Redraw the Spline with 8 points
	 
	splinePoints.push_back(glm::vec3(glm::vec3(25, 5, 25)));					//Tree1 spline point
	splinePoints.push_back(glm::vec3(glm::vec3(20, 8, 15)));					//Use as spline to move over mountain between points 1 & 6
	splinePoints.push_back(glm::vec3(glm::vec3(17.8, 5, -4)));					//Tree6 spline point
	splinePoints.push_back(glm::vec3(glm::vec3(42, 3, -23)));					//Tree5 spline point
	splinePoints.push_back(glm::vec3(glm::vec3(20, 8, -33)));					//Use as spline to move over mountain between points 4 & 5
	splinePoints.push_back(glm::vec3(glm::vec3(8, 1, -39)));					//Tree4 spline point
	splinePoints.push_back(glm::vec3(glm::vec3(-1, 5, -34)));					//Use as spline to move over mountain between points 3 & 4
	splinePoints.push_back(glm::vec3(glm::vec3(-12, 6, -32)));
	splinePoints.push_back(glm::vec3(glm::vec3(-18, 10, -26)));
	splinePoints.push_back(glm::vec3(glm::vec3(-34, 5, -24)));					//Tree3  spline point
	splinePoints.push_back(glm::vec3(glm::vec3(-48, 8, -15)));		
	splinePoints.push_back(glm::vec3(glm::vec3(-46, 3, 20)));					//Tree2 spline point
	splinePoints.push_back(glm::vec3(glm::vec3(-6, 8, 40)));					

	SphereGeometry sphereSp(10);
	TexturedLit* sphereRenderSp = new TexturedLit(sphereSp, "textures/Tree Textures/Moss/Moss002_1K_Color.jpg", "textures/Tree Textures/Moss/Moss002_1K_NormalGL.jpg");
	GeometryNode* sphereNode = new GeometryNode(sphereRenderSp);
	//Uncommenting lines in spline points causes Trees and a capsule to move along spline
	//for (glm::vec3 pos : splinePoints)
	//{
	//	TransformNode* cuboidTransSp = new TransformNode(glm::translate(glm::mat4(), pos));
	//	light->AddChild(cuboidTransSp);
	//	cuboidTransSp->AddChild(sphereNode); //BUG: drawing new trees at random points. Draws 8 trees as there are 8 splinePoints. Commenting it out removes trees and changes capsule pos
	//}

	CRSpline c(splinePoints, true, 0.5f);
	for (int i = 0; i < 500; i++)
	{
		float u = 8.0f * i / 499.0f;
		lineVerts.push_back(c.GetPoint(u));
	}
	PolyLine* p = new PolyLine(lineVerts, glm::vec4(0, 1, 0, 1));
	light->AddChild(new GeometryNode(p));

	TransformNode* sphereTransSp = new TransformNode(glm::mat4());
	light->AddChild(sphereTransSp);
	//Move sphere along spline
	sphereTransSp->AddChild(sphereNode);
	sphereTransSp->SetUpdateCallback(new SplineAnimationCallback(sphereTransSp, new CRSpline(splinePoints, true, 0.5f), 0.5f)); //or CRSpline with 0.5f as tension


	return light;
}
