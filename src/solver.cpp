#include <vector>
#include <deque>
#include "symbol.h"
#include <cmath>
#include <QDebug>

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
                    qDebug() << mem[j] << '\n';
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

