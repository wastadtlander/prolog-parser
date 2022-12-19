#include "catch.hpp"

#include "Unification.hpp"

#include <algorithm>

using namespace vtpl;

TEST_CASE("Unification") {
    SECTION("Simple Unification") {
        ExpressionTreeNode etn1 = ExpressionTreeNode(ExpressionTreeNodeType::ATOM, "a", std::list<ExpressionTreeNode>());
        std::list<ExpressionTreeNode> list1 = {etn1};
        ExpressionTreeNode etn2 = ExpressionTreeNode(ExpressionTreeNodeType::VARIABLE, "X", std::list<ExpressionTreeNode>());
        std::list<ExpressionTreeNode> list2 = {etn2};
        ExpressionTreeNode etn3 = ExpressionTreeNode(ExpressionTreeNodeType::COMPOUND, "f", list1);
        ExpressionTreeNode etn4 = ExpressionTreeNode(ExpressionTreeNodeType::COMPOUND, "f", list2); 
        
        UnificationResult ur;

        unify(etn3, etn4, ur);

        REQUIRE(!ur.failed);

        std::list<ExpressionTreeNode> lookupList = ur.substitution.lookup(etn2);

        REQUIRE(std::find(lookupList.begin(), lookupList.end(), etn1) != lookupList.end());

        std::list<ExpressionTreeNode> testList = {etn1};

        REQUIRE(lookupList == testList);
    }

    SECTION("Atom Unification") {
        ExpressionTreeNode etn1 = ExpressionTreeNode(ExpressionTreeNodeType::ATOM, "a", std::list<ExpressionTreeNode>());
        ExpressionTreeNode etn2 = ExpressionTreeNode(ExpressionTreeNodeType::VARIABLE, "X", std::list<ExpressionTreeNode>());
        
        UnificationResult ur;

        unify(etn1, etn2, ur);

        REQUIRE(!ur.failed);

        std::list<ExpressionTreeNode> lookupList = ur.substitution.lookup(etn2);

        REQUIRE(std::find(lookupList.begin(), lookupList.end(), etn1) != lookupList.end());

        std::list<ExpressionTreeNode> testList = {etn1};

        REQUIRE(lookupList == testList);
    }

    SECTION("Duplicate Atom Unification Non-Variable") {
        ExpressionTreeNode etn1 = ExpressionTreeNode(ExpressionTreeNodeType::ATOM, "f", std::list<ExpressionTreeNode>());
        ExpressionTreeNode etn2 = ExpressionTreeNode(ExpressionTreeNodeType::ATOM, "f", std::list<ExpressionTreeNode>());
        
        UnificationResult ur;

        unify(etn1, etn2, ur);

        REQUIRE(!ur.failed);

        std::list<ExpressionTreeNode> lookupList = ur.substitution.lookup(etn1);

        REQUIRE(std::find(lookupList.begin(), lookupList.end(), etn1) == lookupList.end());

        std::list<ExpressionTreeNode> testList = {};

        REQUIRE(lookupList == testList);
    }

    SECTION("Simple Compound Unification") {
        ExpressionTreeNode etn1 = ExpressionTreeNode(ExpressionTreeNodeType::ATOM, "a", std::list<ExpressionTreeNode>());
        ExpressionTreeNode etn2 = ExpressionTreeNode(ExpressionTreeNodeType::ATOM, "b", std::list<ExpressionTreeNode>());
        std::list<ExpressionTreeNode> list1 = {etn1, etn2};
        ExpressionTreeNode etn3 = ExpressionTreeNode(ExpressionTreeNodeType::VARIABLE, "X", std::list<ExpressionTreeNode>());
        ExpressionTreeNode etn4 = ExpressionTreeNode(ExpressionTreeNodeType::ATOM, "b", std::list<ExpressionTreeNode>());
        std::list<ExpressionTreeNode> list2 = {etn3, etn4};
        ExpressionTreeNode etn5 = ExpressionTreeNode(ExpressionTreeNodeType::COMPOUND, "f", list1);
        ExpressionTreeNode etn6 = ExpressionTreeNode(ExpressionTreeNodeType::COMPOUND, "f", list2); 
        
        UnificationResult ur;

        unify(etn5, etn6, ur);

        REQUIRE(!ur.failed);

        std::list<ExpressionTreeNode> lookupList = ur.substitution.lookup(etn3);

        REQUIRE(std::find(lookupList.begin(), lookupList.end(), etn1) != lookupList.end());

        std::list<ExpressionTreeNode> testList = {etn1};

        REQUIRE(lookupList == testList);
    }

    SECTION("Simple Compound Unification") {
        ExpressionTreeNode etn1 = ExpressionTreeNode(ExpressionTreeNodeType::ATOM, "a", std::list<ExpressionTreeNode>());
        ExpressionTreeNode etn2 = ExpressionTreeNode(ExpressionTreeNodeType::ATOM, "b", std::list<ExpressionTreeNode>());
        std::list<ExpressionTreeNode> list1 = {etn1, etn2};
        ExpressionTreeNode etn3 = ExpressionTreeNode(ExpressionTreeNodeType::VARIABLE, "X", std::list<ExpressionTreeNode>());
        ExpressionTreeNode etn4 = ExpressionTreeNode(ExpressionTreeNodeType::VARIABLE, "X", std::list<ExpressionTreeNode>());
        std::list<ExpressionTreeNode> list2 = {etn3, etn4};
        ExpressionTreeNode etn5 = ExpressionTreeNode(ExpressionTreeNodeType::COMPOUND, "f", list1);
        ExpressionTreeNode etn6 = ExpressionTreeNode(ExpressionTreeNodeType::COMPOUND, "f", list2); 
        
        UnificationResult ur;

        unify(etn5, etn6, ur);

        REQUIRE(ur.failed);
    }

    SECTION("Compound Unification") {
        ExpressionTreeNode etn1 = ExpressionTreeNode(ExpressionTreeNodeType::ATOM, "c", std::list<ExpressionTreeNode>());
        ExpressionTreeNode etn2 = ExpressionTreeNode(ExpressionTreeNodeType::ATOM, "b", std::list<ExpressionTreeNode>());
        std::list<ExpressionTreeNode> list1 = {etn2, etn1};
        ExpressionTreeNode etn3 = ExpressionTreeNode(ExpressionTreeNodeType::COMPOUND, "g", list1);
        ExpressionTreeNode etn4 = ExpressionTreeNode(ExpressionTreeNodeType::ATOM, "a", std::list<ExpressionTreeNode>());
        std::list<ExpressionTreeNode> list2 = {etn4, etn3};
        ExpressionTreeNode etn5 = ExpressionTreeNode(ExpressionTreeNodeType::COMPOUND, "f", list2);
        
        ExpressionTreeNode etn6 = ExpressionTreeNode(ExpressionTreeNodeType::VARIABLE, "X", std::list<ExpressionTreeNode>());
        ExpressionTreeNode etn7 = ExpressionTreeNode(ExpressionTreeNodeType::ATOM, "a", std::list<ExpressionTreeNode>());
        std::list<ExpressionTreeNode> list4 = {etn7, etn6};
        ExpressionTreeNode etn8 = ExpressionTreeNode(ExpressionTreeNodeType::COMPOUND, "f", list4);
        
        UnificationResult ur;

        unify(etn5, etn8, ur);

        REQUIRE(!ur.failed);

        std::list<ExpressionTreeNode> lookupList = ur.substitution.lookup(etn6);

        REQUIRE(std::find(lookupList.begin(), lookupList.end(), etn3) != lookupList.end());

        std::list<ExpressionTreeNode> testList = {etn3};

        REQUIRE(lookupList == testList);
    }

    SECTION("Compound Unification Done") {
        ExpressionTreeNode etn1 = ExpressionTreeNode(ExpressionTreeNodeType::ATOM, "c", std::list<ExpressionTreeNode>());
        ExpressionTreeNode etn2 = ExpressionTreeNode(ExpressionTreeNodeType::ATOM, "b", std::list<ExpressionTreeNode>());
        std::list<ExpressionTreeNode> list1 = {etn2, etn1};
        ExpressionTreeNode etn3 = ExpressionTreeNode(ExpressionTreeNodeType::COMPOUND, "g", list1);
        ExpressionTreeNode etn4 = ExpressionTreeNode(ExpressionTreeNodeType::ATOM, "a", std::list<ExpressionTreeNode>());
        std::list<ExpressionTreeNode> list2 = {etn4, etn3};
        ExpressionTreeNode etn5 = ExpressionTreeNode(ExpressionTreeNodeType::COMPOUND, "f", list2);

        ExpressionTreeNode etn6 = ExpressionTreeNode(ExpressionTreeNodeType::ATOM, "c", std::list<ExpressionTreeNode>());
        ExpressionTreeNode etn7 = ExpressionTreeNode(ExpressionTreeNodeType::ATOM, "b", std::list<ExpressionTreeNode>());
        std::list<ExpressionTreeNode> list3 = {etn7, etn6};
        ExpressionTreeNode etn8 = ExpressionTreeNode(ExpressionTreeNodeType::COMPOUND, "g", list3);
        ExpressionTreeNode etn9 = ExpressionTreeNode(ExpressionTreeNodeType::ATOM, "a", std::list<ExpressionTreeNode>());
        std::list<ExpressionTreeNode> list4 = {etn9, etn8};
        ExpressionTreeNode etn10 = ExpressionTreeNode(ExpressionTreeNodeType::COMPOUND, "f", list4);
        
        UnificationResult ur;

        unify(etn5, etn10, ur);

        REQUIRE(!ur.failed);

        std::list<ExpressionTreeNode> lookupList = ur.substitution.lookup(etn1);

        REQUIRE(std::find(lookupList.begin(), lookupList.end(), etn1) == lookupList.end());

        std::list<ExpressionTreeNode> testList = {};

        REQUIRE(lookupList == testList);
    }   

    SECTION("Invalid Unification") {
        ExpressionTreeNode etn1 = ExpressionTreeNode(ExpressionTreeNodeType::ATOM, "a", std::list<ExpressionTreeNode>());
        ExpressionTreeNode etn2 = ExpressionTreeNode(ExpressionTreeNodeType::ATOM, "b", std::list<ExpressionTreeNode>());
        std::list<ExpressionTreeNode> list1 = {etn1, etn2};
        ExpressionTreeNode etn3 = ExpressionTreeNode(ExpressionTreeNodeType::VARIABLE, "X", std::list<ExpressionTreeNode>());
        ExpressionTreeNode etn4 = ExpressionTreeNode(ExpressionTreeNodeType::ATOM, "c", std::list<ExpressionTreeNode>());
        std::list<ExpressionTreeNode> list2 = {etn3, etn4};
        ExpressionTreeNode etn5 = ExpressionTreeNode(ExpressionTreeNodeType::COMPOUND, "f", list1);
        ExpressionTreeNode etn6 = ExpressionTreeNode(ExpressionTreeNodeType::COMPOUND, "f", list2); 
        
        UnificationResult ur;

        unify(etn5, etn6, ur);

        REQUIRE(ur.failed);
    }

    SECTION("Unify Variable Multiple Times") {
        ExpressionTreeNode etn1 = ExpressionTreeNode(ExpressionTreeNodeType::VARIABLE, "X", std::list<ExpressionTreeNode>());
        ExpressionTreeNode etn2 = ExpressionTreeNode(ExpressionTreeNodeType::ATOM, "a", std::list<ExpressionTreeNode>());
        std::list<ExpressionTreeNode> list1 = {etn1, etn2};
        ExpressionTreeNode etn3 = ExpressionTreeNode(ExpressionTreeNodeType::ATOM, "a", std::list<ExpressionTreeNode>());
        ExpressionTreeNode etn4 = ExpressionTreeNode(ExpressionTreeNodeType::VARIABLE, "Y", std::list<ExpressionTreeNode>());
        std::list<ExpressionTreeNode> list2 = {etn3, etn4};
        ExpressionTreeNode etn5 = ExpressionTreeNode(ExpressionTreeNodeType::COMPOUND, "f", list1);
        ExpressionTreeNode etn6 = ExpressionTreeNode(ExpressionTreeNodeType::COMPOUND, "f", list2); 
        
        UnificationResult ur;

        unify(etn5, etn6, ur);

        REQUIRE(!ur.failed);

        std::list<ExpressionTreeNode> lookupListX = ur.substitution.lookup(etn1);
        std::list<ExpressionTreeNode> lookupListY = ur.substitution.lookup(etn4);
    }

    SECTION("Variable Odd Naming") {
        ExpressionTreeNode etn1 = ExpressionTreeNode(ExpressionTreeNodeType::ATOM, "a", std::list<ExpressionTreeNode>());
        ExpressionTreeNode etn2 = ExpressionTreeNode(ExpressionTreeNodeType::ATOM, "b", std::list<ExpressionTreeNode>());
        std::list<ExpressionTreeNode> list1 = {etn1, etn2};
        ExpressionTreeNode etn3 = ExpressionTreeNode(ExpressionTreeNodeType::VARIABLE, "X1", std::list<ExpressionTreeNode>());
        ExpressionTreeNode etn4 = ExpressionTreeNode(ExpressionTreeNodeType::VARIABLE, "X2", std::list<ExpressionTreeNode>());
        std::list<ExpressionTreeNode> list2 = {etn3, etn4};
        ExpressionTreeNode etn5 = ExpressionTreeNode(ExpressionTreeNodeType::COMPOUND, "f", list1);
        ExpressionTreeNode etn6 = ExpressionTreeNode(ExpressionTreeNodeType::COMPOUND, "f", list2); 
        
        UnificationResult ur;

        unify(etn5, etn6, ur);

        REQUIRE(!ur.failed);

        std::list<ExpressionTreeNode> result = ur.substitution.lookup(makeVariable("X1"));
        REQUIRE(result.size() == 1);
        REQUIRE(result.front() == makeAtom("a"));

        result = ur.substitution.lookup(makeVariable("X2"));
        REQUIRE(result.size() == 1);
        REQUIRE(result.front() == makeAtom("b"));
    }
}