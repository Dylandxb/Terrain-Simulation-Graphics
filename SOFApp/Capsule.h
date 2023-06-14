#pragma once
#include "stdafx.h"
//#include "CylinderGeom.h"
//#include "SphereGeom.h"
//#include "GeometryHelper.h"
#include "GeometryHelper.h"
//Capsule is same as a cylinder, but has hemi spherical caps instead of flat circles at the top and bottom vertex positions
//Temporary cyclinder, temporary sphere, add an offset to the sphere so at one point the top is within the top of cyclinder etc

class Capsule : public GeometryHelper
{
	std::vector<glm::vec3> positions;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec3> tangents;
	std::vector<glm::vec2> uvs;


	void CreateCapsuleGeometry(float height, int subDivisions)
	{
		//For loop to loop through the cylinder and push it back onto the vec3s positions etc, Loop through spheres twice one with offset one without
		CylinderGeom* cyl;
		cyl = new CylinderGeom(height, subDivisions);


		//Use the functions then push onto the data declared at beginning of class

		for (int i = 0; i < cyl->GetNumVertices(); i++)						//Get the number of vertices on cylinder to loop through
		{
			positions.push_back(cyl->GetPositionVector()[i]);			//Push back onto vertex arrays the index of each value
			normals.push_back(cyl->GetNormalVector()[i]);
			tangents.push_back(cyl->GetTangentVector()[i]);
			uvs.push_back(cyl->GetUVVector()[i]);
		}

		SphereGeometry* sph;											//Create a sphere object				
		sph = new SphereGeometry(subDivisions);

		//Push data of sphere 
		//Add height(offset) to the sphere so it spawns half of it at the bottom of the cylinder
		for (int i = 0; i < sph->GetNumVertices(); i++)											//Condition, loop through the sphere and find all the vertices on it
		{
			positions.push_back(sph->GetPositionVector()[i] - glm::vec3(0, height, 0));			//Push back onto vertex arrays the index of each value, -height in the y
			normals.push_back(sph->GetNormalVector()[i]);
			tangents.push_back(sph->GetTangentVector()[i]);
			uvs.push_back(sph->GetUVVector()[i]);
		}
		for (int i = 0; i < sph->GetNumVertices(); i++)
		{
			positions.push_back(sph->GetPositionVector()[i] + glm::vec3(0, height, 0));			//Push back onto vertex arrays the index of each value, positive height in the y
			normals.push_back(sph->GetNormalVector()[i]);
			tangents.push_back(sph->GetTangentVector()[i]);
			uvs.push_back(sph->GetUVVector()[i]);
		}

	}
public:
	Capsule(float height = 1.0f, int subDivisions = 16) { CreateCapsuleGeometry(height, subDivisions); }
	const std::vector<glm::vec3>& GetPositionVector() const { return positions; }
	const std::vector<glm::vec3>& GetNormalVector() const { return normals; }
	const std::vector<glm::vec2>& GetUVVector() const { return uvs; }
	const std::vector<glm::vec3>& GetTangentVector() const { return tangents; }
	int GetNumVertices() const { return positions.size(); }


};
