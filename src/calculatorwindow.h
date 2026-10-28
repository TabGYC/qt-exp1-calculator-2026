#pragma once
#include <QWidget>
#include "calculatorengine.h"
namespace Ui { class CalculatorWindow; }
class CalculatorWindow : public QWidget {
    Q_OBJECT
public:
    explicit CalculatorWindow(QWidget *parent = nullptr);
    ~CalculatorWindow();
    void dispatch(const QString &token);
    QString value() const { return engine.display(); }
protected:
    bool eventFilter(QObject *, QEvent *) override;
private:
    Ui::CalculatorWindow *ui;
    CalculatorEngine engine;
};
