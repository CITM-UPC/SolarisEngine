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

    void Render() override; // Implementa el m�todo Render
    void RenderContext() override; // Implementa el m�todo Render

    void PlayPauseOverlay();

    ImVec2 GetMousePos();

    


    float width;
    float height;

    ImVec2 scenePanelPos;
    ImVec2 scenePanelSize;

private:
   
};

#endif // __PANEL_SCENE_H__
