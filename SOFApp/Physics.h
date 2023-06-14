#pragma once
#include "stdafx.h"
#include "TransformNode.h"
#include "MemTexture.h"
#include "Terrain.h"

struct ContactInfo
{
    glm::vec3 normal;           //Public by default in a struct
    float depth;
};



class StaticCollider
{
    // abstract interface for something that static that collides with a sphere
    ////***********ADD SPHERE COLLIDER, CUBOID COLLIDER, TERRAIN COLLIDER, CYLINDER COLLIDER
  
public:
    //virtual bool Collide(const glm::vec3& r, float rad) const = 0;
    virtual bool Collide(const glm::vec3& r, float rad, glm::vec3& normal, float& depth) const = 0;
    virtual ~StaticCollider() {}

};


struct Projectile
{
    glm::vec3 pos;
    glm::vec3 vel;
    TransformNode* sceneGraphNode;
    Projectile(const glm::vec3& pos, const glm::vec3& vel, TransformNode* sgNode) : pos(pos), vel(vel), sceneGraphNode(sgNode) {}
    ~Projectile()
    {
        // disconnect and delete the scene graph node
        delete sceneGraphNode;
    }
};

// scene graph update callback for physics objects
class PhysicsCallback : public UpdateCallback
{
    TransformNode* sgNode;
    Projectile* physics;
public:
    PhysicsCallback(TransformNode* sgNode, Projectile* physics) : sgNode(sgNode), physics(physics) {}
    virtual void operator()(float dt)
    {
        // copy the position from one to the other
        sgNode->SetTransform(glm::translate(glm::mat4(), physics->pos));
    }
};

class Physics
{
    glm::vec3 gravity = glm::vec3(0, -10, 0);           //Initialize gravity variable
    std::vector<StaticCollider*> staticColliders;									//Colliders
    std::vector<Projectile*> projectiles;				//Vector of projectiles from the struct in physics header file

public:
    void Update(float deltaTime);
    void AddCollider(StaticCollider* coll) { staticColliders.push_back(coll); }
    void AddProjectile(Projectile* proj)
    {
        projectiles.push_back(proj);
        proj->sceneGraphNode->SetUpdateCallback(new PhysicsCallback(proj->sceneGraphNode, proj));

    }
    std::vector<ContactInfo> CollideWithWorld(const glm::vec3& spherePos, float sphereRad) const;   //Checks a sphere against all colliders
    //bool CollideWithWorld(const glm::vec3& spherePos, float sphereRad) const;   //Checks a sphere against all colliders

};


class QuadCollider : public StaticCollider
{
    glm::vec3 r, a, b;
    glm::vec3 aHat, bHat; // unit vectors along sides a and b
    float lena, lenb; // lengths of sides a and b


public:
    QuadCollider(const glm::vec3& r, const glm::vec3& a, const glm::vec3& b) : r(r), a(a), b(b)
    {
        lena = glm::length(a);
        lenb = glm::length(b);
        aHat = a / lena;
        bHat = b / lenb;
    }
    QuadCollider(glm::mat4 trans)
    {
        // this constructor assumes a unit quad centered on the origin with sides x, z
        // transform transforms this into place
        glm::vec3 rt = trans * glm::vec4(-0.5f, 0.0f, -0.5f, 1.0f);
        glm::vec3 at = trans * glm::vec4(1.0f, 0.0f, 0.0f, 0.0f);
        glm::vec3 bt = trans * glm::vec4(0.0f, 0.0f, 1.0f, 0.0f);
        r = glm::vec3(rt.x, rt.y, rt.z);                            //r is point on quad
        a = glm::vec3(at.x, at.y, at.z);
        b = glm::vec3(bt.x, bt.y, bt.z);
        lena = glm::length(a);
        lenb = glm::length(b);
        aHat = a / lena;
        bHat = b / lenb;
    }
    ~QuadCollider() {}

    bool Collide(const glm::vec3& p, float rad, glm::vec3& normal, float& depth) const override      //Test collide with nearpoint
    {
        float la = dot(p - r, aHat);
        float lb = dot(p - r, bHat);
        if (la < 0.0f) la = 0.0f; if (la > lena) la = lena;         //clamp and reconstruct
        if (lb < 0.0f) lb = 0.0f; if (lb > lenb) lb = lenb;
        //glm::vec3 rNear = glm::vec3( aHat * la + bHat * lb);
        glm::vec3 rNear = glm::vec3(r + aHat * la + bHat * lb);         //Lecture 16, rNear is nearest point on quad = point + clamp of la, + point of clamp lb
        if (glm::dot(p - rNear, p - rNear) < rad * rad)
        {
            normal = glm::normalize(p - rNear);
            depth = rad - glm::length(p - rNear);
            return true;
        }
        else
            return false;
        // return false;
    }
};

class SphereCollider : StaticCollider
{
    float r1, r2;
    float R1, R2;
    glm::vec3 r, a, b;
    glm::vec3 aHat, bHat; // unit vectors along sides a and b
    float lena, lenb; // lengths of sides a and b
//If distance of centre point is less than Radius 1 + radius2 then collision is true
    //(( )) = magnitude
    float distance = ((r2 - r1)) < R2 - R1;
    float normal = r2 - r1 / r2 - r1;
    float depth = R1 + R2 - ((r2 - r1));
};

class TerrainCollider : public StaticCollider
{
    MemTexture* memTexture;
    glm::vec3 scales;
public:
    TerrainCollider(const std::string& heightMapFile, const glm::vec3 scales) : scales(scales)
    {
        // load texture here by calling memTexture
        std::string heightmapFile = "textures/terrainTextures/mountains512.png";
        //New variable to point to heightmapData
        memTexture = new MemTexture(heightMapFile);
    }
    ~TerrainCollider() { delete memTexture; }
    bool Collide(const glm::vec3& p, float rad, glm::vec3& normal, float& depth) const override
    {
        //Height is radius of texture, access memTexture rad.r rad.g rad.b rad. a multiply by scale.y
        glm::vec3 scales = glm::vec3(100.0f, 20.0f, 100.0f);

        // use the heightmap to detect collision with a sphere
        Terrain* terrainY = {};


        //****** CAUSES CRASH
        //Sample point on memTexture using at Point Y
        // scales.y = terrainY->AtPointY(p);
       
        //Get texture colours
        //memTexture has rgba values
        
        //Lags because its continuisly sampling points
        //Sample the terrain data at different points
        MemTexture mt = MemTexture("textures/stone.png");
        glm::vec4 c0, c1, c2, c3;
        //check if there is a collision at each vec4
        //Sample height at 4 points
        c0 = mt.Sample(glm::vec2(0.2f, 0.2f));
        c1 = mt.Sample(glm::vec2(0.8f, 0.2f));
        c2 = mt.Sample(glm::vec2(0.8f, 0.8f));
        c3 = mt.Sample(glm::vec2(0.2f, 0.8f));
        //Set height = memTexture colour, sample it and multiply by scales.y
        //glm::vec3 height = /*memTexture->Sample(glm::vec2(0.2f, 0.2f))*/ c0 * scales.y;
        glm::vec3 widthX = c1 * scales.x;
        glm::vec3 widthZ = c2 * scales.z;
        //Sample the texture colours
        glm::vec3 height = scales.y * c0;
        if (p.y < /*height*/scales.y + rad)             //if p.y is less than the terrain height + rad then collision is true
        {
            depth = p.y - (scales.y + rad);
            normal = glm::vec3(0, 1, 0);
            //There is collision
            return true;
        }
        else
        {
            return false;
        }
    }


};
