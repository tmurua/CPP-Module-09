/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BitcoinExchange.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurua <tmurua@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/06 09:07:20 by tmurua            #+#    #+#             */
/*   Updated: 2026/06/09 09:09:54 by tmurua           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BITCOINEXCHANGE_HPP
#define BITCOINEXCHANGE_HPP

#include <map>
#include <string>

class BitcoinExchange
{
private:
	std::map<std::string, double>	_database;

	std::string	trimSpaces(const std::string &text) const;
	bool		isValidDate(const std::string &date) const;
	bool		parseValue(const std::string &valueString, double &value) const;
	bool		findRateForDate(const std::string &date, double &rate) const;
	void		processLine(const std::string &line) const;

public:
	BitcoinExchange();
	BitcoinExchange(const BitcoinExchange &other);
	BitcoinExchange &operator=(const BitcoinExchange &other);
	~BitcoinExchange();

	void	loadDatabase(const std::string &databaseFile);
	void	processInput(const std::string &inputFile) const;
};

#endif
