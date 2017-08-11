#include <cassert>
#include <iostream>

#include <regex>

#include "PowerMatch.hpp"

int main(void) {
    // Level 0 : substring match
    assert(PowerMatch::get_match("abc", {"xyzabcxyz"}, 0) == "xyzabcxyz");

}