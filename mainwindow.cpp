#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <string>
#include <cmath>
#include <deque>
#include <QDebug>
#include <symbol.h>
#include <shunter.h>
#include <solver.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->resultBox->setFocusPolicy(Qt::NoFocus);
}

MainWindow::~MainWindow()
{
    delete ui;
}

std::string expression = "";
void clear_last(std::string& e){
    if(!e.empty()){
        e.pop_back();
    }
    return;
}



void MainWindow::updateText(){
    ui->resultBox->setText(QString::fromStdString(expression));
}

void MainWindow::displayAnswer(std::string ans){
    ui->resultBox->setText(QString::fromStdString(ans));

}

void MainWindow::keyPressEvent(QKeyEvent* event){
    if(event->key()>=Qt::Key_0 && event->key() <=Qt::Key_9) {
        char num = event->key() - Qt::Key_0 + 48;//char(48) = '0';
        qDebug() << num << '\n';
        expression += num;
        updateText();
        return;
    }
    switch(event->key()){
    case Qt::Key_Plus:
        expression += '+';
        updateText();
        break;
    case Qt::Key_Minus:
        expression += '-';
        updateText();
        break;
    case Qt::Key_Asterisk:
        expression += '*';
        updateText();
        break;
    case Qt::Key_Slash:
        expression += '/';
        updateText();
        break;
    case Qt::Key_AsciiCircum:
        expression += '^';
        updateText();
        break;
    case Qt::Key_Backspace:
        clear_last(expression);
        updateText();
        break;
    case Qt::Key_Period:
        expression += '.';
        updateText();
        break;
    case Qt::Key_Return:
        std::deque<sSymbol> rpn = shunt(expression);
        if(rpn.front().symbol == "x"){
            qDebug() << "ERROR IN SHUNT\n";
            return;
        }
        double ans = rpnSolver(rpn);
        expression = std::to_string(ans);
        updateText();
        break;
    }

}

void MainWindow::on_btnAdd_clicked()
{
    expression += '+';
    updateText();
}

void MainWindow::on_btnSubtract_clicked()
{
    expression += '-';
    updateText();
}


void MainWindow::on_btnMultiply_clicked()
{
    expression += '*';
    updateText();
}


void MainWindow::on_btnDivide_clicked()
{
    expression += '/';
    updateText();
}


void MainWindow::on_btnExp_clicked()
{
    expression += '^';
    updateText();
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
    qDebug() << "ans: " << ans << '\n';
    qDebug() << expression << '\n';
    updateText();
}


void MainWindow::on_btn0_clicked()
{
    expression += '0';
    updateText();

}
void MainWindow::on_btn1_clicked()
{
    expression += '1';
    updateText();
}
void MainWindow::on_btn2_clicked()
{
    expression += '2';
    updateText();
}
void MainWindow::on_btn3_clicked()
{
    expression += '3';
    updateText();
}
void MainWindow::on_btn4_clicked()
{
    expression += '4';
    updateText();
}
void MainWindow::on_btn5_clicked()
{
    expression += '5';
    updateText();
}
void MainWindow::on_btn6_clicked()
{
    expression += '6';
    updateText();
}
void MainWindow::on_btn7_clicked()
{
    expression += '7';
    updateText();
}
void MainWindow::on_btn8_clicked()
{
    expression += '8';
    updateText();
}
void MainWindow::on_btn9_clicked()
{
    expression += '9';
    updateText();
}


void MainWindow::on_btnAC_clicked()
{
    expression.clear();
    updateText();
}


void MainWindow::on_btnCE_clicked()
{
    clear_last(expression);
    updateText();
}


void MainWindow::on_btnPeriod_clicked()
{
    expression += '.';
    updateText();
}

