#include <cassert>
#include <iostream>
#include <string>
#include <vector>

#include "PowerMatch.hpp"

int main(void) {
    using namespace std;

    // Level 0 : substring match
    auto matchList = vector<string>{"abcxyz", "xyzabc", "xyzabcxyz"};
    assert(PowerMatch::get_match({"xyzabcxyz"}, "abc", 0) == "xyzabcxyz");
    assert(PowerMatch::get_match(matchList, "abc", 0) == "");
    assert(PowerMatch::get_matches(matchList, "abc", 0) == matchList);
    assert(PowerMatch::is_match("xyzabcxyz", "abc", 0));

    // Level 1: prefix match
    matchList = vector<string>{"abcxyz", "abcdef", "abcdefghi"};
    assert(PowerMatch::get_match({"abcxyz"}, "abc", 1) == "abcxyz");
    assert(PowerMatch::get_match(matchList, "abc", 1) == "");
    assert(PowerMatch::get_matches(matchList, "abc", 1) == matchList);
    assert(PowerMatch::is_match("abcxyz", "abc", 1));

    // Level 2: acronym match
    matchList = vector<string>{"daebfc", "adbecf"};
    assert(PowerMatch::get_match({"waxbycz"}, "abc", 2) == "waxbycz");
    assert(PowerMatch::get_match(matchList, "abc", 2) == "");
    assert(PowerMatch::get_matches(matchList, "abc", 2) == matchList);
    assert(PowerMatch::is_match("waxbycz", "abc", 2));

    // Level 3: permutation match
    matchList = vector<string>{"bca", "cba"};
    assert(PowerMatch::get_match({"xcbay"}, "abc", 3) == "xcbay");
    assert(PowerMatch::get_match(matchList, "abc", 3) == "");
    assert(PowerMatch::get_matches(matchList, "abc", 3) == matchList);
    assert(PowerMatch::is_match("xcbay", "abc", 3));

}
