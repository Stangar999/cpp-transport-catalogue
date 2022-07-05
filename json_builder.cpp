#include "json_builder.h"
#include <array>
#include <tuple>
#include <utility>
#include <variant>
#include <algorithm>

namespace json {
Builder::Builder() {
}

Builder& Builder::Value(Node::Value value) {
    IsReady();
    ExpectKey();
    nodes_stack_.emplace_back(new Node(std::move(value)));
    KeyComplite();
    return *this;
}

DictItemContext Builder::StartDict() {
    IsReady();
    ExpectKey();
    nodes_stack_.emplace_back(new Node(Dict{}));
    stck_lst_call_Dict.push(nodes_stack_.size() - 1 );
    KeyComplite();
    stack_cmds.push(CMDS::DICT);
    return DictItemContext{*this};
}

KeyItemContext Builder::Key(std::string key) {
    IsReady();
    ExpectValue();
    nodes_stack_.emplace_back(new Node(std::move(key)));
    stack_cmds.push(CMDS::KEY);
    return KeyItemContext{*this};
}

Builder& Builder::EndDict() {
    IsReady();
    ExpectValue();
    stack_cmds.pop();
    auto it = nodes_stack_.begin() + stck_lst_call_Dict.top();
    if((*it)->IsDict()){
        Dict cont {};
        for( auto it_d = (it + 1) ; it_d != nodes_stack_.end(); ++it_d ) {
            std::string key = std::move((*it_d)->AsString());
            cont[key] = std::move((*(++it_d))->GetValue());
        }
        (*it)->GetValue() = std::move(cont);
        nodes_stack_.erase((it + 1), nodes_stack_.end());
        stck_lst_call_Dict.pop();
    }
    return *this;
}

ArrayItemContext Builder::StartArray() {
    IsReady();
    ExpectKey();
    nodes_stack_.emplace_back(new Node(Array{}));
    stck_lst_call_Arr.push(nodes_stack_.size() - 1 );
    KeyComplite();
    stack_cmds.push(CMDS::ARR);
    return ArrayItemContext{*this};
}

Builder& Builder::EndArray() {
    IsReady();
    if ( ! stack_cmds.empty() && stack_cmds.top() != CMDS::ARR) {
        throw std::logic_error("Вызов EndArray в неверном месте");
    }
    stack_cmds.pop();
    auto it = nodes_stack_.begin() + stck_lst_call_Arr.top();
    if((*it)->IsArray()){
        stck_lst_call_Arr.pop();
        Array cont {};
        for( auto it_d = (it+1); it_d != nodes_stack_.end(); ++it_d ) {
            cont.emplace_back(std::move((*it_d)->GetValue()));
        }
        (*it)->GetValue() = std::move(cont);
        nodes_stack_.erase((it+1), nodes_stack_.end());
    }
    return *this;
}

Node Builder::Build() {
    if( ! stack_cmds.empty() ) {
        throw std::logic_error("Вызов метода Build при неготовом описываемом объекте");
    }
    if(nodes_stack_.size() != 1) {
        throw std::logic_error("Вызов метода Build сразу после конструктора");
    }
    if( ! nodes_stack_.empty()) {
        root_ = std::move(*nodes_stack_.back());
    }
    return root_;
}

void Builder::IsReady()
{
    if(nodes_stack_.size() == 1 && stack_cmds.empty()) {
        throw std::logic_error("Вызов при готовом объекте");
    }
}

void Builder::ExpectKey()
{
    if ( ! stack_cmds.empty() && stack_cmds.top() == CMDS::DICT) {
        throw std::logic_error("Ожидался вызов key или EndDict");
    }
}

void Builder::KeyComplite()
{
    if ( ! stack_cmds.empty() && stack_cmds.top() == CMDS::KEY) {
        stack_cmds.pop();
    }
}

void Builder::ExpectValue()
{
    if ( ! stack_cmds.empty() && stack_cmds.top() != CMDS::DICT) {
        throw std::logic_error("Вызов EndDict в неверном месте или "
                               "Вызов key вне словаря или после Key");
    }
}

KeyItemContext DictItemContext::Key(std::string key){
    return builder_.Key(std::move(key));
}

}
