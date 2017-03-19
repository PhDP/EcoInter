#ifndef MERCURE_RECOMMEND_HH__
#define MERCURE_RECOMMEND_HH__

#include <vector>
#include "mercure/common.hh"

namespace mercure {

auto recommendations(
    size_t k,
    std::vector<set<uint32_t>> const& data,
    std::vector<set<uint32_t>> const& traits,
    size_t species_idx,
    double w_t)
    -> std::vector<uint32_t>;

} /* end namespace mercure */

#endif
