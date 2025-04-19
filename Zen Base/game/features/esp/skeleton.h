void DrawSkeleton(DWORD_PTR mesh) {
	Vector2 bonePositions[16];
	const int boneIndices[] = { 110, 3, 66, 9, 38, 10, 39, 11, 40, 78, 71, 79, 72, 75, 82, 67 };
	for (int i = 0; i < 16; ++i)
	{
		bonePositions[i] = ProjectWorldToScreen(GetBoneWithRotation(mesh, boneIndices[i]));
	}

	std::pair<int, int> bonePairs[] =
	{
		{1, 2}, {3, 4}, {4, 3}, {5, 3}, {6, 4}, {5, 7}, {6, 8},
		{10, 1}, {9, 1}, {12, 10}, {11, 9}, {13, 12}, {14, 11}, {2, 15}, { 15, 0 }
	};

	ImGui::GetBackgroundDrawList()->PushClipRectFullScreen();
	for (const auto& pair : bonePairs)
	{
		ImVec2 start(bonePositions[pair.first].x, bonePositions[pair.first].y);
		ImVec2 end(bonePositions[pair.second].x, bonePositions[pair.second].y);

		if (pair.first == 110)
		{
			end.y += 20;
		}

		if (!isVisible(mesh))
		{
			ImGui::GetBackgroundDrawList()->AddLine(start, end, ImColor(255, 0, 0), esp::thickness);
		}
		else
		{
			ImGui::GetBackgroundDrawList()->AddLine(start, end, ImColor(0, 255, 0), esp::thickness);
		}
	}
	ImGui::GetBackgroundDrawList()->PopClipRect();
}