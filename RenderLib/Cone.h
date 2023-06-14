#pragma once
#include "stdafx.h"
#include "GeometryHelper.h"

class Cone : public GeometryHelper
{
	std::vector<glm::vec3> positions;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec3> tangents;
	std::vector<glm::vec2> uvs;

	//Cone same as cylinder except top face has a radius of 0 (upper radius) and normals within the zero radius circle are 0
	//Base radius wide

	void CreateConeGeometry(float height, int subDivisions)
	{
		glm::vec3* circle = new glm::vec3[subDivisions];						//Arrays of positions and uvs on the circle
		glm::vec2* circleUV = new glm::vec2[subDivisions];
		for (int i = 0; i < subDivisions; i++)
		{
			float theta = glm::radians(360.0) * (float)i / (subDivisions - 1);
			circle[i] = glm::vec3(cos(theta) * 5, 0.0f, sin(theta) * 5);							//Increase the cone size by 5 on x and z plane
			circleUV[i] = (glm::vec2(circle[i].x, circle[i].z) + glm::vec2(1.0f, 1.0f)) * 0.5f; //* by a value on x and z increased the number of UV faces on the circle
		}
		//Top section, working within here to make the point of the cone. Doesnt need a top circle
		//glm::vec3 normal = glm::vec3(-1, 1, -1);
		//glm::vec3 tangent = glm::vec3(1, 0, 0);

		//for (int i = 0; i < subDivisions - 1; i++)								//Loop, 2 less triangles than there are vertices
		//{
		//	int i0, i1, i2;													//Indices to signify the faces on a cone is 1 and a curved surface of 1
		//	i0 = 0; i1 = i + 1; i2 = i + 2;

		//	positions.push_back(glm::vec3(circle[i0].x/glm::radians(360.0), height, circle[i0].z/glm::radians(360.0)));			//Draws the positions from 3 points within the circle
		//	positions.push_back(glm::vec3(circle[i1].x/glm::radians(360.0), height, circle[i1].z/glm::radians(360.0)));			//height is the position of the vertex points of the circle
		//	positions.push_back(glm::vec3(circle[i2].x/glm::radians(360.0), height, circle[i2].z/glm::radians(360.0)));
		//	uvs.push_back(circleUV[i0]); uvs.push_back(circleUV[i1]); uvs.push_back(circleUV[i2]);
		//	normals.push_back(normal); normals.push_back(normal); normals.push_back(normal);
		//	tangents.push_back(tangent); tangents.push_back(tangent); tangents.push_back(tangent);
		//}

		//Bottom section
		glm::vec3 normal = glm::vec3(0, -1, 0);											//Normal pointing downwards
		glm::vec3 tangent = glm::vec3(1, 0, 0);

		for (int i = 0; i < subDivisions - 2; i++)								//Loop, 2 less than there are vertices
		{
			int i0, i1, i2;
			i0 = 0; i1 = i + 1; i2 = i + 2;

			positions.push_back(glm::vec3(circle[i0].x, -height, circle[i0].z));
			positions.push_back(glm::vec3(circle[i1].x, -height, circle[i1].z));
			positions.push_back(glm::vec3(circle[i2].x, -height, circle[i2].z));
			uvs.push_back(circleUV[i0]); uvs.push_back(circleUV[i1]); uvs.push_back(circleUV[i2]);
			normals.push_back(normal); normals.push_back(normal); normals.push_back(normal);
			tangents.push_back(tangent); tangents.push_back(tangent); tangents.push_back(tangent);
		}

		//Sides

		for (int i = 0; i < subDivisions; i++)
		{
			glm::vec3 pos[4]; glm::vec3 normal[4]; glm::vec3 tangent[4]; glm::vec2 uv[4];		//Quad for each division within the loop
			//Vertex 0																			//Assign the data attributes, from diagram on Moodle
			pos[0] = circle[i] + glm::vec3(0, -height, 0);
			normal[0] = circle[i];
			tangent[0] = glm::cross(circle[i], glm::vec3(0, 1, 0));
			uv[0] = glm::vec2((float)i / subDivisions, 0);
			//CONE SHAPE, remove the circle[i] from the two top vertices X positions, leave them with just a height
			//Vertex 1
			pos[1] = /*circle[i] +*/ glm::vec3(0, height, 0);
			normal[1] = circle[i];
			tangent[1] = glm::cross(circle[i], glm::vec3(0, 1, 0));
			uv[1] = glm::vec2((float)i / subDivisions, 1);
			//Vertex 2
			pos[2] = /*circle[(i + 1) % subDivisions] + */glm::vec3(0, height, 0);
			normal[2] = circle[(i + 1) % subDivisions];
			tangent[2] = glm::cross(circle[(i + 1) % subDivisions], glm::vec3(0, 1, 0));
			uv[2] = glm::vec2((float)(i + 1) / subDivisions, 1);
			//Vertex 3
			pos[3] = circle[(i + 1) % subDivisions] + glm::vec3(0, -height, 0);
			normal[3] = circle[(i + 1) % subDivisions];
			tangent[3] = glm::cross(circle[i], glm::vec3(0, 1, 0));
			uv[3] = glm::vec2((float)(i + 1) / subDivisions, 0);

			int triangles[] = { 0,1,2,0,2,3 };				//Draws a triangle from the vertex positions

			for (int i = 0; i < 6; i++)						//6 vertices to loop through
			{
				positions.push_back(pos[triangles[i]]);			//Push back onto triangle vertex arrays
				normals.push_back(normal[triangles[i]]);
				tangents.push_back(tangent[triangles[i]]);
				uvs.push_back(uv[triangles[i]]);
			}

		}



	}


public:
	Cone(float height = 1.0f, int subDivisions = 16) { CreateConeGeometry(height, subDivisions); }
	const std::vector<glm::vec3>& GetPositionVector() const { return positions; }
	const std::vector<glm::vec3>& GetNormalVector() const { return normals; }
	const std::vector<glm::vec2>& GetUVVector() const { return uvs; }
	const std::vector<glm::vec3>& GetTangentVector() const { return tangents; }
	int GetNumVertices() const { return positions.size(); }

};