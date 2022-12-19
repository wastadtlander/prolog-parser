#include "Lexer.hpp"

#include <cctype>

namespace vtpl {
    TokenList tokenize(std::istream & input) {
        TokenList tl;

        char c;
        std::size_t line = 1;
        std::string str = "";
        
        while (input.get(c)) {
            if (!tl.empty() && tl.back().type() == TokenType::ERROR) {
                return tl;
            }

            if (c == '(') {
                tl.emplace_back(TokenType::OPEN, line);
            } else if (c == ')') {
                tl.emplace_back(TokenType::CLOSE, line);
            } else if (c == ',') {
                tl.emplace_back(TokenType::COMMA, line);
            } else if (c == ':') {
                input.get(c);
                if (c != '-') {
                    tl.emplace_back(TokenType::ERROR, line, "Error: malformed implication");
                } else { 
                    tl.emplace_back(TokenType::IMP, line);
                }
            } else if (c == '.') { 
                tl.emplace_back(TokenType::END, line);
            } else if (isalpha(c) || c == '_') {
                while (isalnum(input.peek()) && input.good()) {
                    str += std::string(1, c);
                    input.get(c);
                } 
                if (isalnum(c) || c == '_') { 
                    str += std::string(1, c);
                }
                tl.emplace_back(TokenType::STRING, line, str);
                str = "";
            } else if (c == '%') {
                while (input.peek() != '\n' && input.good()) {
                    input.get(c);
                }
            } else if (c == '\n') line++;
        }

        return tl;
    }
} // namespace vtpl