#pragma once
#include "stdafx.h"
#include "ShaderProgram.h"
#include "Geometry.h"
#include "SphereGeom.h"
#include "Transforms.h"
#include "Drawable.h"
#include "PointLight.h"




class Emissive : public Drawable
{
	SOF::ShaderProgram* shader;
	SOF::Geometry* geom;
	glm::vec4 color;
	float cycleRate;


	void CreateGeometry( const GeometryHelper &geometry)
	{
		shader = new SOF::ShaderProgram("shaders/v_emissive.glsl", "shaders/f_emissive.glsl");
		geom = new SOF::Geometry(geometry.GetNumVertices());
		geom->AddAttribute(geometry.GetPositionVector(), "vertexPos");
		geom->Finalize(shader);
	}

public:
	Emissive(const GeometryHelper &geometry, const glm::vec4& color ) : color(color) {CreateGeometry(geometry);}
	Emissive(const GeometryHelper& geometry) { CreateGeometry(geometry); }
	virtual void Draw(const Transforms &trans, const PointLight& light) override
	{
		glm::mat4 mvp;
		mvp = trans.GetProj() * trans.GetView() * trans.GetModel();
		shader->Use();
		shader->SetUniformMat4("mvp", mvp);
		shader->SetUniformVec4("color", color);
		shader->SetUniformFloat("cycleTime", cycleRate);
		geom->DrawPrimitives();
	}
	void Draw(const Transforms& trans, const glm::vec4& color) 
	{
		glm::mat4 mvp;
		mvp = trans.GetProj() * trans.GetView() * trans.GetModel();
		shader->Use();
		shader->SetUniformMat4("mvp", mvp);
		shader->SetUniformVec4("color", color);
		geom->DrawPrimitives();
	}
	void SetTime(float newTime) { cycleRate = newTime; }
	float GetTime() const { return cycleRate; }
};

//Set updatecall back in emissive class
//Use moodle Q&a to change colour over time
//Cycle - Sin functions for r,g,b channel
//Use different time constants, every 2 seconds change value
//pass time as uniform float into f shader