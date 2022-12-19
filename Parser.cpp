#include "Parser.hpp"

namespace vtpl {
    void ParseError::set(const char * str) {
        error = str;
    }

    void ParseError::set(const char * str, const Token token) {
        error = str;
        t = Token(token.type(), token.line(), token.value());
    }

    bool ParseError::isSet() const noexcept {
        if (strcmp(error, "") == 0) {
            return false;
        }
        return true;
    }

    std::string ParseError::message() {
        return error;
    }

    std::tuple<ParseError, KnowledgeBase> parseKnowledgeBase(const TokenList& tokens) {
        std::tuple<ParseError, KnowledgeBase> t;

        TokenList::const_iterator it = tokens.begin();
        TokenList::const_iterator end = tokens.end();

        if (it == end) {
            std::get<0>(t).set("ERROR: Empty KB");
        }

        int parenCount = 0;

        parseKnowledgeBaseHelper(std::get<1>(t), std::get<0>(t), it, end, parenCount);

        if (!std::get<0>(t).isSet() && parenCount != 0) {
            std::get<0>(t).set("ERROR: Missing Parens");
        }

        return t;
    }

    std::tuple<ParseError, KnowledgeBase> parseKnowledgeBase(const std::string& input) {
        std::tuple<ParseError, KnowledgeBase> t;

        std::istringstream iss(input);
        TokenList tokens = tokenize(iss);

        TokenList::const_iterator it = tokens.begin();
        TokenList::const_iterator end = tokens.end();

        if (it == end) {
            std::get<0>(t).set("ERROR: Empty KB");
        }

        int parenCount = 0;

        parseKnowledgeBaseHelper(std::get<1>(t), std::get<0>(t), it, end, parenCount);

        if (!std::get<0>(t).isSet() && parenCount != 0) {
            std::get<0>(t).set("ERROR: Missing Parens");
        }

        return t;
    }

    void parseKnowledgeBaseHelper(KnowledgeBase& kb, ParseError& pe, TokenList::const_iterator& it, TokenList::const_iterator& end, int& parenCount) {
        while (it != end) {
            std::tuple<bool, TokenList::const_iterator> t1 = findEnd(it, end);
        
            if (!std::get<0>(t1) && std::get<1>(t1) == end) {
                pe.set("ERROR: No End");
                return;
            }
            
            std::tuple<bool, TokenList::const_iterator> t2 = findImp(it, std::get<1>(t1));

            Clause c;

            TokenList::const_iterator endIt = std::get<1>(t1);
            if (std::get<0>(t2)) {
                TokenList::const_iterator impIt = std::get<1>(t2);
                if (isValid(it, impIt)) {
                    parseExpressionHelper(c.head, pe, it, impIt, parenCount);
                } else {
                    pe.set("ERROR: Malformed Head");
                }
                
                if (++endIt == end) {
                    parseQueryHelper(c.body, pe, impIt, end, parenCount);
                } else {
                    parseQueryHelper(c.body, pe, impIt, endIt, parenCount);
                }
            } else {
                if (++endIt == end) {
                    if (isValid(it, end)) {
                        parseExpressionHelper(c.head, pe, it, end, parenCount);
                    } else {
                        pe.set("ERROR: Malformed Head");
                    }   
                } else {
                    if (isValid(it, endIt)) {
                        parseExpressionHelper(c.head, pe, it, endIt, parenCount);
                    } else {
                        pe.set("ERROR: Malformed Head");
                    }   
                }
            }

            if (!c.head.children.empty()) {
                c.head = ExpressionTreeNode(c.head.children.front());
            } else {
                pe.set("ERROR: Malformed Head");
            }

            kb.tell(c);

            it = std::get<1>(t1);
            it++;
        }
    }

    std::tuple<bool, TokenList::const_iterator> findEnd(TokenList::const_iterator it, TokenList::const_iterator end) {
        while (it != end) {
            if (it->type() == TokenType::END) {
                return {true, it};
            }
            it++;
        }
        return {false, end};
    }

    std::tuple<bool, TokenList::const_iterator> findImp(TokenList::const_iterator it, TokenList::const_iterator end) {
        while (it != end) {
            if (it->type() == TokenType::IMP) {
                return {true, it};
            }
            it++;
        }
        return {false, end};
    }

    bool isValid(TokenList::const_iterator it, TokenList::const_iterator end) {
        while (it != end) {
            TokenList::const_iterator tempIt = it;
            if (tempIt->type() == TokenType::CLOSE && ++tempIt != end && tempIt->type() == TokenType::COMMA) {
                return false;
            }
            it++;
        }
        return true;
    }

    std::pair<ParseError, ExpressionTreeNode> parseQuery(const TokenList& tokens) {
        std::pair<ParseError, ExpressionTreeNode> p;

        TokenList::const_iterator it = tokens.begin();
        TokenList::const_iterator end = tokens.end();
        
        if (it == end) {
            p.first.set("ERROR: Empty Expression");
        }

        int parenCount = 0;

        parseQueryHelper(p.second, p.first, it, end, parenCount);

        if (!p.first.isSet() && parenCount != 0) {
            p.first.set("ERROR: Missing Parens");
        }

        if (!p.first.isSet() && it != end) {
            p.first.set("ERROR: Didn't Complete");
        }

        return p;
    }

    std::pair<ParseError, ExpressionTreeNode> parseQuery(const std::string& input) {
        std::pair<ParseError, ExpressionTreeNode> p;

        std::istringstream iss(input);
        TokenList tokens = tokenize(iss);

        TokenList::const_iterator it = tokens.begin();
        TokenList::const_iterator end = tokens.end();

        if (it == end) {
            p.first.set("ERROR: Empty Expression");
        }

        int parenCount = 0;

        parseQueryHelper(p.second, p.first, it, end, parenCount);

        if (!p.first.isSet() && parenCount != 0) {
            p.first.set("ERROR: Missing Parens");
        }

        return p;
    }

    void parseQueryHelper(ExpressionTreeNode& node, ParseError& pe, TokenList::const_iterator& it, TokenList::const_iterator& end, int& parenCount) {
        while (it != end) {    
            if (it->type() == TokenType::ERROR) {
                pe.set(it->value().c_str());
            }

            if (it->type() == TokenType::OPEN) {
                parenCount++;
                if (++it != end && it->type() == TokenType::CLOSE) {
                    pe.set("ERROR: Empty Arg");
                    break;
                }
                it--;
            } else if (it->type() == TokenType::CLOSE) {
                parenCount--;
                if (++it != end && it->type() == TokenType::STRING) {
                    pe.set("ERROR: Invalid/Missing Comma Placement");
                    break;
                }
                it--;
            }

            if (parenCount <= -1) {
                return;
            }

            TokenList::const_iterator tempIt = it;

            if (tempIt->type() != TokenType::END && ++tempIt == end) {
                pe.set("ERROR: Missing End");
            }

            tempIt = it;
            
            if (tempIt->type() == TokenType::END && ++tempIt != end) {
                if (tempIt->type() == TokenType::END) {
                    pe.set("ERROR: Extra End");
                } else {
                    pe.set("ERROR: Misplaced End");
                }
            }

            if (it->type() != TokenType::STRING) {
                it++;
                continue;
            }

            if (++it != end && it->type() == TokenType::STRING) {
                pe.set("ERROR: Missing Parens");
            }
            it--;

            for (std::size_t i = 0; i < it->value().size(); i++) {
                if (!isalnum(it->value()[i]) && it->value()[i] != '_') {
                    pe.set("ERROR: Invalid Arg");
                }
            }

            if (isLeafQuery(it, end)) {
                if (isAtom(makeAtom(it->value()))) {
                    node.children.push_back(makeAtom(it->value()));
                } else {
                    node.children.push_back(makeVariable(it->value()));
                }
                it++;
                if (it->type() == TokenType::COMMA) {
                    if ((++it == end) || (it != end && (it--->type() != TokenType::STRING))) {
                        pe.set("ERROR: Truncated List");
                    } else {
                        continue;
                    }
                }
                return;
            } else {
                std::list<ExpressionTreeNode> list = {makeAtom("a")};
                node.children.push_back(makeCompound(it->value(), list));
                node.children.back().children.clear();
                parseQueryHelper(node.children.back(), pe, ++it, end, parenCount);
            }
        }
    }

    std::pair<ParseError, ExpressionTreeNode> parseExpression(const TokenList& tokens) {
        std::pair<ParseError, ExpressionTreeNode> p;

        TokenList::const_iterator it = tokens.begin();
        TokenList::const_iterator end = tokens.end();
        
        if (it == end) {
            p.first.set("ERROR: Empty Expression");
        }

        int parenCount = 0;

        parseExpressionHelper(p.second, p.first, it, end, parenCount);

        if (!p.first.isSet() && parenCount != 0) {
            p.first.set("ERROR: Missing Parens");
        }

        return p;
    }

    std::pair<ParseError, ExpressionTreeNode> parseExpression(const std::string& input) {
        std::pair<ParseError, ExpressionTreeNode> p;

        std::istringstream iss(input);
        TokenList tokens = tokenize(iss);

        TokenList::const_iterator it = tokens.begin();
        TokenList::const_iterator end = tokens.end();

        if (it == end) {
            p.first.set("ERROR: Empty Expression");
        }

        int parenCount = 0;

        parseExpressionHelper(p.second, p.first, it, end, parenCount);

        if (!p.first.isSet() && parenCount != 0) {
            p.first.set("ERROR: Missing Parens");
        }

        if (!p.first.isSet() && it != end) {
            p.first.set("ERROR: Didn't Complete");
        }

        return p;
    }

    void parseExpressionHelper(ExpressionTreeNode& node, ParseError& pe, TokenList::const_iterator& it, TokenList::const_iterator& end, int& parenCount) {
        while (it != end) {    
            if (it->type() == TokenType::ERROR) {
                pe.set(it->value().c_str());
            }
            
            if (it->type() == TokenType::OPEN) {
                parenCount++;
                if (++it != end && it->type() == TokenType::CLOSE) {
                    pe.set("ERROR: Empty Arg");
                }
                it--;
            } else if (it->type() == TokenType::CLOSE) {
                parenCount--;
                if (++it != end && it->type() == TokenType::STRING) {
                    pe.set("ERROR: Invalid/Missing Comma Placement");
                    break;
                }
                it--;
            }

            if (parenCount <= -1) {
                return;
            }

            TokenList::const_iterator tempIt = it;

            if ((tempIt->type() == TokenType::COMMA) && ((++tempIt == end) || (tempIt != end && tempIt->type() != TokenType::STRING))) {
                pe.set("ERROR: Truncated List");
            }

            if (it->type() != TokenType::STRING) {
                it++;
                continue;
            }

            if (++it != end && it->type() == TokenType::STRING) {
                pe.set("ERROR: Missing Parens");
            }
            it--;

            for (std::size_t i = 0; i < it->value().size(); i++) {
                if (!isalnum(it->value()[i])) {
                    pe.set("ERROR: Invalid Arg");
                }
            }

            if (isLeafExpression(it, end)) {
                if (isAtom(makeAtom(it->value()))) {
                    node.children.push_back(makeAtom(it->value()));
                } else {
                    node.children.push_back(makeVariable(it->value()));
                }
                it++;
                if (it->type() == TokenType::COMMA) {
                    if ((++it == end) || (it != end && it--->type() != TokenType::STRING)) {
                        pe.set("ERROR: Truncated List");
                    } else {
                        continue;
                    }
                }
                return;
            } else {
                std::list<ExpressionTreeNode> list = {makeAtom("a")};
                node.children.push_back(makeCompound(it->value(), list));
                node.children.back().children.clear();
                parseExpressionHelper(node.children.back(), pe, ++it, end, parenCount);
            }
        }
    }

    bool isLeafQuery(TokenList::const_iterator it, TokenList::const_iterator& end) {
        bool seenComma = false;

        while (++it != end) {
            TokenList::const_iterator tempIt = it;

            if (it->type() == TokenType::OPEN) {
                return false;
            } else if (it->type() == TokenType::CLOSE || (++tempIt == end && it->type() == TokenType::END) || (it->type() == TokenType::STRING && seenComma)) {
                return true;
            } else if (it->type() == TokenType::COMMA) {
                seenComma = true;
            }
        }

        return false;
    }

    bool isLeafExpression(TokenList::const_iterator it, TokenList::const_iterator& end) {
        bool seenComma = false;

        while (++it != end) {
            if (it->type() == TokenType::OPEN) {
                return false;
            } else if (it->type() == TokenType::CLOSE || it->type() == TokenType::END || (it->type() == TokenType::STRING && seenComma)) {
                return true;
            } else if (it->type() == TokenType::COMMA) {
                seenComma = true;
            }
        }

        return false;
    }
}