// 5.exercise.13.cpp
//
// The program is a bit tedious because the answer is hard-coded into the
// program. Make a version where the user can play repeatedly (without stopping
// and restarting the program) and each game has a new set of four digits. You
// can get four random digits by calling the random number generator
// randint(10) from std_lib_facilities.h four times. You will note that if you
// run that program repeatedly, it will pick the same sequence of four digits
// each time you start the program. To avoid that, ask the user to enter
// a number (any number) and call seed_rand(n), also from std_lib_facilities.h,
// where n is the number the user entered before calling randint(10). Such n is
// called a seed, and different seeds give different sequences of random
// numbers.
//
// Comments:
//  I don't really like to ask the user for a number to seed the random
//  generator. I suppose it is another point to check, if the user introduces
//  a number or not, but I prefer to make it transparent and funnier. It seems
//  that a very common way to seed the random generator is to use the
//  time(nullptr) function to get the number of seconds since the Epoch.
//
//  RUN! IT'S A TRAP!
//   - It seems that seed_rand() is not defined in std_lib_facilities.h, so we
//  use srand().
//   - Also randint(10) could return a value of 10, so we must use randint(9) instead.
//  
//  TRAPPED!!!
//      http://stackoverflow.com/questions/31361968/why-isnt-srand-seed-changing-results
//      srand() has no effect on numbers generated by randint(). There are two 
//      solutions, each of them imply to not use randint():
//          - use classical srand() and rand() as inherited from C
//          - do as randint(), in C++ 11 style,  but explicitly in our
//          code (I'll take this one, although obviously advanced).
//
//  To avoid use 4 as a magic number, I declare a constant and made some changes
//  to take it into account. It's not considered in the problem but however ...
//  this enabled us to change easily the program to ramp up difficulty until
//  10 digits.

#include "std_lib_facilities.h"

constexpr int no_of_digits = 4;     // No magic numbers, and some flexibility.

static const string ex_msg_digits_too_large =
        "Not possible to think on more than 10 digits.";
static const string ex_msg_repeated_gen_digits =
        "gen_digits() has failed to constraint digit uniqueness.";
static const string ex_msg_no_no_of_digits =
        "Your input has less or more (or none) digits than those to be guessed.";
static const string ex_msg_repeated_digits =
        "You've entered repeated digits. This way you're never going to guess it!";
static const string ex_msg_distinct_size =
        "Something nasty has happened; guess and/or number to be guessed do not have same length ";

vector<int> gen_digits(int size, default_random_engine& engine,
    uniform_int_distribution<>& dist)
// Generate a vector of ints containing 'size' distinct elements on the range [0, 9].
// Pre-condition:
//  size could not be greater than 10, since each number from [0, 9] could be included
//  exactly once at most.
// Post-condition:
//  Each element of the generated vector must be unique.
{
    vector<int> digits;

    if (size > 10) throw runtime_error(ex_msg_digits_too_large);

    // This loop will end, but not on a determined time, since we are
    // randomizing and each element has to be unique. I'm sure there's a 
    // better way to do it.
    while (digits.size() < size_t(size)) {
        bool unique = true;
        int digit = dist(engine);

        for (size_t i = 0; i < digits.size() && unique; ++i)
            if (digits[i] == digit) unique = false;

        if (unique) digits.push_back(digit);
    }
    
    for (size_t i = 0; i < digits.size() - 1; ++i)
        for (size_t j = i + 1; j < digits.size(); ++j)
            if (i != j && digits[i] == digits[j])
                throw runtime_error(ex_msg_repeated_gen_digits);

    return digits;
}

bool check_input(const string& input)
// Checks input to be a no_of_digits-unique-digit string.
// Pre-conditions (or post-conditions?):
//  The length of the string must be exactly no_of_digits
//  Each character of the string must be an ASCII (or compatible, as ISO8815 or UTF8)
//      value for chars from '0' to '9' (they are consecutive).
//  Each character of the string must be different
try
{
    if (input.length() != no_of_digits) throw runtime_error(ex_msg_no_no_of_digits);

    for (size_t i = 0; i < input.length(); ++i)
        if (input[i] < '0' || input [i] > '9')
            throw runtime_error(ex_msg_no_no_of_digits);

    for (size_t i = 0; i < input.length() - 1; ++i)
        for (size_t j = i + 1; j < input.length(); ++j)
            if (i != j && input[i] == input[j])
                throw runtime_error(ex_msg_repeated_digits);

    return true;
}
catch (exception& e)
{
    cout << e.what() << '\n';
    return false;
}

vector<int> parse_input(const string& input)
// Converts a no_of_digits-unique-digit string to a vector of ints with no_of_digits elements
// Pre-condition:
//  input has to have been checked by check_input()
{
    vector<int> parse;

    for (char d : input)
        parse.push_back(d - '0');

    return parse;
}

int get_bulls(const vector<int>& digits, const vector<int>& guess)
// Get number of positions in two vectors wich have the same value
// Pre-condition:
//  Both vectors have the same number of elements
// Post-condition:
//  bulls must be no_of_digits or less
{
    int bulls = 0;

    if (digits.size() != guess.size())
        throw runtime_error(ex_msg_distinct_size);

    for (size_t i = 0; i < digits.size(); ++i)
        if (digits[i] == guess[i]) ++bulls;

    if (bulls > no_of_digits)
        throw runtime_error(ex_msg_distinct_size);

    return bulls;
}

int get_cows(const vector<int>& digits, const vector<int>& guess)
// Get how many numbers from guess are present on digits but not in
// the same position.
// Post-condition:
//  cows must be no_of_digits or less (given that elements on guess and digits are unique)
{
    int cows = 0;

    for (size_t i = 0; i < guess.size(); ++i)
        for (size_t j = 0; j < digits.size(); ++j)
            if (i != j && guess[i] == digits[j]) ++cows;

    if (cows > no_of_digits)
        throw runtime_error(ex_msg_distinct_size);

    return cows;
}

void print_bulls_and_cows(int bulls, int cows)
// Print number of bulls and cows taking account of singular
// or plural forms.
{
    cout << "You've got ";

    if (bulls == 1) cout << bulls << " bull";
    else cout << bulls << " bulls";

    cout << " and ";

    if (cows == 1) cout << cows << " cow";
    else cout << cows << " cows";
}

int main()
try
{
    string input = "";
    vector<int> guess;
    int bulls = 0;
    int cows = 0;

    // Randomize in C++11 style
    default_random_engine engine(time(nullptr));
    uniform_int_distribution<> dist(0, 9);
    // First number generated from dist seems to be, empirically, always the same;
    // take it and discard it.
    dist(engine);

    cout << "Welcome to Bulls and Cows.\n"
         << "Try to guess the " << no_of_digits << " digit sequence I'm thinking about.\n"
         << "No digit appears more than once and I will give you clues,\n"
         << "with \"bulls\" being correct digits in the right position\n"
         << "and \"cows\" being correct digits in wrong position.\n"
         << "Let's start! Your guess ...\n? ";

    vector<int> digits = gen_digits(no_of_digits, engine, dist);

    while (cin >> input) {
        if (check_input(input)) {
            guess = parse_input(input);
            bulls = get_bulls(digits, guess);
            cows = get_cows(digits, guess);
            if (bulls == no_of_digits) {
                cout << "Congratulations! You guessed right!\n"
                     << "Let's play again ...\n? ";
                bulls = 0;
                cows = 0;
                digits = gen_digits(no_of_digits, engine, dist);
            }
            else {
                print_bulls_and_cows(bulls, cows);
                cout << "\nGuess again!\n? ";
            }
        }
    }
    if (cin.eof()) {
        cout << "Bye!\n";
        return 0;
    }
}
catch (exception& e)
{
    cout << "Error: " << e.what() << '\n';
    return 1;
}
catch (...)
{
    cout << "Unknown exception!\n";
    return 2;
}
