#pragma once
#ifndef __PANEL_SCENE_H__
#define __PANEL_SCENE_H__

#include <vector>
#include <string>
#include "Panel.h" // Incluir la clase base Panel



class PanelScene : public Panel { // Hereda de Panel
public:
    PanelScene();
    ~PanelScene();

    void Render() override; // Implementa el método Render
    void RenderContext() override; // Implementa el método Render

private:
   
};

#endif // __PANEL_SCENE_H__
