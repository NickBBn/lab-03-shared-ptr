// Copyright 2020 Your Name <your_email>

#ifndef INCLUDE_SHAREDPTR_HPP_
#define INCLUDE_SHAREDPTR_HPP_

#include <atomic>
#include <iostream>
#include <utility>

template <typename T>
class SharedPtr {
 public:
  SharedPtr()
      : pointer(nullptr)
      , numberOfPointers(nullptr)
  {
    std::cout << "Constructor void is called" << std::endl;
  }
  explicit SharedPtr(T* ptr)
      : pointer(ptr)
      , numberOfPointers(nullptr)
  {
    std::cout << "Constructor T* is called" << std::endl;
    if (pointer){
      numberOfPointers = new std::atomic_uint;
      *numberOfPointers = 1;
    } else {
      numberOfPointers = nullptr;
    }
  }
  SharedPtr(const SharedPtr& r)
      : pointer(r.get())
      , numberOfPointers(r.getNumberOfPointers())
  {
    std::cout << "Constructor const SharedPtr& r is called" << std::endl;
    if (numberOfPointers) ++(*numberOfPointers);
  }
  SharedPtr(SharedPtr&& r)
      : pointer(r.get())
      , numberOfPointers(r.getNumberOfPointers())
  {
    std::cout << "Constructor SharedPtr&& r is called " << std::endl;
    if (numberOfPointers) ++(*numberOfPointers);
  }
  ~SharedPtr()
  {
    std::cout << "Destructor called" << std::endl; //*numberOfPointers;
    destructorFunc();
  }

  auto operator=(const SharedPtr& r) -> SharedPtr&
  {
    if (this != &r){
      pointer = r.get();
      numberOfPointers = r.getNumberOfPointers();
      if (numberOfPointers) ++(*numberOfPointers);
    }
    return *this;
  }
  auto operator=(SharedPtr&& r)  noexcept -> SharedPtr&
  {
    if (this != &r){
      pointer = r.get();
      numberOfPointers = r.getNumberOfPointers();
    }
    return *this;
  }
  // проверяет, указывает ли указатель на объект
  operator bool() const
  {
    if (pointer) return true;
    else
      return false;
  }
  auto operator*() const -> T& { return *pointer; }
  auto operator->() const -> T* { return pointer; }
  auto get() const -> T*  { return pointer; }
  void reset()
  {
    destructorFunc();
    numberOfPointers = nullptr;
    pointer = nullptr;
  }
  void reset(T* ptr)
  {
    destructorFunc();
    pointer = ptr;
    numberOfPointers = new std::atomic_uint;
    *numberOfPointers = 1;
  }
  void swap(SharedPtr& r)
  {
    T* tmpPointer (std::move(r.get()));
    std::atomic_uint* tmpNumberOfPointers = r.getNumberOfPointers();
    r.pointer = std::move(pointer);
    r.numberOfPointers = numberOfPointers;
    pointer = std::move(tmpPointer);
    numberOfPointers = tmpNumberOfPointers;
  }
  // возвращает количество объектов SharedPtr,
  // которые ссылаются на тот же управляемый объект
  auto useCount() const -> size_t
  {
    if (numberOfPointers) return *numberOfPointers;
    else
      return 0;
  }

 private:
  void destructorFunc()
  {
    if (numberOfPointers){
      if ((*numberOfPointers) == 1){
        delete numberOfPointers;
        numberOfPointers = nullptr;
        delete pointer;
        pointer = nullptr;
      } else
        --(*numberOfPointers);
    }
  }
  auto getNumberOfPointers() const -> std::atomic_uint*
  {
    return numberOfPointers;
  }
  T* pointer;
  std::atomic_uint* numberOfPointers;
};

#endif // INCLUDE_SHAREDPTR_HPP_
