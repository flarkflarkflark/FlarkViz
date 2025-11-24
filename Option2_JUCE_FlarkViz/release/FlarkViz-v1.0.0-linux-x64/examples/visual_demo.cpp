/**
 * Visual ASCII Demo - Shows presets animating in the terminal
 */

#include "../Source/Rendering/RenderState.h"
#include "../Source/Presets/Preset.h"
#include <iostream>
#include <iomanip>
#include <cmath>
#include <thread>
#include <chrono>

void clearScreen()
{
    std::cout << "\033[2J\033[1;1H";
}

void printBar(const std::string& label, float value, int width = 40)
{
    std::cout << std::setw(12) << label << " [";
    int filled = static_cast<int>(value * width);
    for (int i = 0; i < width; ++i)
    {
        if (i < filled)
            std::cout << "█";
        else
            std::cout << "░";
    }
    std::cout << "] " << std::fixed << std::setprecision(3) << value << "\n";
}

void printColorBar(const std::string& label, float r, float g, float b)
{
    std::cout << std::setw(12) << label << " ";
    std::cout << "R[" << std::string(int(r * 10), '█') << std::string(10 - int(r * 10), '░') << "] ";
    std::cout << "G[" << std::string(int(g * 10), '█') << std::string(10 - int(g * 10), '░') << "] ";
    std::cout << "B[" << std::string(int(b * 10), '█') << std::string(10 - int(b * 10), '░') << "]\n";
}

void visualizeZoom(float zoom)
{
    int size = static_cast<int>(10 * zoom);
    size = std::max(5, std::min(30, size));

    std::cout << "\n  Visual Zoom:\n";
    for (int i = 0; i < size; ++i)
    {
        std::cout << "  ";
        for (int j = 0; j < size; ++j)
        {
            if (i == 0 || i == size - 1 || j == 0 || j == size - 1)
                std::cout << "█";
            else if (i == size / 2 && j == size / 2)
                std::cout << "●";
            else
                std::cout << " ";
        }
        std::cout << "\n";
    }
}

void demo1_BassPulse()
{
    std::cout << "\n╔════════════════════════════════════════════════════════════════╗\n";
    std::cout << "║           DEMO 1: BASS PULSE ZOOM (ANIMATED)                 ║\n";
    std::cout << "╚════════════════════════════════════════════════════════════════╝\n\n";

    // Create preset
    MilkDropPreset preset;
    preset.name = "Bass Pulse";
    preset.perFrameCode = R"(
        zoom = zoom + bass * 0.15;
        zoom = zoom * 0.95 + 1.0 * 0.05;
        rot = rot + 0.01;
        wave_r = 0.5 + 0.5 * sin(time);
        wave_g = 0.5 + 0.5 * cos(time);
        wave_b = 0.8;
    )";

    RenderState renderState;
    renderState.loadPreset(preset);

    std::cout << "Simulating 30 frames with pumping bass...\n";
    std::cout << "Watch the zoom and colors change!\n\n";
    std::cout << "Press Ctrl+C to stop...\n";
    std::this_thread::sleep_for(std::chrono::seconds(2));

    for (int frame = 0; frame < 30; ++frame)
    {
        clearScreen();

        // Simulate bass pumping at ~120 BPM (2 beats per second at 60fps)
        float t = frame / 60.0f;
        float bass = 0.3f + 0.7f * std::abs(std::sin(t * 4.0f));
        float mid = 0.5f;
        float treb = 0.3f;

        renderState.updateAudioData(bass, mid, treb,
                                   bass * 0.8f, mid * 0.8f, treb * 0.8f);
        auto& ctx = renderState.executeFrame(1.0f / 60.0f);

        std::cout << "╔════════════════════════════════════════════════════════════════╗\n";
        std::cout << "║           BASS PULSE ZOOM - Frame " << std::setw(3) << frame << "                     ║\n";
        std::cout << "╚════════════════════════════════════════════════════════════════╝\n\n";

        std::cout << "Audio Input:\n";
        printBar("Bass", bass);
        printBar("Mid", mid);
        printBar("Treble", treb);

        std::cout << "\nPreset State:\n";
        printBar("Zoom", (ctx.zoom - 0.8f) / 0.4f);  // Normalize for display
        printBar("Rotation", std::abs(std::fmod(ctx.rot, 3.14159f)) / 3.14159f);

        std::cout << "\nColors:\n";
        printColorBar("Wave RGB", ctx.wave_r, ctx.wave_g, ctx.wave_b);

        visualizeZoom(ctx.zoom);

        std::cout << "\nValues:\n";
        std::cout << "  Time   = " << std::fixed << std::setprecision(3) << ctx.time << "s\n";
        std::cout << "  Zoom   = " << ctx.zoom << "\n";
        std::cout << "  Rot    = " << ctx.rot << " rad\n";

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void demo2_SpiralAnimation()
{
    std::cout << "\n╔════════════════════════════════════════════════════════════════╗\n";
    std::cout << "║           DEMO 2: SPIRAL TUNNEL (ANIMATED)                   ║\n";
    std::cout << "╚════════════════════════════════════════════════════════════════╝\n\n";

    MilkDropPreset preset;
    preset.name = "Spiral Tunnel";
    preset.perFrameCode = R"(
        rot = rot + 0.015 + bass * 0.02;
        zoom = 1.0 + 0.05 * sin(time) + bass * 0.1;
        wave_r = 0.5 + 0.5 * sin(time * 2.1);
        wave_g = 0.5 + 0.5 * sin(time * 1.7 + 2.094);
        wave_b = 0.5 + 0.5 * sin(time * 1.3 + 4.188);
        q1 = time;
    )";

    RenderState renderState;
    renderState.loadPreset(preset);

    std::cout << "Rainbow colors cycling with continuous rotation...\n\n";
    std::this_thread::sleep_for(std::chrono::seconds(2));

    for (int frame = 0; frame < 40; ++frame)
    {
        clearScreen();

        float t = frame / 60.0f;
        float bass = 0.5f + 0.3f * std::sin(t * 3.0f);

        renderState.updateAudioData(bass, 0.5f, 0.3f, bass * 0.8f, 0.4f, 0.2f);
        auto& ctx = renderState.executeFrame(1.0f / 60.0f);

        std::cout << "╔════════════════════════════════════════════════════════════════╗\n";
        std::cout << "║           SPIRAL TUNNEL - Frame " << std::setw(3) << frame << "                      ║\n";
        std::cout << "╚════════════════════════════════════════════════════════════════╝\n\n";

        // Draw spinning spiral (ASCII art)
        int size = 20;
        float angle = ctx.rot;
        std::cout << "  Spiral Animation:\n\n";

        for (int y = 0; y < size; ++y)
        {
            std::cout << "  ";
            for (int x = 0; x < size * 2; ++x)
            {
                float dx = (x - size) / (float)size;
                float dy = (y - size / 2.0f) / (float)size;
                float dist = std::sqrt(dx * dx + dy * dy);
                float a = std::atan2(dy, dx);

                float spiral = std::fmod(a + angle + dist * 5.0f, 0.5f);

                if (spiral < 0.25f && dist < 1.0f && dist > 0.1f)
                    std::cout << "█";
                else
                    std::cout << " ";
            }
            std::cout << "\n";
        }

        std::cout << "\nState:\n";
        printBar("Bass", bass);
        std::cout << "  Rotation  = " << std::fixed << std::setprecision(3) << ctx.rot << " rad\n";
        std::cout << "  Zoom      = " << ctx.zoom << "\n";

        std::cout << "\nRainbow Colors:\n";
        printColorBar("Wave RGB", ctx.wave_r, ctx.wave_g, ctx.wave_b);

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void demo3_QVariableAccumulation()
{
    std::cout << "\n╔════════════════════════════════════════════════════════════════╗\n";
    std::cout << "║        DEMO 3: Q VARIABLE ACCUMULATION (ANIMATED)           ║\n";
    std::cout << "╚════════════════════════════════════════════════════════════════╝\n\n";

    MilkDropPreset preset;
    preset.name = "Q Variable Demo";
    preset.perFrameInitCode = "q1 = 0; q2 = 0; q3 = 0;";
    preset.perFrameCode = R"(
        q1 = q1 + bass * 0.1;
        q2 = q2 * 0.9 + mid * 0.1;
        q3 = q3 + 0.05;
        zoom = 1.0 + q1 * 0.1;
        wave_r = q1;
        wave_g = q2;
        wave_b = q3;
    )";

    RenderState renderState;
    renderState.loadPreset(preset);

    std::cout << "Q variables accumulate over time...\n\n";
    std::this_thread::sleep_for(std::chrono::seconds(2));

    for (int frame = 0; frame < 50; ++frame)
    {
        clearScreen();

        float bass = 0.3f + 0.7f * std::abs(std::sin(frame * 0.3f));
        float mid = 0.5f;

        renderState.updateAudioData(bass, mid, 0.3f, bass * 0.8f, 0.4f, 0.2f);
        auto& ctx = renderState.executeFrame(1.0f / 60.0f);

        std::cout << "╔════════════════════════════════════════════════════════════════╗\n";
        std::cout << "║        Q VARIABLE ACCUMULATION - Frame " << std::setw(3) << frame << "              ║\n";
        std::cout << "╚════════════════════════════════════════════════════════════════╝\n\n";

        std::cout << "Custom Variables (q1-q3):\n";
        printBar("q1 (accumulator)", std::min(1.0f, (float)ctx.q[0]));
        printBar("q2 (smoothed)", ctx.q[1]);
        printBar("q3 (counter)", std::fmod(ctx.q[2], 1.0f));

        std::cout << "\nRaw Values:\n";
        std::cout << "  q1 = " << std::fixed << std::setprecision(4) << ctx.q[0] << " (bass accumulator)\n";
        std::cout << "  q2 = " << ctx.q[1] << " (smoothed mid)\n";
        std::cout << "  q3 = " << ctx.q[2] << " (frame counter)\n";

        std::cout << "\nEffect on visuals:\n";
        printBar("Zoom (1.0 + q1*0.1)", (ctx.zoom - 0.8f) / 0.4f);
        printColorBar("Colors (q1,q2,q3)",
                     std::min(1.0f, (float)ctx.q[0]),
                     std::min(1.0f, (float)ctx.q[1]),
                     std::fmod(ctx.q[2], 1.0f));

        std::this_thread::sleep_for(std::chrono::milliseconds(150));
    }
}

int main(int argc, char** argv)
{
    std::cout << "╔════════════════════════════════════════════════════════════════╗\n";
    std::cout << "║                                                                ║\n";
    std::cout << "║         FlarkViz Visual Demo - Presets in Action              ║\n";
    std::cout << "║         ASCII Animation of MilkDrop Effects                   ║\n";
    std::cout << "║                                                                ║\n";
    std::cout << "╚════════════════════════════════════════════════════════════════╝\n";

    if (argc > 1)
    {
        std::string demo = argv[1];
        if (demo == "1") {
            demo1_BassPulse();
        } else if (demo == "2") {
            demo2_SpiralAnimation();
        } else if (demo == "3") {
            demo3_QVariableAccumulation();
        }
    }
    else
    {
        std::cout << "\nAvailable demos:\n";
        std::cout << "  ./visual_demo 1  - Bass Pulse Zoom (30 frames)\n";
        std::cout << "  ./visual_demo 2  - Spiral Tunnel (40 frames)\n";
        std::cout << "  ./visual_demo 3  - Q Variable Accumulation (50 frames)\n\n";

        std::cout << "Running all demos...\n";
        std::this_thread::sleep_for(std::chrono::seconds(2));

        demo1_BassPulse();
        std::this_thread::sleep_for(std::chrono::seconds(1));

        demo2_SpiralAnimation();
        std::this_thread::sleep_for(std::chrono::seconds(1));

        demo3_QVariableAccumulation();
    }

    clearScreen();
    std::cout << "\n╔════════════════════════════════════════════════════════════════╗\n";
    std::cout << "║                    All Demos Complete!                        ║\n";
    std::cout << "║                                                                ║\n";
    std::cout << "║  In the real FlarkViz application, these run at 60fps         ║\n";
    std::cout << "║  with full OpenGL rendering and visual effects!               ║\n";
    std::cout << "╚════════════════════════════════════════════════════════════════╝\n\n";

    return 0;
}
