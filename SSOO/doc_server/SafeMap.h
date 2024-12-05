#ifndef SAFEMAP_H
#define SAFEMAP_H

#include <string_view>
#include <sys/mman.h>
#include <unistd.h>

class SafeMap {
 public: 
  SafeMap() noexcept : sv_{}, sz_{0} {}
  SafeMap(std::string_view sv, size_t size) noexcept : sv_{sv}, sz_{size} {}
  SafeMap(const SafeMap&) = delete;
  SafeMap& operator=(const SafeMap&) = delete;
  SafeMap(SafeMap&& other) noexcept : sv_{other.sv_}, sz_{other.sz_} {
    other.sv_ = {};
    other.sz_ = 0;
  }
  SafeMap& operator=(SafeMap&& other) noexcept {
    if (this != &other) {
      unmp();
      sv_ = other.sv_;
      sz_ = other.sz_;
      other.sv_ = {};
      other.sz_ = 0;
    }
    return *this;
  }

  ~SafeMap() {
    unmp();
  }

  [[nodiscard]] bool is_valid() const noexcept {
    return !sv_.empty();
  }

  [[nodiscard]] std::string_view get() const noexcept {
    return sv_;
  }

 private:
  std::string_view sv_;
  size_t sz_;

  void unmp() {
    if (is_valid()) {
      munmap(const_cast<char*>(sv_.data()), sz_);
    }
  }
};

#endif