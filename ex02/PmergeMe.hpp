/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurua <tmurua@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/08 16:17:17 by tmurua            #+#    #+#             */
/*   Updated: 2026/06/12 16:24:53 by tmurua           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include <vector>	// std::vector
#include <deque>	// std::deque
#include <cstddef>	// std::size_t

class PmergeMe
{
private:
	std::vector<int>	_vectorNumbers;
	std::deque<int>		_dequeNumbers;
	double				_vectorTime;
	double				_dequeTime;

	static int	parseNumber(const char *argument);

	static std::vector<std::size_t>	buildVectorOrder(std::size_t pairCount);
	static std::deque<std::size_t>	buildDequeOrder(std::size_t pairCount);

	static void	sortVector(std::vector<int> &numbers);
	static void	sortDeque(std::deque<int> &numbers);

public:
	PmergeMe();
	PmergeMe(const PmergeMe &other);
	PmergeMe &operator=(const PmergeMe &other);
	~PmergeMe();

	void	process(int argc, char **argv);
	void	printResult(int argc, char **argv) const;
};

#endif
