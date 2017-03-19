#ifndef MERCURE_SYMAT_HH__
#define MERCURE_SYMAT_HH__

#include <iostream>
#include <cstring>

namespace mercure {

template<typename T = double>
class symat {
  T* m_elems;
  size_t* m_n; // n(n-1)/2 for i = [0, m_dim]
  T m_diag;
  size_t m_dim;
  size_t m_size;

public:
  symat(size_t dim, T diag = T(0)) {
    m_diag = diag;
    m_dim = dim;
    m_n = new size_t[dim + 1];
    for (auto n = 0u; n <= dim; ++n) {
      m_n[n] = (n * (n - 1)) >> 1;
    }
    m_size = m_n[dim];
    m_elems = new T[m_size];
  }

  // move!!!

  symat(symat const& that) {
    m_diag = that.m_diag;
    m_dim = that.m_dim;
    m_size = that.m_size;
    m_elems = new T[that.m_size];
    m_n = new size_t[m_dim + 1];
    std::memcpy(m_elems, that.m_elems, m_size * sizeof(T));
    std::memcpy(m_n, that.m_n, (m_dim + 1) * sizeof(size_t));
  }

  ~symat() {
    if (m_elems)
      delete[] m_elems;
    if (m_n)
      delete[] m_n;
  }

  /** Dimension of the matrix (the number of rows and columns). */
  auto dim() const -> size_t {
    return m_dim;
  }

  /** Number of elements in the lower triangle of the matrix. */
  auto size() const -> size_t {
    return m_size;
  }

  /** */
  auto operator()(size_t row, size_t col) const -> T {
    if (row == col)
      return m_diag;
    if (col > row)
      return (*this)(col, row);
    return m_elems[row - 1 - col + m_size - m_n[m_dim - col]];
  }

  /** */
  auto operator()(size_t i) -> T& {
    return m_elems[i];
  }

  /** */
  auto raw_ptr() const -> T* {
    return m_elems;
  }

  auto col_idx(size_t idx) const -> size_t {
    for (auto i = m_dim - 1; i >= 0; --i) {
      if (idx < m_size - m_n[i])
        return m_dim - i - 1;
    }
    return m_dim;
  }

  auto row_idx(size_t idx) const -> size_t {
    auto const col = col_idx(idx);
    return idx + 1 + col - m_size + m_n[m_dim - col];
  }

  /** */
  template<typename T_>
  friend auto operator<<(std::ostream&, symat<T_> const&) -> std::ostream&;
};

template<typename T>
auto operator<<(std::ostream& os, symat<T> const& m) -> std::ostream& {
  for (auto r = 0u; r < m.m_dim; ++r) {
    auto c = 0u;
    for (; c < m.m_dim - 1; ++c) {
      os << m(r, c) << ' ';
    }
    os << m(r, c) << '\n';
  }
  return os;
}

} /* end namespace mercure */

#endif
