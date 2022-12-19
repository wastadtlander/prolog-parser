#ifndef LEXER_HPP
#define LEXER_HPP

#include "Token.hpp"

#include <istream>
#include <list>
#include <string>

namespace vtpl {
    typedef std::list<Token> TokenList;

    TokenList tokenize(std::istream &);
} // namespace vtpl

#endif // LEXER_HPP
