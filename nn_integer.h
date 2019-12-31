#ifndef NEW_VM_VM_INTEGER_H
#define NEW_VM_VM_INTEGER_H

template <typename T>
class CNNInt {
 public:
  explicit CNNInt(T val) : m_value(val) {}
  CNNInt() : m_value(0) {}

  FORCEINLINE T value() const {
    return m_value;
  }

    explicit operator T ()
    {
        return m_value;
    }

    FORCEINLINE CNNInt& operator~() {
    m_value = NotR<T ,RND>(m_value);
    return *this;
  }

  FORCEINLINE CNNInt operator|(const CNNInt& rhs) {
    T tmp = OrR<T ,RND>(m_value,rhs.value());
    return tmp;
  }

  FORCEINLINE CNNInt operator&(const CNNInt& rhs) {
    T tmp = AndR<T, RND>(m_value,rhs.value());
    return tmp;
  }

  FORCEINLINE CNNInt operator^(const CNNInt& rhs) {
    T tmp = XorR<T, RND>(m_value,rhs.value());
    return tmp;
  }

  FORCEINLINE CNNInt operator+(const CNNInt& rhs) {
    T tmp = m_value + rhs.value();
    return tmp;
  }

  FORCEINLINE CNNInt operator-(const CNNInt& rhs) {
    T tmp = m_value - rhs.value();
    return tmp;
  }

  FORCEINLINE CNNInt& operator|=(const CNNInt& rhs) {
    m_value = OrR<T, RND>(rhs.value(),m_value);
    return *this;
  }

  FORCEINLINE CNNInt& operator&=(const CNNInt& rhs) {
    m_value = AndR<T ,RND>(rhs.value(),m_value);
    return *this;
  }

  FORCEINLINE CNNInt& operator^=(const CNNInt& rhs) {
    m_value = XorR<T, RND>(rhs.value(),m_value);
    return *this;
  }

  FORCEINLINE CNNInt& operator+=(const CNNInt& rhs) {
    m_value = rhs.value() + m_value;
    return *this;
  }

  FORCEINLINE CNNInt& operator-=(const CNNInt& rhs) {
    m_value = rhs.value() - m_value;
    return *this;
  }

  FORCEINLINE CNNInt& operator*=(const CNNInt& rhs) {
    m_value = rhs.value() * m_value;
    return *this;
  }

  FORCEINLINE CNNInt& operator=(CNNInt rhs) {
    m_value = rhs.value();
    return *this;
  }

  FORCEINLINE CNNInt& operator=(T rhs) {
    m_value = rhs;
    return *this;
  }

  FORCEINLINE CNNInt& operator++() {
    m_value++;
    return *this;
  }

  FORCEINLINE CNNInt<T>  operator++(int) {
    CNNInt temp = *this;
    ++*this;
    return temp;
  }

  FORCEINLINE CNNInt& operator--() {
    m_value=DecR<T,RND>(m_value);
    return *this;
  }

  FORCEINLINE CNNInt<T>  operator--(int) {
    CNNInt temp = *this;
    --*this;
    return temp;
  }

 private:
  volatile T m_value;
};



template <typename T>
FORCEINLINE bool operator==(const CNNInt<T>& lhs, const CNNInt<T>& rhs) {
  return lhs.value() == rhs.value();
}
template <typename T>
FORCEINLINE bool operator!=(const CNNInt<T>& lhs, const CNNInt<T>& rhs) {
  return !operator==(lhs, rhs);
}
template <typename T>
FORCEINLINE bool operator<(const CNNInt<T>& lhs, const CNNInt<T>& rhs) {
  return lhs.value() < rhs.value();
}
template <typename T>
FORCEINLINE bool operator>(const CNNInt<T>& lhs, const CNNInt<T>& rhs) {
  return operator<(rhs, lhs);
}
template <typename T>
FORCEINLINE bool operator<=(const CNNInt<T>& lhs, const CNNInt<T>& rhs) {
  return !operator>(lhs, rhs);
}
template <typename T>
FORCEINLINE bool operator>=(const CNNInt<T>& lhs, const CNNInt<T>& rhs) {
  return !operator<(lhs, rhs);
}

#endif  // NEW_VM_VM_INTEGER_H
