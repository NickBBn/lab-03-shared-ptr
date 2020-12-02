// Copyright 2020 Your Name <your_email>

#include <gtest/gtest.h>
#include <utility>
#include "SharedPtr.hpp"

TEST(CasuaMethodsTests, CountTest) {
  auto ptr = new uint (15);
  SharedPtr<uint> shared1(ptr);
  SharedPtr<uint> shared2 = shared1;
  SharedPtr<uint> shared3;
  EXPECT_EQ(shared1.useCount(), 2);
  EXPECT_EQ(shared3.useCount(), 0);
}
TEST(CasuaMethodsTests, GetTest) {
  auto ptr = new uint(15);
  SharedPtr<uint> shared1(ptr);
  SharedPtr<uint> shared2 = shared1;
  EXPECT_EQ(shared2.get(), ptr);
}

TEST(ConstructorTest, NullPointer) {
  char* regularPtr = nullptr;
  SharedPtr<char> charShared(regularPtr);
  EXPECT_EQ(charShared.get(), nullptr);
}
TEST(ConstructorTest, RegularPointer) {
  char* firstptr = new char ('a');
  SharedPtr<char> charShared(firstptr);
  EXPECT_EQ(*charShared, 'a');
}
TEST(ConstructorTest, Lvalue) {
  int* firstptr = new int (23);
  SharedPtr<int> intShared1(firstptr);
  SharedPtr<int> intShared2(intShared1);
  int* ptr = intShared2.get();
  *ptr = 40;
  EXPECT_EQ((*intShared1.get()), 40);
  EXPECT_EQ(intShared1.useCount(), 2);
}

SharedPtr<int> returnObject(const SharedPtr<int> &ptr)
{
  SharedPtr<int> tmp(ptr);
  return tmp;
}
SharedPtr<int> returnObject(int* ptr)
{
  SharedPtr<int> tmp(ptr);
  return tmp;
}

TEST(ConstructorTest, Rvalue) {
  //int value = 23;
  int *regularPtr1 = new int (23);
  //int *regularPtr2 = new int (99);
  std::cout << "SharedPtr<int> intShared1(regularPtr1);" << std::endl;
  SharedPtr<int> intShared1(regularPtr1);
  std::cout << "SharedPtr<int> intShared2(intShared1);" << std::endl;
  //SharedPtr<int> intShared2(intShared1);
  //std::cout << "SharedPtr<int> intShared3(std::move(intShared1));"
  //<< std::endl;
  SharedPtr<int> intShared2(std::move(intShared1));
  std::cout << "finished" << std::endl;
  //SharedPtr<int> intShared2(returnObject());
  //SharedPtr<int> intShared2 = returnObject(regularPtr2);
  //int* ptr = intShared2.get();
  //*ptr = 40;
  EXPECT_EQ(intShared2.get(), regularPtr1);
  EXPECT_EQ(intShared2.useCount(), 1);
}

struct ArrowExample
{ int a; };

TEST(OperatorsTest, OperatorBool) {
  SharedPtr<int> b;
  EXPECT_EQ(static_cast<bool>(b), false);
}
TEST(OperatorsTest, AsteriskAndArrow) {
  int* firstptr = new int (23);
  SharedPtr<int> intShared(firstptr);
  auto arrowptr = new ArrowExample;
  arrowptr->a = 10;
  SharedPtr<ArrowExample> arrowShared(arrowptr);
  std::cout << *firstptr << std::endl;
  std::cout << arrowShared->a << std::endl;
  EXPECT_EQ(*intShared, *firstptr);
  EXPECT_EQ(arrowShared->a, arrowptr->a);
}
TEST(OperatorsTest, EqualLvalue) {
  auto regularPtr1 = new uint32_t (123);
  SharedPtr<uint32_t> shared1(regularPtr1);
  SharedPtr<uint32_t> shared2;
  SharedPtr<uint32_t> shared3 = shared2 = shared1;
  EXPECT_EQ(shared1.useCount(), 3);
  EXPECT_EQ(shared2.useCount(), 3);
  EXPECT_EQ(shared3.useCount(), 3);
  EXPECT_EQ(shared1.get(), regularPtr1);
  EXPECT_EQ(shared2.get(), regularPtr1);
  EXPECT_EQ(shared3.get(), regularPtr1);
}

TEST(ResetTest, WithNoParams) {
  int* regularPtr = new int (23);
  SharedPtr<int> intShared1(regularPtr);
  SharedPtr<int> intShared2(intShared1);
  SharedPtr<int> intShared3(intShared2);
  intShared2.reset();
  EXPECT_EQ(intShared3.useCount(), 2);
  EXPECT_EQ(intShared2.useCount(), 0);
}
TEST(ResetTest, WithPointer) {
  auto regularPtr1 = new int (23);
  auto regularPtr2 = new int (45);
  SharedPtr<int> intShared1(regularPtr1);
  SharedPtr<int> intShared2(intShared1);
  SharedPtr<int> intShared3(intShared2);
  intShared2.reset(regularPtr2);
  SharedPtr<int> intShared4(intShared2);
  SharedPtr<int> intShared5(intShared2);
  EXPECT_EQ(intShared3.useCount(), 2);
  EXPECT_EQ(intShared5.useCount(), 3);
  EXPECT_EQ(intShared5.get(), regularPtr2);
}
TEST(ResetTest, VoidSharedPtr) {
  SharedPtr<int> intShared1;
  SharedPtr<int> intShared2(intShared1);
  SharedPtr<int> intShared3(intShared2);
  intShared2.reset();
  EXPECT_EQ(intShared3.useCount(), 0);
  EXPECT_EQ(intShared2.useCount(), 0);
  EXPECT_EQ(intShared1.useCount(), 0);
}

TEST(SwapTest, Casual) {
  auto regularPtr1 = new double (23.567);
  auto regularPtr2 = new double (77.892);
  SharedPtr<double> Shared1(regularPtr1);
  SharedPtr<double> Shared2(regularPtr2);
  SharedPtr<double> Shared3(Shared2);
  Shared1.swap(Shared2);
  EXPECT_EQ(Shared1.useCount(), 2);
  EXPECT_EQ(Shared2.useCount(), 1);
  EXPECT_EQ(Shared3.useCount(), 2);
  EXPECT_EQ(Shared1.get(), regularPtr2);
  EXPECT_EQ(Shared2.get(), regularPtr1);
}
TEST(SwapTest, VoidSharedPtr) {
//  auto regularPtr1 = new double (23.567);
  auto regularPtr2 = new double (77.892);
  SharedPtr<double> shared1;
  SharedPtr<double> shared2(regularPtr2);
  SharedPtr<double> shared3(shared2);
  shared1.swap(shared2);
  EXPECT_EQ(shared1.useCount(), 2);
  EXPECT_EQ(shared2.useCount(), 0);
  EXPECT_EQ(shared3.useCount(), 2);
  EXPECT_EQ(shared1.get(), regularPtr2);
  EXPECT_EQ(shared2.get(), nullptr);
}
