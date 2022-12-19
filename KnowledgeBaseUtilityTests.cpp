#include "KnowledgeBaseUtility.hpp"

#include "catch.hpp"

using namespace vtpl;

TEST_CASE("KnowledgeBaseUtility : apply") {
    SECTION("Small apply") {
        ExpressionTreeNode etn1 = ExpressionTreeNode(ExpressionTreeNodeType::ATOM, "a", std::list<ExpressionTreeNode>());
        std::list<ExpressionTreeNode> list1 = {etn1};
        ExpressionTreeNode etn2 = ExpressionTreeNode(ExpressionTreeNodeType::VARIABLE, "X", std::list<ExpressionTreeNode>());
        std::list<ExpressionTreeNode> list2 = {etn2};
        ExpressionTreeNode etn3 = ExpressionTreeNode(ExpressionTreeNodeType::COMPOUND, "f", list1);
        ExpressionTreeNode etn4 = ExpressionTreeNode(ExpressionTreeNodeType::COMPOUND, "f", list2); 
        
        UnificationResult ur;

        unify(etn3, etn4, ur);

        ExpressionTreeNode etn5 = apply(etn4, ur.substitution);

        REQUIRE(etn5 == etn3);
    } 
}

TEST_CASE("KnowledgeBaseUtility : apart") {
    SECTION("Standard Test") {
        ExpressionTreeNode etn1 = ExpressionTreeNode(ExpressionTreeNodeType::VARIABLE, "X", std::list<ExpressionTreeNode>());
        std::list<ExpressionTreeNode> list1 = {etn1};
        ExpressionTreeNode etn2 = ExpressionTreeNode(ExpressionTreeNodeType::COMPOUND, "f", list1);

        ExpressionTreeNode etn3 = ExpressionTreeNode(ExpressionTreeNodeType::ATOM, "a", std::list<ExpressionTreeNode>());
        ExpressionTreeNode etn4 = ExpressionTreeNode(ExpressionTreeNodeType::VARIABLE, "X", std::list<ExpressionTreeNode>());
        std::list<ExpressionTreeNode> list2 = {etn4, etn3};
        ExpressionTreeNode etn5 = ExpressionTreeNode(ExpressionTreeNodeType::COMPOUND, "g", list2);

        Clause c1;
        c1.head = etn2;
        c1.body = etn5;

        ExpressionTreeNode etn6 = ExpressionTreeNode(ExpressionTreeNodeType::VARIABLE, "X_152", std::list<ExpressionTreeNode>());
        std::list<ExpressionTreeNode> list3 = {etn6};
        ExpressionTreeNode etn7 = ExpressionTreeNode(ExpressionTreeNodeType::COMPOUND, "f", list3);

        ExpressionTreeNode etn8 = ExpressionTreeNode(ExpressionTreeNodeType::ATOM, "a", std::list<ExpressionTreeNode>());
        ExpressionTreeNode etn9 = ExpressionTreeNode(ExpressionTreeNodeType::VARIABLE, "X_152", std::list<ExpressionTreeNode>());
        std::list<ExpressionTreeNode> list4 = {etn9, etn8};
        ExpressionTreeNode etn10 = ExpressionTreeNode(ExpressionTreeNodeType::COMPOUND, "g", list4);

        Clause c2;
        c2.head = etn7;
        c2.body = etn10;

        Clause c3 = apart(c1);

        REQUIRE(c3.head == c2.head);
        REQUIRE(c3.body == c2.body);

        ExpressionTreeNode etn11 = ExpressionTreeNode(ExpressionTreeNodeType::VARIABLE, "Y", std::list<ExpressionTreeNode>());
        ExpressionTreeNode etn12 = ExpressionTreeNode(ExpressionTreeNodeType::VARIABLE, "X", std::list<ExpressionTreeNode>());
        std::list<ExpressionTreeNode> list5 = {etn12, etn11};
        ExpressionTreeNode etn13 = ExpressionTreeNode(ExpressionTreeNodeType::COMPOUND, "g", list5);

        Clause c4;
        c4.head = etn13;
        c4.body = etn13;

        Clause c5 = apart(c4);
    }
    
    SECTION("Slightly Invalid Test") {
        ExpressionTreeNode etn1 = ExpressionTreeNode(ExpressionTreeNodeType::VARIABLE, "A", std::list<ExpressionTreeNode>());
        ExpressionTreeNode etn2 = ExpressionTreeNode(ExpressionTreeNodeType::VARIABLE, "B", std::list<ExpressionTreeNode>());
        std::list<ExpressionTreeNode> list1 = {etn1, etn2};
        ExpressionTreeNode etn3 = ExpressionTreeNode(ExpressionTreeNodeType::COMPOUND, "f", list1);

        ExpressionTreeNode etn4 = ExpressionTreeNode(ExpressionTreeNodeType::VARIABLE, "A", std::list<ExpressionTreeNode>());
        ExpressionTreeNode etn5 = ExpressionTreeNode(ExpressionTreeNodeType::VARIABLE, "B", std::list<ExpressionTreeNode>());
        ExpressionTreeNode etn6 = ExpressionTreeNode(ExpressionTreeNodeType::VARIABLE, "C", std::list<ExpressionTreeNode>());
        ExpressionTreeNode etn7 = ExpressionTreeNode(ExpressionTreeNodeType::VARIABLE, "D", std::list<ExpressionTreeNode>());
        std::list<ExpressionTreeNode> list2 = {etn4, etn5, etn6, etn7};
        ExpressionTreeNode etn8 = ExpressionTreeNode(ExpressionTreeNodeType::COMPOUND, "f", list2);

        Clause c1;
        c1.head = etn3;
        c1.body = etn8;

        Clause c3 = apart(c1);

        ExpressionTreeNode etn9 = ExpressionTreeNode(ExpressionTreeNodeType::VARIABLE, "A_155", std::list<ExpressionTreeNode>());
        ExpressionTreeNode etn10 = ExpressionTreeNode(ExpressionTreeNodeType::VARIABLE, "B_156", std::list<ExpressionTreeNode>());
        std::list<ExpressionTreeNode> list3 = {etn9, etn10};
        ExpressionTreeNode etn11 = ExpressionTreeNode(ExpressionTreeNodeType::COMPOUND, "f", list3);

        ExpressionTreeNode etn12 = ExpressionTreeNode(ExpressionTreeNodeType::VARIABLE, "A_155", std::list<ExpressionTreeNode>());
        ExpressionTreeNode etn13 = ExpressionTreeNode(ExpressionTreeNodeType::VARIABLE, "B_156", std::list<ExpressionTreeNode>());
        ExpressionTreeNode etn14 = ExpressionTreeNode(ExpressionTreeNodeType::VARIABLE, "C", std::list<ExpressionTreeNode>());
        ExpressionTreeNode etn15 = ExpressionTreeNode(ExpressionTreeNodeType::VARIABLE, "D", std::list<ExpressionTreeNode>());
        std::list<ExpressionTreeNode> list4 = {etn12, etn13, etn14, etn15};
        ExpressionTreeNode etn16 = ExpressionTreeNode(ExpressionTreeNodeType::COMPOUND, "f", list4);

        Clause c2;
        c2.head = etn11;
        c2.body = etn16;

        REQUIRE(c3.head == c2.head);
        REQUIRE(c3.body == c2.body);
    }
}

TEST_CASE("KnowledgeBaseUtility : compose") {
    ExpressionTreeNode etn1 = ExpressionTreeNode(ExpressionTreeNodeType::ATOM, "a", std::list<ExpressionTreeNode>());
    std::list<ExpressionTreeNode> list1 = {etn1};
    ExpressionTreeNode etn2 = ExpressionTreeNode(ExpressionTreeNodeType::VARIABLE, "X", std::list<ExpressionTreeNode>());
    std::list<ExpressionTreeNode> list2 = {etn2};
    ExpressionTreeNode etn3 = ExpressionTreeNode(ExpressionTreeNodeType::COMPOUND, "f", list1);
    ExpressionTreeNode etn4 = ExpressionTreeNode(ExpressionTreeNodeType::COMPOUND, "f", list2);
    
    Substitution s1, s2;

    s1.insert(etn2, etn1);
    s2.insert(etn2, etn1);

    Substitution s3;
    s3.insert(etn2, etn1);
    s3.insert(etn2, etn1);

    Substitution s4 = compose(s1, s2);

    REQUIRE(s3.lookup(etn2) == s4.lookup(etn2));
}

