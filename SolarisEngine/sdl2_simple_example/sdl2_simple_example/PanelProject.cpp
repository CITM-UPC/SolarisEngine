#include "PanelProject.h"
#include "imgui.h"
#include "Defs.h"
#include "App.h"
#include "stb_image.h"
#include "TextureLoader.h"


namespace fs = std::filesystem;
// Constructor
PanelProject::PanelProject() {
    projectPath = ASSETS_DIR;
    icons["folder"] = (int*)app->textureLoader->LoadTextureDevIL("./Assets/file2.png");
    icons["file"] = (int*)app->textureLoader->LoadTextureDevIL("./Assets/file2.png");
    currentPath = projectPath; // 当前路径
    selectedItem = ""; // 选中的项目
    pathStack.push(currentPath);
}

// Destructor
PanelProject::~PanelProject() {
	// Cleanup if necessary
}

void PanelProject::Render() {
    if (!app->windowEditor->GetImGuiWindow()->showprojectPanel) {
        return;
    }

    ImGui::Begin("Project Explorer");
    ShowFileSystemTree(currentPath); // 显示当前路径的文件系统树


    ImGui::End();
}

void PanelProject::ShowFileSystemTree(const std::filesystem::path& path) {
    const float iconSize = 64.0f;
    const float padding = 20.0f;
    const float textMaxWidth = iconSize;
    const float itemTotalWidth = iconSize + padding;

    float panelWidth = ImGui::GetContentRegionAvail().x;
    int maxItemsPerRow = static_cast<int>(panelWidth / itemTotalWidth);
    if (maxItemsPerRow < 1) maxItemsPerRow = 1;

    int itemsInRow = 0;

    for (const auto& entry : fs::directory_iterator(path)) {
        const auto& entryPath = entry.path();
        std::string fileName = entryPath.filename().string();

        ImGui::BeginGroup();

        // 检查选中状态和悬停状态
        bool isHovered = false;
        bool isSelected = (fileName == selectedItem);

        // 使用透明背景的按钮样式
        ImGui::PushStyleColor(ImGuiCol_Button, isSelected ? ImVec4(0.0f, 0.0f, 1.0f, 0.5f) : ImVec4(0, 0, 0, 0)); // 默认透明背景

        if (entry.is_directory()) {
            // 点击文件夹时，进入该文件夹并更新路径栈

            isHovered = ImGui::ImageButton((ImTextureID)icons["folder"], ImVec2(iconSize, iconSize));
            if (isHovered) {
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 1.0f, 0.5f)); // 悬停背景
            }

            if (isHovered || isSelected) {
                selectedItem = fileName; // 更新选中项
                pathStack.push(currentPath); // 压入当前路径到栈
                currentPath = entryPath; // 更新当前路径
               
            }
        }
        else {
            // 点击文件时，更新选中项
            isHovered = ImGui::ImageButton((ImTextureID)icons["file"], ImVec2(iconSize, iconSize));
            if (isHovered) {
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 1.0f, 0.5f)); // 悬停背景
            }

            if (isHovered) {
                selectedItem = fileName; // 更新选中项
            }
        }

        ImGui::PopStyleColor(); // 恢复按钮样式

        ImGui::PushTextWrapPos(ImGui::GetCursorPosX() + textMaxWidth);
        float textWidth = ImGui::CalcTextSize(fileName.c_str()).x;
        if (textWidth > textMaxWidth) {
            fileName = fileName.substr(0, 10) + "...";
        }

        float centeredTextPos = ImGui::GetCursorPosX() + (iconSize - std::min(textWidth, textMaxWidth)) / 2;
        ImGui::SetCursorPosX(centeredTextPos);
        ImGui::Text("%s", fileName.c_str());
        ImGui::PopTextWrapPos();
        ImGui::EndGroup();

        itemsInRow++;
        if (itemsInRow >= maxItemsPerRow) {
            ImGui::NewLine();
            itemsInRow = 0;
        }
        else {
            ImGui::SameLine(0, padding);
        }
    }
}









