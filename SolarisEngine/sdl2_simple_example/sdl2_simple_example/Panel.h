#ifndef __PANEL_H__
#define __PANEL_H__



class Panel {
public:
    virtual ~Panel() = default; // Destructor virtual
    virtual void Render() = 0; // Método puro virtual para renderizar el panel
    virtual void RenderContext() = 0; // Método puro virtual para renderizar el panel
};

#endif // __PANEL_H__
