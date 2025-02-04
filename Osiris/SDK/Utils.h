#pragma once

#include <numbers>
#include <tuple>

[[deprecated]] constexpr auto degreesToRadians = [](float degrees) constexpr noexcept { return degrees * std::numbers::pi_v<float> / 180.0f; };
[[deprecated]] constexpr auto radiansToDegrees = [](float radians) constexpr noexcept { return radians * 180.0f / std::numbers::pi_v<float>; };

std::tuple<float, float, float> rainbowColor(float speed) noexcept;
