#include "calculatorengine.h"
#include <cmath>

QString CalculatorEngine::format(double value) {
    return QString::number(value == 0 ? 0 : value, 'g', 15);
}
void CalculatorEngine::clear() {
    entry = "0"; history.clear(); op.clear(); accumulator = 0;
    currentState = State::InputFirst;
}
void CalculatorEngine::fail(const QString &reason) {
    entry = reason; history.clear(); op.clear(); accumulator = 0;
    currentState = State::Error;
}
void CalculatorEngine::digit(const QString &token) {
    if (currentState == State::Error || currentState == State::Result) clear();
    if (currentState == State::OperatorPending) {
        entry = "0";
        currentState = State::InputSecond;
    }
    if (token == ".") {
        if (!entry.contains('.')) entry += '.';
    } else if (entry.size() < 16) {
        if (entry == "0") entry = token;
        else if (entry == "-0") entry = "-" + token;
        else entry += token;
    }
}
bool CalculatorEngine::calculate(double right) {
    // 精确的零判断不会把很小但非零的除数当作零。
    if (op == "/" && right == 0.0) { fail("不能除以零"); return false; }
    if (op == "+") accumulator += right;
    else if (op == "-") accumulator -= right;
    else if (op == "*") accumulator *= right;
    else if (op == "/") accumulator /= right;
    if (!std::isfinite(accumulator)) { fail("结果超出范围"); return false; }
    entry = format(accumulator);
    return true;
}
void CalculatorEngine::operation(const QString &token) {
    if (currentState == State::Error) return;
    if (currentState == State::InputSecond) {
        if (!calculate(entry.toDouble())) return;
    } else if (currentState != State::OperatorPending) accumulator = entry.toDouble();
    // 等待第二操作数时，再次按运算符只替换操作符。
    op = token;
    history = format(accumulator) + " " + op;
    currentState = State::OperatorPending;
}
void CalculatorEngine::input(const QString &token) {
    if (token == "C") { clear(); return; }
    if (token.size() == 1 && (token.at(0).isDigit() || token == ".")) {
        digit(token); return;
    }
    if (token == "+" || token == "-" || token == "*" || token == "/") {
        operation(token); return;
    }
    if (token == "=") {
        if (currentState != State::InputSecond) return;
        const QString previous = format(accumulator) + " " + op + " " + entry + " =";
        if (!calculate(entry.toDouble())) return;
        history = previous; op.clear(); currentState = State::Result;
        return;
    }
    if (token == "BS") {
        if (currentState == State::Error) { clear(); return; }
        if (currentState == State::OperatorPending || currentState == State::Result) return;
        entry.chop(1);
        if (entry.isEmpty() || entry == "-") entry = "0";
    } else if (token == "SIGN" && currentState != State::Error) {
        if (currentState == State::OperatorPending) {
            entry = "0"; currentState = State::InputSecond;
        }
        if (entry.startsWith('-')) entry.remove(0, 1);
        else if (entry != "0") entry.prepend('-');
        if (currentState == State::Result) { accumulator = entry.toDouble(); history.clear(); }
    }
}
