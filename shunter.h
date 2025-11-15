#ifndef SHUNTER_H
#define SHUNTER_H

#include<string>
#include <symbol.h>
#include <deque>

std::deque<sSymbol> shunt(std::string expr);

#endif // SHUNTER_H
