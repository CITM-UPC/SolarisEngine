#include "PanelProject.h"
#include "imgui.h"
#include "Defs.h"
#include "App.h"
#include "stb_image.h"
#include "TextureLoader.h"
#include "Debug.h"
#include "Input.h"

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

	static std::string lastClickedItem;
	static float lastClickTime = 0.0f;
	const float doubleClickTime = 0.3f; // Double click interval

	for (const auto& entry : fs::directory_iterator(path)) {
		i++;

		const auto& entryPath = entry.path();
		std::string fileName = entryPath.filename().string();
		bool isDirectory = entry.is_directory();
		bool isHovered = false;
		bool isSelected = (fileName == selectedItem);

		ImGui::BeginGroup();

		// Determine icon
		void* icon;
		if (isDirectory) {
			icon = icons["folder"]; // Folder
		}
		else {
			std::string extension = entryPath.extension().string();
			if (iconTypes.find(extension) != iconTypes.end()) {
				icon = icons[iconTypes[extension]]; // Icon based on type
			}
			else {
				icon = icons["unknown"]; // Other
			}
		}

		// Transparent background
		ImGui::PushStyleColor(ImGuiCol_Button, isSelected ? ImVec4(0.0f, 0.0f, 1.0f, 0.5f) : ImVec4(0, 0, 0, 0));

		std::string uniqueId = "##" + fileName + "_" + std::to_string(i);
		isHovered = ImGui::ImageButton(uniqueId.c_str(), (ImTextureID)icon, ImVec2(iconSize, iconSize));

		if (ImGui::IsItemHovered()) {
			isHovered = true;
		}

		if (isHovered) {
			float currentTime = ImGui::GetTime();
			if (ImGui::IsMouseClicked(0)) {
				if (fileName == lastClickedItem && (currentTime - lastClickTime) < doubleClickTime) {
					// Open file or folder on double click
					if (isDirectory) {
						selectedItem = fileName;
						pathStack.push(currentPath);
						currentPath = entryPath;
					}
					else {
						selectedItem = fileName;
						// Logic to open file
					}
				}
				else {
					// Select item and change color on single click
					selectedItem = fileName;
					lastClickedItem = fileName;
					lastClickTime = currentTime;
				}
			}

			// Combine both conditions to ensure only one popup is triggered
			if (ImGui::IsMouseClicked(1)) {
				if (ImGui::IsAnyItemHovered()) {
					// If hovering over an item, set selectedItem and open the corresponding menu
					Debug::Log("ProjectContextMenuFile");
					selectedItem = fileName;
					ImGui::OpenPopup("ProjectContextMenuFile");
				}
				else {
					// If not hovering over any item, clear selection and open the empty space menu
					Debug::Log("ProjectContextMenuEmpty");
					selectedItem = "";
					ImGui::OpenPopup("ProjectContextMenu");
				}
			}

		}

		//设置拖动源
		if (ImGui::BeginDragDropSource()) {
			ImGui::SetDragDropPayload("DND_FILE", fileName.c_str(), fileName.size() + 1);
			ImGui::Text("Dragging %s", fileName.c_str());
			ImGui::EndDragDropSource();
			userDrop = true;
		}
		else
		{
			userDrop = false;
		}


		// Scene Panel中的代码
		if ( userDrop == true) { // 确保这个名称与你现有的Scene Panel名称一致
			ImGui::Begin("Scene");
			// 获取面板的可用内容区域
			ImVec2 availableRegion = ImGui::GetContentRegionAvail();
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 1.0f, 0.0f));
			ImGui::Button("##dummy", availableRegion);
			ImGui::PopStyleColor();

			// 确保拖动目标在整个内容区域内有效
			if (ImGui::BeginDragDropTarget()) {
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DND_FILE")) {
					const char* payloadFileName = static_cast<const char*>(payload->Data);
					std::filesystem::path draggedFilePath = currentPath / payloadFileName;

					// 打印消息，验证是否进入了拖放目标
					printf("File '%s' dropped into Scene Panel\n", payloadFileName);

					// 检查文件扩展名是否为 .fbx
					if (draggedFilePath.extension() == ".fbx") {
						GameObject* gameObject = app->importer->Importar(draggedFilePath.string());
						app->actualScene->AddGameObject(gameObject);
					}
					else {
						std::cerr << "Only .fbx files can be imported into the scene." << std::endl;
					}
				}
				ImGui::EndDragDropTarget();
			}

			ImGui::End();
		}











		//// 设置拖动源
		//if (ImGui::BeginDragDropSource()) {
		//	ImGui::SetDragDropPayload("DND_FILE", fileName.c_str(), fileName.size() + 1);
		//	ImGui::Text("Dragging %s", fileName.c_str());
		//	ImGui::EndDragDropSource();
		//}

		//// 设置拖动目标
		//if (ImGui::BeginDragDropTarget()) {
		//	if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DND_FILE")) {
		//		const char* payloadFileName = static_cast<const char*>(payload->Data);
		//		std::filesystem::path srcPath = currentPath / payloadFileName;
		//		std::filesystem::path destPath = entryPath / payloadFileName;
		//		std::filesystem::rename(srcPath, destPath);
		//	}
		//	ImGui::EndDragDropTarget();
		//}

		//// 设置拖动目标（例如，在你的场景渲染函数中）
		//if (ImGui::BeginDragDropTarget()) {
		//	if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DND_FILE")) {
		//		const char* payloadFileName = static_cast<const char*>(payload->Data);
		//		std::filesystem::path draggedFilePath = currentPath / payloadFileName;

		//		// 检查文件扩展名是否为 .fbx
		//		if (draggedFilePath.extension() == ".fbx") {
		//			GameObject* gameObject = app->importer->Importar(draggedFilePath.string());
		//			app->actualScene->AddGameObject(gameObject);
		//		}
		//		else {
		//			std::cerr << "Only .fbx files can be imported into the scene." << std::endl;
		//		}
		//	}
		//	ImGui::EndDragDropTarget();
		//}

		/*GameObject* gameObject = app->importer->Importar(draggedFilePath.string());
		app->actualScene->AddGameObject(gameObject);*/

		ImGui::PopStyleColor();

		// File name
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

	//// Detect right-click on empty space
	//if (ImGui::IsMouseClicked(1) && !ImGui::IsAnyItemHovered()) {
	//	selectedItem = "";
	//	ImGui::OpenPopup("ProjectContextMenuEmpty");
	//}

	// Deselect item on left-click outside any items
	if (ImGui::IsMouseClicked(0) && !ImGui::IsAnyItemHovered()) {
		selectedItem = "";
	}

	if (showDeleteConfirmation) {
		ImGui::OpenPopup("Confirm Delete");
		ShowDeleteConfirmation();
	}

}




void PanelProject::RenderContext() {
	if (ImGui::BeginPopup("ProjectContextMenuFile")) {
		if (ImGui::MenuItem("Delete File/Folder")) {
			if (!selectedItem.empty()) {
				showDeleteConfirmation = true;
				//selectedItem = "";
			}
		}

		if (ImGui::MenuItem("Copy")) {
			// Copy logic
		}

		if (ImGui::MenuItem("Paste")) {
			// Paste logic
		}

		ImGui::EndPopup();
	}
	else if ((ImGui::BeginPopupContextWindow("ProjectContextMenuEmpty"))) {
		if (ImGui::MenuItem("New Folder")) {
			CreateFolder(currentPath);
		}

		if (ImGui::MenuItem("Load GameObject")) {
			// Logic to load GameObject
		}

		ImGui::EndPopup();
	}

}
void PanelProject::ShowDeleteConfirmation() {

	//if (ImGui::BeginPopupModal("Confirm Delete", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
	//	ImGui::Text("Are you sure you want to delete '%s'?", itemToDelete.c_str());
	//	ImGui::Separator();
	//	bool enterPressed = ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Enter));
	//	if (ImGui::Button("OK", ImVec2(120, 0)) || enterPressed) {
	//		pathToDelete = deletePath / itemToDelete;
	//		pendingDelete = true;        // Activa el estado pendiente de eliminación
	//		showDeletePopup = false;     // Cierra el popup
	//		ImGui::CloseCurrentPopup();
	//	}
	//	ImGui::SameLine();
	//	if (ImGui::Button("Cancel", ImVec2(120, 0))) {
	//		showDeletePopup = false;  // Cierra el popup sin eliminar
	//		ImGui::CloseCurrentPopup();
	//	}
	//	ImGui::EndPopup();
	//}

	if (ImGui::BeginPopupModal("Confirm Delete", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
		ImGui::Text("Are you sure you want to delete '%s'?", selectedItem.c_str());
		ImGui::Separator();
		bool enterPressed = ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Enter));
		if (ImGui::Button("OK", ImVec2(120, 0)) || enterPressed) {
			std::filesystem::path deletePath = currentPath / selectedItem;
			DeleteFolder(deletePath);
			selectedItem = "";
			showDeleteConfirmation = false;
			ImGui::CloseCurrentPopup();
		}

		ImGui::SameLine();

		if (ImGui::Button("Cancel", ImVec2(120, 0))) {
			showDeleteConfirmation = false;
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}
}



void PanelProject::ShowBreadcrumbNavigation() {
	std::filesystem::path pathPart;
	bool showBreadcrumb = false;

	int i = 0;
	for (const auto& part : currentPath) {
		i++;
		if (part == "Assets") {
			showBreadcrumb = true;
		}

		if (showBreadcrumb) {
			pathPart /= part;

			std::string fileName = part.string();
			std::string buttonLabel = fileName + "##" + std::to_string(i);

			if (ImGui::Button(buttonLabel.c_str())) {

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

bool PanelProject::DeleteFolder(const std::filesystem::path& folderPath) {
	try {
		if (std::filesystem::exists(folderPath) && std::filesystem::is_directory(folderPath)) {
			std::filesystem::remove_all(folderPath);
			Debug::Log("Delete ");
			return true;
		}
		else if (std::filesystem::exists(folderPath)) {
			std::filesystem::remove(folderPath);
			Debug::Log("Delete ");

			return true;
		}
	}
	catch (const std::filesystem::filesystem_error& e) {
		Debug::Log("Error deleting folder: ", e.what());

	}
	return false;
}




