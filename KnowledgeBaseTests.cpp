#include "catch.hpp"

#include "KnowledgeBase.hpp"
#include "KnowledgeBaseUtility.hpp"
#include "Parser.hpp"

using namespace vtpl;

TEST_CASE("Knowledge Base") {
    KnowledgeBase kb;
    
    SECTION("Adding to Database") {
        REQUIRE(kb.size() == 0);
        
        Clause c;
        c.head = ExpressionTreeNode();
        c.body = ExpressionTreeNode();

        kb.tell(c);

        REQUIRE(kb.size() == 1);
    }
}

TEST_CASE("Test Ask Functionality") {
    SECTION("Example One") {
        std::string expressions = "friends(X,Y) :- likes(X,Z), likes(Y,Z)."
                                    "likes(bill,movies)."
                                    "likes(sally,movies)."
                                    "likes(bob,pizza).";

        std::tuple<ParseError, KnowledgeBase> kb = parseKnowledgeBase(expressions);

        REQUIRE_FALSE(std::get<0>(kb).isSet());

        std::string q = "friends(sally,X).";

        std::pair<ParseError, ExpressionTreeNode> query = parseQuery(q);

        REQUIRE_FALSE(query.first.isSet());

        std::list<Substitution> sub = std::get<1>(kb).ask(query.second);

        REQUIRE_FALSE(sub.empty());

        REQUIRE(apply(query.second, sub.front()).toString() == "(friends(sally,sally))");
        REQUIRE(apply(query.second, sub.back()).toString() == "(friends(sally,bill))");
    }

    SECTION("Example Two") {
        std::string expressions = "hangout(X,Y) :- friends(X,Y),intown(X),intown(Y)."
                                    "friends(X,Y) :- likes(X,Z), likes(Y,Z)."
                                    "likes(bill,movies)."
                                    "likes(sally,movies)."
                                    "likes(bob,pizza)."
                                    "likes(joe, movies)."
                                    "intown(bill)."
                                    "intown(sally)."
                                    "intown(bob).";

        std::tuple<ParseError, KnowledgeBase> kb = parseKnowledgeBase(expressions);

        REQUIRE_FALSE(std::get<0>(kb).isSet());

        std::string q = "hangout(X,Y).";

        std::pair<ParseError, ExpressionTreeNode> query = parseQuery(q);

        REQUIRE_FALSE(query.first.isSet());

        std::list<Substitution> sub = std::get<1>(kb).ask(query.second);

        REQUIRE_FALSE(sub.empty());

        std::vector<std::string> correctSubs = {"(hangout(bob,bob))", "(hangout(sally,sally))", "(hangout(bill,sally))", "(hangout(sally,bill))", "(hangout(bill,bill))"};

        int index = 0;
        for (Substitution s : sub) {
            REQUIRE(apply(query.second, s).toString() == correctSubs[index]);
            index++;
        }
    }

    SECTION("Example Three") {
        std::string expressions = "a(X,Y) :- e(X), f(Z)."
                                    "e(fred)."
                                    "f(john).";

        std::tuple<ParseError, KnowledgeBase> kb = parseKnowledgeBase(expressions);

        REQUIRE_FALSE(std::get<0>(kb).isSet());

        std::string q = "a(X,Y).";

        std::pair<ParseError, ExpressionTreeNode> query = parseQuery(q);

        REQUIRE_FALSE(query.first.isSet());

        std::list<Substitution> sub = std::get<1>(kb).ask(query.second);

        REQUIRE(sub.empty());
    }

    SECTION("Example Four") {
        std::string expressions = "mortal(X) :- person(X)."
                                    "person(socrates).";

        std::tuple<ParseError, KnowledgeBase> kb = parseKnowledgeBase(expressions);

        REQUIRE_FALSE(std::get<0>(kb).isSet());

        std::string q = "immortal(X).";

        std::pair<ParseError, ExpressionTreeNode> query = parseQuery(q);

        REQUIRE_FALSE(query.first.isSet());

        std::list<Substitution> sub = std::get<1>(kb).ask(query.second);

        REQUIRE(sub.empty());
    }

    SECTION("Example Five") {
        std::string expressions = "f(a)."
                                    "h(b)."
                                    "g(X,X) :- f(X),h(X).";

        std::tuple<ParseError, KnowledgeBase> kb = parseKnowledgeBase(expressions);

        REQUIRE_FALSE(std::get<0>(kb).isSet());

        std::string q = "g(Y,Y).";

        std::pair<ParseError, ExpressionTreeNode> query = parseQuery(q);

        REQUIRE_FALSE(query.first.isSet());

        std::list<Substitution> sub = std::get<1>(kb).ask(query.second);

        REQUIRE(sub.empty());
    }
}