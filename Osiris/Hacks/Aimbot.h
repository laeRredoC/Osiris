#pragma once

struct UserCmd;
struct Vector;

namespace Aimbot
{
    Vector calculateRelativeAngle(const Vector& source, const Vector& destination, const Vector& viewAngles) noexcept;
    void run(UserCmd*) noexcept;
    void autoStop(UserCmd* cmd) noexcept;
    void init() noexcept;

    void updateInput() noexcept;
}
