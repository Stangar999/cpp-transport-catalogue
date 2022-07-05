#pragma once
#include <sstream>
#include <stack>

#include "json.h"

namespace json {
class ArrayItemContext;
class KeyItemContext;
class DictItemContext;
class Builder;


class Builder
{
    enum class CMDS {
        ARR,
        DICT,
        KEY
    };

public:
    Builder();
    template<typename T>
    T Value(Node::Value value);
    Builder& Value(Node::Value value);
    DictItemContext StartDict();
    Builder& EndDict();
    ArrayItemContext StartArray();
    Builder& EndArray();
    json::Node Build();
    KeyItemContext Key(std::string key);

private:
    Node root_;
    std::vector<Node*> nodes_stack_;
    std::stack<CMDS> stack_cmds;
    std::stack<size_t> stck_lst_call_Arr;
    std::stack<size_t> stck_lst_call_Dict;

    void IsReady();
    void ExpectKey();
    void KeyComplite();
    void ExpectValue();
};

template<typename T>
T Builder::Value(Node::Value value) {
    IsReady();
    ExpectKey();
    nodes_stack_.emplace_back(new Node(std::move(value)));
    KeyComplite();
    return T{*this};
}

class DictItemContext {
public:
    Builder& builder_;
    Builder& EndDict(){
        return builder_.EndDict();
    };
    KeyItemContext Key(std::string key);
};

class ArrayItemContext {
public:
    Builder& builder_;
    ArrayItemContext StartArray() {
        return builder_.StartArray();
    }
    DictItemContext StartDict() {
        return builder_.StartDict();
    }
    ArrayItemContext Value(Node::Value value) {
        return builder_.Value<ArrayItemContext>(std::move(value));
    }
    Builder& EndArray(){
        return builder_.EndArray();
    };
};

class KeyItemContext {
public:
    Builder& builder_;

    DictItemContext StartDict() {
        return builder_.StartDict();
    }
    ArrayItemContext StartArray() {
        return builder_.StartArray();
    }
    DictItemContext Value(Node::Value value) {
        return builder_.Value<DictItemContext>(std::move(value));
    }
};

}
