inline int tab = 0;
void DrawMenu()
{
	ImGui::SetNextWindowSize({ 575, 350 });
	ImGui::SetNextWindowPos(ImVec2(100, 100), ImGuiCond_FirstUseEver);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(5.0f, 5.0f));
	ImGui::Begin("Base", &menu::ShowMenu, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar);

	if (ImGui::Button("Aimbot", { 196, 20 })) tab = 0;
	ImGui::SameLine();
	if (ImGui::Button("Visuals", { 196, 20 })) tab = 1;
	switch (tab)
	{
	case 0:
	{
		ImGui::Checkbox("Enable", &aimbot::enabled);
		ImGui::Checkbox("Draw-FOV", &aimbot::drawfov);
		HotkeyButton(hotkeys::aimkey, ChangeKey, keystatus); ImGui::SameLine(); ImGui::Text("Aimkey");
		ImGui::Combo("Aimbone", &aimbot::current_aimbone, aimbot::aimbone, sizeof(aimbot::aimbone) / sizeof(*aimbot::aimbone));
		ImGui::SliderInt("Aim-FOV", &aimbot::fovsize, 1, 400);
		ImGui::SliderFloat("Smoothness", &aimbot::smoothing, 1, 10);

		break;
	}
	case 1:
	{
		ImGui::Checkbox("Corner Boxes", &esp::corneredboxes);
		ImGui::Checkbox("Box", &esp::boxes);
		ImGui::Checkbox("Skeleton", &esp::skeleton);
		ImGui::Checkbox("Lines", &esp::snaplines);
		ImGui::SliderFloat("ThickNess", &esp::thickness, 1, 10);

		break;
	}
	}

	ImGui::PopStyleVar();
	ImGui::End();
}