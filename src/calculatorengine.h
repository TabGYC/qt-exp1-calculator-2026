#pragma once
#include <QString>

// GUI 与键盘共用的即时计算状态机。
class CalculatorEngine {
public:
    enum class State { InputFirst, OperatorPending, InputSecond, Result, Error };
    void input(const QString &token);
    QString display() const { return entry; }
    QString expression() const { return history; }
    State state() const { return currentState; }
    void clear();
private:
    QString entry = "0", history, op;
    double accumulator = 0;
    State currentState = State::InputFirst;
    void digit(const QString &token);
    void operation(const QString &token);
    bool calculate(double right);
    void fail(const QString &reason);
    static QString format(double value);
};
