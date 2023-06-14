#pragma once
#include "stdafx.h"
#include "Drawable.h"
#include <string>
#include "MemTexture.h"

class Terrain : public Drawable
{
	float xzScale;
	float yScale;
	float deltaU;
	float deltaV;
	SOF::Texture* heightMap;
	SOF::Texture* splatMap;
	SOF::Texture* tex0;
	SOF::Texture* tex1;
	SOF::Texture* tex2;
	SOF::Geometry* geom;
	SOF::ShaderProgram* shader;
	MemTexture* memTexture;
public:
	Terrain(const std::string& heightMapFile, const std::string& splatMapFile,
		const std::string& tex0File, const std::string& tex1File,
		const std::string& tex2File, float xzScale, float yScale) : xzScale(xzScale), yScale(yScale)
	{
		heightMap = new SOF::Texture(heightMapFile);
		splatMap = new SOF::Texture(splatMapFile);
		memTexture = new MemTexture(heightMapFile);

		tex0 = new SOF::Texture(tex0File);
		tex1 = new SOF::Texture(tex1File);
		tex2 = new SOF::Texture(tex2File);
		int width, height;
		width = heightMap->GetWidth();
		height = heightMap->GetHeight();
		std::vector<glm::vec2> uv;
		// size of a triangle in uv coordinates
		deltaU = 1.0f / (float)(width - 1);
		deltaV = 1.0f / (float)(height - 1);
		for (int i = 0; i < width - 1; i++)
		{
			for (int j = 0; j < height - 1; j++)
			{
				glm::vec2 verts[4] = { glm::vec2(i * deltaU, j * deltaV), glm::vec2(i * deltaU, (j + 1) * deltaV),
				glm::vec2((i + 1) * deltaU, (j + 1) * deltaV), glm::vec2((i + 1) * deltaU, j * deltaV)
				};
				// 2 triangles - 0,1,2 and 1,2,3
				uv.push_back(verts[0]);
				uv.push_back(verts[1]);
				uv.push_back(verts[2]);
				uv.push_back(verts[0]);
				uv.push_back(verts[2]);
				uv.push_back(verts[3]);
			}
		}
		geom = new SOF::Geometry(uv.size());
		shader = new SOF::ShaderProgram("shaders/v_terrain.glsl", "shaders/f_terrain.glsl");
		geom->AddAttribute(uv, "vertexUV");
		geom->Finalize(shader);
	}
	void Draw(const Transforms& renderTransform, const PointLight& light) override
	{
		glm::mat4 mvp = renderTransform.GetProj() * renderTransform.GetView() * renderTransform.GetModel();
		shader->Use();
		shader->SetUniformMat4("mvp", mvp);
		shader->SetUniformFloat("xzScale", xzScale);
		shader->SetUniformFloat("yScale", yScale);
		shader->SetUniformFloat("deltaU", deltaU);
		shader->SetUniformFloat("deltaV", deltaV);
		shader->SetUniformFloat("texRepeat", 128.0f);
		shader->AssignTextureSampler("heightMap", heightMap);
		shader->AssignTextureSampler("splatMap", splatMap);
		shader->AssignTextureSampler("tex0", tex0);
		shader->AssignTextureSampler("tex1", tex1);
		shader->AssignTextureSampler("tex2", tex2);
		geom->DrawPrimitives();
	}

	//Function to spawn an object at a point Y on the terrain
	float AtPointY(const glm::vec3& point)
	{
		//make float x and z
		// point.x, point.y clamp them between 0 and 1
		// divide x by xz Scale an y by yScale
		// use MemTexture to look it up
		// return heightAtMapY which is a value
		glm::vec2 uv;

		uv.x = fmin(1.0f, fmax(point.x/xzScale, 0.0f));
		uv.y = fmin(1.0f, fmax(point.z/xzScale, 0.0f));

		float height = yScale*memTexture->Sample(uv).x;
	
		//MemTexture* heightMapPoint;
		//heightMapPoint->Sample(uv);
		return height;
		//heightMapPoint->Sample(mem);
		//return heightMapPoint;
	 
		
		

	}
		

	virtual ~Terrain() {};
};


