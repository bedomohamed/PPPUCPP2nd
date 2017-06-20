// 10.exercise.06.roman.h

#include "std_lib_facilities.h"

#ifndef ROMAN_H
#define ROMAN_H

namespace Roman {

class Not_a_particle { };   // Exception when get_particle or lut_idx doesn't
                            // found a particle

struct lut_row {
    string particle;    // particle of Roman number, p.e. M, XL, V or I
    int value;          // associated decimal value
    size_t next;        // next particle to look ahead after this particle
};

constexpr size_t lut_end{100};

class Roman_int {
public:
    Roman_int(string s);
    Roman_int(int a);

    string value() const;
    int as_int() const { return m_value; }
private:
    int m_value;
};

size_t lut_idx(string& particle, size_t start);

string get_particle(istream& is, size_t start);
int parse_roman(istream& is);
string compose_roman(int a);

ostream& operator<<(ostream& os, const Roman_int& rhs);
istream& operator>>(istream& is, Roman_int& rhs);

int parse_roman(istream& is);
} // namespace Roman

#endif