#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <string>
#include <vector>
#include <cmath>
#include <deque>
#include <unordered_map>
#include <iostream>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

std::string expression = "";
std::vector<std::string> split(std::string s, char c){
    std::vector<std::string> vec = {};
    std::string curr = "";
    for(const char ch : s){
        if(ch!=c){
            curr+=ch;
        }
        else{
            vec.push_back(curr);
                curr = "";
        }
    }
    vec.push_back(curr);
    return vec;

}
struct sOperator{
    uint8_t precedence;
    uint8_t arguments;
};

struct sSymbol
{
    std::string symbol = "";
    enum class Type : uint8_t
    {
        Unknown,
        Literal_Numeric,
        Operator,
        pOpen,
        pClosed,
    }
    type = Type::Unknown;
    sOperator op = {0,2};
};


std::deque<sSymbol> shunt(std::string expr){ // i think should replace this with shunting yard algorithm for eval
    std::unordered_map<char,sOperator> operations;
    operations['/'] = {1,2};
    operations['*'] = {1,2};
    operations['+'] = {0,2};
    operations['-'] = {0,2};
    //operations['^'] = {2,2};  do exponents at some point
    qDebug() << expr << '\n';

    std::deque<sSymbol> error; //to return for errors
    error.push_back({"x",sSymbol::Type::Literal_Numeric,{0,0}});

    std::deque<sSymbol> st;
    std::deque<sSymbol> output;

    sSymbol prev = {"0", sSymbol::Type::Literal_Numeric};
    for(const char& c: expr){
        qDebug() << c << '\n';
        //If it is digit, push it to output
        if(std::isdigit(c)){
            output.push_back({std::string(1,c), sSymbol::Type::Literal_Numeric});
            prev = output.back();
        }
        else if(c == '('){
            st.push_front({std::string(1,c), sSymbol::Type::pOpen});
            prev = st.front();
        }
        //At closing parenthesis, flush out stack until open parenthesis found
        else if(c == ')'){
            while(!st.empty() && st.front().type != sSymbol::Type::pOpen){
                output.push_back(st.front());
                st.pop_front();
            }
            if(st.empty()){
                qDebug() <<"!!!ERROR    UNEXPECTED PARENTHESIS\n";
                return error;
            }
            //Remove the open parenthesis from top of stack;
            if(!st.empty() && st.front().type == sSymbol::Type::pOpen){
                st.pop_front();
            }
            prev = {")", sSymbol::Type::pClosed};

        }
        //If it is operator, find the precedence
        else if(operations.count(c)){
            sOperator nOp = operations[c];
            uint8_t nPrec = nOp.precedence;

            if(c == '+' || c== '-'){
                if(prev.type != sSymbol::Type::Literal_Numeric && prev.type != sSymbol::Type::pClosed){
                    nOp.precedence = 100;
                    nOp.arguments = 1;
                }
            }

            while(!st.empty() && st.front().type != sSymbol::Type::pOpen){
                //Iterate over operators in holding stack
                if(st.front().type == sSymbol::Type::Operator){
                    const uint8_t hPrec = st.front().op.precedence;
                    //If its precedence is greater or equal to the new operator's, then push it to output and pop it from holding stack
                    if(hPrec >= nPrec){
                        output.push_back(st.front());
                        st.pop_front();
                    }
                    //Otherwise stop iterating (because the first operator has the maximum precedence of the stack)
                    else{
                        break;
                    }
                }
            }
            st.push_front({std::string(1,c), sSymbol::Type::Operator, nOp});
            prev = st.front();

        }
        else{
            qDebug() << "Invalid character" << std::string(1,c) << ", please try again.\n";
            return error;

        }
    } //When all new characters have been added, move operators from holding stack to output
    while(!st.empty()){
        output.push_back(st.front());
        st.pop_front();
    }
    return output;



}

double rpnSolver(std::deque<sSymbol>rpn){
    std::deque<double> stackSolve;

    for (const sSymbol& s : rpn){
        switch(s.type){
        case sSymbol::Type::Literal_Numeric:
        {
            stackSolve.push_front(std::stod(s.symbol)); //If symbol is a number, push it to solving stack
        }
        break;
        case sSymbol::Type::Operator:
        {
            std::vector<double> mem(s.op.arguments);
            for(uint8_t j=0;j<s.op.arguments;j++){
                if(stackSolve.empty()){
                    std::cout << "!!!ERROR!!! Bad Expression\n";
                }
                else{
                    mem[j] = stackSolve[0];
                    stackSolve.pop_front();
                }
            }

            double result = 0.0;
            if(s.op.arguments==2){
                if(s.symbol[0] == '/') result = mem[1] / mem[0];
                else if(s.symbol[0] == '*') result = mem[1] * mem[0];
                else if(s.symbol[0] == '+') result = mem[1] + mem[0];
                else if(s.symbol[0] == '-') result = mem[1] - mem[0];
            }
            else if(s.op.arguments==1){
                if(s.symbol[0] == '+') result = +mem[0];
                else if(s.symbol[0] == '-') result = -mem[0];
            }
            stackSolve.push_front(result);
        }
        break;
        }
    }
    return stackSolve.front();
}

void MainWindow::updateText(){
    ui->resultBox->setText(QString::fromStdString(expression));
}

void MainWindow::keyPressEvent(QKeyEvent* event){
    if(event->key()>=Qt::Key_0 && event->key() <=Qt::Key_9) {
        char num = event->key() - Qt::Key_0 + 48;//char(48) = '0';
        qDebug() << num << '\n';
        expression += num;
        updateText();
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
    case Qt::Key_Return:
        std::deque<sSymbol> rpn = shunt(expression);
        if(rpn.front().symbol == "x"){
            qDebug() << "ERROR IN SHUNT\n";
            return;
        }
        double ans = rpnSolver(rpn);
        ui->resultBox->setText(QString::number(ans));
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



void MainWindow::on_btnEquals_clicked()
{
    qDebug() << expression << '\n';
    std::deque<sSymbol> rpn = shunt(expression);
    if(rpn.front().symbol == "x"){
        qDebug() << "ERROR IN SHUNT\n";
        return;
    }
    double ans = rpnSolver(rpn);
    expression = std::to_string(ans);
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

