#include "PanelProject.h"
#include "imgui.h"
#include "Defs.h"
#include "App.h"
#include "stb_image.h"
#include "TextureLoader.h"
#include "Debug.h"
#include "Input.h"
#include "imgui_internal.h"


namespace fs = std::filesystem;
// Constructor
PanelProject::PanelProject() {
	projectPath = ASSETS_DIR;
	icons["unknow"] = (int*)app->textureLoader->LoadTextureDevIL("./Packages/Icons/desconocido.png");
	icons["folder"] = (int*)app->textureLoader->LoadTextureDevIL("./Packages/Icons/folder.png");
	icons["file"] = (int*)app->textureLoader->LoadTextureDevIL("./Packages/Icons/file.png");
	icons["text"] = (int*)app->textureLoader->LoadTextureDevIL("./Packages/Icons/txt.png");
	icons["png"] = (int*)app->textureLoader->LoadTextureDevIL("./Packages/Icons/png.png");
	icons["jpg"] = (int*)app->textureLoader->LoadTextureDevIL("./Packages/Icons/jpg.png");
	icons["fbx"] = (int*)app->textureLoader->LoadTextureDevIL("./Packages/Icons/fbx.png");
	icons["image"] = (int*)app->textureLoader->LoadTextureDevIL("./Packages/Icons/image.png");
	icons["material"] = (int*)app->textureLoader->LoadTextureDevIL("./Packages/Icons/material.png");
	icons["gameObject"] = (int*)app->textureLoader->LoadTextureDevIL("./Packages/Icons/gameObject.png");
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

	projectExplorerPos = ImGui::GetWindowPos();
	projectExplorerSize = ImGui::GetWindowSize();
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

	// 添加静态变量以跟踪重命名状态
	static bool isRenaming = false;
	static std::string renameBuffer;
	static std::string renameTarget;

	for (const auto& entry : fs::directory_iterator(path)) {
		i++;

		const auto& entryPath = entry.path();
		std::string fileName = entryPath.filename().string();
		// 跳过 .meta 文件 
		if (entryPath.extension() == ".meta")
		{
			continue;
		}
		bool isDirectory = entry.is_directory();
		bool isHovered = false;
		bool isSelected = (fileName == selectedItem);

		ImGui::BeginGroup();

		ShowIcon(isDirectory, entryPath);

		// 透明背景
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
					// 双击打开文件或文件夹
					if (isDirectory) {
						selectedItem = fileName;
						pathStack.push(currentPath);
						currentPath = entryPath;
					}
					else {
						selectedItem = fileName;
						// 打开文件的逻辑
					}
				}
				else {
					// 单击选择项目并改变颜色
					selectedItem = fileName;
					lastClickedItem = fileName;
					lastClickTime = currentTime;
				}
			}

			// 确保只触发一个弹出菜单
			if (ImGui::IsMouseClicked(1)) {
				if (ImGui::IsAnyItemHovered()) {
					// 如果悬停在项目上，设置 selectedItem 并打开相应的菜单
					Debug::Log("ProjectContextMenuFile");
					selectedItem = fileName;
					ImGui::OpenPopup("ProjectContextMenuFile");
				}
				else {
					// 如果未悬停在任何项目上，清除选择并打开空白区域菜单
					Debug::Log("ProjectContextMenuEmpty");
					selectedItem = "";
					ImGui::OpenPopup("ProjectContextMenu");
				}
			}

			// 按下 F2 键开始重命名
			if (ImGui::IsKeyPressed(ImGuiKey_F2)) {
				isRenaming = true;
				renameBuffer = fileName;
				renameTarget = fileName;
			}
		}

		// 处理重命名
		if (isRenaming && isSelected && fileName == renameTarget) {
			char buffer[256];
			strncpy_s(buffer, sizeof(buffer), renameBuffer.c_str(), _TRUNCATE); 

			// 设置输入框的最大宽度
			ImGui::SetNextItemWidth(iconSize + padding + textMaxWidth - 75);
			if (ImGui::InputText("##Rename", buffer, sizeof(buffer), ImGuiInputTextFlags_EnterReturnsTrue)) {
				std::filesystem::path newFilePath = entryPath.parent_path() / buffer;
				std::filesystem::rename(entryPath, newFilePath);
				fileName = buffer;
				isRenaming = false;
				renameBuffer.clear();
				renameTarget.clear();
			}
			else {
				renameBuffer = buffer;
			}
		}

		// 设置拖动源
		if (ImGui::BeginDragDropSource()) {
			ImGui::SetDragDropPayload("DND_FILE", fileName.c_str(), fileName.size() + 1);
			ImGui::Text("Dragging %s", fileName.c_str());
			ImGui::EndDragDropSource();
		}

		DropTarget(entryPath);
		ShowFileName(textMaxWidth, fileName, maxItemsPerRow, isRenaming, renameTarget);

		itemsInRow++;
		if (itemsInRow >= maxItemsPerRow) {
			ImGui::NewLine();
			itemsInRow = 0;
		}
		else {
			ImGui::SameLine(0, padding);
		}

	}

	// 在任何项目之外左击时取消选择项目
	if (ImGui::IsMouseClicked(0) && !ImGui::IsAnyItemHovered()) {
		selectedItem = "";
		isRenaming = false; // 取消重命名状态
		renameBuffer.clear();
		renameTarget.clear();
	}

	if (showDeleteConfirmation) {
		ImGui::OpenPopup("Confirm Delete");
		ShowDeleteConfirmation();
	}
}


void PanelProject::ShowIcon(bool isDirectory, std::filesystem::path entryPath)
{

	// Determine icon

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

			if (ImGui::BeginDragDropTarget()) {
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DND_FILE")) {
					// Extraer el nombre del archivo del origen del arrastre
					const char* fileName = static_cast<const char*>(payload->Data);

					// Construir la ruta de origen (suponiendo que `currentPath` es el directorio actual mostrado)
					std::filesystem::path sourcePath = currentPath / fileName;

					// Construir la ruta de destino (la ruta de la miga de pan)
					std::filesystem::path destinationPath = pathPart / fileName;

					// Comprobar si la ruta de origen y la ruta de destino son iguales
					if (sourcePath == destinationPath) {
						Debug::Log("No se puede mover el archivo/carpeta al mismo directorio.");
						return;  // Si la ruta de origen y la ruta de destino son iguales, omitir la operación
					}

					try {
						// Comprobar si la carpeta/archivo de origen existe
						if (!std::filesystem::exists(sourcePath)) {
							Debug::Log("El archivo/carpeta de origen no existe: ", sourcePath.string());
							return;
						}

						// Si la ruta de destino ya existe
						if (std::filesystem::exists(destinationPath)) {
							// Si la ruta de destino es el directorio actual, omitir el movimiento
							if (std::filesystem::is_directory(destinationPath) && destinationPath == currentPath) {
								Debug::Log("El destino es el directorio actual, omitiendo el movimiento.");
								return;  // Si el destino es el directorio actual, no hacer nada
							}

							// Si la ruta de destino es otra carpeta y ya existe un archivo o carpeta con el mismo nombre
							if (std::filesystem::is_directory(destinationPath)) {
								// Agregar sufijo numérico
								std::filesystem::path uniqueDestinationPath = destinationPath;
								int counter = 1;
								while (std::filesystem::exists(uniqueDestinationPath)) {
									// Obtener el nombre y la extensión del archivo
									std::string stem = uniqueDestinationPath.stem().string();  // Nombre del archivo sin la extensión
									std::string extension = uniqueDestinationPath.extension().string();  // Extensión del archivo

									// Agregar sufijo numérico al nombre del archivo
									uniqueDestinationPath = pathPart / (stem + " (" + std::to_string(counter) + ")" + extension);
									counter++;  // Incrementar el número del sufijo
								}
								destinationPath = uniqueDestinationPath;
							}
						}

						// Ejecutar la operación de movimiento
						std::filesystem::rename(sourcePath, destinationPath);
						Debug::Log("Movido a: ", destinationPath.string());
					}
					catch (const std::exception& e) {
						Debug::Log("Error al mover el archivo/carpeta: ", e.what());
					}
				}
				ImGui::EndDragDropTarget();
			}






			ImGui::SameLine();
			ImGui::Text(">");
			ImGui::SameLine();
		}
	}

}

void PanelProject::DropTarget(std::filesystem::path entryPath)
{

	ImGui::Begin("Scene");

	ImVec2 windowPos = ImGui::GetWindowPos();
	ImVec2 availableRegion = ImGui::GetContentRegionAvail();

	// 手动计算矩形区域
	ImVec2 rectMin = windowPos;
	ImVec2 rectMax = ImVec2(windowPos.x + availableRegion.x + 20, windowPos.y + availableRegion.y + 50);

	// 定义拖放区域
	ImRect dropRect(rectMin, rectMax);

	// 使用自定义拖放目标
	if (ImGui::BeginDragDropTargetCustom(dropRect, ImGui::GetID("SceneDragDropTarget"))) {
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DND_FILE")) {
			const char* payloadFileName = static_cast<const char*>(payload->Data);
			std::filesystem::path draggedFilePath = currentPath / payloadFileName;

			printf("File '%s' dropped into Scene Panel\n", payloadFileName);
			app->inputEditor->handleDroppedFile(draggedFilePath.string().c_str());
			/*if (draggedFilePath.extension() == ".fbx") {
				GameObject* gameObject = app->importer->Importar(draggedFilePath.string());
				app->actualScene->AddGameObject(gameObject);
			}
			else {
				std::cerr << "Only .fbx files can be imported into the scene." << std::endl;
			}*/
		}
		ImGui::EndDragDropTarget();
	}

	ImGui::End();




	// 设置拖动目标
	if (ImGui::BeginDragDropTarget()) {
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DND_FILE")) {
			const char* payloadFileName = static_cast<const char*>(payload->Data);
			std::filesystem::path srcPath = currentPath / payloadFileName;
			std::filesystem::path destPath = entryPath / payloadFileName;
			std::filesystem::rename(srcPath, destPath);
		}
		ImGui::EndDragDropTarget();
	}


	ImGui::PopStyleColor();
}

void PanelProject::ShowFileName(float textMaxWidth, std::string fileName, int maxItemsPerRow, bool isRenaming, std::string renameTarget)
{
	//// File name
	//ImGui::PushTextWrapPos(ImGui::GetCursorPosX() + textMaxWidth);
	//float textWidth = ImGui::CalcTextSize(fileName.c_str()).x;
	//if (textWidth > textMaxWidth) {
	//	fileName = fileName.substr(0, 10) + "...";
	//}

	//float centeredTextPos = ImGui::GetCursorPosX() + (iconSize - std::min(textWidth, textMaxWidth)) / 2;
	//ImGui::SetCursorPosX(centeredTextPos);
	//ImGui::Text("%s", fileName.c_str());
	//ImGui::PopTextWrapPos();
	//ImGui::EndGroup();

	//itemsInRow++;
	//if (itemsInRow >= maxItemsPerRow) {
	//	ImGui::NewLine();
	//	itemsInRow = 0;
	//}
	//else {
	//	ImGui::SameLine(0, padding);
	//}


			// 文件名
	ImGui::PushTextWrapPos(ImGui::GetCursorPosX() + textMaxWidth);
	float textWidth = ImGui::CalcTextSize(fileName.c_str()).x;

	if (!(isRenaming && fileName == renameTarget)) {
		if (textWidth > textMaxWidth) {
			fileName = fileName.substr(0, 10) + "...";
		}

		float centeredTextPos = ImGui::GetCursorPosX() + (iconSize - std::min(textWidth, textMaxWidth)) / 2;
		ImGui::SetCursorPosX(centeredTextPos);
		ImGui::Text("%s", fileName.c_str());
	}

	ImGui::PopTextWrapPos();
	ImGui::EndGroup();




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

void PanelProject::CopyToAssetsFolder(const std::string& filePath, const std::string& destinationFolder) {
	// Use the filesystem library to copy files or directories
	try {
		std::filesystem::path src(filePath);
		std::filesystem::path dest = std::filesystem::path(destinationFolder) / src.filename();

		// Check if the destination file already exists
		if (std::filesystem::exists(dest)) {
			// Generate a new file name with a number appended
			int copyIndex = 1;
			std::filesystem::path newDest;
			do {
				newDest = dest.parent_path() / (dest.stem().string() + " (" + std::to_string(copyIndex) + ")" + dest.extension().string());
				copyIndex++;
			} while (std::filesystem::exists(newDest));
			dest = newDest;
		}

		// Copy the file or directory
		std::filesystem::copy(src, dest, std::filesystem::copy_options::recursive | std::filesystem::copy_options::overwrite_existing);
		Debug::Log("File copied to destination folder: ", dest.string());
	}
	catch (const std::filesystem::filesystem_error& e) {
		Debug::Log("Error copying file to destination folder: ", e.what());
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

std::filesystem::path PanelProject::getCurrentPath()
{
	return currentPath;
}




