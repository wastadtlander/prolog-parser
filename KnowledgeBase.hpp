#ifndef KNOWLEDGE_BASE_HPP
#define KNOWLEDGE_BASE_HPP

#include <future>
#include <list>
#include <sstream>
#include <stack>

#include "ExpressionTree.hpp"
#include "Unification.hpp"

namespace vtpl {
    struct Clause {
        ExpressionTreeNode head;
        ExpressionTreeNode body;
    };

    class KnowledgeBase {
        public:
            KnowledgeBase() = default;

            typedef std::list<Clause>::const_iterator Iterator;

            std::list<Substitution> ask(const ExpressionTreeNode & query) const;

            std::list<Substitution> folbc(const std::list<ExpressionTreeNode> & goals, const Substitution & s) const;

            /// add a clause to the database
            void tell(const Clause & clause);

            /// return the number of clauses in the database
            std::size_t size() const;
            
            /// get an iterator to the first clause
            Iterator begin() const;

            /// get an iterator to one-past the last clause
            Iterator end() const;

            void setTrace(bool t);

            std::string getLog();
        private:
            bool isValid(const ExpressionTreeNode etn) const;

            std::list<Clause> clauseList;
            mutable std::stringstream log;
            bool trace = false;
    };
}

#endif // KNOWLEDGE_BASE_HPP