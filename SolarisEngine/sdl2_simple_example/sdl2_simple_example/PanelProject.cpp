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
    icons["unknow"] = (int*)app->textureLoader->LoadTextureDevIL("./Library/Icons/desconocido.png");
    icons["folder"] = (int*)app->textureLoader->LoadTextureDevIL("./Library/Icons/folder.png");
    icons["file"] = (int*)app->textureLoader->LoadTextureDevIL("./Library/Icons/file.png");
    icons["text"] = (int*)app->textureLoader->LoadTextureDevIL("./Library/Icons/txt.png");
    icons["png"] = (int*)app->textureLoader->LoadTextureDevIL("./Library/Icons/png.png");
    icons["jpg"] = (int*)app->textureLoader->LoadTextureDevIL("./Library/Icons/jpg.png");
    icons["fbx"] = (int*)app->textureLoader->LoadTextureDevIL("./Library/Icons/fbx.png");
    icons["image"] = (int*)app->textureLoader->LoadTextureDevIL("./Library/Icons/image.png");
    icons["material"] = (int*)app->textureLoader->LoadTextureDevIL("./Library/Icons/material.png");
    icons["gameObject"] = (int*)app->textureLoader->LoadTextureDevIL("./Library/Icons/gameObject.png");
    currentPath = projectPath; // ruta actual
    selectedItem = ""; // item selecionado
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

    // breadnavigation
    ImGui::BeginChild("BreadcrumbNavigation", ImVec2(0, 20), false, ImGuiWindowFlags_NoScrollbar);
    ShowBreadcrumbNavigation();
    ImGui::EndChild();

    
    ImGui::Separator(); 

    // ruta y arbol
    ShowFileSystemTree(currentPath);

    RenderContext();

    ImGui::End();



    //// Realizar la eliminación después de la iteración
    // NO FUNCIONA BIEN DEJAR COMENTADO
    //if (pendingDelete) {
    //    try {
    //        if (fs::is_directory(pathToDelete)) {
    //            fs::remove_all(pathToDelete);
    //        }
    //        else {
    //            fs::remove(pathToDelete);
    //        }
    //        selectedItem = "";         // Limpia el elemento seleccionado
    //    }
    //    catch (const fs::filesystem_error& e) {
    //        printf("Error al eliminar '%s': %s\n", pathToDelete.string().c_str(), e.what());
    //    }

    //    pendingDelete = false;          // Restablece el estado
    //    pathToDelete.clear();           // Limpia la ruta a eliminar
    //}

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

    int i = 0;

    for (const auto& entry : fs::directory_iterator(path)) {


        i++;

        const auto& entryPath = entry.path();
        std::string fileName = entryPath.filename().string();
        bool isDirectory = entry.is_directory();
        bool isHovered = false;
        bool isSelected = (fileName == selectedItem);

        ImGui::BeginGroup();

        // que icono
        void* icon;
        if (isDirectory) {
            icon = icons["folder"]; // carpeta
        }
        else {
            std::string extension = entryPath.extension().string();

            if (iconTypes.find(extension) != iconTypes.end()) {
                icon = icons[iconTypes[extension]]; // Icono según tipo
            }
            else {
                icon = icons["unknow"]; // Otros
            }
        }

        // fondo trasparente
        ImGui::PushStyleColor(ImGuiCol_Button, isSelected ? ImVec4(0.0f, 0.0f, 1.0f, 0.5f) : ImVec4(0, 0, 0, 0));

        std::string uniqueId = "##" + fileName + "_" + std::to_string(i);
        isHovered = ImGui::ImageButton(uniqueId.c_str(), (ImTextureID)icon, ImVec2(iconSize, iconSize));

        if (isHovered && isDirectory) {
            selectedItem = fileName; 
            pathStack.push(currentPath); 
            currentPath = entryPath;
        }
        else if (isHovered && !isDirectory) {
            selectedItem = fileName; 
           
        }

        ImGui::PopStyleColor(); 

        // nombre de fitxe
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

  
    for (const auto& part : currentPath) {
        if (part == "Assets") {
            showBreadcrumb = true; 
        }

        if (showBreadcrumb) {
            pathPart /= part;

            
            if (ImGui::Button(part.string().c_str())) {
               
                currentPath = pathPart;

               
                while (!pathStack.empty() && pathStack.top() != currentPath) {
                    pathStack.pop();
                }
                pathStack.push(currentPath); 
                selectedItem = ""; 
                break;
            }

            
            ImGui::SameLine();
            ImGui::Text(">");
            ImGui::SameLine();
        }
    }



    if (showDeletePopup) {
        ImGui::OpenPopup("Confirm Delete");  // Abre el popup de confirmación
    }

    if (ImGui::BeginPopupModal("Confirm Delete", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::Text("Are you sure you want to delete '%s'?", itemToDelete.c_str());
        ImGui::Separator();

        bool enterPressed = ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Enter));

        if (ImGui::Button("OK", ImVec2(120, 0)) || enterPressed) {
            pathToDelete = deletePath / itemToDelete;
            pendingDelete = true;        // Activa el estado pendiente de eliminación
            showDeletePopup = false;     // Cierra el popup
            ImGui::CloseCurrentPopup();
        }

        ImGui::SameLine();

        if (ImGui::Button("Cancel", ImVec2(120, 0))) {
            showDeletePopup = false;  // Cierra el popup sin eliminar
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }

}

void PanelProject::RenderContext()
{

    if (ImGui::BeginPopupContextWindow("ProjectContextMenu", 1)) {
        if (ImGui::MenuItem("New Folder")) {
            CreateFolder(currentPath);
        }

        if (ImGui::MenuItem("Load GameObject")) {
            
        }

        if (ImGui::MenuItem("Delete")) {
            itemToDelete = selectedItem;
            deletePath = currentPath / itemToDelete;
            showDeletePopup = true;  // Activa el estado para mostrar el popup
        }

        if (ImGui::MenuItem("Copy")) {

           
        }

        if (ImGui::MenuItem("Paste")) {
            
        }

        ImGui::EndPopup();
    }




}


void PanelProject::CreateFolder(const std::filesystem::path& parentPath) {
    std::string folderName = "Nueva carpeta";
    std::filesystem::path newFolderPath = parentPath / folderName;

    int i = 1;
    while (fs::exists(newFolderPath)) {
        newFolderPath = parentPath / (folderName + " (" + std::to_string(i) + ")");
        ++i;
    }

    try {
        fs::create_directory(newFolderPath);
    }
    catch (const fs::filesystem_error& e) {
        // Manejar el error si ocurre
    }
}








