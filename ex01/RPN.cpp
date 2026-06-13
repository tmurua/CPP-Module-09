/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RPN.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurua <tmurua@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/07 12:41:48 by tmurua            #+#    #+#             */
/*   Updated: 2026/06/12 16:23:03 by tmurua           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RPN.hpp"

#include <stack>
#include <sstream>		// std::stringstream
#include <stdexcept>	// std::runtime_error
#include <cctype>		// std::isdigit

// check if token is one of the 4 allowed operators
static bool	isOperator(const std::string &token){
	return (token == "+" || token == "-" || token == "*"
		|| token == "/");
}

// apply one operation to the 2 top values from the stack
static void	doOperation(std::stack<int> &numbers, const std::string &token){
	int rightValue;
	int leftValue;
	int result;

	// RPN needs 2 values before applying an operator
	if (numbers.size() < 2)
		throw std::runtime_error("Error");

	// the first value read and removed is the right value
	rightValue = numbers.top();
	numbers.pop();
	leftValue = numbers.top();
	numbers.pop();

	if (token == "+")
		result = leftValue + rightValue;
	else if (token == "-")
		result = leftValue - rightValue;
	else if (token == "*")
		result = leftValue * rightValue;
	else{
		if (rightValue == 0)
			throw std::runtime_error("Error");
		result = leftValue / rightValue;
	}
	numbers.push(result);
}

RPN::RPN(){}

RPN::RPN(const RPN &other){
	(void)other;
}

RPN &RPN::operator=(const RPN &other){
	(void)other;
	return (*this);
}

RPN::~RPN(){}

// evaluate one reverse polish expression and return the final result
int	RPN::evaluate(const std::string &expression){
	std::stringstream	tokenStream(expression);
	std::stack<int>		numbers;
	std::string			token;

	while (tokenStream >> token){
		// numbers in the input are single digits only, < 10
		if (token.length() == 1 && std::isdigit(token[0]))
			numbers.push(token[0] - '0');
		else if (isOperator(token))
			doOperation(numbers, token);
		else
			throw std::runtime_error("Error");
	}

	// valid RPN must finish with exactly one result left
	if (numbers.size() != 1)
		throw std::runtime_error("Error");
	return (numbers.top());
}
