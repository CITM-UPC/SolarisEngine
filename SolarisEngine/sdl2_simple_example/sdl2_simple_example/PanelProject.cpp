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
    const float iconSize = 64.0f;       // Tamaño del ícono
    const float padding = 20.0f;         // Espacio entre íconos
    const float textMaxWidth = iconSize; // Ancho máximo del texto
    const float itemTotalWidth = iconSize + padding; // Ancho total que ocupa cada ítem

    // Obtener el ancho disponible del panel y calcular el máximo de ítems por fila
    float panelWidth = ImGui::GetContentRegionAvail().x;
    int maxItemsPerRow = static_cast<int>(panelWidth / itemTotalWidth);
    if (maxItemsPerRow < 1) maxItemsPerRow = 1; // Al menos un ítem por fila

    int itemsInRow = 0;  // Número de ítems en la fila actual

    for (const auto& entry : fs::directory_iterator(path)) {
        const auto& entryPath = entry.path();

        ImGui::BeginGroup();

        // Seleccionar el ícono correspondiente según si es un directorio
        if (entry.is_directory()) {
            ImGui::Image(icons["folder"], ImVec2(iconSize, iconSize));
        }
        else {
            ImGui::Image(icons["file"], ImVec2(iconSize, iconSize));
        }

        // Obtener el nombre del archivo y calcular el desplazamiento para centrar
        std::string fileName = entryPath.filename().string();
        ImGui::PushTextWrapPos(ImGui::GetCursorPosX() + textMaxWidth);

        // Si el nombre del archivo es muy largo, truncarlo
        float textWidth = ImGui::CalcTextSize(fileName.c_str()).x;
        if (textWidth > textMaxWidth) {
            fileName = fileName.substr(0, 10) + "...";
        }

        // Establecer el desplazamiento horizontal para centrar el texto
        float centeredTextPos = ImGui::GetCursorPosX() + (iconSize - std::min(textWidth, textMaxWidth)) / 2;
        ImGui::SetCursorPosX(centeredTextPos);
        ImGui::Text("%s", fileName.c_str());

        ImGui::PopTextWrapPos();
        ImGui::EndGroup();

        itemsInRow++;

        // Al llegar al máximo de ítems por fila, hacer salto de línea; en caso contrario, mantener el espacio horizontal
        if (itemsInRow >= maxItemsPerRow) {
            ImGui::NewLine();
            itemsInRow = 0;
        }
        else {
            ImGui::SameLine(0, padding);  // Añadir el espacio entre ítems
        }
    }
}






