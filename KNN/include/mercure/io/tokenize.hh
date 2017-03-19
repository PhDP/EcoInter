/**
  \file   tokenize.hh
  \brief  Simple tokenizer.
 */
#ifndef TOKENIZE_HH__
#define TOKENIZE_HH__

#include <string>
#include <vector>
#include <functional>

namespace mercure {

/**
  \brief Creates a function to test membership.

  \param s   The set of characters as a string.
  \return    A function that returns true if its input character is in the set.
 */
inline auto member_of(std::string const& s) -> std::function<bool(char)> {
  return [=](char const c) -> bool {
    for (char const& x : s) {
      if (c == x)
        return true;
    }
    return false;
  };
}

/**
  \brief Build a tokenizer given a set of characters (represented by a string)
  to split the string and ignore, and a set to spit the string and keep as a
  token.

  \param split_ignore    Split the string at these characters but don't add as tokens.
  \param split_keep      Split the string at these characters and add as tokens.
  \return                A vector of tokens.
 */
inline auto build_tokenizer(std::string const& si,
                            std::string const& sk)
                            -> std::function<std::vector<std::string>(std::string)> {
  return [=](std::string const& s) -> std::vector<std::string> {
    auto const split_ignore = member_of(si);
    auto const split_keep = member_of(sk);

    auto tks = std::vector<std::string> {};

    auto i = 0u, len = 0u;
    for (; i < s.size(); ++i) {
      if (split_ignore(s[i])) {
        if (len > 0) {
          tks.push_back(s.substr(i - len, len));
          len = 0;
        }
      } else if (split_keep(s[i])) {
        if (len > 0) {
          tks.push_back(s.substr(i - len, len));
          len = 0;
        }
        tks.push_back(std::string { s[i] });
      } else {
        ++len;
      }
    }
    if (len > 0) {
      tks.push_back(s.substr(i - len, len));
    }
    return tks;
  };
}

}

#endif
