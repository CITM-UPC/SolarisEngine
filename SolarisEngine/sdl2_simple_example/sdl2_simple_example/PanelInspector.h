#ifndef __PANEL_INSPECTOR_H__
#define __PANEL_INSPECTOR_H__

#include "Panel.h" // Incluir la clase base Panel

class PanelInspector : public Panel { // Hereda de Panel
public:
    PanelInspector();
    ~PanelInspector();

    void Render() override; // Implementa el m�todo Render
};

#endif // __PANEL_INSPECTOR_H__
