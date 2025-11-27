#include <string>
#include <cmath>
#include <deque>
#include <unordered_map>
#include <QDebug>
#include "symbol.h"

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
        if(std::isdigit(c) || c == '.'){
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

