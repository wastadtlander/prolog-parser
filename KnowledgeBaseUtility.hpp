#ifndef KNOWLEDGE_BASE_UTILITY_HPP
#define KNOWLEDGE_BASE_UTILITY_HPP

#include <string>
#include <unordered_map>

#include "ExpressionTree.hpp"
#include "KnowledgeBase.hpp"
#include "Unification.hpp"

namespace vtpl {
    // walk the expression tree, if you encounter a variable look it up and replace it 
    // with the value if found (just use the first value in the substitution list). 
    // If the variable is not in the substitution set leave it as is.
    ExpressionTreeNode apply(const ExpressionTreeNode & t, const Substitution & sub);
    
    void applyHelper(ExpressionTreeNode & t, const Substitution & sub);

    // scan the head of the clause for variables, if found modify the name to be unique 
    // and store the information needed in a local dictionary scan the body clause, 
    // replacing variable names from the dictionary built when scanning the head
    // return the modified clause
    Clause apart(const Clause & clause);

    void apartHelper(ExpressionTreeNode & etn, std::unordered_map<std::string, std::string> & umap, bool readOrWrite);

    // for each key k and value v1 in sub1, lookup the v1 in sub2
    // if no substitution for v2 is found in sub2, insert k/v1 into result
    // if a substitution for v2 is found in sub2, for each value v2 found apply sub1 
    // to v2 and insert into result with the key k.
    // finally insert each substitution in sub2 into result
    // return result
    Substitution compose(const Substitution & sub1, const Substitution & sub2);
}

#endif