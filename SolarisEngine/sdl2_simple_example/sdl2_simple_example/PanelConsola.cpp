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

    if (!app->windowEditor->_windowImGui->showconsolaPanel) {
        return;
    }

    ImGui::Begin("Console"); // Begin creating the panel window

    // Example scrollable area
    //static ImGuiTextBuffer logBuffer; // Text buffer for storing log messages
    static ImGuiTextFilter filter;    // Filter for searching within log content

    // Add search filter
    ImGui::Text("Console Output:");
    filter.Draw("Filter", 180); // Create filter input box

    // Begin scrollable region
    ImGui::BeginChild("ScrollRegion", ImVec2(0, 400), true, ImGuiWindowFlags_HorizontalScrollbar);
    if (filter.IsActive()) {
        // Display filtered log messages
        const char* buf = logBuffer.begin();
        const char* line = buf;
        for (int line_no = 0; line != nullptr; line_no++) {
            const char* line_end = strchr(line, '\n');
            if (filter.PassFilter(line, line_end)) {
                ImGui::TextUnformatted(line, line_end); // Display log messages matching filter criteria
            }
            line = line_end && line_end[1] ? line_end + 1 : nullptr;
        }
    }
    else {
        // Display all log messages
        ImGui::TextUnformatted(logBuffer.begin());
    }
    ImGui::EndChild();

    // Example: Add a test log button
    if (ImGui::Button("Add Test Log")) {
        logBuffer.append("This is a test log message.\n");
    }

    ImGui::End(); // End panel window
}

void PanelConsola::AddLog(const std::string& message) {
    // Append log message to the buffer
    logBuffer.appendf("%s\n", message.c_str());
}
