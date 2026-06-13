/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurua <tmurua@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/08 16:22:06 by tmurua            #+#    #+#             */
/*   Updated: 2026/06/12 16:34:29 by tmurua           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PmergeMe.hpp"

#include <iostream>
#include <exception>

int	main(int argc, char **argv){
	PmergeMe sorter;

	try{
		sorter.process(argc, argv);
		sorter.printResult(argc, argv);
	}
	catch (const std::exception &e){
		std::cerr << e.what() << std::endl;
		return (1);
	}

	return (0);
}
