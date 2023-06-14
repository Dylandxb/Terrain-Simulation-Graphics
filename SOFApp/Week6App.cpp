#include "stdafx.h"
#include "week6app.h"

 Node* Scene(Physics* phys);
extern Node* treeGroup;
extern Node* treeSubGraph;
extern Terrain* height;

extern Node* coneGroup;
extern Node* coneSubGraph;

extern std::vector<glm::vec3> splinePoints;

float Week6App::Random()
{
	static std::mt19937 rng = std::mt19937(std::random_device()());
	static std::uniform_real_distribution<float> dist;
	return dist(rng);
}

class CamColl : public CameraCollisionCallback
{
	Physics* phys;
public:
	CamColl(Physics* phys) : phys(phys) {}
	glm::vec3 operator()(const glm::vec3& pos, const glm::vec3& newPos, float rad) const override
	{
		//Collision with camera works but can move directly through a plane at fast pace
		std::vector<ContactInfo> contacts = phys->CollideWithWorld(newPos, rad);
		glm::vec3 finalPos = newPos;
		for (auto c : contacts)
		{
			finalPos += c.normal * c.depth;

		}
		return finalPos;
	}
	virtual ~CamColl() {}
};

void Week6App::Init()
{
	SphereGeometry sphereGeom(20);

	// setup the camera
	eye = glm::vec3(0.0f, 30.0f, -50.0f);
	lookAt = glm::vec3(0.0f, 0.0f, 0.0f);
	up = glm::vec3(0.0f, 1.0f, 0.0); // y is up!
	physics = new Physics();
	sceneGraphRoot = Scene(physics);
	camera = new FlyCam(eye, lookAt, true, new CamColl(physics)); //Adds collision to the camera);
	treeNode = (Node*)(new GroupNode());  //GroupNode is an empty node
	tentNode = (Node*)(new GroupNode());
	cameraSpline = new CRSpline(splinePoints, true);
	lightSphere = new Emissive(sphereGeom);
	// scene graph for dynamic objects 
	dynamicObjectNode = new GroupNode();
	sceneGraphRoot->AddChild(dynamicObjectNode);
	//initialize transform node
	projectileNode = new TransformNode(glm::scale(glm::mat4(), glm::vec3(0.4f, 0.4f, 0.4f)));
	SphereGeometry geom(20);
	TexturedLit* mesh = new TexturedLit(geom, "textures/stone.png");
	projectileNode->AddChild(new GeometryNode(mesh));

	meshObject = new MeshObject("Meshes/FBX/Campfire.fbx");

	lightColors[0] = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	lightColors[1] = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
	lightColors[2] = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
	lightColors[3] = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

	lightPos[0] = glm::vec4(0.5f, 0.6f, 1.0f, 1.0f);
	lightPos[1] = glm::vec3(100.0f, 50.0f, 100.0f);
	lightPos[2] = glm::vec3(100.0f, 50.0f, -100.0f);
	lightPos[3] = glm::vec3(-100.0f, 50.0f, -100.0f);

	lightRadius[0] = lightRadius[1] = lightRadius[2] = lightRadius[3] = 200.0f;
	// render states
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//Add light to mesh

}

void Week6App::Draw()
{
	// camera
	glm::mat4 view;
	glm::mat4 proj;

	eye = camera->GetPos();
	lookAt = camera->GetLookAt();
	up = camera->GetUp();

	if (SOF::Window::IsKeyDown(GLFW_KEY_9))													//When key is released move back to regular cam view
	{
		//Enter camera on Spline
		eye = camera->GetPos();
		lookAt = camera->GetLookAt();
		up = camera->GetUp();
		glm::vec3 splinePos = cameraSpline->GetPoint(splineParameter);
		glm::vec3 splineTangent = cameraSpline->GetPoint(splineParameter);

		eye = splinePos;
		lookAt = splinePos - splineTangent; // " - " inverts the view of LookAt. Add keypress to change LookAt value
		up = glm::vec3(0.0f, 1.0f, 0.0f);
	}

	view = glm::lookAt(eye, lookAt, up);
	proj = glm::perspective(glm::radians(60.0f), (float)SOF::Window::GetWidth() / (float)SOF::Window::GetHeight(), 0.1f, 1000.0f);

	// Render the scene graph //FIXES MESH LIGHTING ISSUE, DRAW SCENE GRAPH BEFORE RENDERING MESH
	RenderVisitor rv(view, proj);
	rv.Traverse(sceneGraphRoot);
	//Position of mesh in scene
	/////********ADD ITS OWN LIGHT SOURCE TO MESH
	glm::mat4 model = glm::mat4();
	model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
	model = glm::translate(model, glm::vec3(-200.5f, 110.5f, -90.5f));

	Transforms trans(model, view, proj);
	glm::mat4 lightPositionMat;
	glm::mat4 lightColorMat;
	for (int i = 0; i < 4; i++)
	{
		lightPositionMat[i] = glm::vec4(lightPos[i], 0.0f);
		lightColorMat[i] = lightColors[i];
	}
	PointLight light(lightPositionMat, lightColorMat, 0.1f, 0.8f, 0.5f, 20, glm::vec4(lightRadius[0], lightRadius[1], lightRadius[2], lightRadius[3]));
	//Use lightNode

	meshObject->Draw(trans, light);
	for (int i = 0; i < 4; i++)
	{
		model = glm::translate(glm::mat4(), lightPos[i]);
		trans.SetModel(model);
		lightSphere->Draw(trans, lightColors[i]);
	}


}

void Week6App::SpawnTents(float timeSpawn)
{
	glm::vec3 heightCap = glm::vec3(0.0f, 5.0f, 0.0f);
	glm::vec3 pos = glm::vec3(40.0f * Random(), 0, 40.0f * Random());
	pos.y = terrCoord->AtPointY(pos);													//Argument pos is type vec3
	if (pos.y <= heightCap.y)															//Prevents objects spawning at a great height
	{
		Node* tentTrans = new TransformNode(glm::translate(glm::mat4(), pos));
		TransformNode* tentScale = new TransformNode(glm::scale(glm::mat4(), glm::vec3(0.5f, 0.5f, 0.5f))); //Sets a random scale for the trees


		coneGroup->AddChild(tentTrans);
		tentTrans->AddChild(tentScale);
		tentScale->AddChild(coneSubGraph);
		if (SOF::Window::IsKeyPressed(GLFW_KEY_6))
		{
			//Prevents spawning any more cones. The tree node becomes empty
			coneGroup->RemoveChild(tentTrans);
			tentTrans->RemoveChild(tentScale);
			tentScale->RemoveChild(coneSubGraph);
		}
	}
	else
	{
		return;
	}
}

void Week6App::DeleteTrees()
{
	//Remove treeNodes from subgraph
	treeGroup->RemoveChild(treeSubGraph);
	//Use mouseClick to delete objects

}

void Week6App::SpawnTrees(float timeSpawn)
{
	//play arounnd with Y value
	glm::vec3 treeHeight = glm::vec3(0.0f, 1.0f, 0.0f);
	//delta U and deltaV on terrain

	//Only spawn a Tree if it is at a point Y in terrain
	glm::vec3 pos = glm::vec3(50.0f * Random(), 0, 50.0f * Random());
	//glm::vec3 pos1 = glm::vec3(-2.0f + 4.0f * Random(), 0, 0.0f * Random() + 2.0f);

	pos.y = terrCoord->AtPointY(pos);														//Get the height	
	pos += glm::vec3(-50, -3, -50);															//Change y value to lower tree Height
	//Create multiple transforms and add them to the sub graph. Make parent transform at different trees
	if (pos.y <= treeHeight.y)															//Set boundaries for where trees spawn ADD HEIGHT CAP
	{
		Node* treeTrans = new TransformNode(glm::translate(glm::mat4(), pos));


		TransformNode* treeScale = new TransformNode(glm::scale(glm::mat4(), glm::vec3(1.0f, float(rand() / float((RAND_MAX)) + 1),1.0f))); //Sets a random scale for the trees


		treeGroup->AddChild(treeTrans);
		treeTrans->AddChild(treeScale);
		treeScale->AddChild(treeSubGraph);

		if (SOF::Window::IsKeyPressed(GLFW_KEY_6))
		{
			//Prevents spawning any more trees. The tree node becomes empty
			treeGroup->RemoveChild(treeTrans);
			treeTrans->RemoveChild(treeScale);
			treeScale->RemoveChild(treeSubGraph);

		}
	}

}

void Week6App::Shoot()
{
	//Create transform Node 
	TransformNode* node = new TransformNode(glm::mat4());
	dynamicObjectNode->AddChild(node);
	node->AddChild(projectileNode);
	//Create new Projectile
	float speed = 20.0f;
	glm::vec3 camDirection = (camera->GetLookAt() - camera->GetPos());
	glm::vec3 velocity = glm::normalize(camera->GetLookAt() - camera->GetPos()) * speed;					//Speed
	Projectile* proj = new Projectile(camera->GetPos(), velocity, node);
	physics->AddProjectile(proj);
}

void Week6App::Update(float deltaT)
{
	physics->Update(deltaT);							//Call physics update function with timestep
	// Upadate the scene graph
	UpdateVisitor uv(deltaT);
	uv.Traverse(sceneGraphRoot);
	//Spline Animation
	splineParameter += speed * deltaT;
	if (splineParameter > splinePoints.size())
	{
		splineParameter -= splinePoints.size();
	}
	if (SOF::Window::IsKeyDown(GLFW_KEY_W))
		camera->Move(40.0f * deltaT);
	if (SOF::Window::IsKeyDown(GLFW_KEY_S))
		camera->Move(-40.0f * deltaT);
	if (SOF::Window::IsKeyDown(GLFW_KEY_A))
		camera->Strafe(-40.0f * deltaT);
	if (SOF::Window::IsKeyDown(GLFW_KEY_D))
		camera->Strafe(40.0f * deltaT);

	if (SOF::Window::IsKeyDown(GLFW_KEY_LEFT))
		camera->Pan(80.0f * deltaT);
	if (SOF::Window::IsKeyDown(GLFW_KEY_RIGHT))
		camera->Pan(-80.0f * deltaT);
	if (SOF::Window::IsKeyDown(GLFW_KEY_UP))
		camera->LookUpDown(80.0f * deltaT);
	if (SOF::Window::IsKeyDown(GLFW_KEY_DOWN))
		camera->LookUpDown(-80.0f * deltaT);
	if (SOF::Window::IsKeyDown(GLFW_KEY_T))
		SpawnTrees(0.0002f * deltaT);
	if (SOF::Window::IsKeyDown(GLFW_KEY_C))
		//Key press to delete trees
		DeleteTrees();
	if (SOF::Window::IsKeyDown(GLFW_KEY_V))
		SpawnTents(0.1f * deltaT);
	//Key pressed is bugged
	if (SOF::Window::IsKeyDown(GLFW_KEY_SPACE))
		Shoot();

}


//Key Codes
//---------------------------------
//WASD -> Cam Movement
//T -> Spawn trees           //APP
//V -> Spawn tents (cones)   //APP
//Space -> Shoot projectiles //APP
//6 -> Prevent spawning of nodes //APP
//8 -> Cycle Emissives           //SCENE
//9 -> Change spline view   //APP
//J -> Move sphere in '-' y //SCENE
//K -> Move sphere in '+' y //SCENE

