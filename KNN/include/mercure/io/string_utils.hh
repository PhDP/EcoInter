/**
  \file   string_utils.hh
  \brief  Useful function to handle strings.
 */
#ifndef IO_STRING_UTILS_HH__
#define IO_STRING_UTILS_HH__

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <set>
#include <map>
#include <algorithm>
#include <cerrno>
#include <cstdlib>
#include <cctype>
#include "mercure/common.hh"

namespace mercure {

/**
  \brief Returns true for lines made only of spaces and end-of-line characters.
 */
inline auto blank_line(std::string const& s) -> bool {
  return std::all_of(s.cbegin(), s.cend(), [](char c) {
    return c == ' ' || c == '\n'; }
  );
}

/**
  \brief Reads an entire file into a single string.
 */
inline auto read_file(char const* filename) -> std::string {
  std::ifstream in(filename, std::ios::in | std::ios::binary);
  if (in) {
    auto contents = std::string{};
    in.seekg(0, std::ios::end);
    contents.resize(in.tellg());
    in.seekg(0, std::ios::beg);
    in.read(&contents[0], contents.size());
    in.close();
    return(contents);
  }
  throw(errno);
}

/**
  \brief Split a string given a deliminator, into a vector of strings.
 */
inline auto split(std::string const& s, char delim) -> std::vector<std::string> {
  std::stringstream ss(s);
  auto tks = std::vector<std::string>{};
  auto item = std::string{};
  while (std::getline(ss, item, delim)) {
    tks.push_back(item);
  }
  return tks;
}

/**
  \brief Reads a csv file into sets.
 */
inline auto csv_into_sets(char const* filename) -> std::vector<set<uint32_t>> {

  auto const lines = split(read_file(filename), '\n');
  auto dataset = std::vector<set<uint32_t>>{};
  for (auto const& line : lines) {
    auto new_entry = set<uint32_t>{};
    auto values = split(line, ',');
    for (auto const& v : values) {
      if (!blank_line(v))
        new_entry.insert(std::atoi(v.c_str()));
    }
    dataset.push_back(new_entry);
  }
  return dataset;
}

inline auto csv_into_vectors(char const* filename) -> std::vector<std::vector<double>> {
  auto const lines = split(read_file(filename), '\n');
  auto dataset = std::vector<std::vector<double>>{};
  for (auto const& line : lines) {
    auto new_entry = std::vector<double>{};
    auto values = split(line, ',');
    for (auto const& v : values) {
      if (!blank_line(v))
        new_entry.push_back(std::atof(v.c_str()));
    }
    dataset.push_back(new_entry);
  }
  return dataset;
}

template<typename T, typename V>
auto set_to_vector(set<T> const& s, size_t size) -> std::vector<V> {
  auto xs = std::vector<V>(size, V(0));
  for (auto const& i : s) {
    if (i < xs.size()) {
      xs[i] = V(1);
    }
  }
  return xs;
}

/**
  \brief Reads a csv file into vectors.
 */
inline auto csv_sets_into_vectors(char const* filename, size_t size) -> std::vector<std::vector<uint32_t>> {
  auto dataset = std::vector<std::vector<uint32_t>>{};
  auto sets = csv_into_sets(filename);
  for (auto const& s : sets)
    dataset.push_back(set_to_vector<uint32_t, uint32_t>(s, size));
  return dataset;
}

/**
  \brief Tests whether the string represents a number (positive, negative, integer or float).
 */
inline auto is_number(std::string const& s) -> bool {
  auto start = s.begin() + (s[0] == '-');
  return
    !s.empty() &&
    s.end() == std::find_if(start, s.end(), [](char c) {
      return !std::isdigit(c) && c != '.';
    }) &&
    std::count(start, s.end(), '.') <= 1;
}

} /* end namespace mercure */

#endif
