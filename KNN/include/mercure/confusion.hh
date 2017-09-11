#ifndef MERCURE_STATISTICS_CONFUSION_HH_
#define MERCURE_STATISTICS_CONFUSION_HH_

#include <iostream>

namespace mercure {

// http://www.dataschool.io/simple-guide-to-confusion-matrix-terminology/
struct confusion {
  size_t tp; // True positives
  size_t tn; // True negatives
  size_t fp; // False positives
  size_t fn; // False negatives

  confusion(size_t true_positives = 0, size_t true_negatives = 0,
            size_t false_positives = 0, size_t false_negatives = 0)
    noexcept : tp(true_positives), tn(true_negatives), fp(false_positives),
               fn(false_negatives) {
  }

  /**
    \brief Total number of observations.
   */
  auto size() const noexcept -> size_t {
    return tp + tn + fp + fn;
  }
};

inline auto accuracy(confusion const& c) noexcept -> double {
  return (c.tp + c.tn) / double(c.size());
}

/**
  \brief True skill statistic (or Hanssen-Kuiper skill score).
 */
inline auto true_skill_statistic(confusion const& c) noexcept -> double {
  return (c.tp * c.tn - c.fp * c.fn) / double((c.tp + c.fp) * (c.tn + c.fn));
}

inline auto error_rate(confusion const& c) noexcept -> double {
  return (c.fp + c.fn) / double(c.size());
}

inline auto recall(confusion const& c) noexcept -> double {
  return c.tp / double(c.tp + c.fp);
}

inline auto specificity(confusion const& c) noexcept -> double {
  return c.tn / double(c.tn + c.fn);
}

inline auto operator<<(std::ostream& os, confusion const& c) noexcept -> std::ostream& {
  os
    << "TSS = " << true_skill_statistic(c) << ", "
    << "accuracy = " << accuracy(c) << ", "
    << "recall (accuracy_1) = " << recall(c) << ", "
    << "specificity (accuracy_0) = " << specificity(c) << " ("
    << "true positives = " << c.tp << ", "
    << "true negatives = " << c.tn << ", "
    << "false positives = " << c.fp << ", "
    << "false negatives = " << c.fn << ')';
  return os;
}

} /* end namespace mercure */

#endif
