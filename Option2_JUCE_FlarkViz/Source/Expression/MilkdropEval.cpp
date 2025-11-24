#include "MilkdropEval.h"
#include <cctype>
#include <cmath>
#include <cstdlib>
#include <sstream>

MilkdropEval::MilkdropEval()
    : currentToken(0)
{
}

MilkdropEval::~MilkdropEval()
{
}

void MilkdropEval::clear()
{
    compiled.clear();
    stack.clear();
    tokens.clear();
    currentToken = 0;
    lastError.clear();
}

// ============================================================================
// LEXER
// ============================================================================

bool MilkdropEval::isWhitespace(char c)
{
    return c == ' ' || c == '\t' || c == '\r' || c == '\n';
}

bool MilkdropEval::isDigit(char c)
{
    return c >= '0' && c <= '9';
}

bool MilkdropEval::isAlpha(char c)
{
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

bool MilkdropEval::isAlphaNumeric(char c)
{
    return isAlpha(c) || isDigit(c);
}

std::vector<MilkDrop::Token> MilkdropEval::tokenize(const std::string& expression)
{
    std::vector<MilkDrop::Token> result;
    size_t pos = 0;

    while (pos < expression.length())
    {
        // Skip whitespace
        if (isWhitespace(expression[pos]))
        {
            pos++;
            continue;
        }

        // Numbers
        if (isDigit(expression[pos]) || expression[pos] == '.')
        {
            std::string numStr;
            while (pos < expression.length() &&
                   (isDigit(expression[pos]) || expression[pos] == '.'))
            {
                numStr += expression[pos++];
            }
            result.push_back(MilkDrop::Token(MilkDrop::TokenType::Number, std::stod(numStr)));
            continue;
        }

        // Identifiers and keywords
        if (isAlpha(expression[pos]))
        {
            std::string ident;
            while (pos < expression.length() && isAlphaNumeric(expression[pos]))
            {
                ident += expression[pos++];
            }
            result.push_back(MilkDrop::Token(MilkDrop::TokenType::Identifier, ident));
            continue;
        }

        // Operators and punctuation
        char c = expression[pos];

        if (c == '+') { result.push_back(MilkDrop::Token(MilkDrop::TokenType::Plus)); pos++; }
        else if (c == '-') { result.push_back(MilkDrop::Token(MilkDrop::TokenType::Minus)); pos++; }
        else if (c == '*') { result.push_back(MilkDrop::Token(MilkDrop::TokenType::Multiply)); pos++; }
        else if (c == '/') { result.push_back(MilkDrop::Token(MilkDrop::TokenType::Divide)); pos++; }
        else if (c == '%') { result.push_back(MilkDrop::Token(MilkDrop::TokenType::Modulo)); pos++; }
        else if (c == '(') { result.push_back(MilkDrop::Token(MilkDrop::TokenType::LeftParen)); pos++; }
        else if (c == ')') { result.push_back(MilkDrop::Token(MilkDrop::TokenType::RightParen)); pos++; }
        else if (c == ',') { result.push_back(MilkDrop::Token(MilkDrop::TokenType::Comma)); pos++; }
        else if (c == ';') { result.push_back(MilkDrop::Token(MilkDrop::TokenType::Semicolon)); pos++; }
        else if (c == '=' && pos + 1 < expression.length() && expression[pos + 1] == '=')
        {
            result.push_back(MilkDrop::Token(MilkDrop::TokenType::Equal));
            pos += 2;
        }
        else if (c == '=')
        {
            result.push_back(MilkDrop::Token(MilkDrop::TokenType::Assign));
            pos++;
        }
        else if (c == '!' && pos + 1 < expression.length() && expression[pos + 1] == '=')
        {
            result.push_back(MilkDrop::Token(MilkDrop::TokenType::NotEqual));
            pos += 2;
        }
        else if (c == '<' && pos + 1 < expression.length() && expression[pos + 1] == '=')
        {
            result.push_back(MilkDrop::Token(MilkDrop::TokenType::LessEqual));
            pos += 2;
        }
        else if (c == '<')
        {
            result.push_back(MilkDrop::Token(MilkDrop::TokenType::Less));
            pos++;
        }
        else if (c == '>' && pos + 1 < expression.length() && expression[pos + 1] == '=')
        {
            result.push_back(MilkDrop::Token(MilkDrop::TokenType::GreaterEqual));
            pos += 2;
        }
        else if (c == '>')
        {
            result.push_back(MilkDrop::Token(MilkDrop::TokenType::Greater));
            pos++;
        }
        else if (c == '&' && pos + 1 < expression.length() && expression[pos + 1] == '&')
        {
            result.push_back(MilkDrop::Token(MilkDrop::TokenType::LogicalAnd));
            pos += 2;
        }
        else if (c == '|' && pos + 1 < expression.length() && expression[pos + 1] == '|')
        {
            result.push_back(MilkDrop::Token(MilkDrop::TokenType::LogicalOr));
            pos += 2;
        }
        else
        {
            // Unknown character, skip it
            pos++;
        }
    }

    result.push_back(MilkDrop::Token(MilkDrop::TokenType::End));
    return result;
}

// ============================================================================
// COMPILER
// ============================================================================

bool MilkdropEval::compile(const std::string& expression)
{
    clear();

    try
    {
        tokens = tokenize(expression);
        currentToken = 0;

        parseStatement();

        emit(MilkDrop::OpCode::Halt);
        return true;
    }
    catch (const std::exception& e)
    {
        lastError = std::string("Compilation error: ") + e.what();
        return false;
    }
}

bool MilkdropEval::compileBlock(const std::string& code)
{
    clear();

    try
    {
        // Split by semicolons and newlines
        std::vector<std::string> statements;
        std::stringstream ss(code);
        std::string line;

        while (std::getline(ss, line))
        {
            // Split line by semicolons
            size_t pos = 0;
            while (pos < line.length())
            {
                size_t semicolon = line.find(';', pos);
                if (semicolon == std::string::npos)
                    semicolon = line.length();

                std::string stmt = line.substr(pos, semicolon - pos);

                // Trim whitespace
                size_t start = stmt.find_first_not_of(" \t\r\n");
                size_t end = stmt.find_last_not_of(" \t\r\n");
                if (start != std::string::npos && end != std::string::npos)
                {
                    stmt = stmt.substr(start, end - start + 1);
                    if (!stmt.empty())
                        statements.push_back(stmt);
                }

                pos = semicolon + 1;
            }
        }

        // Compile each statement
        for (const auto& stmt : statements)
        {
            tokens = tokenize(stmt);
            currentToken = 0;
            parseStatement();
        }

        emit(MilkDrop::OpCode::Halt);
        return true;
    }
    catch (const std::exception& e)
    {
        lastError = std::string("Compilation error: ") + e.what();
        return false;
    }
}

// Parser helper methods
bool MilkdropEval::match(MilkDrop::TokenType type)
{
    if (check(type))
    {
        advance();
        return true;
    }
    return false;
}

bool MilkdropEval::check(MilkDrop::TokenType type)
{
    if (isAtEnd()) return false;
    return peek().type == type;
}

MilkDrop::Token MilkdropEval::advance()
{
    if (!isAtEnd()) currentToken++;
    return previous();
}

MilkDrop::Token MilkdropEval::peek()
{
    return tokens[currentToken];
}

MilkDrop::Token MilkdropEval::previous()
{
    return tokens[currentToken - 1];
}

bool MilkdropEval::isAtEnd()
{
    return peek().type == MilkDrop::TokenType::End;
}

void MilkdropEval::emit(MilkDrop::OpCode opcode)
{
    compiled.bytecode.push_back(MilkDrop::Instruction(opcode));
}

void MilkdropEval::emit(MilkDrop::OpCode opcode, double operand)
{
    compiled.bytecode.push_back(MilkDrop::Instruction(opcode, operand));
}

void MilkdropEval::emit(MilkDrop::OpCode opcode, int varIndex)
{
    compiled.bytecode.push_back(MilkDrop::Instruction(opcode, varIndex));
}

// Parser methods (continued in next comment due to length...)
void MilkdropEval::parseStatement()
{
    parseAssignment();
}

void MilkdropEval::parseAssignment()
{
    // Check if this is an assignment
    if (check(MilkDrop::TokenType::Identifier))
    {
        MilkDrop::Token ident = peek();
        advance();

        if (match(MilkDrop::TokenType::Assign))
        {
            // Assignment: var = expr
            parseExpression();

            int varIdx = compiled.addVariable(ident.text);
            emit(MilkDrop::OpCode::Store, varIdx);
            return;
        }
        else
        {
            // Not assignment, backtrack
            currentToken--;
        }
    }

    parseExpression();
}

void MilkdropEval::parseExpression()
{
    parseLogicalOr();
}

void MilkdropEval::parseLogicalOr()
{
    parseLogicalAnd();

    while (match(MilkDrop::TokenType::LogicalOr))
    {
        parseLogicalAnd();
        emit(MilkDrop::OpCode::Or);
    }
}

void MilkdropEval::parseLogicalAnd()
{
    parseComparison();

    while (match(MilkDrop::TokenType::LogicalAnd))
    {
        parseComparison();
        emit(MilkDrop::OpCode::And);
    }
}

void MilkdropEval::parseComparison()
{
    parseTerm();

    if (match(MilkDrop::TokenType::Equal))
    {
        parseTerm();
        emit(MilkDrop::OpCode::CmpEqual);
    }
    else if (match(MilkDrop::TokenType::NotEqual))
    {
        parseTerm();
        emit(MilkDrop::OpCode::CmpNotEqual);
    }
    else if (match(MilkDrop::TokenType::Less))
    {
        parseTerm();
        emit(MilkDrop::OpCode::CmpLess);
    }
    else if (match(MilkDrop::TokenType::Greater))
    {
        parseTerm();
        emit(MilkDrop::OpCode::CmpGreater);
    }
    else if (match(MilkDrop::TokenType::LessEqual))
    {
        parseTerm();
        emit(MilkDrop::OpCode::CmpLessEqual);
    }
    else if (match(MilkDrop::TokenType::GreaterEqual))
    {
        parseTerm();
        emit(MilkDrop::OpCode::CmpGreaterEqual);
    }
}

void MilkdropEval::parseTerm()
{
    parseFactor();

    while (true)
    {
        if (match(MilkDrop::TokenType::Plus))
        {
            parseFactor();
            emit(MilkDrop::OpCode::Add);
        }
        else if (match(MilkDrop::TokenType::Minus))
        {
            parseFactor();
            emit(MilkDrop::OpCode::Subtract);
        }
        else
        {
            break;
        }
    }
}

void MilkdropEval::parseFactor()
{
    parsePower();

    while (true)
    {
        if (match(MilkDrop::TokenType::Multiply))
        {
            parsePower();
            emit(MilkDrop::OpCode::Multiply);
        }
        else if (match(MilkDrop::TokenType::Divide))
        {
            parsePower();
            emit(MilkDrop::OpCode::Divide);
        }
        else if (match(MilkDrop::TokenType::Modulo))
        {
            parsePower();
            emit(MilkDrop::OpCode::Modulo);
        }
        else
        {
            break;
        }
    }
}

void MilkdropEval::parsePower()
{
    parseUnary();
}

void MilkdropEval::parseUnary()
{
    if (match(MilkDrop::TokenType::Minus))
    {
        parseUnary();
        emit(MilkDrop::OpCode::Negate);
    }
    else if (match(MilkDrop::TokenType::Plus))
    {
        parseUnary();
    }
    else
    {
        parsePrimary();
    }
}

void MilkdropEval::parsePrimary()
{
    // Number literal
    if (match(MilkDrop::TokenType::Number))
    {
        emit(MilkDrop::OpCode::Push, previous().value);
        return;
    }

    // Identifier (variable or function)
    if (match(MilkDrop::TokenType::Identifier))
    {
        std::string name = previous().text;

        // Check if it's a function call
        if (match(MilkDrop::TokenType::LeftParen))
        {
            currentToken--; // Backtrack
            currentToken--; // Backtrack identifier too
            advance(); // Re-consume identifier
            parseFunctionCall(name);
            return;
        }

        // It's a variable
        int varIdx = compiled.addVariable(name);
        emit(MilkDrop::OpCode::Load, varIdx);
        return;
    }

    // Parenthesized expression
    if (match(MilkDrop::TokenType::LeftParen))
    {
        parseExpression();
        if (!match(MilkDrop::TokenType::RightParen))
        {
            throw std::runtime_error("Expected ')' after expression");
        }
        return;
    }

    throw std::runtime_error("Expected expression");
}

void MilkdropEval::parseFunctionCall(const std::string& funcName)
{
    if (!match(MilkDrop::TokenType::LeftParen))
    {
        throw std::runtime_error("Expected '(' after function name");
    }

    int argc = 0;

    // Parse arguments
    if (!check(MilkDrop::TokenType::RightParen))
    {
        do
        {
            parseExpression();
            argc++;
        } while (match(MilkDrop::TokenType::Comma));
    }

    if (!match(MilkDrop::TokenType::RightParen))
    {
        throw std::runtime_error("Expected ')' after function arguments");
    }

    // Emit function call opcode
    if (funcName == "sin") emit(MilkDrop::OpCode::Sin);
    else if (funcName == "cos") emit(MilkDrop::OpCode::Cos);
    else if (funcName == "tan") emit(MilkDrop::OpCode::Tan);
    else if (funcName == "asin") emit(MilkDrop::OpCode::ASin);
    else if (funcName == "acos") emit(MilkDrop::OpCode::ACos);
    else if (funcName == "atan") emit(MilkDrop::OpCode::ATan);
    else if (funcName == "atan2") emit(MilkDrop::OpCode::ATan2);
    else if (funcName == "sqrt") emit(MilkDrop::OpCode::Sqrt);
    else if (funcName == "abs") emit(MilkDrop::OpCode::Abs);
    else if (funcName == "sqr") emit(MilkDrop::OpCode::Sqr);
    else if (funcName == "pow") emit(MilkDrop::OpCode::Pow);
    else if (funcName == "exp") emit(MilkDrop::OpCode::Exp);
    else if (funcName == "log") emit(MilkDrop::OpCode::Log);
    else if (funcName == "log10") emit(MilkDrop::OpCode::Log10);
    else if (funcName == "min") emit(MilkDrop::OpCode::Min);
    else if (funcName == "max") emit(MilkDrop::OpCode::Max);
    else if (funcName == "sign") emit(MilkDrop::OpCode::Sign);
    else if (funcName == "rand") emit(MilkDrop::OpCode::Rand);
    else if (funcName == "if") emit(MilkDrop::OpCode::If);
    else if (funcName == "equal") emit(MilkDrop::OpCode::Equal);
    else if (funcName == "above") emit(MilkDrop::OpCode::Above);
    else if (funcName == "below") emit(MilkDrop::OpCode::Below);
    else
    {
        throw std::runtime_error("Unknown function: " + funcName);
    }
}

// ============================================================================
// VIRTUAL MACHINE
// ============================================================================

double MilkdropEval::execute(MilkDrop::ExecutionContext& context)
{
    return executeVM(compiled.bytecode, context);
}

double MilkdropEval::executeVM(const std::vector<MilkDrop::Instruction>& bytecode,
                               MilkDrop::ExecutionContext& context)
{
    stack.clear();
    size_t pc = 0; // Program counter

    while (pc < bytecode.size())
    {
        const auto& instr = bytecode[pc++];

        switch (instr.opcode)
        {
            case MilkDrop::OpCode::Push:
                push(instr.operand);
                break;

            case MilkDrop::OpCode::Load:
            {
                std::string varName = compiled.variableNames[instr.varIndex];
                push(context.getVariable(varName));
                break;
            }

            case MilkDrop::OpCode::Store:
            {
                double value = pop();
                std::string varName = compiled.variableNames[instr.varIndex];
                context.setVariable(varName, value);
                push(value); // Push result back for chained assignments
                break;
            }

            case MilkDrop::OpCode::Add:
            {
                double b = pop();
                double a = pop();
                push(a + b);
                break;
            }

            case MilkDrop::OpCode::Subtract:
            {
                double b = pop();
                double a = pop();
                push(a - b);
                break;
            }

            case MilkDrop::OpCode::Multiply:
            {
                double b = pop();
                double a = pop();
                push(a * b);
                break;
            }

            case MilkDrop::OpCode::Divide:
            {
                double b = pop();
                double a = pop();
                push(b != 0.0 ? a / b : 0.0);
                break;
            }

            case MilkDrop::OpCode::Modulo:
            {
                double b = pop();
                double a = pop();
                push(b != 0.0 ? std::fmod(a, b) : 0.0);
                break;
            }

            case MilkDrop::OpCode::Negate:
                push(-pop());
                break;

            // Math functions
            case MilkDrop::OpCode::Sin:
                push(std::sin(pop()));
                break;

            case MilkDrop::OpCode::Cos:
                push(std::cos(pop()));
                break;

            case MilkDrop::OpCode::Tan:
                push(std::tan(pop()));
                break;

            case MilkDrop::OpCode::ASin:
                push(std::asin(pop()));
                break;

            case MilkDrop::OpCode::ACos:
                push(std::acos(pop()));
                break;

            case MilkDrop::OpCode::ATan:
                push(std::atan(pop()));
                break;

            case MilkDrop::OpCode::ATan2:
            {
                double x = pop();
                double y = pop();
                push(std::atan2(y, x));
                break;
            }

            case MilkDrop::OpCode::Sqrt:
                push(std::sqrt(std::abs(pop())));
                break;

            case MilkDrop::OpCode::Abs:
                push(std::abs(pop()));
                break;

            case MilkDrop::OpCode::Sqr:
            {
                double v = pop();
                push(v * v);
                break;
            }

            case MilkDrop::OpCode::Pow:
            {
                double exp = pop();
                double base = pop();
                push(std::pow(base, exp));
                break;
            }

            case MilkDrop::OpCode::Exp:
                push(std::exp(pop()));
                break;

            case MilkDrop::OpCode::Log:
                push(std::log(std::abs(pop())));
                break;

            case MilkDrop::OpCode::Log10:
                push(std::log10(std::abs(pop())));
                break;

            case MilkDrop::OpCode::Min:
            {
                double b = pop();
                double a = pop();
                push(std::min(a, b));
                break;
            }

            case MilkDrop::OpCode::Max:
            {
                double b = pop();
                double a = pop();
                push(std::max(a, b));
                break;
            }

            case MilkDrop::OpCode::Sign:
            {
                double v = pop();
                push(v > 0.0 ? 1.0 : (v < 0.0 ? -1.0 : 0.0));
                break;
            }

            case MilkDrop::OpCode::Rand:
            {
                double max = pop();
                push(((double)std::rand() / RAND_MAX) * max);
                break;
            }

            case MilkDrop::OpCode::If:
            {
                double falseVal = pop();
                double trueVal = pop();
                double condition = pop();
                push(condition != 0.0 ? trueVal : falseVal);
                break;
            }

            case MilkDrop::OpCode::Equal:
            {
                double b = pop();
                double a = pop();
                push(a == b ? 1.0 : 0.0);
                break;
            }

            case MilkDrop::OpCode::Above:
            {
                double threshold = pop();
                double value = pop();
                push(value > threshold ? 1.0 : 0.0);
                break;
            }

            case MilkDrop::OpCode::Below:
            {
                double threshold = pop();
                double value = pop();
                push(value < threshold ? 1.0 : 0.0);
                break;
            }

            // Comparisons
            case MilkDrop::OpCode::CmpEqual:
            {
                double b = pop();
                double a = pop();
                push(a == b ? 1.0 : 0.0);
                break;
            }

            case MilkDrop::OpCode::CmpNotEqual:
            {
                double b = pop();
                double a = pop();
                push(a != b ? 1.0 : 0.0);
                break;
            }

            case MilkDrop::OpCode::CmpLess:
            {
                double b = pop();
                double a = pop();
                push(a < b ? 1.0 : 0.0);
                break;
            }

            case MilkDrop::OpCode::CmpGreater:
            {
                double b = pop();
                double a = pop();
                push(a > b ? 1.0 : 0.0);
                break;
            }

            case MilkDrop::OpCode::CmpLessEqual:
            {
                double b = pop();
                double a = pop();
                push(a <= b ? 1.0 : 0.0);
                break;
            }

            case MilkDrop::OpCode::CmpGreaterEqual:
            {
                double b = pop();
                double a = pop();
                push(a >= b ? 1.0 : 0.0);
                break;
            }

            // Logical
            case MilkDrop::OpCode::And:
            {
                double b = pop();
                double a = pop();
                push((a != 0.0 && b != 0.0) ? 1.0 : 0.0);
                break;
            }

            case MilkDrop::OpCode::Or:
            {
                double b = pop();
                double a = pop();
                push((a != 0.0 || b != 0.0) ? 1.0 : 0.0);
                break;
            }

            case MilkDrop::OpCode::Halt:
                return stack.empty() ? 0.0 : stack.back();

            default:
                throw std::runtime_error("Unknown opcode");
        }
    }

    return stack.empty() ? 0.0 : stack.back();
}

void MilkdropEval::push(double value)
{
    stack.push_back(value);
}

double MilkdropEval::pop()
{
    if (stack.empty())
        throw std::runtime_error("Stack underflow");

    double value = stack.back();
    stack.pop_back();
    return value;
}
