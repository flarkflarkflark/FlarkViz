/**
 * Animated ASCII Demo - Pure expression evaluation (no JUCE dependencies)
 */

#include "../Source/Expression/MilkdropEval.h"
#include "../Source/Expression/ExpressionTypes.h"
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

void visualizeZoom(float zoom)
{
    int size = static_cast<int>(10 * zoom);
    size = std::max(5, std::min(25, size));

    std::cout << "\n  Visual Zoom Effect:\n";
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

void visualizeSpiral(float rotation, int size = 15)
{
    std::cout << "\n  Spiral Animation:\n\n";

    for (int y = 0; y < size; ++y)
    {
        std::cout << "  ";
        for (int x = 0; x < size * 2; ++x)
        {
            float dx = (x - size) / (float)size;
            float dy = (y - size / 2.0f) / (float)size;
            float dist = std::sqrt(dx * dx + dy * dy);
            float angle = std::atan2(dy, dx);

            float spiral = std::fmod(angle + rotation + dist * 5.0f, 0.5f);

            if (spiral < 0.25f && dist < 1.0f && dist > 0.1f)
                std::cout << "█";
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
    std::cout << "Simulating bass-reactive zoom with smooth decay...\n";
    std::cout << "Watch the zoom pulse with the beat!\n\n";
    std::this_thread::sleep_for(std::chrono::seconds(2));

    MilkdropEval eval;
    MilkDrop::ExecutionContext ctx;

    // Compile per-frame code
    std::string code = R"(
        zoom = zoom + bass * 0.15;
        zoom = zoom * 0.95 + 1.0 * 0.05;
        rot = rot + 0.01;
        wave_r = 0.5 + 0.5 * sin(time);
        wave_g = 0.5 + 0.5 * cos(time);
        wave_b = 0.8;
    )";

    eval.compileBlock(code);

    ctx.zoom = 1.0;
    ctx.rot = 0.0;
    ctx.time = 0.0;

    for (int frame = 0; frame < 30; ++frame)
    {
        clearScreen();

        // Simulate bass pumping at ~120 BPM
        float t = frame / 60.0f;
        ctx.bass = 0.3f + 0.7f * std::abs(std::sin(t * 4.0f));
        ctx.mid = 0.5f;
        ctx.treb = 0.3f;
        ctx.time += 1.0f / 60.0f;

        eval.execute(ctx);

        std::cout << "╔════════════════════════════════════════════════════════════════╗\n";
        std::cout << "║           BASS PULSE ZOOM - Frame " << std::setw(3) << frame << "                     ║\n";
        std::cout << "╚════════════════════════════════════════════════════════════════╝\n\n";

        std::cout << "Audio Input:\n";
        printBar("Bass", ctx.bass);
        printBar("Mid", ctx.mid);
        printBar("Treble", ctx.treb);

        std::cout << "\nPreset State:\n";
        printBar("Zoom", (ctx.zoom - 0.8f) / 0.4f);
        printBar("Rotation", std::abs(std::fmod(ctx.rot, 3.14159f)) / 3.14159f);

        std::cout << "\nWave Colors:\n";
        printBar("Red", ctx.wave_r);
        printBar("Green", ctx.wave_g);
        printBar("Blue", ctx.wave_b);

        visualizeZoom(ctx.zoom);

        std::cout << "\nExact Values:\n";
        std::cout << "  Time = " << std::fixed << std::setprecision(3) << ctx.time << "s\n";
        std::cout << "  Zoom = " << ctx.zoom << " (pulses with bass)\n";
        std::cout << "  Rot  = " << ctx.rot << " rad\n";

        std::this_thread::sleep_for(std::chrono::milliseconds(150));
    }
}

void demo2_SpiralTunnel()
{
    std::cout << "\n╔════════════════════════════════════════════════════════════════╗\n";
    std::cout << "║           DEMO 2: SPIRAL TUNNEL (ANIMATED)                   ║\n";
    std::cout << "╚════════════════════════════════════════════════════════════════╝\n\n";
    std::cout << "Continuous rotation with rainbow colors...\n\n";
    std::this_thread::sleep_for(std::chrono::seconds(2));

    MilkdropEval eval;
    MilkDrop::ExecutionContext ctx;

    std::string code = R"(
        rot = rot + 0.015 + bass * 0.02;
        zoom = 1.0 + 0.05 * sin(time) + bass * 0.1;
        wave_r = 0.5 + 0.5 * sin(time * 2.1);
        wave_g = 0.5 + 0.5 * sin(time * 1.7 + 2.094);
        wave_b = 0.5 + 0.5 * sin(time * 1.3 + 4.188);
    )";

    eval.compileBlock(code);

    ctx.zoom = 1.0;
    ctx.rot = 0.0;
    ctx.time = 0.0;

    for (int frame = 0; frame < 40; ++frame)
    {
        clearScreen();

        float t = frame / 60.0f;
        ctx.bass = 0.5f + 0.3f * std::sin(t * 3.0f);
        ctx.time += 1.0f / 60.0f;

        eval.execute(ctx);

        std::cout << "╔════════════════════════════════════════════════════════════════╗\n";
        std::cout << "║           SPIRAL TUNNEL - Frame " << std::setw(3) << frame << "                      ║\n";
        std::cout << "╚════════════════════════════════════════════════════════════════╝\n\n";

        visualizeSpiral(ctx.rot);

        std::cout << "\nState:\n";
        printBar("Bass", ctx.bass);
        std::cout << "  Rotation = " << std::fixed << std::setprecision(3) << ctx.rot << " rad\n";
        std::cout << "  Zoom     = " << ctx.zoom << "\n";

        std::cout << "\nRainbow Colors:\n";
        printBar("Red", ctx.wave_r);
        printBar("Green", ctx.wave_g);
        printBar("Blue", ctx.wave_b);

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void demo3_QVariables()
{
    std::cout << "\n╔════════════════════════════════════════════════════════════════╗\n";
    std::cout << "║        DEMO 3: Q VARIABLE ACCUMULATION (ANIMATED)           ║\n";
    std::cout << "╚════════════════════════════════════════════════════════════════╝\n\n";
    std::cout << "Q variables accumulate and smooth over time...\n\n";
    std::this_thread::sleep_for(std::chrono::seconds(2));

    MilkdropEval eval;
    MilkDrop::ExecutionContext ctx;

    std::string code = R"(
        q1 = q1 + bass * 0.1;
        q2 = q2 * 0.9 + mid * 0.1;
        q3 = q3 + 0.05;
        zoom = 1.0 + q1 * 0.1;
        wave_r = q1;
        wave_g = q2;
        wave_b = q3;
    )";

    eval.compileBlock(code);

    ctx.q[0] = 0.0;
    ctx.q[1] = 0.0;
    ctx.q[2] = 0.0;
    ctx.zoom = 1.0;

    for (int frame = 0; frame < 50; ++frame)
    {
        clearScreen();

        ctx.bass = 0.3f + 0.7f * std::abs(std::sin(frame * 0.3f));
        ctx.mid = 0.5f;

        eval.execute(ctx);

        std::cout << "╔════════════════════════════════════════════════════════════════╗\n";
        std::cout << "║        Q VARIABLE ACCUMULATION - Frame " << std::setw(3) << frame << "              ║\n";
        std::cout << "╚════════════════════════════════════════════════════════════════╝\n\n";

        std::cout << "Custom Variables:\n";
        printBar("q1 (accumulator)", std::min(1.0, ctx.q[0]));
        printBar("q2 (smoothed)", ctx.q[1]);
        printBar("q3 (counter)", std::fmod(ctx.q[2], 1.0));

        std::cout << "\nAccumulation Graph:\n  ";
        int barSize = static_cast<int>(ctx.q[0] * 50);
        for (int i = 0; i < std::min(60, barSize); ++i)
            std::cout << "█";
        std::cout << "\n";

        std::cout << "\nRaw Values:\n";
        std::cout << "  q1 = " << std::fixed << std::setprecision(4) << ctx.q[0] << " (keeps growing!)\n";
        std::cout << "  q2 = " << ctx.q[1] << " (smoothed mid)\n";
        std::cout << "  q3 = " << ctx.q[2] << " (steady increase)\n";

        std::cout << "\nEffect on Zoom:\n";
        printBar("Zoom (1.0 + q1*0.1)", (ctx.zoom - 0.8f) / 0.4f);

        std::cout << "\nColors from Q:\n";
        printBar("Red (q1)", std::min(1.0, ctx.q[0]));
        printBar("Green (q2)", ctx.q[1]);
        printBar("Blue (q3)", std::fmod(ctx.q[2], 1.0));

        std::this_thread::sleep_for(std::chrono::milliseconds(120));
    }
}

int main(int argc, char** argv)
{
    std::cout << "╔════════════════════════════════════════════════════════════════╗\n";
    std::cout << "║                                                                ║\n";
    std::cout << "║         FlarkViz Animated Demo - Presets in Action            ║\n";
    std::cout << "║         ASCII Animation of MilkDrop Expression Effects        ║\n";
    std::cout << "║                                                                ║\n";
    std::cout << "╚════════════════════════════════════════════════════════════════╝\n";

    if (argc > 1)
    {
        std::string demo = argv[1];
        if (demo == "1") {
            demo1_BassPulse();
        } else if (demo == "2") {
            demo2_SpiralTunnel();
        } else if (demo == "3") {
            demo3_QVariables();
        } else {
            std::cout << "\nUnknown demo. Use: 1, 2, or 3\n";
        }
    }
    else
    {
        std::cout << "\nRunning all 3 demos...\n";
        std::this_thread::sleep_for(std::chrono::seconds(2));

        demo1_BassPulse();
        std::this_thread::sleep_for(std::chrono::seconds(2));

        demo2_SpiralTunnel();
        std::this_thread::sleep_for(std::chrono::seconds(2));

        demo3_QVariables();
    }

    clearScreen();
    std::cout << "\n╔════════════════════════════════════════════════════════════════╗\n";
    std::cout << "║                    All Demos Complete!                        ║\n";
    std::cout << "║                                                                ║\n";
    std::cout << "║  These ASCII animations show the expression evaluation        ║\n";
    std::cout << "║  system in action. In real FlarkViz, this runs at 60fps       ║\n";
    std::cout << "║  with full OpenGL rendering and beautiful visual effects!     ║\n";
    std::cout << "╚════════════════════════════════════════════════════════════════╝\n\n";

    return 0;
}
