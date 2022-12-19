//== vtpl-ide.cpp - main driver for vtpl REPL TUI =============================//
// 
//
//============================================================================//

#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>

#include "KnowledgeBase.hpp"
#include "KnowledgeBaseUtility.hpp"
#include "Parser.hpp"

int main(int argc, char *argv[]) {
  if (argc != 2) {
    std::cerr << "Error: Too many or too little arguments at " << argc << std::endl;
    return EXIT_FAILURE;
  }

  std::tuple<vtpl::ParseError, vtpl::KnowledgeBase> kb;

  std::ifstream f(argv[1]);
  if (!f.good()) {
    std::cerr << "Error: File " << argv[1] << " is invalid" << std::endl;
    return EXIT_FAILURE;
  } else {
    std::stringstream buffer;
    buffer << f.rdbuf();
    std::string input = buffer.str();
    kb = vtpl::parseKnowledgeBase(input);
    if (std::get<0>(kb).isSet()) {
      std::cerr << "Error: " << std::get<0>(kb).message() << std::endl;
    return EXIT_FAILURE;
    }
  }

  bool trace = false;

  std::cout << "vtpl> ";
  std::string input;
  std::getline(std::cin, input);

  while (true) {
    while (input.empty()) {
      std::cout << "Error: unknown command" << std::endl;
      std::cout << "vtpl> ";
      std::getline(std::cin, input);
    }

    if (input[0] == '?') { // ? <query>
      input.erase(std::remove_if(input.begin(), input.end(), ::isspace), input.end());
      std::pair<vtpl::ParseError, vtpl::ExpressionTreeNode> query = vtpl::parseQuery(input.substr(1));
      if (query.first.isSet()) {
        std::cerr << "Parse Error: " << query.first.message() << std::endl;
      } else {
        std::list<vtpl::Substitution> sub = std::get<1>(kb).ask(query.second);
        if (sub.empty()) {
          std::cout << "false." << std::endl;
        } else {
          for (vtpl::Substitution s : sub) {
            std::string output = vtpl::apply(query.second, s).toString();
            std::cout << output << std::endl;
          }
        }  
      }
    } else if (input == "trace on") {
      trace = true;
      std::get<1>(kb).setTrace(trace);
    } else if (input == "trace off") {
      trace = false;
      std::get<1>(kb).setTrace(trace);
    } else if (input == "quit") {
      return EXIT_SUCCESS;
    } else {
      std::cout << "Error: unknown command" << std::endl;
      std::cout << "vtpl> ";
      std::getline(std::cin, input);

      continue;
    }

    std::cout << "vtpl> ";
    std::getline(std::cin, input);
  }

  return 0;
}
