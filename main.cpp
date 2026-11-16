#include <iostream>
#include <stddef.h>

namespace lyuboe_nazvanie
{

class Mtx
{
private:
  size_t rows_;
  size_t cols_;
  int* data_;

public:
  Mtx()
    : rows_(0),
      cols_(0),
      data_(nullptr)
  {
  }

  Mtx(size_t rows, size_t cols)
    : rows_(rows),
      cols_(cols),
      data_(new int[rows * cols])
  {
    for (size_t i = 0; i < rows_ * cols_; ++i) {
      data_[i] = 0;
    }
  }

  Mtx(const Mtx& other)
    : rows_(other.rows_),
      cols_(other.cols_),
      data_(other.rows_ * other.cols_ == 0
        ? nullptr : new int[other.rows_ * other.cols_])
  {
    for (size_t i = 0; i < rows_ * cols_; ++i) {
      data_[i] = other.data_[i];
    }
  }

  Mtx(Mtx&& other) noexcept
    : rows_(other.rows_),
      cols_(other.cols_),
      data_(other.data_)
  {
    other.rows_ = 0;
    other.cols_ = 0;
    other.data_ = nullptr;
  }

  ~Mtx()
  {
    delete[] data_;
  }

  Mtx& operator=(const Mtx& other)
  {
    if (this != &other) {
      delete[] data_;

      rows_ = other.rows_;
      cols_ = other.cols_;

      data_ = rows_ * cols_ == 0
        ? nullptr : new int[rows_ * cols_];

      for (size_t i = 0; i < rows_ * cols_; ++i) {
        data_[i] = other.data_[i];
      }
    }

    return *this;
  }

  Mtx& operator=(Mtx&& other) noexcept
  {
    if (this != &other) {
      delete[] data_;

      rows_ = other.rows_;
      cols_ = other.cols_;
      data_ = other.data_;

      other.rows_ = 0;
      other.cols_ = 0;
      other.data_ = nullptr;
    }

    return *this;
  }

  int& operator()(size_t row, size_t col)
  {
    return data_[row * cols_ + col];
  }

  const int& operator()(size_t row, size_t col) const
  {
    return data_[row * cols_ + col];
  }

  Mtx operator+(const Mtx& other) const
  {
    Mtx result(rows_, cols_);

    for (size_t i = 0; i < rows_ * cols_; ++i) {
      result.data_[i] = data_[i] + other.data_[i];
    }

    return result;
  }

  bool operator==(const Mtx& other) const
  {
    if (rows_ != other.rows_ || cols_ != other.cols_) {
      return false;
    }

    for (size_t i = 0; i < rows_ * cols_; ++i) {
      if (data_[i] != other.data_[i]) {
        return false;
      }
    }

    return true;
  }

  bool operator!=(const Mtx& other) const
  {
    return !(*this == other);
  }

  void print() const
  {
    for (size_t i = 0; i < rows_; ++i) {
      for (size_t j = 0; j < cols_; ++j) {
        std::cout << (*this)(i, j) << ' ';
      }

      std::cout << '\n';
    }
  }
};


class M
{
private:
  size_t k_;
  Mtx* mtxs_;

public:
  M()
    : k_(0),
      mtxs_(nullptr)
  {
  }

  M(size_t k)
    : k_(k),
      mtxs_(k == 0 ? nullptr : new Mtx[k])
  {
  }

  M(const M& other)
    : k_(other.k_),
      mtxs_(k_ == 0 ? nullptr : new Mtx[k_])
  {
    for (size_t i = 0; i < k_; ++i) {
      mtxs_[i] = other.mtxs_[i];
    }
  }

  M(M&& other) noexcept
    : k_(other.k_),
      mtxs_(other.mtxs_)
  {
    other.k_ = 0;
    other.mtxs_ = nullptr;
  }

  ~M()
  {
    delete[] mtxs_;
  }

  M& operator=(const M& other)
  {
    if (this != &other) {
      delete[] mtxs_;

      k_ = other.k_;
      mtxs_ = k_ == 0 ? nullptr : new Mtx[k_];

      for (size_t i = 0; i < k_; ++i) {
        mtxs_[i] = other.mtxs_[i];
      }
    }

    return *this;
  }

  M& operator=(M&& other) noexcept
  {
    if (this != &other) {
      delete[] mtxs_;

      k_ = other.k_;
      mtxs_ = other.mtxs_;

      other.k_ = 0;
      other.mtxs_ = nullptr;
    }

    return *this;
  }

  Mtx& operator[](size_t index)
  {
    return mtxs_[index];
  }

  const Mtx& operator[](size_t index) const
  {
    return mtxs_[index];
  }

  M operator+(const M& other) const
  {
    M result(k_ + other.k_);

    for (size_t i = 0; i < k_; ++i) {
      result.mtxs_[i] = mtxs_[i];
    }

    for (size_t i = 0; i < other.k_; ++i) {
      result.mtxs_[k_ + i] = other.mtxs_[i];
    }

    return result;
  }

  M& operator+=(const M& other)
  {
    *this = *this + other;
    return *this;
  }

  bool operator==(const M& other) const
  {
    if (k_ != other.k_) {
      return false;
    }

    for (size_t i = 0; i < k_; ++i) {
      if (mtxs_[i] != other.mtxs_[i]) {
        return false;
      }
    }

    return true;
  }

  bool operator!=(const M& other) const
  {
    return !(*this == other);
  }

  void insert(size_t index, const Mtx& value)
  {
    M result(k_ + 1);

    for (size_t i = 0; i < index; ++i) {
      result.mtxs_[i] = mtxs_[i];
    }

    result.mtxs_[index] = value;

    for (size_t i = index; i < k_; ++i) {
      result.mtxs_[i + 1] = mtxs_[i];
    }

    *this = result;
  }

  void insert(size_t index, const Mtx* values, size_t count)
  {
    M result(k_ + count);

    for (size_t i = 0; i < index; ++i) {
      result.mtxs_[i] = mtxs_[i];
    }

    for (size_t i = 0; i < count; ++i) {
      result.mtxs_[index + i] = values[i];
    }

    for (size_t i = index; i < k_; ++i) {
      result.mtxs_[i + count] = mtxs_[i];
    }

    *this = result;
  }

  void insert(size_t index, const M& other)
  {
    M copy(other);
    insert(index, copy.mtxs_, copy.k_);
  }

  void erase(size_t index)
  {
    M result(k_ - 1);

    for (size_t i = 0; i < index; ++i) {
      result.mtxs_[i] = mtxs_[i];
    }

    for (size_t i = index + 1; i < k_; ++i) {
      result.mtxs_[i - 1] = mtxs_[i];
    }

    *this = result;
  }

  void erase(size_t first, size_t last)
  {
    M result(k_ - (last - first));

    for (size_t i = 0; i < first; ++i) {
      result.mtxs_[i] = mtxs_[i];
    }

    for (size_t i = last; i < k_; ++i) {
      result.mtxs_[i - (last - first)] = mtxs_[i];
    }

    *this = result;
  }

  void eraseValue(const Mtx& value)
  {
    size_t new_size = 0;

    for (size_t i = 0; i < k_; ++i) {
      if (mtxs_[i] != value) {
        ++new_size;
      }
    }

    M result(new_size);
    size_t j = 0;

    for (size_t i = 0; i < k_; ++i) {
      if (mtxs_[i] != value) {
        result.mtxs_[j] = mtxs_[i];
        ++j;
      }
    }

    *this = result;
  }

  size_t size() const
  {
    return k_;
  }

  bool empty() const
  {
    return k_ == 0;
  }

  void clear()
  {
    delete[] mtxs_;

    mtxs_ = nullptr;
    k_ = 0;
  }

  void print() const
  {
    std::cout << "Number of matrices: " << k_ << '\n';

    for (size_t i = 0; i < k_; ++i) {
      std::cout << "Matrix " << i << ":\n";
      mtxs_[i].print();
      std::cout << '\n';
    }
  }
};


Mtx addMatrices(Mtx a, Mtx b)
{
  return a + b;
}


Mtx conv(const M& arr, Mtx accumulator, Mtx (*t)(Mtx, Mtx))
{
  for (size_t i = 0; i < arr.size(); ++i) {
    accumulator = t(accumulator, arr[i]);
  }

  return accumulator;
}

}


int main()
{
  using namespace lyuboe_nazvanie;

  Mtx a(2, 2);
  Mtx b(2, 2);
  Mtx c(2, 2);

  a(0, 0) = 1;
  a(0, 1) = 2;
  a(1, 0) = 3;
  a(1, 1) = 4;

  b(0, 0) = 5;
  b(0, 1) = 6;
  b(1, 0) = 7;
  b(1, 1) = 8;

  c(0, 0) = 9;
  c(0, 1) = 10;
  c(1, 0) = 11;
  c(1, 1) = 12;

  M matrices;

  matrices.insert(0, a);
  matrices.insert(1, b);
  matrices.insert(2, c);

  std::cout << "Initial array:\n";
  matrices.print();

  std::cout << "First matrix:\n";
  matrices[0].print();

  M copy = matrices;

  M moved = static_cast<M&&>(copy);

  M combined = matrices + moved;

  std::cout << "Combined array:\n";
  combined.print();

  matrices.insert(1, moved);

  std::cout << "After insert:\n";
  matrices.print();

  matrices.erase(0);
  matrices.erase(1, 2);
  matrices.eraseValue(c);

  std::cout << "Size: " << matrices.size() << '\n';
  std::cout << "Empty: " << matrices.empty() << '\n';

  Mtx accumulator(2, 2);

  Mtx result = conv(moved, accumulator, addMatrices);

  std::cout << "Convolution result:\n";
  result.print();

  matrices += moved;

  matrices.clear();

  std::cout << "Empty after clear: " << matrices.empty() << '\n';

  return 0;
}
