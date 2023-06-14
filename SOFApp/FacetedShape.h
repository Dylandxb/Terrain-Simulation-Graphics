#pragma once
#include "stdafx.h"
#include "GeometryHelper.h"

class FacetedShape : GeometryHelper
{
	std::vector<glm::vec3> positions;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec3> tangents;
	std::vector<glm::vec2> uvs;

	void CreateFacetedGeometry()
	{
		glm::vec3 vertexPositions[] = {
			glm::vec3(-0.5, -0.5, -0.5),
			glm::vec3(-0.5, -0.5, 0.5),
			glm::vec3(0.5, -0.5, 0.5),


		};

		glm::vec3 faceTangents = {
			glm::vec3(1,0,0)
		};

		glm::vec3 faceNormals = {
			glm::vec3(0,1,0)
		};

		glm::vec2 faceUVs[] = {
			glm::vec2(0,0),
			glm::vec2(0,1),
			glm::vec2(1,0)
		};

		int triangles[] = { 0,1,2 };

		for (int i = 0; i < 3; i++)						//3 vertices to loop through
		{
			positions.push_back(vertexPositions[triangles[i]]);
			normals.push_back(faceNormals);
			tangents.push_back(faceTangents);
			uvs.push_back(faceUVs[triangles[i]]);
		}



	}

public:
	FacetedShape() { CreateFacetedGeometry(); }
	const std::vector<glm::vec3>& GetPositionVector() const { return positions; }
	const std::vector<glm::vec3>& GetNormalVector() const { return normals; }
	const std::vector<glm::vec2>& GetUVVector() const { return uvs; }
	const std::vector<glm::vec3>& GetTangentVector() const { return tangents; }
	int GetNumVertices() const { return positions.size(); }

};
