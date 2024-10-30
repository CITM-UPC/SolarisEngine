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



	icons["folder"] = app->textureLoader->LoadTexture("./Assets/file.png");
	icons["file"] = app->textureLoader->LoadTexture("./Assets/file.png");
	if (icons.find("folder") == icons.end()) {
		ImGui::Text("Folder icon not loaded!");
		printf("Folder icon not loaded!\n");
	}
	else {
		printf("Icons loaded: %zu\n", icons.size());
	}
	ShowFileSystemTree(projectPath); // Show the file system starting from the project path
	/* textureID = app->textureLoader->LoadTexture("./Assets/file.png");

	 if (textureID) {
		 printf("Texture loaded!");
		 ImGui::Image(textureID, ImVec2(64, 64));
	 }
	 else {
		 printf("Texture not loaded!");
	 }
	 if (textureID == nullptr) {
		 ImGui::Text("Failed to load texture: ./Assets/file.png");
		 printf("Failed to load texture: ./Assets/file.png\n");
	 }
	 else {
		 ImGui::Image(textureID, ImVec2(64, 64));
		 printf("True to load texture: ./Assets/file.png\n");
	 }*/

	ImGui::End(); // End panel window
}

void PanelProject::ShowFileSystemTree(const std::filesystem::path& path) {
    const float iconSize = 64.0f;       // 图标大小
    const float padding = 20.0f;         // 图标间距
    const float textMaxWidth = iconSize; // 文字最大宽度
    const float itemTotalWidth = iconSize + padding; // 每个项目所占的总宽度

    // 获取面板的可用宽度，并计算每行可显示的最大项目数
    float panelWidth = ImGui::GetContentRegionAvail().x;
    int maxItemsPerRow = static_cast<int>(panelWidth / itemTotalWidth);
    if (maxItemsPerRow < 1) maxItemsPerRow = 1; // 至少显示一个项目

    int itemsInRow = 0;  // 当前行的项目数

    for (const auto& entry : fs::directory_iterator(path)) {
        const auto& entryPath = entry.path();

        ImGui::BeginGroup();

        // 根据是否是目录选择相应的图标
        if (entry.is_directory()) {
            ImGui::Image(icons["folder"], ImVec2(iconSize, iconSize));
        }
        else {
            ImGui::Image(icons["file"], ImVec2(iconSize, iconSize));
        }

        // 获取文件名，并计算居中偏移量
        std::string fileName = entryPath.filename().string();
        ImGui::PushTextWrapPos(ImGui::GetCursorPosX() + textMaxWidth);

        // 若文件名过长则截断显示
        float textWidth = ImGui::CalcTextSize(fileName.c_str()).x;
        if (textWidth > textMaxWidth) {
            fileName = fileName.substr(0, 10) + "...";
        }

        // 设置文字的水平居中偏移量
        float centeredTextPos = ImGui::GetCursorPosX() + (iconSize - std::min(textWidth, textMaxWidth)) / 2;
        ImGui::SetCursorPosX(centeredTextPos);
        ImGui::Text("%s", fileName.c_str());

        ImGui::PopTextWrapPos();
        ImGui::EndGroup();

        itemsInRow++;

        // 如果达到每行的最大项目数则换行，否则保持水平间距
        if (itemsInRow >= maxItemsPerRow) {
            ImGui::NewLine();
            itemsInRow = 0;
        }
        else {
            ImGui::SameLine(0, padding);  // 添加间距
        }
    }
}





