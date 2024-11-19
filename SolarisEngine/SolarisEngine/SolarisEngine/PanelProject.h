#pragma once
#ifndef __PANEL_PROJECT_H__
#define __PANEL_PROJECT_H__

#include "Panel.h" // Include the base Panel class
#include "imgui.h"
#include <filesystem> // For file system traversal
#include <string>
#include <unordered_map>
#include <stack>

class PanelProject : public Panel {
public:
    PanelProject();
    ~PanelProject();

    void Render() override; // Implements the Render method
    void RenderContext() override; // Implements the Render method

    void CreateFolder(const std::filesystem::path& parentPath);

    bool DeleteFolder(const std::filesystem::path& folderPath);

private:
    void ShowFileSystemTree(const std::filesystem::path& path); // Recursive method to display file system
    void ShowBreadcrumbNavigation();

    ImTextureID textureID;
    std::string projectPath; // Path to the project directory
    std::unordered_map<std::string, ImTextureID> icons; // Store icons for folders and files

    std::filesystem::path currentPath;
    std::string selectedItem ;
    std::stack<std::filesystem::path> pathStack;

    std::string itemToDelete;           // Guarda el nombre del archivo o carpeta a eliminar
    std::filesystem::path deletePath;    // Guarda la ruta completa del archivo o carpeta a eliminar
    bool showDeletePopup = false;        // Estado para mostrar el popup de confirmación
    bool showDeleteConfirmation = false;
    void ShowDeleteConfirmation();

    std::filesystem::path pathToDelete;    // Guarda la ruta del archivo/carpeta para eliminar después del bucle
    bool pendingDelete = false;            // Indica si hay una eliminación pendiente

    bool userDrop = false;

    bool panelFileOpen = false;
    std::unordered_map<std::string, std::string> iconTypes = {
        {".png", "png"},
        {".jpg", "jpg"},
        {".jpeg", "jpg"},
        {".fbx", "fbx"},
        {".mp3", "audio"},
        {".wav", "audio"},
        {".txt", "text"},
        {".md", "text"},
        {".mat", "material"},
        {".go", "gameObject"}
    };


};

#endif // __PANEL_PROJECT_H__
