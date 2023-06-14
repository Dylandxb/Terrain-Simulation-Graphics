#pragma once
#include "stdafx.h"
#include "GeometryHelper.h"

class CylinderGeom : public GeometryHelper
{
	std::vector<glm::vec3> positions;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec3> tangents;
	std::vector<glm::vec2> uvs;

	void CreateCylinderGeometry(float height, int subDivisions, bool faceted)
	{
		//top and bottom circle faces
		glm::vec3* circle = new glm::vec3[subDivisions];						//Arrays of positions and uvs on the circle
		glm::vec2* circleUV = new glm::vec2[subDivisions];
		for (int i = 0; i < subDivisions; i++)
		{
			float theta = glm::radians(360.0) * (float)i / (subDivisions - 1);
			circle[i] = glm::vec3(cos(theta), 0.0f, sin(theta));
			circleUV[i] = (glm::vec2(circle[i].x, circle[i].z) + glm::vec2(1.0f, 1.0f)) * 0.5f;
		}
		//Top section
		glm::vec3 normal = glm::vec3(0, 1, 0);
		glm::vec3 tangent = glm::vec3(1, 0, 0);

		for (int i = 0; i < subDivisions - 2; i++)								//Loop, 2 less triangles than there are vertices
		{
			int i0, i1, i2;														//Indices to signify the faces on a cylinder
			i0 = 0; i1 = i + 1; i2 = i + 2;

			positions.push_back(glm::vec3(circle[i0].x, height, circle[i0].z));			//Draws the positions from 3 points within the circle
			positions.push_back(glm::vec3(circle[i1].x, height, circle[i1].z));
			positions.push_back(glm::vec3(circle[i2].x, height, circle[i2].z));
			uvs.push_back(circleUV[i0]); uvs.push_back(circleUV[i1]); uvs.push_back(circleUV[i2]);
			normals.push_back(normal); normals.push_back(normal); normals.push_back(normal);
			tangents.push_back(tangent); tangents.push_back(tangent); tangents.push_back(tangent);
		}

		//Bottom section
		normal = glm::vec3(0, -1, 0);											//Normal pointing downwards
		tangent = glm::vec3(1, 0, 0);

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
			if (faceted == false)
			{
				glm::vec3 pos[4]; glm::vec3 normal[4]; glm::vec3 tangent[4]; glm::vec2 uv[4];		//Quad for each division within the loop
				//Vertex 0																			//Assign the data attributes, from diagram on Moodle
				pos[0] = circle[i] + glm::vec3(0, -height, 0);
				normal[0] = circle[i];
				tangent[0] = glm::cross(circle[i], glm::vec3(0, 1, 0));
				uv[0] = glm::vec2((float)i / subDivisions, 0);
				//Vertex 1
				pos[1] = circle[i] + glm::vec3(0, height, 0);
				normal[1] = circle[i];
				tangent[1] = glm::cross(circle[i], glm::vec3(0, 1, 0));
				uv[1] = glm::vec2((float)i / subDivisions, 1);
				//Vertex 2
				pos[2] = circle[(i + 1) % subDivisions] + glm::vec3(0, height, 0);
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


			else if (faceted == true)
			{
				glm::vec3 pos[4]; glm::vec3 normal[4]; glm::vec3 tangent[4]; glm::vec2 uv[4];		//Quad for each division within the loop
				//Vertex 0																			//Assign the data attributes, from diagram on Moodle
				pos[0] = circle[i] + glm::vec3(0, -height, 0);
				normal[0] = circle[i];
				tangent[0] = glm::cross(circle[i], glm::vec3(0, 1, 0));
				uv[0] = glm::vec2((float)i / subDivisions, 0);
				//Vertex 1
				pos[1] = circle[i] + glm::vec3(0, height, 0);
				normal[1] = circle[i];
				tangent[1] = glm::cross(circle[i], glm::vec3(0, 1, 0));
				uv[1] = glm::vec2((float)i / subDivisions, 1);
				//Vertex 2
				pos[2] = circle[(i + 1) % subDivisions] + glm::vec3(0, height, 0);
				normal[2] = circle[(i)]; //+ 1) % subDivisions];												//Sets normals of each vertex point to the same value circle[i]
				tangent[2] = glm::cross(circle[(i + 1) % subDivisions], glm::vec3(0, 1, 0));					//Gives it the facet shape because as you move along the circle, all the normals at the points of the triangle are facing same direction
				uv[2] = glm::vec2((float)(i + 1) / subDivisions, 1);
				//Vertex 3
				pos[3] = circle[(i + 1) % subDivisions] + glm::vec3(0, -height, 0);
				normal[3] = circle[(i)]; //+ 1) % subDivisions];
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



	}

public:
	CylinderGeom(float height = 1.0f, int subDivisions = 16, bool faceted = false) { CreateCylinderGeometry(height, subDivisions, faceted); }
	const std::vector<glm::vec3>& GetPositionVector() const { return positions; }
	const std::vector<glm::vec3>& GetNormalVector() const { return normals; }
	const std::vector<glm::vec2>& GetUVVector() const { return uvs; }
	const std::vector<glm::vec3>& GetTangentVector() const { return tangents; }
	int GetNumVertices() const { return positions.size(); }
};