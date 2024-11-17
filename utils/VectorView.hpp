#ifndef VECTOR_VIEW_HPP_
#define VECTOR_VIEW_HPP_

#include <memory>
#include <sstream>
#include <stdexcept>
#include <vector>

namespace view {

  /**
   * @brief Read only view on vector
   * Indexes is [start, end)
   * @param end -- end border
   * @param start -- start index
   */
template <typename T> class vector {
  protected:
    const std::vector<T> & data_;
    size_t start_, end_;
  public:
  vector(const std::vector<T> & data, size_t start, size_t end)
    : data_(data), start_(start), end_(end) {};

  vector(const view::vector<T> &other) 
    : data_(other.data_), start_(other.start_), end_(other.end_) {};
  vector(const view::vector<T> &other, size_t start, size_t end) 
    : data_(other.data_), start_(other.start_ + start), end_(other.start_ + end) {};

  vector(vector<T> &&other) noexcept
    : data_(std::move(other.data_)), start_(std::move(other.start_)), end_(std::move(other.end_)) {};

  vector<T> & operator=(vector<T> &other) = delete;
  vector<T> & operator=(vector<T> &&other) = delete;

  const std::vector<T> & data() const {return data_;}


  virtual const T &operator[](size_t i) const {
    if(i >= end_ - start_) 
      throw std::runtime_error((std::ostringstream() << "Invalid index [0" << ", " << end_ - start_ << ") allowed\nGiven: " << i).str());
    return data_.at(start_ + i);
  }

  size_t size() const {
    return end_ - start_;
  }
};

template <typename T> class reverseVector : public vector<T> {
  public:
    reverseVector(const std::vector<T> & data, size_t start, size_t end)
      : vector<T>(data, start, end) {}
    reverseVector(const view::vector<T> &other)
      : vector<T>(other) {};
  reverseVector(const view::vector<T> &other, size_t start, size_t end) 
    : vector<T> (other, start, end) {};
  
  const T& operator[](size_t i) const override {
    if(i >= this->end_ - this->start_) 
      throw std::runtime_error((std::ostringstream() << "Invalid index [0" << ", " << this->end_ - this->start_ << ") allowed\nGiven: " << i).str());

    return this->data_.at(this->end_ - 1 - i);
  }

};

} // namespace view

#endif // !VECTOR_VIEW_HPP_
