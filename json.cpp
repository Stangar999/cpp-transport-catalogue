#include "json.h"
#include "sstream"
using namespace std;

namespace json {

namespace {

Node LoadNode(istream& input);

Node LoadArray(istream& input) {
    Array result;
    char c;
    for (; input >> c && c != ']';) {
        if (c != ',') {
            input.putback(c);
        }
        result.push_back(LoadNode(input));
    }
    if (c != ']') {
        // Поток закончился до того, как встретили закрывающую кавычку?
        throw ParsingError("String parsing error");
    }
    return Node(move(result));
}

Node LoadNumber(std::istream& input) {
    using namespace std::literals;

    std::string parsed_num;

    // Считывает в parsed_num очередной символ из input
    auto read_char = [&parsed_num, &input] {
        parsed_num += static_cast<char>(input.get());
        if (!input) {
            throw ParsingError("Failed to read number from stream"s);
        }
    };

    // Считывает одну или более цифр в parsed_num из input
    auto read_digits = [&input, read_char] {
        if (!std::isdigit(input.peek())) {
            throw ParsingError("A digit is expected"s);
        }
        while (std::isdigit(input.peek())) {
            read_char();
        }
    };

    if (input.peek() == '-') {
        read_char();
    }
    // Парсим целую часть числа
    if (input.peek() == '0') {
        read_char();
        // После 0 в JSON не могут идти другие цифры
    } else {
        read_digits();
    }

    bool is_int = true;
    // Парсим дробную часть числа
    if (input.peek() == '.') {
        read_char();
        read_digits();
        is_int = false;
    }

    // Парсим экспоненциальную часть числа
    if (int ch = input.peek(); ch == 'e' || ch == 'E') {
        read_char();
        if (ch = input.peek(); ch == '+' || ch == '-') {
            read_char();
        }
        read_digits();
        is_int = false;
    }

    try {
        if (is_int) {
            // Сначала пробуем преобразовать строку в int
            try {
                return Node(std::stoi(parsed_num));
            } catch (...) {
                // В случае неудачи, например, при переполнении,
                // код ниже попробует преобразовать строку в double
            }
        }
        return Node(std::stod(parsed_num));
    } catch (...) {
        throw ParsingError("Failed to convert "s + parsed_num + " to number"s);
    }
}

Node LoadString(std::istream& input) {
    using namespace std::literals;

    auto it = std::istreambuf_iterator<char>(input);
    auto end = std::istreambuf_iterator<char>();
    std::string s;
    while (true) {
        if (it == end) {
            // Поток закончился до того, как встретили закрывающую кавычку?
            throw ParsingError("String parsing error");
        }
        const char ch = *it;
        if (ch == '"') {
            // Встретили закрывающую кавычку
            ++it;
            break;
        } else if (ch == '\\') {
            // Встретили начало escape-последовательности
            ++it;
            if (it == end) {
                // Поток завершился сразу после символа обратной косой черты
                throw ParsingError("String parsing error");
            }
            const char escaped_char = *(it);
            // Обрабатываем одну из последовательностей: \\, \n, \t, \r, \"
            switch (escaped_char) {
                case 'n':
                    s.push_back('\n');
                    break;
                case 't':
                    s.push_back('\t');
                    break;
                case 'r':
                    s.push_back('\r');
                    break;
                case '"':
                    s.push_back('"');
                    break;
                case '\\':
                    s.push_back('\\');
                    break;
                default:
                    // Встретили неизвестную escape-последовательность
                    throw ParsingError("Unrecognized escape sequence \\"s + escaped_char);
            }
        } else if (ch == '\n' || ch == '\r') {
            // Строковый литерал внутри- JSON не может прерываться символами \r или \n
            throw ParsingError("Unexpected end of line"s);
        } else {
            // Просто считываем очередной символ и помещаем его в результирующую строку
            s.push_back(ch);
        }
        ++it;
    }

    return Node(move(s));
}

std::string AddEscapeInString(std::string str) {
    using namespace std::literals;

    auto it = str.begin();
    auto end = str.end();
    std::string s;
    s.push_back('\"');
    while (it != end) {
        const char escaped_char = *(it);
        // Обрабатываем одну из последовательностей: \\, \n, \t, \r, \"
        switch (escaped_char) {
        case '\n':
            s.push_back('\\');
            s.push_back('n');
            break;
        case '\t':
            s.push_back('\t');
            break;
        case '\r':
            s.push_back('\\');
            s.push_back('r');
            break;
        case '\"':
            s.push_back('\\');
            s.push_back('\"');
            break;
        case '\\':
            s.push_back('\\');
            s.push_back('\\');
            break;
        default:
            s += escaped_char;
            break;
            // Встретили неизвестную escape-последовательность
            //throw ParsingError("Unrecognized escape sequence \\"s + escaped_char);
        }
        ++it;
    }
    s.push_back('\"');
    return s;
}

Node LoadSpecWord(std::istream& input){
    string result;
    char c;
    for (; input >> c && c != ']' && c != '}' && c != ',' && c != ' ' && c != '.';) {
        result.push_back(c);
    }
    input.putback(c); // врозвращвем не прошедший символ обратно та как он нужен в потоке далее
    if(result == "null"s){
        return Node();
    } else if (result == "true"s) {
        return Node(true);
    } else if (result == "false"s) {
        return Node(false);
    } else {
        throw ParsingError("Failed SpecWord"s);
    }
}

Node LoadDict(istream& input) {
    Dict result;
    char c;
    for (; input >> c && c != '}';) {
        if (c == ',') {
            input >> c;
        }

        string key = LoadString(input).AsString();
        input >> c;
        result.insert({move(key), LoadNode(input)});
    }
    if (c != '}') {
        // Поток закончился до того, как встретили закрывающую кавычку?
        throw ParsingError("String parsing error");
    }

    return Node(move(result));
}

Node LoadNode(istream& input) {
    char c;
    input >> c;

    if (c == '[') {
        return LoadArray(input);
    } else if (c == '{') {
        return LoadDict(input);
    } else if (c == '"') {
        return LoadString(input);
    } else if (c == 'n' || c == 't' || c == 'f') {
        input.putback(c);
        return LoadSpecWord(input);
    }else {
        input.putback(c);
        return LoadNumber(input);
    }
}

}  // namespace

Node::Node(Value value)
    :value_(value){;}

const Node::Value &Node::GetValue() const {
    return value_;
}

bool Node::IsInt() const
{
    // std::get_if вернёт указатель на значение нужного типа
    // либо nullptr, если variant содержит значение другого типа.
    return get_if<int>(&value_) != nullptr;

}

bool Node::IsDouble() const
{
    return ( (get_if<double>(&value_) != nullptr) || (get_if<int>(&value_) != nullptr) );
}

bool Node::IsPureDouble() const
{
    return get_if<double>(&value_) != nullptr;
}

bool Node::IsBool() const
{
    return get_if<bool>(&value_) != nullptr;
}

bool Node::IsString() const
{
    return get_if<std::string>(&value_) != nullptr;
}

bool Node::IsNull() const
{
    return get_if<nullptr_t>(&value_) != nullptr; // ATTENTION
}

bool Node::IsArray() const
{
    return get_if<Array>(&value_) != nullptr;
}

bool Node::IsMap() const
{
    return get_if<Dict>(&value_) != nullptr;
}

const Array& Node::AsArray() const {
    if (holds_alternative<Array>(value_)) {
        // Когда мы убедились, что variant сейчас хранит double,
        // можно смело брать это значение при помощи std::get.
        return get<Array>(value_);
    }
    throw std::logic_error("holds_alternative<std::string>(value_)");
}

const Dict& Node::AsMap() const {
    if (holds_alternative<Dict>(value_)) {
        return get<Dict>(value_);
    }
    throw std::logic_error("holds_alternative<std::string>(value_)");
}

int Node::AsInt() const {
    if (holds_alternative<int>(value_)) {
        return get<int>(value_);
    }
    throw std::logic_error("holds_alternative<std::string>(value_)");
}

bool Node::AsBool() const {
    if (holds_alternative<bool>(value_)) {
        return get<bool>(value_);
    }
    throw std::logic_error("holds_alternative<std::string>(value_)");
}

double Node::AsDouble() const
{
    if (holds_alternative<double>(value_) || holds_alternative<int>(value_)) {
        try {
            return get<double>(value_);
        } catch (const bad_variant_access& e) {
            return double(get<int>(value_));
        }
    }
    throw std::logic_error("holds_alternative<std::string>(value_)");
}

const string& Node::AsString() const {
    if (holds_alternative<std::string>(value_)) {
        return get<std::string>(value_);
    }
    throw std::logic_error("holds_alternative<std::string>(value_)");
}

Document::Document(Node root)
    : root_(move(root)) {
}

const Node& Document::GetRoot() const {
    return root_;
}

Document Load(istream& input) {
    return Document{LoadNode(input)};
}

void Print(const Document& doc, std::ostream& output) {
    PrintContext ctx{output};
    PrintNode(doc.GetRoot(), ctx);
}

void PrintValue(std::nullptr_t, const PrintContext& ctx) {
    ctx.out << "null"sv;
}

void PrintValue(const std::string &value, const PrintContext& ctx) {
    std::istringstream str(value);
    ctx.out << AddEscapeInString(value);
}

void PrintValue(bool value, const PrintContext& ctx) {
    boolalpha(ctx.out);
    ctx.out << value;
    noboolalpha(ctx.out);
}

void PrintValue(const Array &value, const PrintContext& ctx) {
    ctx.out << '[' << endl;
    PrintContext p_ctx{ctx.Indented()};
    bool start = true;
    for(const Node& node : value){
        if(!start) {
            p_ctx.out << ',' << endl ;
        }
        p_ctx.PrintIndent();
        PrintNode(node, p_ctx);
        start = false;
    }
    ctx.out << endl;
    ctx.PrintIndent();
    ctx.out << ']' ;
}

void PrintValue(const Dict &value, const PrintContext& ctx) {
    ctx.out << '{' << endl ;
    PrintContext p_ctx{ctx.Indented()};
    bool start = true;
    for(const auto& [key, node] : value){
        if(!start) {
            p_ctx.out << ',' << endl ;
        }
        p_ctx.PrintIndent();
        p_ctx.out <<'\"' << key <<'\"' << ": ";
        PrintNode(node, p_ctx);
        start = false;
    }
    ctx.out << endl;
    ctx.PrintIndent();
    ctx.out << '}' ;
}

void PrintNode(const Node &node, const PrintContext& ctx) {
    std::visit(
                [&ctx](const auto& value){ PrintValue(value, ctx); },
    node.GetValue());
}

bool operator==(const Node &lhs, const Node &rhs)
{
    return lhs.GetValue() == rhs.GetValue();
}

bool operator!=(const Node &lhs, const Node &rhs)
{
    return !(lhs == rhs);
}

bool operator==(const Document &lhs, const Document &rhs)
{
    return lhs.GetRoot() == rhs.GetRoot();
}

bool operator!=(const Document &lhs, const Document &rhs)
{
    return !(lhs == rhs);
}

}  // namespace json
