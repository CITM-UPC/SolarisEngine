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

private:
    void ShowFileSystemTree(const std::filesystem::path& path); // Recursive method to display file system
    void ShowBreadcrumbNavigation();
    ImTextureID textureID;
    std::string projectPath; // Path to the project directory
    std::unordered_map<std::string, ImTextureID> icons; // Store icons for folders and files

    std::filesystem::path currentPath;
    std::string selectedItem ;
    std::stack<std::filesystem::path> pathStack;
};

#endif // __PANEL_PROJECT_H__
