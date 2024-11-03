#include "PanelScene.h"
#include "imgui.h"
#include "App.h"

PanelScene::PanelScene()
{
}

PanelScene::~PanelScene()
{
}

void PanelScene::Render()
{
	ImGui::Begin("Scene");
	{
		ImGui::BeginChild("GameRender");

		float width = ImGui::GetContentRegionAvail().x;
		float height = ImGui::GetContentRegionAvail().y;

		//*m_width = width;
		//*m_height = height;
		//ImGui::Image(
		//	(ImTextureID) app->windowEditor->GetFrameBuffer()->getFrameTexture(),
		//	ImGui::GetContentRegionAvail(),
		//	ImVec2(0, 1),
		//	ImVec2(1, 0)
		//);
	}
	ImGui::EndChild();
	ImGui::End();
}

void PanelScene::RenderContext()
{
}
