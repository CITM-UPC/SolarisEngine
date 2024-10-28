#include "PanelProject.h"
#include "imgui.h"
#include "Defs.h"

namespace fs = std::filesystem;

// Constructor
PanelProject::PanelProject() {
    // Set your project path here
    projectPath =ASSETS_DIR; // Replace with your actual project path
}

// Destructor
PanelProject::~PanelProject() {
    // Cleanup if necessary
}

void PanelProject::Render() {
    ImGui::Begin("Project Explorer"); // Begin creating the panel window

    ShowFileSystemTree(projectPath); // Show the file system starting from the project path

    ImGui::End(); // End panel window
}

void PanelProject::ShowFileSystemTree(const std::filesystem::path& path) {
    for (const auto& entry : fs::directory_iterator(path)) {
        const auto& entryPath = entry.path();
        if (entry.is_directory()) {
            // Render directory with a tree node
            if (ImGui::TreeNode(entryPath.filename().string().c_str())) {
                ShowFileSystemTree(entryPath); // Recursive call for subdirectories
                ImGui::TreePop();
            }
        }
        else if (entry.is_regular_file()) {
            // Render file as a leaf node
            ImGui::BulletText("%s", entryPath.filename().string().c_str());
        }
    }
}
