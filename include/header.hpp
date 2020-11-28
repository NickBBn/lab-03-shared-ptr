// Copyright 2020 Your Name <your_email>

#ifndef INCLUDE_HEADER_HPP_
#define INCLUDE_HEADER_HPP_

#include <atomic>
#include <iostream>

template <typename T>
class SharedPtr {
 public:
  SharedPtr()
      : pointer(nullptr)
      , numberOfPointers(nullptr)
  {
    std::cout << "Constructor void is called" << std::endl;
  }
  SharedPtr(T* ptr)
      : pointer(nullptr)
      , numberOfPointers(nullptr)
  {
    std::cout << "Constructor T* is called" << std::endl;
    pointer = ptr;
    numberOfPointers = new std::atomic_uint;
    ++(*numberOfPointers);
  }
  SharedPtr(const SharedPtr& r)
      : pointer(nullptr)
      , numberOfPointers(nullptr)
  {
    std::cout << "Constructor const SharedPtr& r is called" << std::endl;
    pointer = r.get();
    numberOfPointers = r.getNumberOfPointers();
    if (numberOfPointers) ++(*numberOfPointers);
  }
  SharedPtr(SharedPtr&& r)
      : pointer(nullptr)
      , numberOfPointers(nullptr)
  {
    std::cout << "Constructor SharedPtr&& r is called " << std::endl;
    pointer = r.get();
    numberOfPointers = r.getNumberOfPointers();
    if (numberOfPointers) ++(*numberOfPointers);
  }
  ~SharedPtr()
  {
    std::cout << "Destructor called" << std::endl; //*numberOfPointers;
    destructorFunc();
  }
//  auto operator=(const SharedPtr& r) -> SharedPtr&;
//  auto operator=(SharedPtr&& r) -> SharedPtr&;
  // проверяет, указывает ли указатель на объект
  operator bool() const
  {
    if (pointer) return true;
    else return false;
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
    numberOfPointers= new std::atomic_uint;
    *numberOfPointers = 1;
  }
  void swap(SharedPtr& r)
  {
    SharedPtr<int> tmp;

  }
  // возвращает количество объектов SharedPtr, которые ссылаются на тот же управляемый объект
  auto useCount() const -> size_t
  {
    if (numberOfPointers) return *numberOfPointers;
    else return 0;
  }
 private:
  void destructorFunc()
  {
    if (numberOfPointers){
      if ((*numberOfPointers)==1){
        delete numberOfPointers;
        numberOfPointers = nullptr;
        delete pointer;
        pointer = nullptr;
      }
      else --(*numberOfPointers);
    }
  }
  auto getNumberOfPointers() const -> std::atomic_uint*  { return numberOfPointers; }
  T* pointer;
  std::atomic_uint* numberOfPointers;
};

#endif // INCLUDE_HEADER_HPP_
