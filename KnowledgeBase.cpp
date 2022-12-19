#include "KnowledgeBase.hpp"
#include "KnowledgeBaseUtility.hpp"

#include <iostream>

namespace vtpl {
    std::list<Substitution> KnowledgeBase::ask(const ExpressionTreeNode & query) const {
        log.clear();
        std::list<ExpressionTreeNode> goals = {query.children.front()};
        Substitution s;
        std::list<Substitution> subList = folbc(goals, s);
        std::list<Substitution> removeList;
        for (Substitution s : subList) {
            if (!isValid(apply(query, s))) {
                removeList.push_back(s);
            }
        }
        for (Substitution s : removeList) {
            subList.remove(s);
        }
        return subList;
    }

    std::list<Substitution> KnowledgeBase::folbc(const std::list<ExpressionTreeNode> & goals, const Substitution & s) const {
        if (goals.empty()) {
            return {s};
        }

        std::stack<std::future<std::list<Substitution>>> fut;
        
        std::list<Substitution> answers;
        ExpressionTreeNode q1 = apply(goals.front(), s);
        for (Clause c : clauseList) {
            Clause cTemp = apart(c);

            UnificationResult u;
            unify(cTemp.head, q1, u);
            if (!u.failed && trace) {
                std::cout << "TRACE: Did Fail: " << u.failed << " U1: " << cTemp.head.toString() << " U2: " << q1.toString() << "\n";
                log << "TRACE: Did Fail: " << u.failed << " U1: " << cTemp.head.toString() << " U2: " << q1.toString() << "\n";
            }
            Substitution s2 = u.substitution;

            std::list<ExpressionTreeNode> newGoals(goals.begin(), goals.end());
            newGoals.pop_front();
            if (!u.failed) {
                for (ExpressionTreeNode etn : cTemp.body.children) {
                    if (trace) {
                        std::cout << "TRACE: Adding goal " << etn.toString() << "\n";
                        log << "TRACE: Adding goal " << etn.toString() << "\n";
                    }
                    newGoals.push_front(etn);
                }

                Substitution composition = compose(s2, s);

                fut.push(std::async(&KnowledgeBase::folbc, this, newGoals, composition));
            } 
        }

        while (!fut.empty()) {
            bool isIn = false;
            
            std::list<Substitution> sub1List = fut.top().get();
            fut.pop();
            for (Substitution sub1 : sub1List) {
                for (auto p1 : sub1) {
                    for (auto p2 : sub1) {
                        if (p1.first == p2.first && p1 != p2) {
                            isIn = true;
                        }
                    }
                }

                if (!isIn) {
                    answers.push_back(sub1);
                }
            }
        }
        
        return answers;
    }

    bool KnowledgeBase::isValid(const ExpressionTreeNode etn) const {
        if (etn.type == ExpressionTreeNodeType::VARIABLE) {
            return false;
        }

        auto it = etn.children.begin();
        bool valid = true;

        while (it != etn.children.end()) {
            if (!isValid(*it)) {
                valid = false;
            }
            it++;
        }

        return valid;
    }

    void KnowledgeBase::tell(const Clause & clause) {
        clauseList.push_back(clause);
    }

    std::size_t KnowledgeBase::size() const {
        return clauseList.size();
    }

    KnowledgeBase::Iterator KnowledgeBase::begin() const {
        return clauseList.begin();
    }

    KnowledgeBase::Iterator KnowledgeBase::end() const {
        return clauseList.end();
    }

    void KnowledgeBase::setTrace(bool t) {
        trace = t;
    }

    std::string KnowledgeBase::getLog() {
        return log.str();
    }
}