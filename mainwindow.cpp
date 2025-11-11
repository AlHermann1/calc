#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <string>
#include <vector>
#include <cmath>
#include <deque>
#include <unordered_map>
#include <iostream>

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


std::vector<char> operations;
std::vector<int> nums;
std::vector<int> current_num;
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
    std::unordered_map<char,int> operations;
    operations['/'] = 1;
    operations['*'] = 1;
    operations['+'] = 0;
    operations['-'] = 0;
    operations['^'] = 2;


    std::deque<sSymbol> st;
    std::deque<sSymbol> output;
    for(const char& c: expr){
        //If it is digit, push it to output
        if(std::isdigit(c)){
            output.push_back({std::string(1,c), sSymbol::Type::Literal_Numeric});
        }
        //If it is operator, find the precedence
        else if(operations.count(c)){
            const uint8_t nPrec = operations[c];

            while(!st.empty()){
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
            st.push_front({std::string(1,c), sSymbol::Type::Operator, {nPrec,2}});
        }
        else{
            std::cout << "Invalid character" << std::string(1,c) << ", please try again.\n";
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
                else if(s.symbol[0] == '+') result = mem[1] * mem[0];
                else if(s.symbol[0] == '-') result = mem[1] - mem[0];
            }
            stackSolve.push_front(result);
        }
        break;
        }
    }
    return stackSolve.front();
}


void newnum(){
    int res = 0;
    for(int i=0;i<current_num.size();i++){
        res += current_num[i] * pow(10,i);
    }
    nums.push_back(res);
    current_num = {};
}

void MainWindow::keyPressEvent(QKeyEvent* event){
    if(event->key()>=Qt::Key_0 && event->key() <=Qt::Key_9) {
        int num = event->key() - Qt::Key_0;
        current_num.push_back(num);
    }
    switch(event->key()){
    case Qt::Key_Plus:
        operations.push_back('+');
        newnum();
    case Qt::Key_Minus:
        operations.push_back('-');
        newnum();
    case Qt::Key_Asterisk:
        operations.push_back('*');
        newnum();
    case Qt::Key_Slash:
        operations.push_back('/');
        newnum();
    case Qt::Key_Enter:
        eval(operations,nums);
        newnum();
    }

}

void MainWindow::on_btnAdd_clicked()
{
    operations.push_back('+');
    newnum();
}

void MainWindow::on_btnSubtract_clicked()
{
    operations.push_back('-'); //also want to  add the symbol to the caluclator window
    newnum();

}


void MainWindow::on_btnMultiply_clicked()
{
    operations.push_back('*');
    newnum();

}


void MainWindow::on_btnDivide_clicked()
{
    operations.push_back('/');
    newnum();

}



void MainWindow::on_btnEquals_clicked()
{
    operations.push_back('=');
    newnum();
}

