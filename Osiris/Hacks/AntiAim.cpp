#include "AntiAim.h"
#include "../Interfaces.h"
#include "../SDK/Engine.h"
#include "../SDK/Entity.h"
#include "../SDK/EntityList.h"
#include "../SDK/NetworkChannel.h"
#include "../SDK/UserCmd.h"

#if OSIRIS_ANTIAIM()

struct AntiAimConfig {
    bool enabled = false;
    bool pitch = false;
    bool yaw = false;
    float pitchAngle = 0.0f;
    float yawAngle = 0.0f;
    int yawMode{ 0 };
} antiAimConfig;

void AntiAim::run(UserCmd* cmd, const Vector& previousViewAngles, const Vector& currentViewAngles, bool& sendPacket) noexcept
{
    if (antiAimConfig.enabled) {
        if (!localPlayer)
            return;

        if (antiAimConfig.pitch && cmd->viewangles.x == currentViewAngles.x)
            cmd->viewangles.x = antiAimConfig.pitchAngle;

        if (antiAimConfig.yaw && cmd->viewangles.y == currentViewAngles.y) {
            if (antiAimConfig.yawMode == 0) {
                cmd->viewangles.y = antiAimConfig.yawAngle;
            }
            else if (antiAimConfig.yawMode == 1) {
                cmd->viewangles.y -= antiAimConfig.yawAngle;
            }
            else if (antiAimConfig.yawMode == 2) {
                cmd->viewangles.y += antiAimConfig.yawAngle;
            }
        }
    }
}

static bool antiAimOpen = false;

void AntiAim::menuBarItem() noexcept
{
    if (ImGui::MenuItem("Anti aim")) {
        antiAimOpen = true;
        ImGui::SetWindowFocus("Anti aim");
        ImGui::SetWindowPos("Anti aim", { 100.0f, 100.0f });
    }
}

void AntiAim::tabItem() noexcept
{
    if (ImGui::BeginTabItem("Anti aim")) {
        drawGUI(true);
        ImGui::EndTabItem();
    }
}

void AntiAim::drawGUI(bool contentOnly) noexcept
{
    if (!contentOnly) {
        if (!antiAimOpen)
            return;
        ImGui::SetNextWindowSize({ 0.0f, 0.0f });
        ImGui::Begin("Anti aim", &antiAimOpen, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
    }
    ImGui::Checkbox("Enabled", &antiAimConfig.enabled);
    ImGui::Checkbox("##pitch", &antiAimConfig.pitch);
    ImGui::SameLine();
    ImGui::SliderFloat("Pitch", &antiAimConfig.pitchAngle, -89.0f, 89.0f, "%.2f");
    ImGui::Combo("Yaw mode", &antiAimConfig.yawMode, "Central\0Minus\0Plus\0");
    ImGui::Checkbox("##yaw", &antiAimConfig.yaw);
    ImGui::SameLine();
    ImGui::SliderFloat("Yaw", &antiAimConfig.yawAngle, -180.0f, 180.0f, "%.2f");
    if (!contentOnly)
        ImGui::End();
}

static void to_json(json& j, const AntiAimConfig& o, const AntiAimConfig& dummy = {})
{
    WRITE("Enabled", enabled);
    WRITE("Pitch", pitch);
    WRITE("Pitch angle", pitchAngle);
    WRITE("Yaw", yaw);
    WRITE("Yaw angle", yawAngle);
    WRITE("Yaw mode", yawMode);
}

json AntiAim::toJson() noexcept
{
    json j;
    to_json(j, antiAimConfig);
    return j;
}

static void from_json(const json& j, AntiAimConfig& a)
{
    read(j, "Enabled", a.enabled);
    read(j, "Pitch", a.pitch);
    read(j, "Yaw", a.yaw);
    read(j, "Pitch angle", a.pitchAngle);
    read(j, "Yaw angle", a.yawAngle);
    read(j, "Yaw mode", a.yawMode);
}

void AntiAim::fromJson(const json& j) noexcept
{
    from_json(j, antiAimConfig);
}

void AntiAim::resetConfig() noexcept
{
    antiAimConfig = { };
}

#else

namespace AntiAim
{
    void run(UserCmd*, const Vector&, const Vector&, bool&) noexcept {}

    // GUI
    void menuBarItem() noexcept {}
    void tabItem() noexcept {}
    void drawGUI(bool contentOnly) noexcept {}

    // Config
    json toJson() noexcept { return {}; }
    void fromJson(const json& j) noexcept {}
    void resetConfig() noexcept {}
}

#endif
