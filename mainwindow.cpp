#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <string>
#include <vector>
#include <cmath>
#include

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

double eval(std::vector<char> ops,std::vector<int> n){ // i think should replace this with shunting yard algorithm for eval
    double ans = n[0]; //init ans to first number
    for(int i=1;i<ops.size();i++){
        char c = ops[i-1];
        char c2 = ops[i];
        if(c == '*'){
            ans *= n[1];
        }
        else if(c == '/'){
            ans /= n[1];
        }
        else if(c2 == '*' || c2 == '/'){
            if(c == '+'){
                std::vector<char> k;
                for(int o=i;i<ops.size();o++){
                    k.push_back(ops[o]);
                }
                std::vector<int> p;
                for(int o=i;i<n.size();o++){
                    p.push_back(n[o]);
                }

                ans+= eval(k,p);
            }
        }
        else if(c == '+'){
            ans+=n[i];
        }
        else{
            ans-=n[i];
        }
    }
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

