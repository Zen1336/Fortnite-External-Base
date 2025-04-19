D3DMATRIX to_matrix(Vector3 rot, Vector3 origin = Vector3(0, 0, 0))
{
    float radpitch = (rot.x * M_PI / 180);
    float radyaw = (rot.y * M_PI / 180);
    float radroll = (rot.z * M_PI / 180);
    float sp = sinf(radpitch);
    float cp = cosf(radpitch);
    float sy = sinf(radyaw);
    float cy = cosf(radyaw);
    float sr = sinf(radroll);
    float cr = cosf(radroll);
    D3DMATRIX matrix{};
    matrix.m[0][0] = cp * cy;
    matrix.m[0][1] = cp * sy;
    matrix.m[0][2] = sp;
    matrix.m[0][3] = 0.f;
    matrix.m[1][0] = sr * sp * cy - cr * sy;
    matrix.m[1][1] = sr * sp * sy + cr * cy;
    matrix.m[1][2] = -sr * cp;
    matrix.m[1][3] = 0.f;
    matrix.m[2][0] = -(cr * sp * cy + sr * sy);
    matrix.m[2][1] = cy * sr - cr * sp * sy;
    matrix.m[2][2] = cr * cp;
    matrix.m[2][3] = 0.f;
    matrix.m[3][0] = origin.x;
    matrix.m[3][1] = origin.y;
    matrix.m[3][2] = origin.z;
    matrix.m[3][3] = 1.f;
    return matrix;
}

struct Camera
{
    Vector3 location;
    Vector3 rotation;
    float fov;
};

Camera get_view_point()
{
    auto location_pointer = read<uintptr_t>(Actors::Uworld + 0x138);
    auto rotation_pointer = read<uintptr_t>(Actors::Uworld + 0x148);

    struct RotationInfo
    {
        double pitch;
        char pad_0008[24];
        double yaw;
        char pad_0028[424];
        double roll;
    } rotationInfo;

    rotationInfo.pitch = read<double>(rotation_pointer);
    rotationInfo.yaw = read<double>(rotation_pointer + 0x20);
    rotationInfo.roll = read<double>(rotation_pointer + 0x1D0);

    Camera camera;
    camera.location = read<Vector3>(location_pointer);
    camera.rotation.x = asin(rotationInfo.roll) * (180.0 / M_PI);
    camera.rotation.y = ((atan2(rotationInfo.pitch * -1, rotationInfo.yaw) * (180.0 / M_PI)) * -1) * -1;
    camera.fov = read<float>((uintptr_t)Actors::PlayerController + 0x3AC) * 90.f;
    return camera;
}

Vector2 ProjectWorldToScreen(Vector3 worldLocation)
{
    Camera cam = get_view_point();
    D3DMATRIX temp_matrix = to_matrix(cam.rotation);
    Vector3 vaxisx = Vector3(temp_matrix.m[0][0], temp_matrix.m[0][1], temp_matrix.m[0][2]);
    Vector3 vaxisy = Vector3(temp_matrix.m[1][0], temp_matrix.m[1][1], temp_matrix.m[1][2]);
    Vector3 vaxisz = Vector3(temp_matrix.m[2][0], temp_matrix.m[2][1], temp_matrix.m[2][2]);
    Vector3 vdelta = worldLocation - cam.location;
    Vector3 vtransformed = Vector3(vdelta.dot(vaxisy), vdelta.dot(vaxisz), vdelta.dot(vaxisx));
    if (vtransformed.z < 1) vtransformed.z = 1;
    return Vector2(settings::screen_center_x + vtransformed.x * ((settings::screen_center_x / tanf(cam.fov * M_PI / 360))) / vtransformed.z, settings::screen_center_y - vtransformed.y * ((settings::screen_center_x / tanf(cam.fov * M_PI / 360))) / vtransformed.z);
}