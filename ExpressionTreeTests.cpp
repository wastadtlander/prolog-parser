#include "catch.hpp"

#include "ExpressionTree.hpp"

using namespace vtpl;

TEST_CASE("expression tree : construction", "[ExpressionTree]") {
    ExpressionTreeNode etn = ExpressionTreeNode();
    REQUIRE(isNone(etn));

    ExpressionTreeNode etnNone = ExpressionTreeNode(ExpressionTreeNodeType::NONE, "", std::list<ExpressionTreeNode>());
    REQUIRE(isNone(etnNone));

    ExpressionTreeNode etnAtom = ExpressionTreeNode(ExpressionTreeNodeType::ATOM, "", std::list<ExpressionTreeNode>());
    REQUIRE(isAtom(etnAtom));
    
    ExpressionTreeNode etnVariable = ExpressionTreeNode(ExpressionTreeNodeType::VARIABLE, "", std::list<ExpressionTreeNode>());
    REQUIRE(isVariable(etnVariable));
    
    ExpressionTreeNode etnCompound = ExpressionTreeNode(ExpressionTreeNodeType::COMPOUND, "", std::list<ExpressionTreeNode>());
    REQUIRE(isCompound(etnCompound));

    // No need to test construction of ExpressionTreeNodeHasher as it doesn't store any data or have a non-default constructor
}

TEST_CASE("expression tree : toString", "[ExpressionTree]") {
    SECTION("root only") {
        ExpressionTreeNode etn = ExpressionTreeNode(ExpressionTreeNodeType::NONE, "f", std::list<ExpressionTreeNode>());
        REQUIRE(etn.toString() == "f");
    }
    
    SECTION("root with children") {
        ExpressionTreeNode etn1 = ExpressionTreeNode(ExpressionTreeNodeType::NONE, "a", std::list<ExpressionTreeNode>());
        ExpressionTreeNode etn2 = ExpressionTreeNode(ExpressionTreeNodeType::NONE, "b", std::list<ExpressionTreeNode>());
        std::list<ExpressionTreeNode> list = {etn1, etn2};
        ExpressionTreeNode etn3 = ExpressionTreeNode(ExpressionTreeNodeType::NONE, "f", list); 
        REQUIRE(etn3.toString() == "f(a,b)");
    }

    SECTION("root with a lot of children") {
        ExpressionTreeNode etn1 = ExpressionTreeNode(ExpressionTreeNodeType::NONE, "a", std::list<ExpressionTreeNode>());
        std::list<ExpressionTreeNode> list1 = {etn1};
        ExpressionTreeNode etn2 = ExpressionTreeNode(ExpressionTreeNodeType::NONE, "b", std::list<ExpressionTreeNode>());
        ExpressionTreeNode etn3 = ExpressionTreeNode(ExpressionTreeNodeType::NONE, "c", std::list<ExpressionTreeNode>());
        std::list<ExpressionTreeNode> list2 = {etn2, etn3};
        ExpressionTreeNode etn4 = ExpressionTreeNode(ExpressionTreeNodeType::NONE, "g", list1);
        ExpressionTreeNode etn5 = ExpressionTreeNode(ExpressionTreeNodeType::NONE, "h", list2);
        std::list<ExpressionTreeNode> list3 = {etn4, etn5};
        ExpressionTreeNode etn6 = ExpressionTreeNode(ExpressionTreeNodeType::NONE, "f", list3); 
        REQUIRE(etn6.toString() == "f(g(a),h(b,c))");
    }
}

TEST_CASE("expression tree : make functions", "[ExpressionTree]") {
    ExpressionTreeNode etn = ExpressionTreeNode(ExpressionTreeNodeType::NONE, "", std::list<ExpressionTreeNode>());

    SECTION("makeAtom") {
        etn = makeAtom("atom");
        REQUIRE(isAtom(etn));
    }

    SECTION("makeVariable") {
        etn = makeVariable("VARIABLE");
        REQUIRE(isVariable(etn));

        etn = makeVariable("_VARI_A_BLE");
        REQUIRE(isVariable(etn));

        etn = makeVariable("ZZZZZ");
        REQUIRE(isVariable(etn));
    }

    SECTION("makeCompound") {
        std::list<ExpressionTreeNode> list;
        list.push_back(ExpressionTreeNode(ExpressionTreeNodeType::NONE, "", std::list<ExpressionTreeNode>()));
        etn = makeCompound("compound", list);
        REQUIRE(isCompound(etn));
    }
}

TEST_CASE("expression tree : arity", "[ExpressionTree]") {
    SECTION("root only") {
        ExpressionTreeNode etn = ExpressionTreeNode(ExpressionTreeNodeType::NONE, "", std::list<ExpressionTreeNode>());
        REQUIRE(arity(etn) == 0);
    }

    SECTION("root with children") {
        ExpressionTreeNode etn1 = ExpressionTreeNode(ExpressionTreeNodeType::NONE, "", std::list<ExpressionTreeNode>());
        ExpressionTreeNode etn2 = ExpressionTreeNode(ExpressionTreeNodeType::NONE, "", std::list<ExpressionTreeNode>());
        std::list<ExpressionTreeNode> list = {etn1, etn2};
        ExpressionTreeNode etn3 = ExpressionTreeNode(ExpressionTreeNodeType::NONE, "", list);
        REQUIRE(arity(etn3) == 2);
    }

    SECTION("root with a lot of children") {
        ExpressionTreeNode etn1 = ExpressionTreeNode(ExpressionTreeNodeType::NONE, "a", std::list<ExpressionTreeNode>());
        std::list<ExpressionTreeNode> list1 = {etn1};
        ExpressionTreeNode etn2 = ExpressionTreeNode(ExpressionTreeNodeType::NONE, "b", std::list<ExpressionTreeNode>());
        ExpressionTreeNode etn3 = ExpressionTreeNode(ExpressionTreeNodeType::NONE, "c", std::list<ExpressionTreeNode>());
        std::list<ExpressionTreeNode> list2 = {etn2, etn3};
        ExpressionTreeNode etn4 = ExpressionTreeNode(ExpressionTreeNodeType::NONE, "g", list1);
        ExpressionTreeNode etn5 = ExpressionTreeNode(ExpressionTreeNodeType::NONE, "h", list2);
        std::list<ExpressionTreeNode> list3 = {etn4, etn5};
        ExpressionTreeNode etn6 = ExpressionTreeNode(ExpressionTreeNodeType::NONE, "f", list3); 
        REQUIRE(arity(etn6) == 5);
    }
}

TEST_CASE("expression tree : == operator", "[ExpressionTree") {
    SECTION("root only") {
        ExpressionTreeNode etn1 = ExpressionTreeNode(ExpressionTreeNodeType::NONE, "", std::list<ExpressionTreeNode>());
        ExpressionTreeNode etn2 = ExpressionTreeNode(ExpressionTreeNodeType::NONE, "", std::list<ExpressionTreeNode>());
        REQUIRE(etn1 == etn2);
    }

    SECTION("root with children") {
        ExpressionTreeNode etn1 = ExpressionTreeNode(ExpressionTreeNodeType::NONE, "", std::list<ExpressionTreeNode>());
        ExpressionTreeNode etn2 = ExpressionTreeNode(ExpressionTreeNodeType::NONE, "", std::list<ExpressionTreeNode>());
        std::list<ExpressionTreeNode> list = {etn1, etn2};
        ExpressionTreeNode etn3 = ExpressionTreeNode(ExpressionTreeNodeType::NONE, "", list);
        ExpressionTreeNode etn4 = ExpressionTreeNode(ExpressionTreeNodeType::NONE, "", list);
        REQUIRE(etn3 == etn4);
    }
}