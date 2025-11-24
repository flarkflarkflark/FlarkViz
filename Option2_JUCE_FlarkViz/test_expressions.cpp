#include "Source/Expression/MilkdropEval.h"
#include "Source/Expression/ExpressionTypes.h"
#include <iostream>
#include <iomanip>

void testExpression(const std::string& expr, MilkDrop::ExecutionContext& ctx, const std::string& description)
{
    MilkdropEval eval;

    if (eval.compile(expr))
    {
        double result = eval.execute(ctx);
        std::cout << std::setw(40) << std::left << description
                  << " = " << std::setw(10) << std::right << result
                  << std::endl;
    }
    else
    {
        std::cout << "ERROR: " << description << " - " << eval.getLastError() << std::endl;
    }
}

int main()
{
    std::cout << "============================================" << std::endl;
    std::cout << "  FlarkViz Expression System Test" << std::endl;
    std::cout << "============================================" << std::endl << std::endl;

    MilkDrop::ExecutionContext ctx;
    ctx.time = 1.0;
    ctx.bass = 0.8;
    ctx.mid = 0.5;
    ctx.treb = 0.3;
    ctx.zoom = 1.0;
    ctx.rot = 0.0;

    std::cout << "Basic Arithmetic:" << std::endl;
    testExpression("2 + 3", ctx, "2 + 3");
    testExpression("10 - 4", ctx, "10 - 4");
    testExpression("5 * 7", ctx, "5 * 7");
    testExpression("20 / 4", ctx, "20 / 4");
    testExpression("2 + 3 * 4", ctx, "2 + 3 * 4 (precedence)");
    testExpression("(2 + 3) * 4", ctx, "(2 + 3) * 4 (parentheses)");
    std::cout << std::endl;

    std::cout << "Math Functions:" << std::endl;
    testExpression("sin(0)", ctx, "sin(0)");
    testExpression("cos(0)", ctx, "cos(0)");
    testExpression("sqrt(16)", ctx, "sqrt(16)");
    testExpression("abs(-5)", ctx, "abs(-5)");
    testExpression("pow(2, 3)", ctx, "pow(2, 3)");
    testExpression("min(5, 3)", ctx, "min(5, 3)");
    testExpression("max(5, 3)", ctx, "max(5, 3)");
    std::cout << std::endl;

    std::cout << "Variable Access:" << std::endl;
    testExpression("time", ctx, "time");
    testExpression("bass", ctx, "bass");
    testExpression("mid", ctx, "mid");
    testExpression("treb", ctx, "treb");
    testExpression("zoom", ctx, "zoom");
    std::cout << std::endl;

    std::cout << "Expressions with Variables:" << std::endl;
    testExpression("time + bass", ctx, "time + bass");
    testExpression("sin(time)", ctx, "sin(time)");
    testExpression("0.5 + 0.5 * sin(time)", ctx, "0.5 + 0.5 * sin(time)");
    testExpression("bass * mid * treb", ctx, "bass * mid * treb");
    std::cout << std::endl;

    std::cout << "Assignments:" << std::endl;
    testExpression("zoom = 1.5", ctx, "zoom = 1.5");
    testExpression("zoom", ctx, "zoom (after assignment)");
    testExpression("rot = rot + 0.1", ctx, "rot = rot + 0.1");
    testExpression("rot", ctx, "rot (after increment)");
    std::cout << std::endl;

    std::cout << "Complex MilkDrop-style Expressions:" << std::endl;
    testExpression("zoom = zoom + 0.02 * sin(time)", ctx, "zoom = zoom + 0.02 * sin(time)");
    testExpression("zoom", ctx, "zoom (after complex update)");

    // Test multi-line block
    std::cout << std::endl << "Multi-line Code Block:" << std::endl;
    MilkdropEval blockEval;
    std::string code = R"(
        zoom = 1.0 + 0.1 * sin(time + bass);
        rot = rot + 0.05 * cos(time);
        wave_r = 0.5 + 0.5 * sin(time);
        wave_g = 0.5 + 0.5 * sin(time + 2.0);
        wave_b = 0.5 + 0.5 * sin(time + 4.0)
    )";

    if (blockEval.compileBlock(code))
    {
        blockEval.execute(ctx);
        std::cout << "  Block executed successfully!" << std::endl;
        std::cout << "  zoom    = " << ctx.zoom << std::endl;
        std::cout << "  rot     = " << ctx.rot << std::endl;
        std::cout << "  wave_r  = " << ctx.wave_r << std::endl;
        std::cout << "  wave_g  = " << ctx.wave_g << std::endl;
        std::cout << "  wave_b  = " << ctx.wave_b << std::endl;
    }
    else
    {
        std::cout << "  ERROR: " << blockEval.getLastError() << std::endl;
    }

    std::cout << std::endl << "============================================" << std::endl;
    std::cout << "  All tests completed!" << std::endl;
    std::cout << "============================================" << std::endl;

    return 0;
}
