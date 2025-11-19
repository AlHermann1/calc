#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QKeyEvent>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void keyPressEvent(QKeyEvent* event) override;

private slots:
    void on_btnAdd_clicked();

    void on_btnSubtract_clicked();

    void on_btnMultiply_clicked();

    void on_btnDivide_clicked();

    void on_btnEquals_clicked();

    void on_btn0_clicked();

    void on_btn1_clicked();

    void on_btn2_clicked();

    void on_btn3_clicked();

    void on_btn4_clicked();

    void on_btn5_clicked();

    void on_btn6_clicked();

    void on_btn7_clicked();

    void on_btn8_clicked();

    void on_btn9_clicked();

    void on_btnExp_clicked();

    void on_btnAC_clicked();

    void on_btnCE_clicked();

    void on_btnPeriod_clicked();

    void on_btnTheme_clicked();

private:
    Ui::MainWindow *ui;

    void loadStyles();

    void darkMode(bool enabled);

    void updateText();

    void displayAnswer(std::string ans);
};
#endif // MAINWINDOW_H
