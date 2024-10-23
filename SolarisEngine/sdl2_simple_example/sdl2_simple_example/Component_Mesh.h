#pragma once
#ifndef __COMPONENT_MESH_H__
#define __COMPONENT_MESH_H__

#include "Component.h"
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <GL/glew.h>


class Component_Mesh : public Component {
public:
    Component_Mesh(std::shared_ptr<GameObject> containerGO);
    virtual ~Component_Mesh() override;

    void Enable() override;
    void Disable() override;
    void Update(double dt) override;
    void DrawComponent() override;

    void LoadMesh(aiMesh* ai_mesh);

    // Métodos específicos para el mesh
    void LoadMesh(const std::string& path);

    GLuint vao;
};

#endif // !__COMPONENT_MESH_H__
