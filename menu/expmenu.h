std::stack<std::string> back_stack;
std::stack<std::string> forward_stack;
std::string current_directory = "C:\\";

std::vector<std::string> get_available_drives() {
    std::vector<std::string> drives;
    char buffer[256];
    DWORD size = GetLogicalDriveStringsA(sizeof(buffer), buffer);
    if (size > 0 && size <= sizeof(buffer)) {
        char* drive = buffer;
        while (*drive) {
            drives.push_back(drive);
            drive += strlen(drive) + 1;
        }
    }
    return drives;
}


std::vector<std::string> get_files_in_directory(const std::string& path) {
    std::vector<std::string> files;
    for (const auto& entry : fs::directory_iterator(path)) {
        files.push_back(entry.path().string());
    }
    return files;
}

void ChangeDirectory(const std::string& new_directory) {
    if (new_directory != current_directory) {
        back_stack.push(current_directory);
        current_directory = new_directory;
        while (!forward_stack.empty()) {
            forward_stack.pop();
        }
    }
}

void GoBack() {
    if (!back_stack.empty()) {
        forward_stack.push(current_directory);
        current_directory = back_stack.top();
        back_stack.pop();
    }
}

void GoForward() {
    if (!forward_stack.empty()) {
        back_stack.push(current_directory);
        current_directory = forward_stack.top();
        forward_stack.pop();
    }
}

void GradientProgressBar(float fraction, const ImVec2& size_arg, const ImVec4& col0, const ImVec4& col1) {
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems)
        return;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    ImVec2 pos = window->DC.CursorPos;
    ImVec2 size = ImGui::CalcItemSize(size_arg, ImGui::CalcItemWidth(), g.FontSize + style.FramePadding.y * 2.0f);

    const ImRect bb(pos, ImVec2(pos.x + size.x, pos.y + size.y));
    ImGui::ItemSize(size, style.FramePadding.y);
    if (!ImGui::ItemAdd(bb, 0))
        return;

    fraction = ImSaturate(fraction);
    window->DrawList->AddRectFilledMultiColor(bb.Min, ImVec2(bb.Min.x + size.x * fraction, bb.Max.y), ImGui::GetColorU32(col0), ImGui::GetColorU32(col1), ImGui::GetColorU32(col1), ImGui::GetColorU32(col0));
    window->DrawList->AddRect(bb.Min, bb.Max, ImGui::GetColorU32(ImGuiCol_Border));
}

void Decoration() {
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.1f, 0.1f, 0.1f, 1.0f)); // Darker background
    ImGui::BeginChild("LeftPanelDecoration", ImVec2(100, 0), true);
    ImGui::EndChild();
    ImGui::PopStyleColor();

    ImGui::SameLine();
    ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.2f, 0.2f, 0.2f, 1.0f)); // Gray separator
    ImGui::BeginChild("Separator", ImVec2(2, 0), false, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
    ImGui::EndChild();
    ImGui::PopStyleColor();
    ImGui::SameLine();

    ImGui::BeginChild("MainAreaDecoration", ImVec2(0, 0), true);
    ImGui::EndChild();
}

void ExplorerMenu() {
    std::vector<std::string> files = get_files_in_directory(current_directory);

    ImGui::BeginChild("LeftPanel", ImVec2(100, 0), true);

    for (const auto& drive : files) {
        if (ImGui::Button(drive.c_str(), ImVec2(69, 15))) {
            ChangeDirectory(drive);
        }
    }

    ImGui::Spacing();
    ImGui::Separator();

    ImGui::Button("Disk Name", ImVec2(69, 9));
    float progress1 = 0.4f; // example value
    ImVec4 green = ImVec4(0.0f, 1.0f, 0.0f, 1.0f);
    ImVec4 red = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
    GradientProgressBar(progress1, ImVec2(69, 3), green, red);

    ImGui::Button("Disk Name", ImVec2(69, 9));
    float progress2 = 0.6f; // example value
    GradientProgressBar(progress2, ImVec2(69, 3), green, red);

    ImGui::Button("Disk Name", ImVec2(69, 9));
    float progress3 = 0.3f; // example value
    GradientProgressBar(progress3, ImVec2(69, 3), green, red);

    ImGui::EndChild();

    ImGui::SameLine();

    ImGui::BeginChild("RightPanel", ImVec2(0, 0), true);

    if (ImGui::Button("Back") || ImGui::IsMouseClicked(3)) { // Mouse 4 (Back) button
        GoBack();
    }
    ImGui::SameLine();
    if (ImGui::Button("Forward") || ImGui::IsMouseClicked(4)) { // Mouse 5 (Forward) button
        GoForward();
    }
    ImGui::SameLine();
    if (ImGui::Button("Up")) {
        fs::path parent_path = fs::path(current_directory).parent_path();
        if (!parent_path.empty()) {
            ChangeDirectory(parent_path.string());
        }
    }

    ImGui::Separator();

    for (const auto& file : files) {
        if (fs::is_directory(file)) {
            if (ImGui::Button(file.c_str())) {
                ChangeDirectory(file);
            }
        }
        else {
            ImGui::Text("%s", file.c_str());
        }
    }

    ImGui::EndChild();
}