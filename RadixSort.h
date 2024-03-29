#pragma once

#include "CountingSort.h"
#include <cmath>
#include <tuple>

/******************************************************************************
 * finds the minimum and maximum value within the container of
 * objects and returns as an std::tuple(min_value,max_value).
 *
 * @param objects[in,out] STL container of objects to be sorted.
 * @param value_func[in] a function which returns int value to be sorted
 *                       from an object within the container objects.
 *****************************************************************************/
template<typename Container>
std::tuple<int, int> FindMinMax(Container& objects, std::function<int(const typename Container::value_type&) > value_func)
{
  using DataType = Container::value_type;

  auto comp{ [value_func](const DataType& a, const DataType& b) {return value_func(a) < value_func(b); } };
  auto min_max_iters{ std::minmax_element(objects.cbegin(), objects.cend(), comp) };
  auto min_value{ value_func(*min_max_iters.first) };
  auto max_value{ value_func(*min_max_iters.second) };
  return std::make_tuple(min_value, max_value);
}

/******************************************************************************
 * returns the number of digits within a base 10 number.
 *
 * @param input[in] input value to find number of digits.
 * @returns number of digits within input
 *****************************************************************************/
template <typename DataType>
int NumDigits(DataType input)
{
  int digits{ 0 };
  input = abs(input);
  if (input <= 9)
  {
    return 1;
  }
  while (input)
  {
    input /= 10;
    digits++;
  }
  return digits;
}

/******************************************************************************
 * returns the value of a specific digit within a base 10 number
 *
 * @param number[in] number to extract digit from.
 * @param position[in] position of the digit within the number. 0 is the least
 *                     significant digit.
 * @returns digit at the specified position within number
 *****************************************************************************/
template<typename DataType>
int GetDigit(DataType number, int position)
{
  const int base{ 10 };
  return (static_cast<int>(std::floor(number / std::pow(base, position))) % base);
}

/******************************************************************************
 * sorts a container of objects by the output of function value_func using the
 * radix sort algorithm
 *
 * Time complexity O(d * (n + m))
 * d = maximum number of digits
 * n = number of objects
 * m = 10 (number of values mapped from 0 to 9)
 *
 * @param objects[in,out] STL container of objects to be sorted.
 * @param value_func[in] a function which returns int value to be sorted
 *                       from an object within the container objects.
 *****************************************************************************/
template<typename Container>
void RadixSort(Container& objects, std::function<int(const typename Container::value_type&)> value_func)
{
  using DataType = Container::value_type;
  const auto min_base_ten{ int{ 0 } };
  const auto max_base_ten{ int{ 9 } };

  auto [min_value, max_value] = FindMinMax(objects, value_func);
  auto max_digits{ NumDigits(max_value) };


  for (int digit{ 0 }; digit < max_digits; digit++)
  {
    auto radix_value_func{ [value_func, digit](const DataType& o) {return GetDigit(value_func(o), digit); } };
    CountingSort(objects, min_base_ten, max_base_ten, radix_value_func);
  }
};