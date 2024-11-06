#include "PanelConsola.h"
#include "imgui.h"
#include "App.h"

// Constructor and Destructor
PanelConsola::PanelConsola() {
    // Initialization if necessary
}

PanelConsola::~PanelConsola() {
    // Cleanup if necessary
}

void PanelConsola::Render() {
    if (!app->windowEditor->GetImGuiWindow()->showconsolaPanel) {
        return;
    }

    ImGui::Begin("Console"); // Start drawing the panel

    // Filter: for searching log content
    static ImGuiTextFilter filter;

    // Draw the search box
    ImGui::Text("Console Output:");
    filter.Draw("Filter", 180); // Create a filter box

    // Get the current available width and height of the panel each frame
    ImVec2 panelSize = ImGui::GetContentRegionAvail();

    // Manually set the available area width to ensure text wrapping
    ImGui::SetNextItemWidth(panelSize.x);

    // Create a scrollable region with dynamic width and height
    // Here, set the height of the child window to the panel's available height minus 50 (leaving space for the search box and other controls)
    ImGui::BeginChild("ScrollRegion", ImVec2(panelSize.x, panelSize.y - 50), true);

    // Check if the filter is active
    if (filter.IsActive()) {
        // Show filtered log information
        const char* buf = logBuffer.begin();
        const char* line = buf;
        for (int line_no = 0; line != nullptr; line_no++) {
            const char* line_end = strchr(line, '\n');
            if (filter.PassFilter(line, line_end)) {
                // Use TextWrapped to ensure text wrapping
                ImGui::TextWrapped("%.*s", (line_end ? line_end - line : (int)strlen(line)), line);
            }
            line = line_end && line_end[1] ? line_end + 1 : nullptr;
        }
    }
    else {
        // Show all log information
        ImGui::TextWrapped("%s", logBuffer.begin());
    }

    ImGui::EndChild(); // End the scrollable region

    ImGui::End(); // End the panel window
}









void PanelConsola::RenderContext()
{
}

void PanelConsola::AddLog(const std::string& message) {
    // Append log message to the buffer
    logBuffer.appendf("%s\n", message.c_str());
}
