#pragma once
#include <iostream>
#include <string>

class Nif {
public:
    Nif() : number_(0) {}
    Nif(unsigned num) : number_(num) {}
    unsigned get_number() const { return number_; }

    bool operator<(const Nif& other) const { return number_ < other.number_; }
    bool operator>(const Nif& other) const { return number_ > other.number_; }
    bool operator==(const Nif& other) const { return number_ == other.number_; }

    friend std::ostream& operator<<(std::ostream& os, const Nif& nif);

private:
    unsigned number_;
};

std::ostream& operator<<(std::ostream& os, const Nif& nif) {
    os << nif.get_number();
    return os;
}