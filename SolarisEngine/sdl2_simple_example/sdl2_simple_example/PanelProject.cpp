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
    icons["image"] = (int*)app->textureLoader->LoadTextureDevIL("./Assets/file2.png");
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

    // 在一个单独的区域显示面包屑导航
    ImGui::BeginChild("BreadcrumbNavigation", ImVec2(0, 20), false, ImGuiWindowFlags_NoScrollbar);
    ShowBreadcrumbNavigation();
    ImGui::EndChild();

    // 添加一些垂直间距，使文件和文件夹显示在面包屑导航下方
    ImGui::Separator(); // 添加一个分隔线，使布局更清晰

    // 显示当前路径的文件系统树
    ShowFileSystemTree(currentPath);

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
        bool isDirectory = entry.is_directory();
        bool isHovered = false;
        bool isSelected = (fileName == selectedItem);

        ImGui::BeginGroup();

        // 判断文件类型并选择合适的图标
        void* icon;
        if (isDirectory) {
            icon = icons["folder"]; // 文件夹图标
        }
        else {
            std::string extension = entryPath.extension().string();
            if (extension == ".png" || extension == ".jpg" || extension == ".jpeg") {
                icon = icons["image"]; // 图片文件图标
            }
            else if (extension == ".mp3" || extension == ".wav") {
                icon = icons["audio"]; // 音频文件图标
            }
            else if (extension == ".txt" || extension == ".md") {
                icon = icons["text"]; // 文本文件图标
            }
            else {
                icon = icons["file"]; // 通用文件图标
            }
        }

        // 使用透明背景的按钮样式
        ImGui::PushStyleColor(ImGuiCol_Button, isSelected ? ImVec4(0.0f, 0.0f, 1.0f, 0.5f) : ImVec4(0, 0, 0, 0));

        // 根据文件类型绘制图标按钮
        isHovered = ImGui::ImageButton((ImTextureID)icon, ImVec2(iconSize, iconSize));

        if (isHovered && isDirectory) {
            selectedItem = fileName; // 更新选中项
            pathStack.push(currentPath); // 将当前路径压入栈
            currentPath = entryPath; // 更新当前路径到选中文件夹
        }
        else if (isHovered && !isDirectory) {
            selectedItem = fileName; // 更新选中项
            // 可以在这里添加文件被选中时的行为，比如打开文件等
        }

        ImGui::PopStyleColor(); // 恢复按钮样式

        // 显示文件名
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


void PanelProject::ShowBreadcrumbNavigation() {
    std::filesystem::path pathPart;
    bool showBreadcrumb = false;

    // 遍历 currentPath 的每一部分
    for (const auto& part : currentPath) {
        if (part == "Assets") {
            showBreadcrumb = true; // 从 "Assets" 文件夹开始显示面包屑
        }

        if (showBreadcrumb) {
            pathPart /= part; // 累积当前的路径部分

            // 为每个面包屑创建一个可点击的按钮
            if (ImGui::Button(part.string().c_str())) {
                // 点击后，将 currentPath 设置为选中的路径层级
                currentPath = pathPart;

                // 清除 pathStack 中深层次的路径，保持和当前路径一致
                while (!pathStack.empty() && pathStack.top() != currentPath) {
                    pathStack.pop();
                }
                pathStack.push(currentPath); // 将新的 currentPath 压入栈中
                selectedItem = ""; // 清除选中项，因为路径已经改变
                break;
            }

            // 如果不是最后一个路径部分，添加分隔符
            ImGui::SameLine();
            ImGui::Text(">");
            ImGui::SameLine();
        }
    }
}








