void MouseMove(int dx, int dy)
{
	INPUT input = { 1 };
	input.type = INPUT_MOUSE;
	input.mi.dx = dx;
	input.mi.dy = dy;
	input.mi.dwFlags = TOUCHEVENTF_MOVE;
	SendInput(1, &input, sizeof(INPUT));
}

ImVec2 CalculateAimOffset(const Vector2& target2D, const ImVec2& screenCenter, int aimSpeed, float screenWidth, float screenHeight) {
	float targetX = 0.0f;
	float targetY = 0.0f;

	if (target2D.x != 0) {
		targetX = (target2D.x > screenCenter.x) ? -(screenCenter.x - target2D.x) / aimSpeed : (target2D.x - screenCenter.x) / aimSpeed;
		if ((target2D.x > screenCenter.x && targetX + screenCenter.x > screenWidth) ||
			(target2D.x < screenCenter.x && targetX + screenCenter.x < 0)) {
			targetX = 0.0f;
		}
	}

	if (target2D.y != 0) {
		targetY = (target2D.y > screenCenter.y) ? -(screenCenter.y - target2D.y) / aimSpeed : (target2D.y - screenCenter.y) / aimSpeed;
		if ((target2D.y > screenCenter.y && targetY + screenCenter.y > screenHeight) ||
			(target2D.y < screenCenter.y && targetY + screenCenter.y < 0)) {
			targetY = 0.0f;
		}
	}

	return ImVec2(targetX, targetY);
}

void aimbotPlayer(uintptr_t pawn_private)
{
	uintptr_t target_mesh = read<uintptr_t>(pawn_private + OFFSETS::Mesh);
	if (!target_mesh)
		return;
	if (!isVisible(target_mesh))
		return;

	Vector3 head3d;
	Vector2 head2d;
	head3d = GetBoneWithRotation(target_mesh, 66); //Chest

	ImGuiIO& io = ImGui::GetIO();
	ImVec2 screenCenter(io.DisplaySize.x / 2, io.DisplaySize.y / 2);
	head2d = ProjectWorldToScreen(head3d);

	ImVec2 target = CalculateAimOffset(head2d, screenCenter, aimbot::smoothing, io.DisplaySize.x, io.DisplaySize.y);
	MouseMove(target.x, target.y);
}