#include "PanelProject.h"
#include "imgui.h"
#include "Defs.h"
#include "App.h"
#include "stb_image.h"
#include "TextureLoader.h"


namespace fs = std::filesystem;

// Constructor
PanelProject::PanelProject() {
    // Set your project path here
    projectPath = ASSETS_DIR; // Replace with your actual project path
    // Load your icons here, for example:
    icons["folder"] = app->textureLoader->LoadTexture("./Assets/file.png");
    icons["file"] = app->textureLoader->LoadTexture("./Assets/file.png");
    if (icons.find("folder") == icons.end()) {
        ImGui::Text("Folder icon not loaded!");
        printf("Folder icon not loaded!\n");
    }
    else {
        printf("Icons loaded: %zu\n", icons.size()); 
    }

    //ImTextureID textureID = app->textureLoader->LoadTexture("./Assets/file.png");

    //if (textureID) {
    //    printf("Texture loaded!");
    //    ImGui::Image(textureID, ImVec2(64, 64));
    //}
    //else {
    //   printf("Texture not loaded!");
    //}
    //if (textureID == nullptr) {
    //    //ImGui::Text("Failed to load texture: ./Assets/file.png");
    //    printf("Failed to load texture: ./Assets/file.png\n");
    //}
    //else {
    //    ImGui::Image(textureID, ImVec2(64, 64));
    //    printf("True to load texture: ./Assets/file.png\n");
    //}

}

// Destructor
PanelProject::~PanelProject() {
    // Cleanup if necessary
}

void PanelProject::Render() {
    if (!app->windowEditor->_windowImGui->projectPanel) {
        return;
    }

    ImGui::Begin("Project Explorer"); // Begin creating the panel window

    ShowFileSystemTree(projectPath); // Show the file system starting from the project path

    ImGui::End(); // End panel window
}

void PanelProject::ShowFileSystemTree(const std::filesystem::path& path) {
    for (const auto& entry : fs::directory_iterator(path)) {
        const auto& entryPath = entry.path();
        if (entry.is_directory()) {
            if (ImGui::TreeNode(entryPath.filename().string().c_str())) {
                ImGui::Image(app->textureLoader->LoadTexture("./Assets/file.png"), ImVec2(64, 64));
                ShowFileSystemTree(entryPath); // Recursive call for subdirectories
                ImGui::TreePop();
            }
        }
        else if (entry.is_regular_file()) {
            ImGui::Bullet();
            ImGui::Image(app->textureLoader->LoadTexture("./Assets/file.png"), ImVec2(64, 64)); // Display file icon
            ImGui::SameLine();
            ImGui::Text("%s", entryPath.filename().string().c_str());
        }
    }
}