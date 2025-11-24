#pragma once

#include <string>
#include <vector>
#include <memory>
#include <cmath>
#include <map>

namespace MilkDrop {

/**
 * @enum TokenType
 * @brief Token types for expression parsing
 */
enum class TokenType
{
    Number,
    Identifier,
    Plus,
    Minus,
    Multiply,
    Divide,
    Modulo,
    Power,
    LeftParen,
    RightParen,
    Comma,
    Semicolon,
    Assign,
    Equal,
    NotEqual,
    Less,
    Greater,
    LessEqual,
    GreaterEqual,
    LogicalAnd,
    LogicalOr,
    End
};

/**
 * @struct Token
 * @brief Represents a single token in the expression
 */
struct Token
{
    TokenType type;
    std::string text;
    double value;

    Token(TokenType t) : type(t), value(0.0) {}
    Token(TokenType t, const std::string& txt) : type(t), text(txt), value(0.0) {}
    Token(TokenType t, double val) : type(t), value(val) {}
};

/**
 * @enum OpCode
 * @brief Bytecode operations for the VM
 */
enum class OpCode
{
    // Stack operations
    Push,           // Push constant
    Load,           // Load variable
    Store,          // Store to variable

    // Arithmetic
    Add,
    Subtract,
    Multiply,
    Divide,
    Modulo,
    Negate,

    // Math functions
    Sin,
    Cos,
    Tan,
    ASin,
    ACos,
    ATan,
    ATan2,
    Sqrt,
    Abs,
    Sqr,
    Pow,
    Exp,
    Log,
    Log10,

    // Utility functions
    Min,
    Max,
    Sign,
    Rand,
    If,
    Equal,
    Above,
    Below,

    // Comparison
    CmpEqual,
    CmpNotEqual,
    CmpLess,
    CmpGreater,
    CmpLessEqual,
    CmpGreaterEqual,

    // Logical
    And,
    Or,

    // Control
    Jump,
    JumpIfFalse,
    Halt
};

/**
 * @struct Instruction
 * @brief Single VM instruction
 */
struct Instruction
{
    OpCode opcode;
    double operand;
    int varIndex;

    Instruction(OpCode op) : opcode(op), operand(0.0), varIndex(-1) {}
    Instruction(OpCode op, double val) : opcode(op), operand(val), varIndex(-1) {}
    Instruction(OpCode op, int idx) : opcode(op), operand(0.0), varIndex(idx) {}
};

/**
 * @class CompiledExpression
 * @brief Compiled bytecode for an expression
 */
class CompiledExpression
{
public:
    std::vector<Instruction> bytecode;
    std::vector<std::string> variableNames;

    void clear()
    {
        bytecode.clear();
        variableNames.clear();
    }

    int addVariable(const std::string& name)
    {
        for (size_t i = 0; i < variableNames.size(); ++i)
        {
            if (variableNames[i] == name)
                return static_cast<int>(i);
        }
        variableNames.push_back(name);
        return static_cast<int>(variableNames.size() - 1);
    }
};

/**
 * @class ExecutionContext
 * @brief Runtime context for expression evaluation
 */
class ExecutionContext
{
public:
    std::map<std::string, double> variables;

    // Audio variables
    double bass = 0.0;
    double mid = 0.0;
    double treb = 0.0;
    double bass_att = 0.0;
    double mid_att = 0.0;
    double treb_att = 0.0;

    // Time variables
    double time = 0.0;
    double frame = 0.0;
    double fps = 60.0;

    // State variables (per-frame)
    double zoom = 1.0;
    double rot = 0.0;
    double cx = 0.5;
    double cy = 0.5;
    double dx = 0.0;
    double dy = 0.0;
    double warp = 1.0;
    double sx = 1.0;
    double sy = 1.0;

    // Wave colors
    double wave_r = 1.0;
    double wave_g = 1.0;
    double wave_b = 1.0;
    double wave_a = 1.0;

    // Custom variables (q1-q32)
    double q[32] = {0};

    // Per-pixel variables (additional)
    double x = 0.0;  // Normalized x coordinate (0-1)
    double y = 0.0;  // Normalized y coordinate (0-1)
    double rad = 0.0; // Distance from center
    double ang = 0.0; // Angle from center

    ExecutionContext()
    {
        for (int i = 0; i < 32; ++i)
            q[i] = 0.0;
    }

    double getVariable(const std::string& name) const
    {
        // Check built-in variables first
        if (name == "bass") return bass;
        if (name == "mid") return mid;
        if (name == "treb") return treb;
        if (name == "bass_att") return bass_att;
        if (name == "mid_att") return mid_att;
        if (name == "treb_att") return treb_att;
        if (name == "time") return time;
        if (name == "frame") return frame;
        if (name == "fps") return fps;
        if (name == "zoom") return zoom;
        if (name == "rot") return rot;
        if (name == "cx") return cx;
        if (name == "cy") return cy;
        if (name == "dx") return dx;
        if (name == "dy") return dy;
        if (name == "warp") return warp;
        if (name == "sx") return sx;
        if (name == "sy") return sy;
        if (name == "wave_r") return wave_r;
        if (name == "wave_g") return wave_g;
        if (name == "wave_b") return wave_b;
        if (name == "wave_a") return wave_a;
        if (name == "x") return x;
        if (name == "y") return y;
        if (name == "rad") return rad;
        if (name == "ang") return ang;

        // Check q variables (q1-q32)
        if (name.length() >= 2 && name[0] == 'q')
        {
            int idx = std::stoi(name.substr(1)) - 1;
            if (idx >= 0 && idx < 32)
                return q[idx];
        }

        // Check custom variables
        auto it = variables.find(name);
        if (it != variables.end())
            return it->second;

        return 0.0;
    }

    void setVariable(const std::string& name, double value)
    {
        // Set built-in variables
        if (name == "bass") { bass = value; return; }
        if (name == "mid") { mid = value; return; }
        if (name == "treb") { treb = value; return; }
        if (name == "bass_att") { bass_att = value; return; }
        if (name == "mid_att") { mid_att = value; return; }
        if (name == "treb_att") { treb_att = value; return; }
        if (name == "time") { time = value; return; }
        if (name == "frame") { frame = value; return; }
        if (name == "fps") { fps = value; return; }
        if (name == "zoom") { zoom = value; return; }
        if (name == "rot") { rot = value; return; }
        if (name == "cx") { cx = value; return; }
        if (name == "cy") { cy = value; return; }
        if (name == "dx") { dx = value; return; }
        if (name == "dy") { dy = value; return; }
        if (name == "warp") { warp = value; return; }
        if (name == "sx") { sx = value; return; }
        if (name == "sy") { sy = value; return; }
        if (name == "wave_r") { wave_r = value; return; }
        if (name == "wave_g") { wave_g = value; return; }
        if (name == "wave_b") { wave_b = value; return; }
        if (name == "wave_a") { wave_a = value; return; }
        if (name == "x") { x = value; return; }
        if (name == "y") { y = value; return; }
        if (name == "rad") { rad = value; return; }
        if (name == "ang") { ang = value; return; }

        // Set q variables
        if (name.length() >= 2 && name[0] == 'q')
        {
            int idx = std::stoi(name.substr(1)) - 1;
            if (idx >= 0 && idx < 32)
            {
                q[idx] = value;
                return;
            }
        }

        // Custom variables
        variables[name] = value;
    }
};

} // namespace MilkDrop
