#include "catch.hpp"

#include "ExpressionTree.hpp"
#include "Lexer.hpp"
#include "Parser.hpp"
#include "Token.hpp"

#include <sstream>

using namespace vtpl;

TEST_CASE("Parse Expression") {
    SECTION("Parse From Token List 1") {
        std::string input = "f(g(a),c,h(b,c))";
        std::istringstream iss(input);

        TokenList tl = tokenize(iss);

        std::pair<ParseError, ExpressionTreeNode> ret;

        ret = parseExpression(tl);

        REQUIRE(ret.second.toString() == "(" + input + ")");
    }

    SECTION("Parse from Token List 2") {
        std::string input = "f(g(a),h(b,c))";
        std::istringstream iss(input);
        
        TokenList tl = tokenize(iss);

        std::pair<ParseError, ExpressionTreeNode> ret;

        ret = parseExpression(tl);

        ExpressionTreeNode etn1 = ExpressionTreeNode(ExpressionTreeNodeType::ATOM, "a", std::list<ExpressionTreeNode>());
        std::list<ExpressionTreeNode> list1 = {etn1};
        ExpressionTreeNode etn2 = ExpressionTreeNode(ExpressionTreeNodeType::ATOM, "b", std::list<ExpressionTreeNode>());
        ExpressionTreeNode etn3 = ExpressionTreeNode(ExpressionTreeNodeType::ATOM, "c", std::list<ExpressionTreeNode>());
        std::list<ExpressionTreeNode> list2 = {etn2, etn3};
        ExpressionTreeNode etn4 = ExpressionTreeNode(ExpressionTreeNodeType::COMPOUND, "g", list1);
        ExpressionTreeNode etn5 = ExpressionTreeNode(ExpressionTreeNodeType::COMPOUND, "h", list2);
        std::list<ExpressionTreeNode> list3 = {etn4, etn5};
        ExpressionTreeNode etn6 = ExpressionTreeNode(ExpressionTreeNodeType::COMPOUND, "f", list3); 
        std::list<ExpressionTreeNode> list4 = {etn6};
        ExpressionTreeNode etn7 = ExpressionTreeNode(ExpressionTreeNodeType::NONE, "", list4); 

        REQUIRE(ret.second == etn7);
    }

    SECTION("Parse from Token List 3") {
        std::string input = "f(a),g(b,c),h(x)";
        std::istringstream iss(input);
        
        TokenList tl = tokenize(iss);

        std::pair<ParseError, ExpressionTreeNode> ret;

        ret = parseExpression(tl);

        REQUIRE(ret.second.toString() == "(" + input + ")");
    }

    SECTION("Parse from Token List 4") {
        std::string input = "f(a)";
        std::istringstream iss(input);
        
        TokenList tl = tokenize(iss);

        std::pair<ParseError, ExpressionTreeNode> ret;

        ret = parseExpression(tl);

        ExpressionTreeNode etn1 = ExpressionTreeNode(ExpressionTreeNodeType::ATOM, "a", std::list<ExpressionTreeNode>());
        std::list<ExpressionTreeNode> list1 = {etn1};
        ExpressionTreeNode etn2 = ExpressionTreeNode(ExpressionTreeNodeType::COMPOUND, "f", list1); 
        std::list<ExpressionTreeNode> list2 = {etn2};
        ExpressionTreeNode etn3 = ExpressionTreeNode(ExpressionTreeNodeType::NONE, "", list2); 

        REQUIRE(ret.second == etn3);
    }

    SECTION("Parse from Token List 5") {
        std::string input = "f(a,b)";
        std::istringstream iss(input);
        
        TokenList tl = tokenize(iss);

        std::pair<ParseError, ExpressionTreeNode> ret;

        ret = parseExpression(tl);

        ExpressionTreeNode etn1 = ExpressionTreeNode(ExpressionTreeNodeType::ATOM, "a", std::list<ExpressionTreeNode>());
        ExpressionTreeNode etn2 = ExpressionTreeNode(ExpressionTreeNodeType::ATOM, "b", std::list<ExpressionTreeNode>());
        std::list<ExpressionTreeNode> list1 = {etn1, etn2};
        ExpressionTreeNode etn3 = ExpressionTreeNode(ExpressionTreeNodeType::COMPOUND, "f", list1); 
        std::list<ExpressionTreeNode> list2 = {etn3};
        ExpressionTreeNode etn4 = ExpressionTreeNode(ExpressionTreeNodeType::NONE, "", list2); 

        REQUIRE(ret.second == etn4);
    }

    SECTION("Parse From String 1") {
        std::string input = "f(g(a),c,h(b,c))";

        std::pair<ParseError, ExpressionTreeNode> ret;

        ret = parseExpression(input);

        REQUIRE(ret.second.toString() == "(" + input + ")");
    }

    SECTION("Parse from String 2") {
        std::string input = "f(g(a),h(b,c))";

        std::pair<ParseError, ExpressionTreeNode> ret;

        ret = parseExpression(input);

        ExpressionTreeNode etn1 = ExpressionTreeNode(ExpressionTreeNodeType::ATOM, "a", std::list<ExpressionTreeNode>());
        std::list<ExpressionTreeNode> list1 = {etn1};
        ExpressionTreeNode etn2 = ExpressionTreeNode(ExpressionTreeNodeType::ATOM, "b", std::list<ExpressionTreeNode>());
        ExpressionTreeNode etn3 = ExpressionTreeNode(ExpressionTreeNodeType::ATOM, "c", std::list<ExpressionTreeNode>());
        std::list<ExpressionTreeNode> list2 = {etn2, etn3};
        ExpressionTreeNode etn4 = ExpressionTreeNode(ExpressionTreeNodeType::COMPOUND, "g", list1);
        ExpressionTreeNode etn5 = ExpressionTreeNode(ExpressionTreeNodeType::COMPOUND, "h", list2);
        std::list<ExpressionTreeNode> list3 = {etn4, etn5};
        ExpressionTreeNode etn6 = ExpressionTreeNode(ExpressionTreeNodeType::COMPOUND, "f", list3); 
        std::list<ExpressionTreeNode> list4 = {etn6};
        ExpressionTreeNode etn7 = ExpressionTreeNode(ExpressionTreeNodeType::NONE, "", list4); 

        REQUIRE(ret.second == etn7);
    }

    SECTION("Parse from String 3") {
        std::string input = "f(a),g(b,c),h(x)";
        
        std::pair<ParseError, ExpressionTreeNode> ret;

        ret = parseExpression(input);

        REQUIRE(ret.second.toString() == "(" + input + ")");
    }

    SECTION("Parse from String 4") {
        std::string input = "f(a)";
        
        std::pair<ParseError, ExpressionTreeNode> ret;

        ret = parseExpression(input);

        ExpressionTreeNode etn1 = ExpressionTreeNode(ExpressionTreeNodeType::ATOM, "a", std::list<ExpressionTreeNode>());
        std::list<ExpressionTreeNode> list1 = {etn1};
        ExpressionTreeNode etn2 = ExpressionTreeNode(ExpressionTreeNodeType::COMPOUND, "f", list1);
        std::list<ExpressionTreeNode> list2 = {etn2};
        ExpressionTreeNode etn3 = ExpressionTreeNode(ExpressionTreeNodeType::NONE, "", list2);  

        REQUIRE(ret.second == etn3);
    }

    SECTION("Parse from String 5") {
        std::string input = "f(a,b)";
        
        std::pair<ParseError, ExpressionTreeNode> ret;

        ret = parseExpression(input);

        ExpressionTreeNode etn1 = ExpressionTreeNode(ExpressionTreeNodeType::ATOM, "a", std::list<ExpressionTreeNode>());
        ExpressionTreeNode etn2 = ExpressionTreeNode(ExpressionTreeNodeType::ATOM, "b", std::list<ExpressionTreeNode>());
        std::list<ExpressionTreeNode> list1 = {etn1, etn2};
        ExpressionTreeNode etn3 = ExpressionTreeNode(ExpressionTreeNodeType::COMPOUND, "f", list1); 
        std::list<ExpressionTreeNode> list2 = {etn3};
        ExpressionTreeNode etn4 = ExpressionTreeNode(ExpressionTreeNodeType::NONE, "", list2); 

        REQUIRE(ret.second == etn4);
    }

    SECTION("Parse from String 6 : Atom only") {
        std::string input = "a";
        
        std::pair<ParseError, ExpressionTreeNode> ret;

        ret = parseExpression(input);

        REQUIRE(ret.second.toString() == "(" + input + ")");
    }

    SECTION("Parse from String 7 : Variable only") {
        std::string input = "A";
        
        std::pair<ParseError, ExpressionTreeNode> ret;

        ret = parseExpression(input);

        REQUIRE(ret.second.toString() == "(" + input + ")");
    }

    SECTION("Empty Arg") {
        std::string input = "";
        
        std::pair<ParseError, ExpressionTreeNode> ret;

        ret = parseExpression(input);

        REQUIRE(ret.second.toString() == input);

        REQUIRE(ret.first.isSet());

        REQUIRE(ret.first.message() == "ERROR: Empty Expression");
    }

    SECTION("Doesn't Finish Expression") {
        std::string input = "f(a)),b";
        
        std::pair<ParseError, ExpressionTreeNode> ret;

        ret = parseExpression(input);

        REQUIRE(ret.first.isSet());

        REQUIRE(ret.first.message() == "ERROR: Missing Parens");
    }

    SECTION("Empty Arg") {
        std::string input = "f()";
        
        std::pair<ParseError, ExpressionTreeNode> ret;

        ret = parseExpression(input);

        REQUIRE(ret.first.isSet());

        REQUIRE(ret.first.message() == "ERROR: Empty Arg");
    }

    SECTION("Truncated List") {
        std::string input = "f(a,)";
        
        std::pair<ParseError, ExpressionTreeNode> ret;

        ret = parseExpression(input);

        REQUIRE(ret.first.isSet());

        REQUIRE(ret.first.message() == "ERROR: Truncated List");
    }
}

TEST_CASE("Parse Error") {
    SECTION("Is Set") {
        ParseError pe;

        const char * error = "This is an error message!";

        pe.set(error);

        REQUIRE(pe.isSet());

        REQUIRE(pe.message() == error);
    }

    SECTION("Is Not Set") {
        ParseError pe;

        REQUIRE_FALSE(pe.isSet());
    }
    
}

TEST_CASE("Parse Query") {
    SECTION("Parse Query 1") {
        std::string input = " a. ";
        
        std::pair<ParseError, ExpressionTreeNode> ret;

        ret = parseQuery(input);

        REQUIRE(ret.second.toString() == "(a)");
    }

    SECTION("Parse Query 2") {
        std::string input = "a ( b ). ";
        
        std::pair<ParseError, ExpressionTreeNode> ret;

        ret = parseQuery(input);

        REQUIRE(ret.second.toString() == "(a(b))");
    }
    
    SECTION("Parse Query 3") {
        std::string input = "a ( X ). ";
        
        std::pair<ParseError, ExpressionTreeNode> ret;

        ret = parseQuery(input);

        REQUIRE(ret.second.toString() == "(a(X))");
    }

    SECTION("Parse Query 4") {
        std::string input = "a ( b , c ). ";
        
        std::pair<ParseError, ExpressionTreeNode> ret;

        ret = parseQuery(input);

        REQUIRE(ret.second.toString() == "(a(b,c))");
    }

    SECTION("Parse Query 5") {
        std::string input = "a ( b, X ). ";
        
        std::pair<ParseError, ExpressionTreeNode> ret;

        ret = parseQuery(input);

        REQUIRE(ret.second.toString() == "(a(b,X))");
    }

    SECTION("Parse Query 6") {
        std::string input = "a ( b(c,d) , c , e(f,g,h)). ";
        
        std::pair<ParseError, ExpressionTreeNode> ret;

        ret = parseExpression(input);

        REQUIRE(ret.second.toString() == "(a(b(c,d),c,e(f,g,h)))");
    }

    SECTION("Atom Only (No end)") {
        std::string input = "a";
        
        std::pair<ParseError, ExpressionTreeNode> ret;

        ret = parseQuery(input);

        REQUIRE(ret.first.isSet());

        REQUIRE(ret.first.message() == "ERROR: Missing End");
    }

    SECTION("Missing End") {
        std::string input = "a ( b(c,d) , c , e(f,g,h)) ";
        
        std::pair<ParseError, ExpressionTreeNode> ret;

        ret = parseQuery(input);

        REQUIRE(ret.first.isSet());

        REQUIRE(ret.first.message() == "ERROR: Missing End");
    }

    SECTION("Extra End") {
        std::string input = "a ( b(c,d) , c , e(f,g,h)).. ";
        
        std::pair<ParseError, ExpressionTreeNode> ret;

        ret = parseQuery(input);

        REQUIRE(ret.first.isSet());

        REQUIRE(ret.first.message() == "ERROR: Extra End");
    }

    SECTION("Misplaced End") {
        std::string input = "a ( b(c,d) , c. , e(f,g,h)). ";
        
        std::pair<ParseError, ExpressionTreeNode> ret;

        ret = parseQuery(input);

        REQUIRE(ret.first.isSet());

        REQUIRE(ret.first.message() == "ERROR: Misplaced End");
    }
}

TEST_CASE("Parse Knowledge Base") {
    SECTION("Parse Knowledge Base Expression Only") {
        std::string input = "a(b). ";

        std::tuple<ParseError, KnowledgeBase> ret;

        ret = parseKnowledgeBase(input);

        KnowledgeBase::Iterator it = std::get<1>(ret).begin();

        REQUIRE(std::get<1>(ret).size() == 1);

        REQUIRE(it->head.toString() == "a(b)");
        REQUIRE(it->body.toString() == "");
    }

    SECTION("Parse Knowledge Base Single Clause") {
        std::string input = "a(b) :- a ( b ).";

        std::tuple<ParseError, KnowledgeBase> ret;

        ret = parseKnowledgeBase(input);

        KnowledgeBase::Iterator it = std::get<1>(ret).begin();

        REQUIRE(std::get<1>(ret).size() == 1);

        REQUIRE(it->head.toString() == "a(b)");
        REQUIRE(it->body.toString() == "(a(b))");
    }

    SECTION("Parse Knowledge Base Multiple Clause") {
        std::string input = "a(b) :- a ( b ). a ( b, X ) :- a ( b , c ).";
    
        std::tuple<ParseError, KnowledgeBase> ret;

        ret = parseKnowledgeBase(input);

        KnowledgeBase::Iterator it = std::get<1>(ret).begin();

        REQUIRE(std::get<1>(ret).size() == 2);

        REQUIRE(it->head.toString() == "a(b)");
        REQUIRE(it->body.toString() == "(a(b))");

        it++;

        REQUIRE(it->head.toString() == "a(b,X)");
        REQUIRE(it->body.toString() == "(a(b,c))");
    }

    SECTION("Parse Knowledge Base Long Test") {
        std::string input = "brother(X,Y):-parent(Z,X),parent(Z,Y),male(X),notequal(X,Y).";

        std::tuple<ParseError, KnowledgeBase> ret;

        ret = parseKnowledgeBase(input);

        KnowledgeBase::Iterator it = std::get<1>(ret).begin();

        REQUIRE(std::get<1>(ret).size() == 1);

        REQUIRE(it->head.toString() == "brother(X,Y)");
        REQUIRE(it->body.toString() == "(parent(Z,X),parent(Z,Y),male(X),notequal(X,Y))");
    }

    SECTION("Parse Knowledge Base Really Long Test With Multiple Clause") { 
        std::string input = "mother(X,Y):- parent(X,Y),female(X). father(X,Y):- parent(X,Y),male(X). haschild(X):- parent(X). sister(X,Y):- parent(Z,X),parent(Z,Y),female(X),notequal(X,Y). brother(X,Y):-parent(Z,X),parent(Z,Y),male(X),notequal(X,Y).";

        std::tuple<ParseError, KnowledgeBase> ret;

        ret = parseKnowledgeBase(input);

        KnowledgeBase::Iterator it = std::get<1>(ret).begin();

        REQUIRE(std::get<1>(ret).size() == 5);

        REQUIRE(it->head.toString() == "mother(X,Y)");
        REQUIRE(it->body.toString() == "(parent(X,Y),female(X))");

        it++;

        REQUIRE(it->head.toString() == "father(X,Y)");
        REQUIRE(it->body.toString() == "(parent(X,Y),male(X))");

        it++;

        REQUIRE(it->head.toString() == "haschild(X)");
        REQUIRE(it->body.toString() == "(parent(X))");

        it++;

        REQUIRE(it->head.toString() == "sister(X,Y)");
        REQUIRE(it->body.toString() == "(parent(Z,X),parent(Z,Y),female(X),notequal(X,Y))");

        it++;

        REQUIRE(it->head.toString() == "brother(X,Y)");
        REQUIRE(it->body.toString() == "(parent(Z,X),parent(Z,Y),male(X),notequal(X,Y))");
    }

    SECTION("Huge Parse Without End") {
        std::string input = "mother(X,Y,X,Y,X,Y,X,Y,X,Y,X,Y,X,Y,X,Y,X,Y,X,Y,X,Y \
        X,Y,X,Y,X,Y,X,Y,X,Y,X,Y,X,Y,X,Y,X,Y,X,Y,X,Y,X,Y,X,Y,X,Y,X,Y,X,Y,X,Y,X,Y,X,Y,X,Y,X,Y,X,Y,X,Y,X,Y,X,Y,X,Y,X,Y \
        X,Y,X,Y,X,Y,X,Y,X,Y,X,Y,X,Y,X,Y,X,Y,X,Y,X,Y,X,Y,X,Y,X,Y,X,Y,X,Y,X,Y,X,Y,X,Y,X,Y,X,Y,X,Y,X,Y,X,Y,X,Y,X,Y,X,Y,X,Y,X,Y)";

        std::tuple<ParseError, KnowledgeBase> ret;

        ret = parseKnowledgeBase(input);

        REQUIRE(std::get<0>(ret).isSet());
        REQUIRE(std::get<0>(ret).message() == "ERROR: No End");
    }

    SECTION("Empty Input") {
        std::string input = "";

        std::tuple<ParseError, KnowledgeBase> ret;

        ret = parseKnowledgeBase(input);

        REQUIRE(std::get<0>(ret).isSet());
        REQUIRE(std::get<0>(ret).message() == "ERROR: Empty KB");
    }

    SECTION("No Imp") {
        std::string input = "a(b(c,d),c,e(f,g,h))a(b).";

        std::tuple<ParseError, KnowledgeBase> ret;

        ret = parseKnowledgeBase(input);

        REQUIRE(std::get<0>(ret).isSet());
        REQUIRE(std::get<0>(ret).message() == "ERROR: Malformed Head");
    }

    SECTION("Simple Facts") {
        std::string input = "mortal(X) :- person(X).";

        std::tuple<ParseError, KnowledgeBase> ret;

        ret = parseKnowledgeBase(input);

        KnowledgeBase::Iterator it = std::get<1>(ret).begin();

        REQUIRE(std::get<1>(ret).size() == 1);

        REQUIRE(it->head.toString() == "mortal(X)");
        REQUIRE(it->body.toString() == "(person(X))");
    }

    SECTION("Simple Facts") {
        std::string input = "friends(X,Y) :- likes(X,Z), likes(Y,Z).";

        std::tuple<ParseError, KnowledgeBase> ret;

        ret = parseKnowledgeBase(input);

        KnowledgeBase::Iterator it = std::get<1>(ret).begin();

        REQUIRE(std::get<1>(ret).size() == 1);

        REQUIRE(it->head.toString() == "friends(X,Y)");
        REQUIRE(it->body.toString() == "(likes(X,Z),likes(Y,Z))");
    } 

    SECTION("More Simple Facts") {   
        std::string input = "a.";

        std::tuple<ParseError, KnowledgeBase> ret;

        ret = parseKnowledgeBase(input);

        KnowledgeBase::Iterator it = std::get<1>(ret).begin();

        REQUIRE(std::get<1>(ret).size() == 1);

        REQUIRE(it->head.toString() == "a");
        REQUIRE(it->body.toString() == "");

        REQUIRE(isAtom(it->head));
    }

    SECTION("Malformed Head") {
        std::string input = ".";

        std::tuple<ParseError, KnowledgeBase> ret;

        ret = parseKnowledgeBase(input);

        REQUIRE(std::get<0>(ret).isSet());
        REQUIRE(std::get<0>(ret).message() == "ERROR: Malformed Head");
    }

    SECTION("Malformed Head 2") {
        std::string input = ".(a).";

        std::tuple<ParseError, KnowledgeBase> ret;

        ret = parseKnowledgeBase(input);

        REQUIRE(std::get<0>(ret).isSet());
        REQUIRE(std::get<0>(ret).message() == "ERROR: Malformed Head");
    }

    SECTION("Malformed Head 3") {
        std::string input = "(.a).";

        std::tuple<ParseError, KnowledgeBase> ret;

        ret = parseKnowledgeBase(input);

        REQUIRE(std::get<0>(ret).isSet());
        REQUIRE(std::get<0>(ret).message() == "ERROR: Malformed Head");
    }

    SECTION("Malformed Head 4") {
        std::string input = "(a.).";

        std::tuple<ParseError, KnowledgeBase> ret;

        ret = parseKnowledgeBase(input);

        REQUIRE(std::get<0>(ret).isSet());
        REQUIRE(std::get<0>(ret).message() == "ERROR: Malformed Head");
    }

    SECTION("Malformed Head 5") {
        std::string input = "(a,.).";

        std::tuple<ParseError, KnowledgeBase> ret;

        ret = parseKnowledgeBase(input);

        REQUIRE(std::get<0>(ret).isSet());
        REQUIRE(std::get<0>(ret).message() == "ERROR: Malformed Head");
    }

    SECTION("Malformed Head 6") {
        std::string input = "f(X),h(Y).";

        std::tuple<ParseError, KnowledgeBase> ret;

        ret = parseKnowledgeBase(input);

        REQUIRE(std::get<0>(ret).isSet());
        REQUIRE(std::get<0>(ret).message() == "ERROR: Malformed Head");
    }

    SECTION("Parse Expression Extra Test") {
        std::string input = "f(abc,X1)";
        
        std::pair<ParseError, ExpressionTreeNode> ret;

        ret = parseExpression(input);

        REQUIRE(ret.second.toString() == "(" + input + ")");
    }

    SECTION("Testss") {
        std::string input = "frien;ds(X,Y) :- likes(X,Z), likes(Y,Z).\n"
                      "likes(bill,movies).\n"
                      "likes(sally,movies).\n"
                      "likes(bob,pizza)."
                      ";"
                      "-";

        std::tuple<ParseError, KnowledgeBase> ret;

        ret = parseKnowledgeBase(input);
    }
}