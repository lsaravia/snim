/**
  \file   matrix.h
  \brief  Column-major matrix.
 */
#ifndef CONTAINER_MATRIX_HH_
#define CONTAINER_MATRIX_HH_

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <random>
#include <initializer_list>
#include <cassert>

namespace snim {

/**
  \brief A simple column-major matrix of numeric types.
 */
template<typename T >
class matrix {
  T* m_elems;
  size_t m_rows;
  size_t m_cols;
  
  void check(size_t r, size_t c) const { assert(r < m_rows && c <= m_cols); };


 public:
  matrix() : m_elems(nullptr), m_rows(0),m_cols(0) {}
  /**
    \brief Allocated memory for a 'rows' by 'cols' matrix.
   */
  matrix(size_t rows, size_t cols)
    : m_elems((T*)std::malloc(rows * cols * sizeof(T))), m_rows(rows), m_cols(cols) {
  }

  /**
    \brief Builds a matrix from an existing array (copies the pointer, not the array).
   */
  matrix(size_t rows, size_t cols, T* elems)
    : m_elems(elems), m_rows(rows), m_cols(cols) {
  }

  /**
    \brief Builds a 'rows' by 'cols' matrix filled with 'val'.
   */
  matrix(size_t rows, size_t cols, T val)
    : m_elems((T*)std::malloc(rows * cols * sizeof(T))), m_rows(rows), m_cols(cols) {
    for (auto i = 0u; i < rows * cols; ++i)
      m_elems[i] = val;
  }

  /**
    \brief Builds a 'rows' by 'cols' matrix filled with random entries between
           min and max.
   */
  matrix(size_t rows, size_t cols, size_t seed, T min, T max)  // Disable for integers with enable_if.
    : m_elems((T*)std::malloc(rows * cols * sizeof(T))), m_rows(rows), m_cols(cols) {
    auto rng = std::mt19937_64(seed);
    auto d = std::uniform_real_distribution<T>(min, max);
    for (auto i = 0u; i < (m_rows * m_cols); ++i)
      m_elems[i] = d(rng);
  }

  matrix(size_t rows, size_t cols, std::initializer_list<T> const& xs)
    : m_elems((T*)std::malloc(rows * cols * sizeof(T))), m_rows(rows), m_cols(cols) {
    T* es = m_elems;
    for (auto x : xs) {
      *es = x;
      ++es;
    }
  }

  ~matrix() {
    if (m_elems)
      std::free(m_elems);
  }

  matrix(matrix const& other) : m_rows(other.m_rows), m_cols(other.m_cols) {
    size_t const bytes = m_rows * m_cols * sizeof(T);
    m_elems = (T*)std::malloc(bytes);
    std::memcpy(m_elems, other.m_elems, bytes);
  }

  auto operator=(matrix const& other) -> matrix& {
    m_rows = other.m_rows;
    m_cols = other.m_cols;
    size_t const bytes = m_rows * m_cols * sizeof(T);
    T* tmp_elems = (T*)std::malloc(bytes);
    std::memcpy(tmp_elems, other.m_elems, bytes);
    if (m_elems)
      std::free(m_elems);
    m_elems = tmp_elems;
    return *this;
  }

  matrix(matrix&& other) : m_elems(other.m_elems), m_rows(other.m_rows), m_cols(other.m_cols) {
    other.m_elems = nullptr;
  }

  auto operator=(matrix&& other) -> matrix& {
    if (this != &other) {
      m_rows = other.m_rows;
      m_cols = other.m_cols;

      if (m_elems)
        std::free(m_elems);
      m_elems = other.m_elems;
      other.m_elems = nullptr;
    }
    return *this;
  }

  auto operator=(std::initializer_list<T> const& xs) -> matrix& {
    assert(size() == xs.size());
    T* es = m_elems;
    for (auto x : xs) {
      *es = x;
      ++es;
    }

    return *this;
  }

  void resize(size_t rows, size_t cols) {
    m_rows = rows;
    m_cols = cols;
    if (m_elems)
      std::free(m_elems);
    size_t const bytes = m_rows * m_cols * sizeof(T);
    m_elems = (T*)std::malloc(bytes);
  
  }

  /**
    \brief Returns a pointer to the raw data.
   */
  auto data() const -> T* {
    return m_elems;
  }

  /**
    \brief Total number of elements in the matrix.
   */
  auto size() const -> size_t {
    return m_rows * m_cols;
  }

  /**
    \brief Whether the matrix is empty.
   */
  auto empty() const -> size_t {
    return m_rows == 0 || m_cols == 0;
  }

  /**
    \brief Number of bytes allocated on the free store for the elements.
   */
  auto bytes() const -> size_t {
    return size() * sizeof(T);
  }

  /**
    \brief Number of rows.
   */
  auto rows() const -> size_t {
    return m_rows;
  }

  /**
    \brief Number of columns.
   */
  auto cols() const -> size_t {
    return m_cols;
  }

  /**
    \brief Whether there is only one element in the matrix.
   */
  auto is_scalar() const -> bool {
    return m_rows == 1 && m_cols == 1;
  }

  /**
    \brief Whether this is a column vector.
   */
  auto is_column_vec() const -> bool {
    return m_rows > 1 && m_cols == 1;
  }

  /**
    \brief Whether this is a row vector.
   */
  auto is_row_vec() const -> bool {
    return m_rows = 1 && m_cols > 1;
  }

  /**
    \brief Whether this is a (row or column) vector.
   */
  auto is_vector() const -> bool {
    return is_column_vec() || is_row_vec();
  }

  /**
    \brief Whether this is a square matrix (a scalar is a square matrix).
   */
  auto is_square() const -> bool {
    return m_rows == m_cols;
  }

  /**
    \brief Stores a copy of a given column in a pointer (must be large enough to
           contain all the elements of the column).
   */
  auto col_cpy(size_t col, T* c) const -> void {
    assert(col < m_cols);
    std::memcpy(c, m_elems + col * m_rows, m_rows * sizeof(T));
  }

  /**
    \brief Creates a copy of a column and returns a pointer to T.
   */
  auto col_cpy(size_t col) const -> T* {
    assert(col < m_cols);
    auto c = (T*)std::malloc(m_rows * sizeof(T));
    col_cpy(col, c);
    return c;
  }

  /// Returns a copy of a column in a new matrix object
  ///
  auto column(size_t col) const -> matrix {
    assert(col < m_cols);
    matrix<T> c(m_rows,1, col_cpy(col));
    return c;
  }

  
  /**
    \brief Stores a copy of a given row in a pointer (must be large enough to
           contain all the elements of the row).
   */
  auto row_cpy(size_t row, T* r) const -> void {
    assert(row < m_rows);
    for (auto i = 0u; i < m_cols; ++i)
      r[i] = m_elems[i * m_rows + row];
  }

  /**
    \brief Creates a copy of a row (more expansive than copying columns).
   */
  auto row_cpy(size_t row) const -> T* {
    assert(row < m_rows);
    auto r = (T*)std::malloc(m_cols * sizeof(T));
    row_cpy(row, r);
    return r;
  }

  /**
    \brief Sum a row of the matrix.
   */
  auto row_sum(size_t row) const -> T {
    assert(row < m_rows);
    T s=0;
    for (auto i = 0u; i < m_cols; ++i)
       s += m_elems[i * m_rows + row];
    return s;
  }
  
    ///
    ///    Average of a row of the matrix with inclusive limits.
    ///
  auto row_mean(size_t row, size_t from, size_t to) const -> double {
    assert(row < m_rows);
    assert(from < m_cols && to < m_cols);
    to++;
    T s=0;
    for (auto i = from; i < to; ++i)
       s += m_elems[i * m_rows + row];

    return static_cast<double>(s/(to-from));
  }

  /**
    \brief Sum of a column.
   */
  auto col_sum(size_t col) const -> T {
    assert(col < m_cols);

    T s=0;
    for (auto i = 0u; i < m_rows; ++i)
      s += m_elems[col * m_rows + i];
    return s;
  }

   /**
    \brief Get element using the index of the flat column-major internal array.
   */
  auto operator[](size_t idx) const -> T& {
    assert(idx < m_rows*m_cols);
    return m_elems[idx];
  }

  /**
    \brief Get element using the index of the flat column-major internal array.
   */
  auto operator()(size_t idx) const -> T& {
    assert(idx < m_rows*m_cols);
    return m_elems[idx];
  }

  /**
    \brief Get element at a given row/col.
   */
  auto operator()(size_t row, size_t col) const -> T& {
    check(row,col);
    return m_elems[row + col * m_rows];
  }

  template<typename T_>
  friend auto operator<(matrix<T_> const&, matrix<T_> const&) -> bool;

  template<typename T_>
  friend auto operator==(matrix<T_> const&, matrix<T_> const&) -> bool;

  template<typename T_>
  friend auto operator!=(matrix<T_> const&, matrix<T_> const&) -> bool;

  template<typename T_>
  friend auto operator<<(std::ostream&, matrix<T_> const&) -> std::ostream&;

  template<typename T_>
  friend void col_cpy(size_t col, matrix<T_> const&, matrix<T_> & );

};

template<typename T>
void col_cpy( size_t col, matrix<T> const& D, matrix<T> & A ) {
    if(A.cols()>1)
       A(D.m_rows,1);
    D.col_cpy(col,A.m_elems);
            
}

template<typename T>
auto operator<(matrix<T> const& lhs, matrix<T> const& rhs) -> bool {
  bool const size_cmp = lhs.size() != rhs.size();
  if (!size_cmp)
    return size_cmp;
  for (auto i = 0u; i < lhs.size(); ++i) {
    if (lhs[i] != rhs[i]) {
      return lhs[i] < rhs[i];
    }
  }
  return true;
}

template<typename T>
auto operator==(matrix<T> const& lhs, matrix<T> const& rhs) -> bool {
  if (lhs.size() != rhs.size())
    return false;
  for (auto i = 0u; i < lhs.size(); ++i) {
    if (lhs[i] != rhs[i]) {
      return false;
    }
  }
  return true;
}

template<typename T>
auto operator!=(matrix<T> const& lhs, matrix<T> const& rhs) -> bool {
  return !(lhs == rhs);
}

template<typename T>
auto operator<<(std::ostream& os, matrix<T> const& m) -> std::ostream& {
  for (auto r = 0u; r < m.m_rows; ++r) {
    os << '[' << m(r, 0);
    for (auto c = 1u; c < m.m_cols; ++c) {
      os << ", " << m(r, c);
    }
    os << "]\n";
  }
  return os;
}

} /* end namespace */


#endif