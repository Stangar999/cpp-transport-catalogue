#pragma once

#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <variant>

namespace json {

using namespace std::literals;

class Node;
// Сохраните объявления Dict и Array без изменения
using Dict = std::map<std::string, Node>;
using Array = std::vector<Node>;

// Эта ошибка должна выбрасываться при ошибках парсинга JSON
class ParsingError : public std::runtime_error {
public:
    using runtime_error::runtime_error;
};

class Node {
public:
    using Value = std::variant<std::nullptr_t, int, double, std::string, bool, Array, Dict>;
    Node() = default;
    Node(Value value);

    template <typename T>
    Node(T value):value_(value){;}

    const Value& GetValue() const;

    bool IsInt() const;
    bool IsDouble() const; // Возвращает true, если в Node хранится int либо double.
    bool IsPureDouble() const; // Возвращает true, если в Node хранится double.
    bool IsBool() const;
    bool IsString() const;
    bool IsNull() const;
    bool IsArray() const;
    bool IsMap() const;

    int AsInt() const;
    bool AsBool() const;
    // Возвращает значение типа double, если внутри хранится double либо int.
    //В последнем случае возвращается приведённое в double значение.
    double AsDouble() const;
    const std::string& AsString() const;
    const Array& AsArray() const;
    const Dict& AsMap() const;

private:
    Value value_;
};

bool operator==(const Node& lhs, const Node& rhs);
bool operator!=(const Node& lhs, const Node& rhs);

class Document {
public:
    explicit Document(Node root);

    const Node& GetRoot() const;

private:
    Node root_;
};

bool operator==(const Document& lhs, const Document& rhs);
bool operator!=(const Document& lhs, const Document& rhs);

Document Load(std::istream& input);

// Контекст вывода, хранит ссылку на поток вывода и текущий отсуп
struct PrintContext {
    std::ostream& out;
    int indent_step = 4;
    int indent = 0;

    void PrintIndent() const {
        for (int i = 0; i < indent; ++i) {
            out.put(' ');
        }
    }

    // Возвращает новый контекст вывода с увеличенным смещением
    PrintContext Indented() const {
        return {out, indent_step, indent_step + indent};
    }
};

// Шаблон, подходящий для вывода double и int
template <typename Value>
void PrintValue(const Value& value, const PrintContext& ctx) {
    ctx.out << value;
}

// Перегрузка функции PrintValue для вывода значений null
void PrintValue(std::nullptr_t, const PrintContext& ctx);

void PrintValue(const std::string& value, const PrintContext& ctx);

void PrintValue(bool value, const PrintContext& ctx);

void PrintValue(const Array& value, const PrintContext& ctx);

void PrintValue(const Dict& value, const PrintContext& ctx);

void PrintNode(const Node& node, const PrintContext& ctx);

void Print(const Document& doc, std::ostream& output);

}  // namespace json
