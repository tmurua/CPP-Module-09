/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurua <tmurua@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/08 16:17:56 by tmurua            #+#    #+#             */
/*   Updated: 2026/06/12 17:36:45 by tmurua           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PmergeMe.hpp"

#include <algorithm>	// std::lower_bound
#include <iostream>		// std::cout
#include <iomanip>		// std::fixed, std::setprecision
#include <stdexcept>	// std::runtime_error
#include <cstdlib>		// std::strtol
#include <cerrno>		// errno, ERANGE
#include <climits>		// INT_MAX
#include <cctype>		// std::isdigit
#include <ctime>		// std::clock
#include <string>		// std::string

// store the smaller and greater value of one pair
struct NumberPair
{
	int		low;
	int		high;
	bool	used;

	NumberPair(int first, int second) : used(false){
		if (first < second){
			low = first;
			high = second;
		}
		else{
			low = second;
			high = first;
		}
	}
};

PmergeMe::PmergeMe() : _vectorTime(0), _dequeTime(0){}

PmergeMe::PmergeMe(const PmergeMe &other)
	: _vectorNumbers(other._vectorNumbers),
	_dequeNumbers(other._dequeNumbers),
	_vectorTime(other._vectorTime),
	_dequeTime(other._dequeTime){}

PmergeMe &PmergeMe::operator=(const PmergeMe &other){
	if (this != &other){
		_vectorNumbers = other._vectorNumbers;
		_dequeNumbers = other._dequeNumbers;
		_vectorTime = other._vectorTime;
		_dequeTime = other._dequeTime;
	}
	return (*this);
}

PmergeMe::~PmergeMe(){}

// accept only positive integers that fit inside int
int	PmergeMe::parseNumber(const char *argument){
	std::string	text(argument);
	char		*parseEnd;
	long		value;
	std::size_t	i;

	if (text.empty())
		throw std::runtime_error("Error");

	i = 0;
	while (i < text.length()){
		if (!std::isdigit(static_cast<unsigned char>(text[i])))
			throw std::runtime_error("Error");
		i++;
	}

	errno = 0;
	value = std::strtol(text.c_str(), &parseEnd, 10);

	if (errno == ERANGE || *parseEnd != '\0'
		|| value <= 0 || value > INT_MAX)
		throw std::runtime_error("Error");

	return (static_cast<int>(value));
}

// create pair indexes in Jacobsthal insertion order
std::vector<std::size_t>	PmergeMe::buildVectorOrder(
	std::size_t pairCount){
	std::vector<std::size_t>	order;
	std::size_t					lastInserted;
	std::size_t					jacobPrevious;
	std::size_t					jacobCurrent;
	std::size_t					upper;
	std::size_t					next;

	lastInserted = 1;
	jacobPrevious = 1;
	jacobCurrent = 3;

	while (lastInserted < pairCount){
		upper = jacobCurrent;
		if (upper > pairCount)
			upper = pairCount;

		while (upper > lastInserted){
			order.push_back(upper - 1);
			upper--;
		}

		lastInserted = jacobCurrent;
		if (lastInserted > pairCount)
			lastInserted = pairCount;

		next = jacobCurrent + (2 * jacobPrevious);
		jacobPrevious = jacobCurrent;
		jacobCurrent = next;
	}

	return (order);
}

// create the same Jacobsthal order inside a deque
std::deque<std::size_t>	PmergeMe::buildDequeOrder(
	std::size_t pairCount){
	std::deque<std::size_t>	order;
	std::size_t				lastInserted;
	std::size_t				jacobPrevious;
	std::size_t				jacobCurrent;
	std::size_t				upper;
	std::size_t				next;

	lastInserted = 1;
	jacobPrevious = 1;
	jacobCurrent = 3;

	while (lastInserted < pairCount){
		upper = jacobCurrent;
		if (upper > pairCount)
			upper = pairCount;

		while (upper > lastInserted){
			order.push_back(upper - 1);
			upper--;
		}

		lastInserted = jacobCurrent;
		if (lastInserted > pairCount)
			lastInserted = pairCount;

		next = jacobCurrent + (2 * jacobPrevious);
		jacobPrevious = jacobCurrent;
		jacobCurrent = next;
	}

	return (order);
}

// sort a vector with the Ford-Johnson merge-insert structure
void	PmergeMe::sortVector(std::vector<int> &numbers){
	std::vector<NumberPair>		pairs;
	std::vector<NumberPair>		orderedPairs;
	std::vector<int>			highNumbers;
	std::vector<int>			mainChain;
	std::vector<std::size_t>	insertOrder;
	std::size_t					pendingCount;
	std::size_t					i;
	bool						hasOdd;
	int							oddNumber;

	if (numbers.size() <= 1)
		return;

	// 1. divide the values into ordered low/high pairs
	hasOdd = (numbers.size() % 2 != 0);
	oddNumber = 0;
	i = 0;

	while (i + 1 < numbers.size()){
		pairs.push_back(NumberPair(numbers[i], numbers[i + 1]));
		i += 2;
	}

	if (hasOdd)
		oddNumber = numbers[numbers.size() - 1];

	// 2. recursively sort only the greater value of every pair
	i = 0;
	while (i < pairs.size()){
		highNumbers.push_back(pairs[i].high);
		i++;
	}

	sortVector(highNumbers);

	// 3. reorder each pair using its sorted high value
	i = 0;
	while (i < highNumbers.size()){
		std::size_t j;

		j = 0;
		while (j < pairs.size()){
			if (!pairs[j].used && pairs[j].high == highNumbers[i]){
				pairs[j].used = true;
				orderedPairs.push_back(pairs[j]);
				break;
			}
			j++;
		}
		i++;
	}

	// 4. start with b1, followed by every sorted high value
	mainChain.push_back(orderedPairs[0].low);

	i = 0;
	while (i < orderedPairs.size()){
		mainChain.push_back(orderedPairs[i].high);
		i++;
	}

	// the odd value becomes one more pending b element.
	pendingCount = orderedPairs.size();
	if (hasOdd)
		pendingCount++;

	// order pending indexes as b3, b2, b5, b4, b11...
	insertOrder = buildVectorOrder(pendingCount);

	i = 0;
	while (i < insertOrder.size()){
		std::size_t					pendingIndex;
		int							pendingValue;
		std::vector<int>::iterator	insertIt;

		pendingIndex = insertOrder[i];

		// the final pending index represents the unpaired odd value
		if (hasOdd && pendingIndex == orderedPairs.size()){
			pendingValue = oddNumber;

			// the odd value has no paired high, so search the full chain
			insertIt = std::lower_bound(
				mainChain.begin(),
				mainChain.end(),
				pendingValue
			);
		}
		else{
			std::size_t highPosition;

			pendingValue = orderedPairs[pendingIndex].low;
			highPosition = 0;

			while (mainChain[highPosition]
				!= orderedPairs[pendingIndex].high)
				highPosition++;

			// the end iterator is excluded, so the paired high is not searched
			insertIt = std::lower_bound(
				mainChain.begin(),
				mainChain.begin() + highPosition,
				pendingValue
			);
		}

		mainChain.insert(insertIt, pendingValue);
		i++;
	}

	numbers = mainChain;
}

// sort a deque separately with the same Ford-Johnson steps
void	PmergeMe::sortDeque(std::deque<int> &numbers){
	std::deque<NumberPair>		pairs;
	std::deque<NumberPair>		orderedPairs;
	std::deque<int>				highNumbers;
	std::deque<int>				mainChain;
	std::deque<std::size_t>		insertOrder;
	std::size_t					pendingCount;
	std::size_t					i;
	bool						hasOdd;
	int							oddNumber;

	if (numbers.size() <= 1)
		return;

	// 1. divide the values into ordered low/high pairs
	hasOdd = (numbers.size() % 2 != 0);
	oddNumber = 0;
	i = 0;

	while (i + 1 < numbers.size()){
		pairs.push_back(NumberPair(numbers[i], numbers[i + 1]));
		i += 2;
	}

	if (hasOdd)
		oddNumber = numbers[numbers.size() - 1];

	// 2. recursively sort only the greater value of every pair
	i = 0;
	while (i < pairs.size()){
		highNumbers.push_back(pairs[i].high);
		i++;
	}

	sortDeque(highNumbers);

	// 3. reorder each pair using its sorted high value
	i = 0;
	while (i < highNumbers.size()){
		std::size_t j;

		j = 0;
		while (j < pairs.size()){
			if (!pairs[j].used && pairs[j].high == highNumbers[i]){
				pairs[j].used = true;
				orderedPairs.push_back(pairs[j]);
				break;
			}
			j++;
		}
		i++;
	}

	// 4. start with b1, followed by every sorted high value
	mainChain.push_back(orderedPairs[0].low);

	i = 0;
	while (i < orderedPairs.size()){
		mainChain.push_back(orderedPairs[i].high);
		i++;
	}

	// the odd value becomes one more pending b element
	pendingCount = orderedPairs.size();
	if (hasOdd)
		pendingCount++;

	// order pending indexes as b3, b2, b5, b4, b11...
	insertOrder = buildDequeOrder(pendingCount);

	i = 0;
	while (i < insertOrder.size()){
		std::size_t					pendingIndex;
		int							pendingValue;
		std::deque<int>::iterator	insertIt;

		pendingIndex = insertOrder[i];

		// the final pending index represents the unpaired odd value
		if (hasOdd && pendingIndex == orderedPairs.size()){
			pendingValue = oddNumber;

			// the odd value has no paired high, so search the full chain
			insertIt = std::lower_bound(
				mainChain.begin(),
				mainChain.end(),
				pendingValue
			);
		}
		else{
			std::size_t highPosition;

			pendingValue = orderedPairs[pendingIndex].low;
			highPosition = 0;

			while (mainChain[highPosition]
				!= orderedPairs[pendingIndex].high)
				highPosition++;

			// the end iterator is excluded, so the paired high is not searched
			insertIt = std::lower_bound(
				mainChain.begin(),
				mainChain.begin() + highPosition,
				pendingValue
			);
		}

		mainChain.insert(insertIt, pendingValue);
		i++;
	}

	numbers = mainChain;
}

// validate input, fill both containers, then measure their work
void	PmergeMe::process(int argc, char **argv){
	std::clock_t	start;
	std::clock_t	end;
	int				i;

	if (argc < 2)
		throw std::runtime_error("Error");

	// validate the complete sequence before displaying a result
	i = 1;
	while (i < argc){
		parseNumber(argv[i]);
		i++;
	}

	_vectorNumbers.clear();

	start = std::clock();
	i = 1;
	while (i < argc){
		_vectorNumbers.push_back(parseNumber(argv[i]));
		i++;
	}
	sortVector(_vectorNumbers);
	end = std::clock();

	_vectorTime = static_cast<double>(end - start)
		* 1000000.0 / CLOCKS_PER_SEC;

	_dequeNumbers.clear();

	start = std::clock();
	i = 1;
	while (i < argc){
		_dequeNumbers.push_back(parseNumber(argv[i]));
		i++;
	}
	sortDeque(_dequeNumbers);
	end = std::clock();

	_dequeTime = static_cast<double>(end - start)
		* 1000000.0 / CLOCKS_PER_SEC;
}

// print the original sequence, sorted sequence, and timings
void	PmergeMe::printResult(int argc, char **argv) const{
	std::size_t	i;
	int			argumentIndex;

	std::cout << "Before:";
	argumentIndex = 1;
	while (argumentIndex < argc){
		std::cout << " " << argv[argumentIndex];
		argumentIndex++;
	}
	std::cout << std::endl;

	std::cout << "After:";
	i = 0;
	while (i < _vectorNumbers.size()){
		std::cout << " " << _vectorNumbers[i];
		i++;
	}
	std::cout << std::endl;

	std::cout << std::fixed << std::setprecision(5);

	std::cout << "Time to process a range of "
		<< _vectorNumbers.size()
		<< " elements with std::vector : "
		<< _vectorTime << " us" << std::endl;

	std::cout << "Time to process a range of "
		<< _dequeNumbers.size()
		<< " elements with std::deque  : "
		<< _dequeTime << " us" << std::endl;
}
