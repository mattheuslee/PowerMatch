#pragma once

#ifndef POWERMATCH_HPP_
#define POWERMATCH_HPP_

#include <regex>
#include <string>

namespace {

/***** utility methods *****/
std::string filter_matches(std::vector<std::string> const & matches) {
    if (matches.size() == 1) {
        return matches[0];
    } else {
        return "";
    }
}

class SubstringMatcher {

public:
    static std::string get_match(std::string const & keyword, std::vector<std::string> const & potentialMatches) {
        if (keyword.empty() || potentialMatches.empty()) {
            return "";
        }
        auto matches = get_matches_(get_regex(keyword), potentialMatches);
        return filter_matches(matches);
    }

    static std::vector<std::string> get_matches(std::string const & keyword, std::vector<std::string> const & potentialMatches) {
        if (keyword.empty() || potentialMatches.empty()) {
            return std::vector<std::string>();
        }
        return get_matches_(get_regex(keyword), potentialMatches);
    }

    static bool is_match(std::string const & keyword, std::string const & potentialMatch) {
        return true;
    }

private:
    static std::regex get_regex(std::string const & keyword) {
        auto anyNonWhitespace = std::string("\\w*");
        return std::regex(std::string(anyNonWhitespace + "(" + keyword + ")" + anyNonWhitespace));
    }

    static std::vector<std::string> get_matches_(std::regex const & regex, std::vector<std::string> const & potentialMatches) {
        auto matches = std::vector<std::string>();
        for (auto const & potentialMatch : potentialMatches) {
            if (std::regex_match(potentialMatch, regex)) {
                matches.push_back(potentialMatch);
            }
        }
        return matches;
    }

};

class PowerMatch {

public:
    static std::string get_match(std::string const & keyword, std::vector<std::string> const & potentialMatches, int const & level = 0) {
        switch (level) {
        case 0:
            return SubstringMatcher::get_match(keyword, potentialMatches);
        }
        return "";
    }


};

}

#endif // POWERMATCH_HPP_
