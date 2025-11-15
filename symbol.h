#ifndef SYMBOL_H
#define SYMBOL_H

#include <string>
#include <cstdint>

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

#endif // SYMBOL_H
