#include "calculatorwindow.h"
#include "ui_calculatorwindow.h"
#include <QKeyEvent>
#include <QPushButton>
#include <QApplication>

CalculatorWindow::CalculatorWindow(QWidget *parent)
    : QWidget(parent), ui(new Ui::CalculatorWindow) {
    ui->setupUi(this);
    setMinimumWidth(380);
    for (auto *button : findChildren<QPushButton *>()) {
        QString token = button->text();
        if (token == "÷") token = "/";
        if (token == "×") token = "*";
        if (token == "−") token = "-";
        if (token == "⌫") token = "BS";
        if (token == "±") token = "SIGN";
        connect(button, &QPushButton::clicked, this, [this, token] { dispatch(token); });
    }
    setStyleSheet("QWidget{background:#f4f6f8;color:#182332;font-size:18px;}"
                  "QPushButton{background:white;border:1px solid #d5dce5;border-radius:7px;}"
                  "QPushButton:hover{background:#e5edf6;} QPushButton:pressed{background:#cbdcf0;}"
                  "QLineEdit{background:white;border:1px solid #d5dce5;border-radius:7px;"
                  "font-size:32px;padding:10px;} QLabel{font-size:14px;color:#5a6779;}"
                  "QPushButton#equalsButton{background:#2164b5;color:white;}"
                  "QPushButton#clearButton{color:#b62929;}");
    // 只在本窗口及其子控件内截获键盘事件，避免编辑框焦点吞掉按键。
    qApp->installEventFilter(this);
}
CalculatorWindow::~CalculatorWindow() { qApp->removeEventFilter(this); delete ui; }
void CalculatorWindow::dispatch(const QString &token) {
    engine.input(token);
    ui->displayEdit->setText(engine.display());
    ui->expressionLabel->setText(engine.expression().isEmpty() ? "即时四则运算" : engine.expression());
}
bool CalculatorWindow::eventFilter(QObject *watched, QEvent *event) {
    auto *widget = qobject_cast<QWidget *>(watched);
    if (event->type() != QEvent::KeyPress || !widget ||
        !(widget == this || isAncestorOf(widget))) return QWidget::eventFilter(watched, event);
    auto *key = static_cast<QKeyEvent *>(event);
    if (key->modifiers() & (Qt::ControlModifier | Qt::AltModifier | Qt::MetaModifier)) return false;
    QString token = key->text();
    if (key->key() == Qt::Key_Return || key->key() == Qt::Key_Enter || key->key() == Qt::Key_Equal) token = "=";
    else if (key->key() == Qt::Key_Backspace) token = "BS";
    else if (key->key() == Qt::Key_Escape || key->key() == Qt::Key_Delete || token.toUpper() == "C") token = "C";
    else if (!(token.size() == 1 && (token.at(0).isDigit() || QString(".+-*/").contains(token)))) return false;
    dispatch(token); return true;
}
