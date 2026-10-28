#include <QtTest>
#include <QLineEdit>
#include <QPushButton>
#include <QDir>
#include "calculatorengine.h"
#include "calculatorwindow.h"

class CalculatorTest : public QObject {
    Q_OBJECT
private slots:
    void arithmetic_data() {
        QTest::addColumn<QString>("keys"); QTest::addColumn<QString>("expected");
        QTest::newRow("decimal") << "12.5+7.25=" << "19.75";
        QTest::newRow("subtract") << "8-13=" << "-5";
        QTest::newRow("multiply") << "2.5*4=" << "10";
        QTest::newRow("divide") << "9/4=" << "2.25";
        QTest::newRow("duplicate-dot") << "1..2+3=" << "4.2";
        QTest::newRow("operator-replacement") << "8+*2=" << "16";
        QTest::newRow("divide-zero") << "8/0=" << "不能除以零";
        QTest::newRow("recover-error") << "8/0=2+3=" << "5";
        QTest::newRow("new-after-result") << "2+3=7" << "7";
        QTest::newRow("continue-result") << "2+3=*4=" << "20";
        QTest::newRow("sequential") << "2+3*4=" << "20";
        QTest::newRow("leading-dot") << ".5+.5=" << "1";
        QTest::newRow("equals-pending") << "8+=" << "8";
        QTest::newRow("small-divisor") << "1/0.0000001=" << "10000000";
    }
    void arithmetic() {
        QFETCH(QString, keys); QFETCH(QString, expected);
        CalculatorEngine engine;
        for (QChar c : keys) engine.input(QString(c));
        QCOMPARE(engine.display(), expected);
    }
    void backspaceAndClear() {
        CalculatorEngine e;
        for (QString t : {"1","2",".","3","BS"}) e.input(t);
        QCOMPARE(e.display(), QString("12."));
        e.input("4"); e.input("+"); e.input("5"); e.input("BS");
        QCOMPARE(e.display(), QString("0"));
        e.input("C"); e.input("9"); e.input("-"); e.input("2"); e.input("=");
        QCOMPARE(e.display(), QString("7"));
    }
    void negativeFraction() {
        CalculatorEngine e;
        for (QString t : {"SIGN", ".", "5", "+", "1", "="}) e.input(t);
        QCOMPARE(e.display(), QString("0.5"));
    }
    void keyboardMouseAndScreenshots() {
        CalculatorWindow w; w.show(); QTest::qWait(80);
        auto click = [&](const char *name) { QTest::mouseClick(w.findChild<QPushButton *>(name), Qt::LeftButton); };
        click("digit1Button"); click("digit2Button"); click("decimalButton"); click("digit5Button"); click("plusButton");
        auto *display = w.findChild<QLineEdit *>("displayEdit"); display->setFocus();
        QTest::keyClicks(display, "7.25"); QTest::keyClick(display, Qt::Key_Return);
        QCOMPARE(w.value(), QString("19.75"));
        const QString path = qEnvironmentVariable("EVIDENCE_DIR");
        if (!path.isEmpty()) { QDir().mkpath(path); w.grab().save(path + "/calculator-decimal.png"); }
        QTest::keyClick(display, Qt::Key_Escape); QTest::keyClicks(display, "8/0"); QTest::keyClick(display, Qt::Key_Return);
        QCOMPARE(w.value(), QString("不能除以零"));
        if (!path.isEmpty()) w.grab().save(path + "/calculator-zero.png");
        QTest::keyClicks(display, "12"); QTest::keyClick(display, Qt::Key_Backspace);
        QCOMPARE(w.value(), QString("1"));
        QTest::keyClick(display, Qt::Key_Escape); QTest::keyClicks(display, "2+3*4"); QTest::keyClick(display, Qt::Key_Return);
        QCOMPARE(w.value(), QString("20"));
        if (!path.isEmpty()) w.grab().save(path + "/calculator-chain.png");
    }
};
QTEST_MAIN(CalculatorTest)
#include "test_calculator.moc"
