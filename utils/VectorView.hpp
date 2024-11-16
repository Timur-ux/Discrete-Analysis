#ifndef VECTOR_VIEW_HPP_
#define VECTOR_VIEW_HPP_

#include <memory>
#include <stdexcept>
#include <vector>

namespace view {

using ll = long long;

template <typename T> class vector {
protected:
  std::shared_ptr<std::vector<T>> data_ = nullptr;
  ll start_;
  ll end_;

  vector() = default;
public:
  vector(std::shared_ptr<std::vector<T>> data, ll start, ll end)
      : data_(data), start_(std::max(start, 0ll)), end_(std::min(end, (ll)(data->size()-1))) {}

  vector(vector<T> &other)
      : data_(other.data_), start_(other.start_), end_(other.end_) {}

  vector(vector<T> &&other)
      : data_(std::move(other.data_)), start_(std::move(other.data_)),
        end_(std::move(other.end_)) {}

  vector<T> &operator=(vector<T> &other) {
    data_ = other.data_;
    start_ = other.start_;
    end_ = other.end_;

    return *this;
  }

  virtual ~vector() {}
  
  vector<T> & operator=(vector<T> && other) {
    data_ = std::move(other.data_);
    start_ = std::move(other.start_);
    end_ = std::move(other.end_);

    return *this;
  }

  virtual T& operator[](size_t i) {
    if(!data_.get()) throw std::runtime_error("Vector view: data is null");
    return data_->at(start_ + i);
  }

  virtual size_t size() {
    if(!data_.get()) throw std::runtime_error("Vector view: data is null");
    return end_ - start_ + 1;
  }

  virtual vector& operator+(vector<T> & other) {return *this;}
};

template <typename T> class vectorPair : public vector<T> {
  vector<T> first_;
  vector<T> second_;

  public:
  vectorPair(vector<T> first, vector<T> second) : first_(first), second_(second) {}

  vectorPair(vectorPair<T> &other) {
    first_ = other.first_;
    second_ = other.second_;
  }

  vectorPair(vectorPair<T> && other) {
    first_ = std::move(other.first_);
    second_ = std::move(other.second_);
    
  }

  T& operator[] (size_t i) override {
    if(i < first_.size()) return first_[i];
    else return second_[i - first_.size()];
  }


  size_t size() override {
    return first_.size() + second_.size();
  }
  
  vector<T> & operator+(vector<T> & other) override {
    first_ = vectorPair<T>(first_, second_);
    second_ = other;

    return *this;
  }

};

} // namespace view

#endif // !VECTOR_VIEW_HPP_
