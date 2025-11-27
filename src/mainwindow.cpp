#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <string>
#include <cmath>
#include <deque>
#include <QDebug>
#include "symbol.h"
#include "shunter.h"
#include "solver.h"
#include <QFile>

QString dark;
QString light;
void MainWindow::loadStyles(){
    QFile dark_file("dark.qss");
    QFile light_file("light.qss");
    bool d = dark_file.open(QFile::ReadOnly);
    bool l = light_file.open(QFile::ReadOnly);
    if(!d){
        qDebug() << "darkfile failed to open\n";
    }
    if(!l){
        qDebug() << "lightfile failed to open\n";
    }

    light = QLatin1String(light_file.readAll());
    dark = QLatin1String(dark_file.readAll());

}

void MainWindow::darkMode(bool enabled){
    if(enabled){
        qApp->setStyleSheet(dark);
    }
    else{
        qApp->setStyleSheet(light);
    }
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->resultBox->setFocusPolicy(Qt::NoFocus);
    loadStyles();
    darkMode(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}


std::string expression = "";
bool theme = false; //false=light, true=dark
void clear_last(std::string& e){
    if(!e.empty()){
        e.pop_back();
    }
    return;
}
void remove_zeroes(std::string& s){
    size_t last_nonzero = s.find_last_not_of('0');
    if(last_nonzero != std::string::npos) {
        s.erase(last_nonzero +1);
    }
    else{
        s = "0.0";
    }
    if(s.back()=='.'){
        s += '0';
    }
}



void MainWindow::updateText(std::string s){
    if(expression == "" || expression=="nan"|| expression == "undefined"){
        expression = s;
    }
    else{
        expression += s;
    }
    ui->resultBox->setText(QString::fromStdString(expression));
}


void MainWindow::keyPressEvent(QKeyEvent* event){
    if(event->key()>=Qt::Key_0 && event->key() <=Qt::Key_9) {
        char num = event->key() - Qt::Key_0 + 48;//char(48) = '0';
        qDebug() << num << '\n';
        std::string s = "";
        s+=num;
        updateText(s);
        return;
    }
    switch(event->key()){
    case Qt::Key_Plus:
        updateText("+");
        break;
    case Qt::Key_Minus:
        updateText("-");
        break;
    case Qt::Key_Asterisk:
        updateText("*");
        break;
    case Qt::Key_Slash:
        updateText("/");
        break;
    case Qt::Key_AsciiCircum:
        updateText("^");
        break;
    case Qt::Key_Backspace:
        clear_last(expression);
        updateText("");
        break;
    case Qt::Key_Period:
        updateText(".");
        break;
    case Qt::Key_ParenLeft:
        updateText("(");
        break;
    case Qt::Key_ParenRight:
        updateText(")");
        break;
    case Qt::Key_Return:
        std::deque<sSymbol> rpn = shunt(expression);
        if(rpn.front().symbol == "x"){
            qDebug() << "ERROR IN SHUNT\n";
            return;
        }
        double ans = rpnSolver(rpn);
        expression = std::to_string(ans);
        remove_zeroes(expression);
        updateText("");
        break;
    }

}

void MainWindow::on_btnAdd_clicked()
{
    updateText("+");
}

void MainWindow::on_btnSubtract_clicked()
{
    updateText("-");
}


void MainWindow::on_btnMultiply_clicked()
{
    updateText("*");
}


void MainWindow::on_btnDivide_clicked()
{
    updateText("/");
}


void MainWindow::on_btnExp_clicked()
{
    updateText("^");
}


void MainWindow::on_btnEquals_clicked()
{
    std::deque<sSymbol> rpn = shunt(expression);
    if(rpn.empty() || rpn.front().symbol == "x"){
        qDebug() << "ERROR IN SHUNT\n";
        return;
    }
    double ans = rpnSolver(rpn);
    expression = std::to_string(ans);
    if(expression == "inf"){
        expression = "undefined";
    }
    remove_zeroes(expression);
    updateText("");
}


void MainWindow::on_btn0_clicked()
{
    updateText("0");

}
void MainWindow::on_btn1_clicked()
{
    updateText("1");
}
void MainWindow::on_btn2_clicked()
{
    updateText("2");
}
void MainWindow::on_btn3_clicked()
{
    updateText("3");
}
void MainWindow::on_btn4_clicked()
{
    updateText("4");
}
void MainWindow::on_btn5_clicked()
{
    updateText("5");
}
void MainWindow::on_btn6_clicked()
{
    updateText("6");
}
void MainWindow::on_btn7_clicked()
{
    updateText("7");
}
void MainWindow::on_btn8_clicked()
{
    updateText("8");
}
void MainWindow::on_btn9_clicked()
{
    updateText("9");
}


void MainWindow::on_btnAC_clicked()
{
    expression.clear();
    updateText("");
}


void MainWindow::on_btnCE_clicked()
{
    clear_last(expression);
    updateText("");
}


void MainWindow::on_btnPeriod_clicked()
{
    updateText(".");
}


void MainWindow::on_btnTheme_clicked()
{
    theme = !theme;
    darkMode(theme);
}

