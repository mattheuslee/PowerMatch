#pragma once

#ifndef POWERMATCH_HPP_
#define POWERMATCH_HPP_

#include <algorithm>
#include <regex>
#include <sstream>
#include <string>

#include <iostream>

namespace {

/***** utility methods *****/
std::string filter_matches(std::vector<std::string> const & matches) {
    if (matches.size() == 1) {
        return matches[0];
    } else {
        return "";
    }
}

std::string assemble_string(std::vector<std::string> const & chars) {
    std::ostringstream oss;
    for (auto const & c : chars) {
        oss << c;
    }
    return oss.str();
}

std::vector<std::string> get_regex_searches(std::vector<std::string> const & potentialMatches, std::regex const & regex) {
    auto matches = std::vector<std::string>();
    for (auto const & potentialMatch : potentialMatches) {
        if (std::regex_search(potentialMatch, regex)) {
            matches.push_back(potentialMatch);
        }
    }
    return matches;
}

template <class RegexFunc>
std::string basic_get_match(std::vector<std::string> const & potentialMatches, std::string const & keyword, RegexFunc const & getRegex) {
    if (keyword.empty() || potentialMatches.empty()) {
        return "";
    }
    auto matches = get_regex_searches(potentialMatches, getRegex(keyword));
    return filter_matches(matches);
}

std::string basic_get_match(std::vector<std::string> const & potentialMatches, std::vector<std::regex> const & regexes) {
    if (regexes.empty() || potentialMatches.empty()) {
        return "";
    }
    auto matches = std::vector<std::string>();
    for (auto const & regex : regexes) {
        auto subMatches = get_regex_searches(potentialMatches, regex);
        matches.insert(matches.end(), subMatches.begin(), subMatches.end());
    }
    return filter_matches(matches);
}

template <class RegexFunc>
std::vector<std::string> basic_get_matches(std::vector<std::string> const & potentialMatches, std::string const & keyword, RegexFunc const & getRegex) {
    if (keyword.empty() || potentialMatches.empty()) {
        return std::vector<std::string>();
    }
    return get_regex_searches(potentialMatches, getRegex(keyword));
}

std::vector<std::string> basic_get_matches(std::vector<std::string> const & potentialMatches, std::vector<std::regex> const & regexes) {
    if (regexes.empty() || potentialMatches.empty()) {
        return std::vector<std::string>();
    }
    auto matches = std::vector<std::string>();
    for (auto const & regex : regexes) {
        auto subMatches = get_regex_searches(potentialMatches, regex);
        matches.insert(matches.end(), subMatches.begin(), subMatches.end());
    }
    return matches;
}

template <class RegexFunc>
bool basic_is_match(std::string const & potentialMatch, std::string const & keyword, RegexFunc const & getRegex) {
    return std::regex_search(potentialMatch, getRegex(keyword));
}

bool basic_is_match(std::string const & potentialMatch, std::vector<std::regex> const & regexes) {
    for (auto const & regex : regexes) {
        if (std::regex_search(potentialMatch, regex)) {
            return true;
        }
    }
    return false;
}

class SubstringMatcher {

public:
    static std::string get_match(std::vector<std::string> const & potentialMatches, std::string const & keyword) {
        return basic_get_match(potentialMatches, keyword, get_regex_);
    }

    static std::vector<std::string> get_matches(std::vector<std::string> const & potentialMatches, std::string const & keyword) {
        return basic_get_matches(potentialMatches, keyword, get_regex_);
    }

    static bool is_match(std::string const & potentialMatch, std::string const & keyword) {
        return basic_is_match(potentialMatch, keyword, get_regex_);
    }

private:
    static std::regex get_regex_(std::string const & keyword) {
        return std::regex(keyword);
    }

};

class PrefixMatcher {

public:
    static std::string get_match(std::vector<std::string> const & potentialMatches, std::string const & keyword) {
        return basic_get_match(potentialMatches, keyword, get_regex_);
    }

    static std::vector<std::string> get_matches(std::vector<std::string> const & potentialMatches, std::string const & keyword) {
        return basic_get_matches(potentialMatches, keyword, get_regex_);
    }

    static bool is_match(std::string const & potentialMatch, std::string const & keyword) {
        return basic_is_match(potentialMatch, keyword, get_regex_);
    }

private:
    static std::regex get_regex_(std::string const & keyword) {
        return std::regex("\\b" + keyword);
    }

};

class AcronymMatcher {

public:
    static std::string get_match(std::vector<std::string> const & potentialMatches, std::string const & keyword) {
        return basic_get_match(potentialMatches, keyword, get_regex_);
    }

    static std::vector<std::string> get_matches(std::vector<std::string> const & potentialMatches, std::string const & keyword) {
        return basic_get_matches(potentialMatches, keyword, get_regex_);
    }

    static bool is_match(std::string const & potentialMatch, std::string const & keyword) {
        return basic_is_match(potentialMatch, keyword, get_regex_);
    }

private:
    static std::regex get_regex_(std::string const & keyword) {
        auto anyNonWhitespace = std::string("\\S*");
        auto regexStr = std::string("");
        for (auto const & c : keyword) {
            regexStr += c;
            regexStr += anyNonWhitespace;
        }
        return std::regex(regexStr.erase(regexStr.find_last_of("\\")));
    }

};

class PermutationMatcher {

public:
    static std::string get_match(std::vector<std::string> const & potentialMatches, std::string const & keyword) {
        auto regexes = get_regexes_(keyword);
        return basic_get_match(potentialMatches, regexes);
    }

    static std::vector<std::string> get_matches(std::vector<std::string> const & potentialMatches, std::string const & keyword) {
        auto regexes = get_regexes_(keyword);
        return basic_get_matches(potentialMatches, regexes);
    }

    static bool is_match(std::string const & potentialMatch, std::string const & keyword) {
        auto regexes = get_regexes_(keyword);
        return basic_is_match(potentialMatch, regexes);
    }

private:
    static std::vector<std::regex> get_regexes_(std::string const & keyword) {
        auto chars = std::vector<std::string>();
        for (auto const & c : keyword) {
            chars.push_back(std::string("") + c);
        }
        std::sort(chars.begin(), chars.end());
        auto regexes = std::vector<std::regex>();
        do {
            regexes.push_back(std::regex(assemble_string(chars)));
        } while (std::next_permutation(chars.begin(), chars.end()));
        return regexes;
    }

};

class PowerMatch {

public:
    static std::string get_match(std::vector<std::string> const & potentialMatches, std::string const & keyword, int const & level = 0) {
        switch (level) {
        case 0:
            return SubstringMatcher::get_match(potentialMatches, keyword);
        case 1:
            return PrefixMatcher::get_match(potentialMatches, keyword);
        case 2:
            return AcronymMatcher::get_match(potentialMatches, keyword);
        case 3:
            return PermutationMatcher::get_match(potentialMatches, keyword);
        }
        return "";
    }

    static std::vector<std::string> get_matches(std::vector<std::string> const & potentialMatches, std::string const & keyword, int const & level = 0) {
        switch (level) {
        case 0:
            return SubstringMatcher::get_matches(potentialMatches, keyword);
        case 1:
            return PrefixMatcher::get_matches(potentialMatches, keyword);
        case 2:
            return AcronymMatcher::get_matches(potentialMatches, keyword);
        case 3:
            return PermutationMatcher::get_matches(potentialMatches, keyword);
        }
        return std::vector<std::string>();
    }

    static bool is_match(std::string const & potentialMatch, std::string const & keyword, int const & level = 0) {
        switch (level) {
        case 0:
            return SubstringMatcher::is_match(potentialMatch, keyword);
        case 1:
            return PrefixMatcher::is_match(potentialMatch, keyword);
        case 2:
            return AcronymMatcher::is_match(potentialMatch, keyword);
        case 3:
            return PermutationMatcher::is_match(potentialMatch, keyword);
        }
        return "";
    }

};

}

#endif // POWERMATCH_HPP_
