// Copyright 2020 Your Name <your_email>

#include <gtest/gtest.h>
#include "header.hpp"
#include <utility>

TEST(Example, EmptyTest) {
  SharedPtr<int> b;
  EXPECT_EQ(static_cast<bool>(b), false);
}
TEST(Example, CountTest) {
  int* ptr = new int (15);
  SharedPtr<int> b (ptr);
  size_t count = b.useCount();
  EXPECT_EQ(count, 1);
}
TEST(Example, GetTest) {
  //char value = 'a';
  char* firstptr = new char ('a');
  SharedPtr<char> charShared (firstptr);
  char* ptr = charShared.get();
  EXPECT_EQ(*ptr, 'a');
}
TEST(Example, FromLvalueTest) {
  //int value = 23;
  int* firstptr = new int (23);
  SharedPtr<int> intShared1 (firstptr);
  SharedPtr<int> intShared2 (intShared1);
  int* ptr = intShared2.get();
  *ptr = 40;
  EXPECT_EQ((*intShared1.get()), 40);
  EXPECT_EQ(intShared1.useCount(), 2);
}

SharedPtr<int> returnObject(/*const SharedPtr<int> &ptr*/)
{
  auto ptr = new int (300);
  SharedPtr<int> tmp (ptr);
  *tmp = 350;
  std::cout << "work pls" << std::endl;
  return tmp;
}

TEST(Example, FromRvalueTest) {
  //int value = 23;
  int *firstptr = new int (23);
  SharedPtr<int> intShared1(firstptr);
  //SharedPtr<int> intShared2(std::move(intShared1));
  SharedPtr<int> intShared2(returnObject());
//  SharedPtr<int> intShared2(returnObject(/*intShared1*/));
  int* ptr = intShared2.get();
  *ptr = 40;
  EXPECT_EQ((*intShared2.get()), 40);
  EXPECT_EQ(intShared2.useCount(), 10);
}

struct ArrowExample
{ int a; };

TEST(Example, OperatorTest) {
  int* firstptr = new int (23);
  SharedPtr<int> intShared (firstptr);
  auto arrowptr = new ArrowExample;
  arrowptr->a = 10;
  SharedPtr<ArrowExample> arrowShared (arrowptr);
  std::cout << *firstptr << std::endl;
  std::cout << arrowShared->a << std::endl;
  EXPECT_EQ(*intShared, *firstptr);
  EXPECT_EQ(arrowShared->a, arrowptr->a);
}
TEST(Example, ResetWithVoidTest) {
  int* regularPtr = new int (23);
  SharedPtr<int> intShared1 (regularPtr);
  SharedPtr<int> intShared2 (intShared1);
  SharedPtr<int> intShared3 (intShared2);
  intShared2.reset();
  EXPECT_EQ(intShared3.useCount(), 2);
  EXPECT_EQ(intShared2.useCount(), 0);
}

TEST(Example, ResetWithPtrTest) {
  auto regularPtr1 = new int (23);
  auto regularPtr2 = new int (45);
  SharedPtr<int> intShared1 (regularPtr1);
  SharedPtr<int> intShared2 (intShared1);
  SharedPtr<int> intShared3 (intShared2);
  intShared2.reset(regularPtr2);
  SharedPtr<int> intShared4 (intShared2);
  SharedPtr<int> intShared5 (intShared2);
  EXPECT_EQ(intShared3.useCount(), 2);
  EXPECT_EQ(intShared5.useCount(), 3);
  EXPECT_EQ(intShared5.get(), regularPtr2);
}

TEST(Example, VoidPointersTest) {
  //int* regularPtr = new int (23);
  SharedPtr<int> intShared1;
  SharedPtr<int> intShared2 (intShared1);
  SharedPtr<int> intShared3 (intShared2);
  intShared2.reset();
  EXPECT_EQ(intShared3.useCount(), 0);
  EXPECT_EQ(intShared2.useCount(), 0);
  EXPECT_EQ(intShared1.useCount(), 0);
}

//SharedPtr<int> &&rref = intShared1.returnObj();
//std::cout << "rref is maken" << std::endl;
//SharedPtr<int> intShared2(rref);