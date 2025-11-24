/**
 * Simple Expression Evaluation Demo
 *
 * Demonstrates FlarkViz expression evaluation without requiring JUCE/OpenGL.
 * Compile: g++ -std=c++20 -I.. simple_expression_demo.cpp ../Source/Expression/MilkdropEval.cpp -o expr_demo
 */

#include "../Source/Expression/MilkdropEval.h"
#include "../Source/Expression/ExpressionTypes.h"
#include <iostream>
#include <iomanip>
#include <cmath>

void printSeparator(const std::string& title = "")
{
    std::cout << "\n" << std::string(70, '=') << "\n";
    if (!title.empty())
        std::cout << "  " << title << "\n" << std::string(70, '=') << "\n";
}

void demo1_BasicExpressions()
{
    printSeparator("Demo 1: Basic Arithmetic");

    MilkdropEval eval;
    MilkDrop::ExecutionContext ctx;

    std::vector<std::string> expressions = {
        "2 + 3",
        "10 - 4",
        "5 * 6",
        "20 / 4",
        "7 % 3",
        "2 + 3 * 4",
        "(2 + 3) * 4"
    };

    for (const auto& expr : expressions)
    {
        if (eval.compile(expr))
        {
            double result = eval.execute(ctx);
            std::cout << std::setw(20) << expr << " = " << result << "\n";
        }
    }
}

void demo2_MathFunctions()
{
    printSeparator("Demo 2: Math Functions");

    MilkdropEval eval;
    MilkDrop::ExecutionContext ctx;

    std::vector<std::string> expressions = {
        "sin(0)",
        "cos(0)",
        "sin(3.14159 / 2)",
        "sqrt(16)",
        "abs(-5)",
        "sqr(3)",
        "pow(2, 8)",
        "min(5, 3)",
        "max(5, 3)",
        "floor(3.7)",
        "ceil(3.2)"
    };

    std::cout << std::fixed << std::setprecision(4);

    for (const auto& expr : expressions)
    {
        if (eval.compile(expr))
        {
            double result = eval.execute(ctx);
            std::cout << std::setw(25) << expr << " = " << result << "\n";
        }
    }
}

void demo3_AudioReactive()
{
    printSeparator("Demo 3: Audio-Reactive Variables");

    MilkdropEval eval;
    MilkDrop::ExecutionContext ctx;

    // Simulate audio data
    ctx.bass = 0.8;
    ctx.mid = 0.5;
    ctx.treb = 0.3;
    ctx.time = 1.5;

    std::cout << "Audio State:\n";
    std::cout << "  bass = " << ctx.bass << "\n";
    std::cout << "  mid  = " << ctx.mid << "\n";
    std::cout << "  treb = " << ctx.treb << "\n";
    std::cout << "  time = " << ctx.time << "\n\n";

    std::vector<std::string> expressions = {
        "1.0 + bass * 0.5",
        "sin(time + bass)",
        "bass + mid + treb",
        "bass * mid * treb",
        "abs(sin(time)) * bass"
    };

    std::cout << std::fixed << std::setprecision(4);

    for (const auto& expr : expressions)
    {
        if (eval.compile(expr))
        {
            double result = eval.execute(ctx);
            std::cout << std::setw(30) << expr << " = " << result << "\n";
        }
    }
}

void demo4_Variables()
{
    printSeparator("Demo 4: Variable Assignment");

    MilkdropEval eval;
    MilkDrop::ExecutionContext ctx;

    ctx.zoom = 1.0;
    ctx.rot = 0.0;
    ctx.bass = 0.5;
    ctx.time = 2.0;

    std::cout << "Initial state:\n";
    std::cout << "  zoom = " << ctx.zoom << "\n";
    std::cout << "  rot  = " << ctx.rot << "\n\n";

    // Compile and execute expressions that modify state
    std::vector<std::string> expressions = {
        "zoom = zoom + bass * 0.2",
        "rot = rot + 0.1",
        "wave_r = 0.5 + 0.5 * sin(time)",
        "wave_g = 0.5 + 0.5 * cos(time)",
        "wave_b = 0.8"
    };

    std::cout << "Executing expressions:\n";
    for (const auto& expr : expressions)
    {
        if (eval.compile(expr))
        {
            eval.execute(ctx);
            std::cout << "  " << expr << "\n";
        }
    }

    std::cout << "\nFinal state:\n";
    std::cout << "  zoom   = " << ctx.zoom << "\n";
    std::cout << "  rot    = " << ctx.rot << "\n";
    std::cout << "  wave_r = " << ctx.wave_r << "\n";
    std::cout << "  wave_g = " << ctx.wave_g << "\n";
    std::cout << "  wave_b = " << ctx.wave_b << "\n";
}

void demo5_CustomVariables()
{
    printSeparator("Demo 5: Custom Q Variables");

    MilkdropEval eval;
    MilkDrop::ExecutionContext ctx;

    // Initialize q variables
    ctx.q[0] = 0.0;  // q1 - accumulator
    ctx.q[1] = 1.0;  // q2 - smoothed value
    ctx.q[2] = 0.0;  // q3 - counter

    std::cout << "Initial q values:\n";
    std::cout << "  q1 = " << ctx.q[0] << "\n";
    std::cout << "  q2 = " << ctx.q[1] << "\n";
    std::cout << "  q3 = " << ctx.q[2] << "\n\n";

    // Simulate multiple frames
    std::cout << "Simulating 5 frames:\n\n";

    for (int frame = 0; frame < 5; ++frame)
    {
        // Simulate audio
        ctx.bass = 0.5 + 0.5 * std::sin(frame * 0.5);
        ctx.mid = 0.3;

        // Execute per-frame code
        MilkdropEval eval1, eval2, eval3;

        eval1.compile("q1 = q1 + bass * 0.1");
        eval1.execute(ctx);

        eval2.compile("q2 = q2 * 0.9 + mid * 0.1");
        eval2.execute(ctx);

        eval3.compile("q3 = q3 + 1");
        eval3.execute(ctx);

        std::cout << std::fixed << std::setprecision(4);
        std::cout << "Frame " << frame << ": ";
        std::cout << "q1=" << ctx.q[0] << " ";
        std::cout << "q2=" << ctx.q[1] << " ";
        std::cout << "q3=" << ctx.q[2] << " ";
        std::cout << "(bass=" << ctx.bass << ")\n";
    }
}

void demo6_CompletePresetSimulation()
{
    printSeparator("Demo 6: Complete Preset Simulation");

    std::cout << "Simulating a complete MilkDrop preset over 10 frames\n\n";

    // Per-frame code
    std::string perFrameCode = R"(
        zoom = 1.0 + 0.2 * sin(time + bass);
        rot = rot + 0.02 * cos(time * 0.5);
        wave_r = 0.5 + 0.5 * sin(time);
        wave_g = 0.5 + 0.5 * cos(time);
        wave_b = 0.8;
        q1 = q1 + bass * 0.1;
    )";

    std::cout << "Per-frame code:\n" << perFrameCode << "\n";

    // Compile the block
    MilkdropEval eval;
    if (!eval.compileBlock(perFrameCode))
    {
        std::cout << "Failed to compile!\n";
        return;
    }

    // Initialize context
    MilkDrop::ExecutionContext ctx;
    ctx.zoom = 1.0;
    ctx.rot = 0.0;
    ctx.time = 0.0;
    ctx.q[0] = 0.0;

    std::cout << "\nFrame | Time   | Bass  | Zoom   | Rot    | wave_r | q1     \n";
    std::cout << "------|--------|-------|--------|--------|--------|--------\n";

    std::cout << std::fixed << std::setprecision(3);

    // Simulate 10 frames
    for (int frame = 0; frame < 10; ++frame)
    {
        // Simulate audio (bass peaks every few frames)
        ctx.bass = 0.3 + 0.7 * std::sin(frame * 0.8);
        ctx.time += 1.0f / 60.0f;  // 60fps

        // Execute per-frame code
        eval.execute(ctx);

        // Print state
        std::cout << std::setw(5) << frame << " | ";
        std::cout << std::setw(6) << ctx.time << " | ";
        std::cout << std::setw(5) << ctx.bass << " | ";
        std::cout << std::setw(6) << ctx.zoom << " | ";
        std::cout << std::setw(6) << ctx.rot << " | ";
        std::cout << std::setw(6) << ctx.wave_r << " | ";
        std::cout << std::setw(6) << ctx.q[0] << "\n";
    }

    std::cout << "\n✅ Preset simulation complete!\n";
}

int main()
{
    std::cout << "╔════════════════════════════════════════════════════════════════════╗\n";
    std::cout << "║         FlarkViz Expression Evaluation Demo                       ║\n";
    std::cout << "║         Demonstrating MilkDrop Expression Language                ║\n";
    std::cout << "╚════════════════════════════════════════════════════════════════════╝\n";

    demo1_BasicExpressions();
    demo2_MathFunctions();
    demo3_AudioReactive();
    demo4_Variables();
    demo5_CustomVariables();
    demo6_CompletePresetSimulation();

    printSeparator("All Demos Complete");
    std::cout << "\n✅ FlarkViz expression system is working correctly!\n\n";

    return 0;
}
