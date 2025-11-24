#pragma once

#include "ExpressionTypes.h"
#include <string>
#include <vector>
#include <memory>

/**
 * @class MilkdropEval
 * @brief Parses and evaluates MilkDrop expression language
 *
 * Compiles MilkDrop equations into bytecode and executes them
 * in a stack-based virtual machine.
 */
class MilkdropEval
{
public:
    MilkdropEval();
    ~MilkdropEval();

    /**
     * @brief Compile an expression string to bytecode
     * @param expression The expression to compile (e.g., "zoom = zoom + 0.02*sin(time)")
     * @return true if compilation succeeded
     */
    bool compile(const std::string& expression);

    /**
     * @brief Compile multiple statements (separated by semicolons)
     * @param code Multi-line code block
     * @return true if all statements compiled successfully
     */
    bool compileBlock(const std::string& code);

    /**
     * @brief Execute the compiled bytecode
     * @param context Execution context with variables
     * @return Result of the last expression evaluated
     */
    double execute(MilkDrop::ExecutionContext& context);

    /**
     * @brief Get the last error message
     */
    std::string getLastError() const { return lastError; }

    /**
     * @brief Clear compiled bytecode
     */
    void clear();

private:
    MilkDrop::CompiledExpression compiled;
    std::string lastError;

    // Lexer
    std::vector<MilkDrop::Token> tokenize(const std::string& expression);
    bool isWhitespace(char c);
    bool isDigit(char c);
    bool isAlpha(char c);
    bool isAlphaNumeric(char c);

    // Parser
    size_t currentToken;
    std::vector<MilkDrop::Token> tokens;

    void parseStatement();
    void parseAssignment();
    void parseExpression();
    void parseLogicalOr();
    void parseLogicalAnd();
    void parseComparison();
    void parseTerm();
    void parseFactor();
    void parsePower();
    void parseUnary();
    void parsePrimary();
    void parseFunctionCall(const std::string& funcName);

    bool match(MilkDrop::TokenType type);
    bool check(MilkDrop::TokenType type);
    MilkDrop::Token advance();
    MilkDrop::Token peek();
    MilkDrop::Token previous();
    bool isAtEnd();

    void emit(MilkDrop::OpCode opcode);
    void emit(MilkDrop::OpCode opcode, double operand);
    void emit(MilkDrop::OpCode opcode, int varIndex);

    // VM
    std::vector<double> stack;
    double executeVM(const std::vector<MilkDrop::Instruction>& bytecode,
                     MilkDrop::ExecutionContext& context);
    double pop();
    void push(double value);
};
