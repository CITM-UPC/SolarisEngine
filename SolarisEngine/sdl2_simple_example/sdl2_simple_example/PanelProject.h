#pragma once
#ifndef __PANEL_PROJECT_H__
#define __PANEL_PROJECT_H__

#include "Panel.h" // Include the base Panel class
#include "imgui.h"
#include <filesystem> // For file system traversal
#include <string>

class PanelProject : public Panel {
public:
    PanelProject();
    ~PanelProject();

    void Render() override; // Implements the Render method

private:
    void ShowFileSystemTree(const std::filesystem::path& path); // Recursive method to display file system

    std::string projectPath; // Path to the project directory
};

#endif // __PANEL_PROJECT_H__
