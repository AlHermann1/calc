#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <string>
#include <vector>
#include <cmath>
#include <deque>
#include <unordered_map>
#include <QDebug>

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
struct sOperator{
    uint8_t precedence;
    uint8_t arguments;
    bool rightAssociative;
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
    sOperator op = {0,0,false};
};


std::deque<sSymbol> shunt(std::string expr){ // i think should replace this with shunting yard algorithm for eval
    std::unordered_map<char,sOperator> operations;
    std::string n = "";
    operations['/'] = {1,2,false};
    operations['*'] = {1,2,false};
    operations['+'] = {0,2,false};
    operations['-'] = {0,2,false};
    operations['^'] = {3,2,true};
    qDebug() << expr << '\n';

    std::deque<sSymbol> error; //to return for errors
    error.push_back({"x",sSymbol::Type::Literal_Numeric,{0,0,false}});

    std::deque<sSymbol> st;
    std::deque<sSymbol> output;

    sSymbol prev = {"0", sSymbol::Type::Unknown};
    for(size_t p=0;p<expr.size();p++){
        char& c = expr[p];
        qDebug() << c << '\n';
        //If it is digit, add it to num string
        if(std::isdigit(c)){
            n+=c;
            if(p >= expr.size()-1){//If the digit is the last character,
                output.push_back({n, sSymbol::Type::Literal_Numeric});
            }
        }
        else {
            if(n!=""){ //if next char isnt digit, push num to stack (if its not empty) and reset it
                output.push_back({n, sSymbol::Type::Literal_Numeric});
                n = "";
                prev = output.back();
            }
            if(c == '('){

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
                        //(c == '+') continue;
                        nOp.precedence = 2;
                        nOp.arguments = 1;
                        nOp.rightAssociative = true;
                    }
                }

                while(!st.empty() && st.front().type != sSymbol::Type::pOpen){
                    //Iterate over operators in holding stack
                    if(st.front().type == sSymbol::Type::Operator){
                        const uint8_t hPrec = st.front().op.precedence;
                        //If its precedence is greater or equal to the new operator's, then push it to output and pop it from holding stack
                        if(hPrec > nPrec || (hPrec == nPrec && !nOp.rightAssociative)){
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
                    qDebug() << "!!!ERROR!!! Bad Expression\n";
                    return NAN;
                }
                else{
                    mem[j] = stackSolve.front();
                    stackSolve.pop_front();
                }
            }

            double result = 0.0;
            if(s.op.arguments==2){
                if(s.symbol[0] == '/') result = mem[1] / mem[0];
                else if(s.symbol[0] == '*') result = mem[1] * mem[0];
                else if(s.symbol[0] == '+') result = mem[1] + mem[0];
                else if(s.symbol[0] == '-') result = mem[1] - mem[0];
                else if(s.symbol[0] == '^')   result = std::pow(mem[1], mem[0]);
                else{
                    qDebug() << "ERROR OPERATOR NOT FOUND\n";
                }
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
        expression.pop_back();
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


void MainWindow::on_btnExp_clicked()
{
    expression += '^';
    updateText();
}


void MainWindow::on_btnEquals_clicked()
{
    qDebug() << expression << '\n';
    std::deque<sSymbol> rpn = shunt(expression);
    if(rpn.empty() || rpn.front().symbol == "x"){
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

