#pragma once

#include <iostream>
#include <string>
#include <vector>

struct Request{
    std::string type;
    std::string data;
};

class InputReader
{
public:
    InputReader(std::istream &input);
    std::vector<Request> Getrequests();
private:
    std::vector<Request> requests;
};

