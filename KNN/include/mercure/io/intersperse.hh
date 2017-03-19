/**
  \file   intersperse.hh
  \brief  Print containers with some string between all elements.
 */
#ifndef INTERSPERSE_HH__
#define INTERSPERSE_HH__

#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>

namespace mercure {

/**
  \brief Builds a string from a container, interspersing a string between all
  elements. The container needs to support backtracking (--).

  \param first          First element of the container.
  \param last           Last element of the container.
  \param inter          What to insert between the elements.
  \param before_each    Optional: What to add before each element.
  \param after_each     Optional: What to add after each element.
  \return               A string with 'inter' interspersed between all elements of the container.
 */
template<typename Iterator>
auto intersperse(Iterator first, Iterator last,
                 std::string const& inter = ", ",
                 std::string const& before_each = "",
                 std::string const& after_each = "")
                 -> std::string {
  if (first == last)
    return "";

  std::ostringstream oss;
  auto const penultimate = (--last);
  auto it = first;
  for (; it != penultimate; ++it)
    oss << before_each << *it << after_each << inter;
  oss << *it;

  // To support containers' without a std::bidirectional_iterator_tag:
  //   std::ostringstream oss;
  //   for (; first != last; ++first)
  //     oss << before_each << *first << after_each << inter;
  //   auto str = oss.str();
  //   str.erase(str.size() - inter.size(), inter.size());
  //   return str;

  return oss.str();
}

} /* end namespace mercure */

#endif
