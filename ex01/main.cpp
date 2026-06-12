/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurua <tmurua@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 22:39:25 by tmurua            #+#    #+#             */
/*   Updated: 2026/06/12 12:57:41 by tmurua           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RPN.hpp"

#include <iostream>
#include <exception>

int	main(int argc, char **argv){
	if (argc != 2){
		std::cerr << "Error" << std::endl;
		return (1);
	}
	try{
		std::cout << RPN::evaluate(argv[1]) << std::endl;
	}
	catch (const std::exception &e){
		std::cerr << e.what() << std::endl;
		return (1);
	}
	return (0);
}
