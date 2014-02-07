#include <iterator>
#include <memory>
#include <vector>

namespace ptr_vec {
namespace detail {
   template <typename T>class _ptr_vec_itr;
   template <typename T>class _ptr_vec_const_itr;
}
template <typename T, typename Alloc = std::allocator<std::unique_ptr<T>>>
class ptr_vector {
   std::vector<std::unique_ptr<T>, Alloc> _vec;
   using _ptr_t = std::unique_ptr<T>;
public:
   using value_type             = T;
   using allocator_type         = Alloc;
   using size_type              = typename std::vector<std::unique_ptr<T>>::size_type;
   using difference_type        = typename std::vector<std::unique_ptr<T>>::difference_type;
   using reference              = T&;
   using const_reference        = const T&;
   using pointer                = T*;
   using const_pointer          = const T*;
   using iterator               = typename detail::_ptr_vec_itr<T>;
   using const_iterator         = typename detail::_ptr_vec_const_itr<T>;
   using reverse_iterator       = std::reverse_iterator<iterator>;
   using const_reverse_iterator = std::reverse_iterator<const_iterator>;

   ptr_vector(): _vec() { }
   ptr_vector(const ptr_vector&) = delete; // no copy semantics
   ptr_vector(ptr_vector&&) noexcept = default;

   template <typename U>
   ptr_vector(const ptr_vector<U>&) = delete;

   template <typename U>
   ptr_vector(ptr_vector<U>&& sink) noexcept: _vec(std::move(sink)) { } // U must be implicitly convertable to T

   ptr_vector& operator=(const ptr_vector&) = delete;
   ptr_vector& operator=(ptr_vector&& sink) = default;

   template <typename U>
   ptr_vector& operator=(const ptr_vector&) = delete;

   template <typename U> // U must be implicitly convertable to T
   ptr_vector& operator=(ptr_vector<U>&& sink) noexcept {_vec = std::move(sink);return *this;}

   reference at(std::size_t i) {return *_vec.at(i);}
   const_reference at(std::size_t i) const {return *_vec.at(i);}

   reference operator[](std::size_t i) noexcept {return *_vec[i];}
   const_reference operator[](std::size_t i) const noexcept {return *_vec[i];}

   reference back() {return *_vec.back();}
   const_reference back() const {return *_vec.back();}
   reference front() {return *_vec.front();}
   const_reference front() const {return *_vec.front();}

   iterator begin() noexcept {return iterator(_vec.begin());}
   iterator end() noexcept {return iterator(_vec.end());}
   const_iterator begin() const noexcept {return cbegin();}
   const_iterator end() const noexcept {return cend();}
   const_iterator cbegin() const noexcept {return const_iterator(_vec.cbegin());}
   const_iterator cend() const noexcept {return const_iterator(_vec.cend());}
   reverse_iterator rbegin() noexcept {return reverse_iterator(end());}
   reverse_iterator rend() noexcept {return reverse_iterator(begin());}
   const_reverse_iterator rbegin() const noexcept {return crbegin();}
   const_reverse_iterator rend() const noexcept {return crend();}
   const_reverse_iterator crbegin() const noexcept {return const_reverse_iterator(cend());}
   const_reverse_iterator crend() const noexcept {return const_reverse_iterator(cbegin());}

   bool empty() const noexcept {return _vec.empty();}
   size_type size() const noexcept {return _vec.size();}

   void reserve(size_type n) {_vec.reserve(n);}
   size_type capacity() const noexcept {return _vec.capacity();}
   void shrink_to_fit() {_vec.shrink_to_fit();}

   void clear() {_vec.clear();}

   iterator insert(iterator pos, const_reference value) {return iterator{_vec.insert(pos._itr, _ptr_t(new value_type(value)))};}
   template <typename U> // U must be implicitly convertable to T
   iterator insert(iterator pos, typename std::remove_reference<U&>::type const& value) {return iterator{_vec.insert(pos._itr, _ptr_t(new U(value)))};}

   iterator insert(const_iterator pos, const_reference value) {return iterator{_vec.insert(pos._citr, _ptr_t(new value_type(value)))};}
   template <typename U> // U must be implicitly convertable to T
   iterator insert(const_iterator pos, typename std::remove_reference<U&>::type const& value) {return iterator{_vec.insert(pos._citr, _ptr_t(new U(value)))};}

   iterator insert(const_iterator pos, value_type&& value) {return iterator{_vec.insert(pos._citr, _ptr_t(new value_type(std::move(value))))};}
   template <typename U> // U must be implicitly convertable to T
   iterator insert(const_iterator pos, typename std::remove_reference<U&>::type&& value) {return iterator{_vec.insert(pos._citr, _ptr_t(new U(std::move(value))))};}

   iterator insert(const_iterator pos, size_type count, const_reference value) {
      iterator it{begin() + std::distance(cbegin(), pos)};
      for (size_type n = 0;n != count;++n) {
         it = insert(it, value);
      }
      return it;
   }
   template <typename U> // U must be implicitly convertable to T
   iterator insert(const_iterator pos, size_type count, typename std::remove_reference<U&>::type const& value) {
      iterator it{begin() + std::distance(cbegin(), pos)};
      for (size_type n = 0;n != count;++n) {
         it = insert(it, value);
      }
      return it;
   }

   template <typename... Args>
   iterator emplace(iterator pos, Args&&... args) {return iterator{_vec.emplace(pos._itr, new value_type(std::forward<Args>(args)...))};} // note: const_iterators don't work in GCC for some reason...
   template <typename U, typename... Args> // U must be implicitly convertable to T
   iterator emplace(iterator pos, Args&&... args) {return iterator{_vec.emplace(pos._itr, new U(std::forward<Args>(args)...))};}

   iterator erase(iterator pos) {return iterator{_vec.erase(pos._itr)};}
   iterator erase(const_iterator pos) {return iterator{_vec.erase(pos._citr)};}
   iterator erase(iterator first, iterator last) {return iterator{_vec.erase(first._itr, last._itr)};}
   iterator erase(const_iterator first, const_iterator last) {return iterator{_vec.erase(first._citr, last._citr)};}

   void push_back(const_reference value) {_vec.push_back(_ptr_t(new value_type(value)));}
   template <typename U> // U must be implicitly convertable to T
   void push_back(typename std::remove_reference<U&>::type const& value) {_vec.push_back(_ptr_t(new U(value)));}

   void push_back(value_type&& value) {_vec.push_back(_ptr_t(new value_type(std::move(value))));}
   template <typename U> // U must be implicitly convertable to T
   void push_back(typename std::remove_reference<U&>::type&& value) {_vec.push_back(_ptr_t(new U(std::move(value))));}

   template <typename... Args>
   void emplace_back(Args&&... args) {_vec.emplace_back(new value_type{std::forward<Args>(args)...});}
   template <typename U, typename... Args> // U must be implicitly convertable to T
   void emplace_back(Args&&... args) {_vec.emplace_back(new U(std::forward<Args>(args)...));}

   void pop_back() {_vec.pop_back();}

   void resize(size_type count, const_reference value) { // capture by const ref so we can instantiate a vector of an abstract type
      if (_vec.size() > count) {
         for (size_type n = _vec.size() - count;n != 0;--n) {
            pop_back();
         }
      }
      else {
         for (size_type n = count - _vec.size();n != 0;--n) {
            push_back(value);
         }
      }
   }

   template <typename U>
   void resize(size_type count, typename std::remove_reference<U&>::type const& value) {
      if (_vec.size() > count) {
         for (size_type n = _vec.size() - count;n != 0;--n) {
            pop_back();
         }
      }
      else {
         for (size_type n = count - _vec.size();n != 0;--n) {
            push_back<U>(value);
         }
      }
   }

   void swap(ptr_vector& other) {_vec.swap(other);}

   void reset_at(std::size_t pos, const_reference value) {_vec.at(pos).reset(new value_type(value));}
   template <typename U> // U must be implicitly convertable to T
   void reset_at(std::size_t pos, typename std::remove_reference<U&>::type const& value) {_vec.at(pos).reset(new U(value));}

   void reset_at(std::size_t pos, value_type&& value) {_vec.at(pos).reset(new value_type(std::move(value)));}
   template <typename U> // U must be implicitly convertable to T
   void reset_at(std::size_t pos, typename std::remove_reference<U&>::type&& value) {_vec.at(pos).reset(new U(std::move(value)));}

   template <typename... Args>
   void emplace_reset_at(std::size_t pos, Args&&... args) {_vec.at(pos).reset(new value_type(std::forward<Args>(args)...));}
   template <typename U, typename... Args> // U must be implicitly convertable to T
   void emplace_reset_at(std::size_t pos, Args&&... args) {_vec.at(pos).reset(new U(std::forward<Args>(args)...));}

   void push_back_own(pointer value) {if (value != nullptr) _vec.emplace_back(value);}
   template <typename U> // U must be implicitly convertable to T
   void push_back_own(typename std::remove_reference<U&>::type* value) {if (value != nullptr) _vec.emplace_back(value);}

   iterator insert_own(iterator itr, pointer value) {if (value != nullptr) iterator{_vec.emplace(itr._itr, value)};}
   template <typename U> // U must be implicitly convertable to T
   iterator insert_own(iterator itr, typename std::remove_reference<U&>::type* value) {if (value != nullptr) iterator{_vec.emplace(itr._itr, value)};}
};
namespace detail {
   template <typename T>
   class _ptr_vec_itr : public std::iterator<std::random_access_iterator_tag, T>{
      using _impl_itr  = typename std::vector<std::unique_ptr<T>>::iterator;
      using _this_base = std::iterator<std::random_access_iterator_tag, T>;
      _impl_itr _itr;
      friend class ptr_vector<T>;
      friend class _ptr_vec_const_itr<T>;
   public:
      using value_type        = typename _this_base::value_type;
      using difference_type   = typename _this_base::difference_type;
      using pointer           = typename _this_base::pointer;
      using reference         = typename _this_base::reference;
      using iterator_category = typename _this_base::iterator_category;

      _ptr_vec_itr(_impl_itr itr) noexcept: _itr(itr) { }
      _ptr_vec_itr() noexcept: _itr() { }
      _ptr_vec_itr(const _ptr_vec_itr&) noexcept = default; // no need to make a move ctor

      _ptr_vec_itr& operator=(const _ptr_vec_itr&) noexcept = default; // no need to make a move operator=
      reference operator*() const noexcept {return **_itr;}
      pointer operator->() const noexcept {return &**this;}
      // fwd-itr
      _ptr_vec_itr& operator++() noexcept {++_itr;return *this;}
      _ptr_vec_itr operator++(int) noexcept {auto tmp = *this;++*this;return tmp;}
      bool operator!=(const _ptr_vec_itr& rhs) const noexcept {return _itr != rhs._itr;}
      bool operator==(const _ptr_vec_itr& rhs) const noexcept {return !(*this != rhs);}
      // bi-dir
      _ptr_vec_itr& operator--() noexcept {--_itr;return *this;}
      _ptr_vec_itr operator--(int) noexcept {auto tmp = *this;--*this;return tmp;}
      // rnd-acc
      _ptr_vec_itr& operator+=(std::size_t n) noexcept {_itr += n;return *this;}
      _ptr_vec_itr& operator-=(std::size_t n) noexcept {*this += -n;return *this;}
      _ptr_vec_itr operator-(std::size_t n) const noexcept {auto tmp = *this;return (tmp -= n);}
      difference_type operator-(const _ptr_vec_itr& rhs) const noexcept {return _itr - rhs._itr;}
      reference operator[](std::size_t n) noexcept {return *_itr[n];}
      bool operator<(const _ptr_vec_itr& rhs) const noexcept {return _itr < rhs._itr;}
      bool operator>(const _ptr_vec_itr& rhs) const noexcept {return _itr > rhs._itr;}
      bool operator<=(const _ptr_vec_itr& rhs) const noexcept {return !(*this > rhs);}
      bool operator>=(const _ptr_vec_itr& rhs) const noexcept {return !(*this < rhs);}
   };
   template <typename T>
   _ptr_vec_itr<T> operator+(const _ptr_vec_itr<T>& lhs, std::size_t rhs) {
      auto tmp = lhs;
      tmp += rhs;
      return tmp;
   }
   template <typename T>
   _ptr_vec_itr<T> operator+(std::size_t lhs, const _ptr_vec_itr<T>& rhs) {
      return rhs + lhs;
   }

   template <typename T>
   class _ptr_vec_const_itr : public std::iterator<std::random_access_iterator_tag, const T> {
      using _impl_itr  = typename std::vector<std::unique_ptr<T>>::const_iterator;
      using _this_base = std::iterator<std::random_access_iterator_tag,const T>;
      _impl_itr _citr;
      friend class ptr_vector<T>;
   public:
      using value_type        = typename _this_base::value_type;
      using difference_type   = typename _this_base::difference_type;
      using pointer           = typename _this_base::pointer;
      using reference         = typename _this_base::reference;
      using iterator_category = typename _this_base::iterator_category;

      _ptr_vec_const_itr(_impl_itr itr) noexcept: _citr(itr) { }
      _ptr_vec_const_itr() noexcept: _citr() { }
      _ptr_vec_const_itr(const _ptr_vec_const_itr&) noexcept = default; // no need for move ctor
      _ptr_vec_const_itr(const _ptr_vec_itr<T>& itr): _citr(itr._itr) { }

      _ptr_vec_const_itr& operator=(const _ptr_vec_const_itr&) noexcept = default; /// no need to make move operator=
      reference operator*() const noexcept {return **_citr;}
      pointer operator->() const noexcept {return &**this;}
      // fwd-itr
      _ptr_vec_const_itr& operator++() noexcept {++_citr;return *this;}
      _ptr_vec_const_itr operator++(int) noexcept {auto tmp = *this;++*this;return tmp;}
      bool operator!=(const _ptr_vec_const_itr& rhs) const noexcept {return _citr != rhs._itr;}
      bool operator==(const _ptr_vec_const_itr& rhs) const noexcept {return !(*this != rhs);}
      // bi-dir
      _ptr_vec_const_itr& operator--() noexcept {++_citr;return *this;}
      _ptr_vec_const_itr operator--(int) noexcept {auto tmp = *this;++*this;return tmp;}
      // rnd-acc
      _ptr_vec_const_itr& operator+=(std::size_t n) noexcept {_citr += n;return *this;}
      _ptr_vec_const_itr& operator-=(std::size_t n) noexcept {_citr -= n;return *this;}
      _ptr_vec_const_itr operator-(std::size_t n) const noexcept {auto tmp = *this;return (tmp -= n);}
      difference_type operator-(const _ptr_vec_const_itr& rhs) const noexcept {return _citr - rhs._citr;}
      reference operator[](std::size_t n) noexcept {return *_citr[n];}
      bool operator<(const _ptr_vec_const_itr& rhs) const noexcept {return _citr < rhs._citr;}
      bool operator>(const _ptr_vec_const_itr& rhs) const noexcept {return _citr > rhs._citr;}
      bool operator<=(const _ptr_vec_const_itr& rhs) const noexcept {return !(*this > rhs);}
      bool operator>=(const _ptr_vec_const_itr& rhs) const noexcept {return !(*this < rhs);}
   };
   template <typename T>
   _ptr_vec_const_itr<T> operator+(const _ptr_vec_const_itr<T>& lhs, std::size_t rhs) {
      auto tmp = lhs;
      tmp += rhs;
      return tmp;
   }
   template <typename T>
   _ptr_vec_const_itr<T> operator+(std::size_t lhs, const _ptr_vec_const_itr<T>& rhs) {
      return rhs + lhs;
   }
} // namespace detail
} // namespace ptr_vec