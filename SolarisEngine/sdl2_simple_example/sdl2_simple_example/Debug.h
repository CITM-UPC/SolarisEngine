#ifndef __DEBUG_H__
#define __DEBUG_H__

#include "App.h"
#include <sstream>
#include <string>
#include <iostream> // Para std::cout

class Debug {
public:
  
    template<typename... Args>
    static void Log(Args&&... args) {
        std::ostringstream oss;
        (oss << ... << args); // Usar fold expression para insertar cada argumento
        LogImpl(oss.str());
    }

private:
    Debug();
    ~Debug();

    // M�todo que realmente maneja la salida
    static void LogImpl(const std::string& message) {
        std::cout << "Log: " << message << std::endl;
        // Suponiendo que app es una variable global o est� disponible de alguna forma aqu�:
        app->windowEditor->GetImGuiWindow()->consolaPanel->AddLog(message);
    }
};

#endif // DEBUG_H
