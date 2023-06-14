#pragma once
#include "Drawable.h"
#include "ShaderProgram.h"
#include "Geometry.h"

class PolyLine : public Drawable
{
    SOF::ShaderProgram* shader;
    SOF::Geometry* geometry;
    glm::vec4 color;
public:
    PolyLine(const std::vector<glm::vec3>& points, const glm::vec4& color) : color(color)           //2 parameters, points of the line and colour of line
    {
        // load the shaders and create the geometry object
        shader = new SOF::ShaderProgram("shaders/v_emissive.glsl", "shaders/f_emissive.glsl");

        geometry = new SOF::Geometry(points.size());                                                //Geom object, size of points array
        geometry->AddAttribute(points, "vertexPos");                                                //Adds points as an attribute called vertexPos
        geometry->Finalize(shader);
    }
    virtual void Draw(const Transforms& t, const PointLight& l)
    {
        shader->Use();
        // set the shader uniforms here
        glm::mat4 mvp = t.GetProj() * t.GetView() * t.GetModel();   //   renderTransform.GetProj()* renderTransform.GetView()* renderTransform.GetModel();

        shader->SetUniformMat4("mvp", mvp);
        shader->SetUniformVec4("color", color);
        geometry->DrawLines();

    };
};