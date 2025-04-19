Vector3 GetBoneWithRotation(uintptr_t mesh, int bone_id) {
    uintptr_t bone_array = read<uintptr_t>(mesh + OFFSETS::BoneArray);
    if (bone_array == 0) bone_array = read<uintptr_t>(mesh + OFFSETS::BoneArray + 0x10);
    FTransform bone = read<FTransform>(bone_array + (bone_id * 0x60));
    FTransform component_to_world = read<FTransform>(mesh + OFFSETS::ComponetToWorld);
    D3DMATRIX matrix = MatrixMultiplication(bone.ToMatrixWithScale(), component_to_world.ToMatrixWithScale());
    return Vector3(matrix._41, matrix._42, matrix._43);
}

bool isVisible(uint64_t Mesh)
{
    auto Seconds = read<double>(Actors::Uworld + 0x160);
    auto LastRenderTime = read<float>(Mesh + 0x32C);
    return Seconds - LastRenderTime <= 0.06f;
}