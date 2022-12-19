#include "ExpressionTree.hpp"

#include <cctype>
#include <functional>
#include <queue>
#include <utility>

namespace vtpl {
    ExpressionTreeNode::ExpressionTreeNode() : type(ExpressionTreeNodeType::NONE) {}

    ExpressionTreeNode::ExpressionTreeNode(ExpressionTreeNodeType type, std::string value, std::list<ExpressionTreeNode> children) 
    : type(type), value(value), children(children) {}

    ExpressionTreeNode::ExpressionTreeNode(const ExpressionTreeNode &etn) 
    : ExpressionTreeNode(etn.type, etn.value, etn.children) {}

    ExpressionTreeNode::ExpressionTreeNode(ExpressionTreeNode &&etn) 
    : ExpressionTreeNode(etn.type, etn.value, etn.children) {
        etn.type = ExpressionTreeNodeType::NONE;
        etn.value = "";
        etn.children = std::list<ExpressionTreeNode>();
    }

    ExpressionTreeNode& ExpressionTreeNode::operator=(const ExpressionTreeNode& etn) {
        if (&etn != this) {
            type = etn.type;
            value = etn.value;
            children = etn.children;
        }
        return *this;
    }

    ExpressionTreeNode& ExpressionTreeNode::operator=(ExpressionTreeNode&& etn) {
        if (&etn != this) {
            std::swap(type, etn.type);
            std::swap(value, etn.value);
            std::swap(children, etn.children);
        }
        
        return *this;
    }

    const std::string ExpressionTreeNode::toString() const {
        std::string ret = "";

        toStringHelper(children, ret);

        ret = value + ret;

        return ret;
    }

    void ExpressionTreeNode::toStringHelper(std::list<ExpressionTreeNode> children, std::string &ret) const {
        if (children.empty()) {
            return;
        }

        ret += "(";
        int count = 0;
        for (ExpressionTreeNode etn : children) {
            if (etn == children.front() && count == 0) {
                ret += etn.value;
            } else {
                ret += "," + etn.value;
            }
            count++;
            toStringHelper(etn.children, ret);
        }
        ret += ")";
    }

    std::size_t ExpressionTreeNodeHasher::operator()(const ExpressionTreeNode &node) const {
        std::string ret;

        std::queue<ExpressionTreeNode> q;

        q.push(node);

        while (!q.empty()) { 
            for (ExpressionTreeNode etn : q.front().children) {
                q.push(etn);
            }
            ExpressionTreeNode etn = q.front();
            ret += etn.value;
            switch (etn.type) {
                case ExpressionTreeNodeType::NONE:
                    ret += "NONE" + etn.value;
                    break;
                case ExpressionTreeNodeType::ATOM:
                    ret += "ATOM" + etn.value;
                    break;
                case ExpressionTreeNodeType::COMPOUND:
                    ret += "COMPOUND" + etn.value;
                    break;
                case ExpressionTreeNodeType::VARIABLE:
                    ret += "VARIABLE" + etn.value;
                    break;
            }
            q.pop();
        }

        return std::hash<std::string>{}(ret);
    }

    ExpressionTreeNode makeAtom(const std::string &value) {
        bool isValid = true;

        for (std::size_t i = 1; i < value.size(); i++) {
            if (!isalnum(value[i])  && value[i] != '_') {
                isValid = false;
            }
        }

        if (isValid && !value.empty() && isalpha(value[0]) && islower(value[0])) {
            return ExpressionTreeNode(ExpressionTreeNodeType::ATOM, value, std::list<ExpressionTreeNode>());
        }
        return ExpressionTreeNode(ExpressionTreeNodeType::NONE, value, std::list<ExpressionTreeNode>());
    }

    ExpressionTreeNode makeVariable(const std::string &value) {
        bool isValid = true;

        for (std::size_t i = 1; i < value.size(); i++) {
            if (!isalnum(value[i]) && value[i] != '_') {
                isValid = false;
            }
        }

        if ((isValid && !value.empty() && isalpha(value[0]) && isupper(value[0])) || value[0] == '_') {
            return ExpressionTreeNode(ExpressionTreeNodeType::VARIABLE, value, std::list<ExpressionTreeNode>());
        }
        return ExpressionTreeNode(ExpressionTreeNodeType::NONE, value, std::list<ExpressionTreeNode>());
    }

    ExpressionTreeNode makeCompound(const std::string &value, const std::list<ExpressionTreeNode> &nodes) {
       if (!value.empty() && value[0] >= 'a' && value[0] <= 'z' && !nodes.empty()) {
            return ExpressionTreeNode(ExpressionTreeNodeType::COMPOUND, value, nodes);
        }
        return ExpressionTreeNode(ExpressionTreeNodeType::NONE, value, std::list<ExpressionTreeNode>());
    }

    bool isNone(const ExpressionTreeNode &node) {
        return node.type == ExpressionTreeNodeType::NONE;
    }

    bool isAtom(const ExpressionTreeNode &node) {
        return node.type == ExpressionTreeNodeType::ATOM;
    }

    bool isVariable(const ExpressionTreeNode &node) {
        return node.type == ExpressionTreeNodeType::VARIABLE;
    }
    
    bool isCompound(const ExpressionTreeNode &node) {
        return node.type == ExpressionTreeNodeType::COMPOUND;
    }

    bool isList(const ExpressionTreeNode &node) {
        return isNone(node) && arity(node) >= 1;
    }

    std::size_t arity(const ExpressionTreeNode &node) {
        std::size_t arity = 0;

        std::queue<ExpressionTreeNode> q;

        q.push(node);

        while (!q.empty()) { 
            for (ExpressionTreeNode etn : q.front().children) {
                q.push(etn);
                arity++;
            }
            q.pop();
        }

        return arity;
    }

    bool operator==(const ExpressionTreeNode &node1, const ExpressionTreeNode &node2) {
        std::queue<ExpressionTreeNode> q1, q2;

        q1.push(node1);
        q2.push(node2);

        while (!q1.empty() && !q2.empty()) {
            ExpressionTreeNode etn1 = q1.front(), etn2 = q2.front(); 

            q1.pop(), q2.pop();

            if (etn1.type != etn2.type || etn1.value != etn2.value
                || etn1.children.size() != etn2.children.size()) {
                return false;
            }

            while (!etn1.children.empty()) {
                q1.push(etn1.children.front());
                q2.push(etn2.children.front());
                etn1.children.pop_front();
                etn2.children.pop_front();
            }
        }

        return true;
    }
}