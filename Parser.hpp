#ifndef PARSER_HPP
#define PARSER_HPP

#include "ExpressionTree.hpp"
#include "KnowledgeBase.hpp"
#include "Lexer.hpp"
#include "Token.hpp"

#include <cstring>
#include <sstream>
#include <string>
#include <tuple>

namespace vtpl {
  /// Class to hold the error code from parsing.
  ///
  /// If isSet() returns true, an error occurred during parsing
  /// and message() returns a description of the error.
  class ParseError {
    public:
      ParseError() = default;

      /// set parse error with optional constant message
      void set(const char * str = nullptr);

      /// set parse error with constant message and token
      void set(const char * str, const Token token);

      /// returns true if error is set, else false
      bool isSet() const noexcept;

      /// returns string that contains the error message if set
      /// returns empty string otherwise
      std::string message();
    private:
      const char * error = "";
      Token t;
  };

  /// Given a TokenList from lexer, attempt to parse as a vt-prolog knowledge base.
  ///
  /// If returned ParseError object is set, then an error occurred and the KnowledgeBase 
  /// may be incomplete.
  std::tuple<ParseError, KnowledgeBase> parseKnowledgeBase(const TokenList& tokens);

  /// convenience function to parse a knowledgebase directly from string
  std::tuple<ParseError, KnowledgeBase> parseKnowledgeBase(const std::string& input);

  void parseKnowledgeBaseHelper(KnowledgeBase& kb, ParseError& pe, TokenList::const_iterator& it, TokenList::const_iterator& end, int& parenCount);

  std::tuple<bool, TokenList::const_iterator> findEnd(TokenList::const_iterator it, TokenList::const_iterator end);

  std::tuple<bool, TokenList::const_iterator> findImp(TokenList::const_iterator it, TokenList::const_iterator end);

  bool isValid(TokenList::const_iterator it, TokenList::const_iterator end);

  // Given a TokenList from lexer, attempt to parse as a vt-prolog query.
  //
  // If returned ParseError object is set, then an error occurred and the ExpressionTree 
  // may be incomplete.
  std::pair<ParseError, ExpressionTreeNode> parseQuery(const TokenList& tokens);

  // Convenience function to parse a query directly from string
  std::pair<ParseError, ExpressionTreeNode> parseQuery(const std::string& input);

  void parseQueryHelper(ExpressionTreeNode& root, ParseError& pe, TokenList::const_iterator& it, TokenList::const_iterator& end, int& parenCount);

  // Given a TokenList from lexer, attempt to parse as a vt-prolog expression.
  //
  // If returned ParseError object is set, then an error occurred and the ExpressionTree 
  // may be incomplete.
  std::pair<ParseError, ExpressionTreeNode> parseExpression(const TokenList& tokens);

  // Convenience function to parse an expression directly from string
  std::pair<ParseError, ExpressionTreeNode> parseExpression(const std::string& input);

  void parseExpressionHelper(ExpressionTreeNode& root, ParseError& pe, TokenList::const_iterator& it, TokenList::const_iterator& end, int& parenCount);

  bool isLeafQuery(TokenList::const_iterator it, TokenList::const_iterator& end);

  bool isLeafExpression(TokenList::const_iterator it, TokenList::const_iterator& end);
}

#endif