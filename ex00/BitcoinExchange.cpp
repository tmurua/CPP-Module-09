/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BitcoinExchange.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurua <tmurua@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/06 11:08:59 by tmurua            #+#    #+#             */
/*   Updated: 2026/06/12 16:01:05 by tmurua           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "BitcoinExchange.hpp"

#include <iostream>
#include <fstream>
#include <stdexcept>	// std::runtime_error
#include <cstdlib>		// std::atoi, std::strtod
#include <cctype>		// std::isdigit

// check leap year for February
static bool	isLeapYear(int year){
	if (year % 400 == 0)
		return (true);
	if (year % 100 == 0)
		return (false);
	return (year % 4 == 0);
}

// return how many days exist in the given month
static int	daysInMonth(int year, int month){
	if (month == 1 || month == 3 || month == 5 || month == 7
		|| month == 8 || month == 10 || month == 12)
		return (31);
	if (month == 4 || month == 6 || month == 9 || month == 11)
		return (30);
	if (month == 2){
		if (isLeapYear(year))
			return (29);
		return (28);
	}
	return (0);
}

BitcoinExchange::BitcoinExchange(){}

BitcoinExchange::BitcoinExchange(const BitcoinExchange &other)
	: _database(other._database){}

BitcoinExchange	&BitcoinExchange::operator=(const BitcoinExchange &other){
	if (this != &other)
		_database = other._database;
	return (*this);
}

BitcoinExchange::~BitcoinExchange(){}

// remove spaces at beginning and end of a string
std::string	BitcoinExchange::trimSpaces(const std::string &text) const{
	std::size_t	start;
	std::size_t	end;

	start = text.find_first_not_of(" \t");
	if (start == std::string::npos)
		return ("");
	end = text.find_last_not_of(" \t");
	return (text.substr(start, end - start + 1));
}

// check format YYYY-MM-DD and real calendar ranges
bool	BitcoinExchange::isValidDate(const std::string &date) const{
	int		year;
	int		month;
	int		day;

	if (date.length() != 10)
		return (false);
	if (date[4] != '-' || date[7] != '-')
		return (false);
	if (!std::isdigit(date[0]) || !std::isdigit(date[1])
		|| !std::isdigit(date[2]) || !std::isdigit(date[3])
		|| !std::isdigit(date[5]) || !std::isdigit(date[6])
		|| !std::isdigit(date[8]) || !std::isdigit(date[9]))
		return (false);

	year = std::atoi(date.substr(0, 4).c_str());
	month = std::atoi(date.substr(5, 2).c_str());
	day = std::atoi(date.substr(8, 2).c_str());

	if (month < 1 || month > 12)
		return (false);
	if (day < 1 || day > daysInMonth(year, month))
		return (false);
	return (true);
}

// parse a numeric value from text
bool	BitcoinExchange::parseValue(const std::string &valueString, double &value) const{
	char	*parseEnd;

	if (valueString.empty())
		return (false);

	parseEnd = NULL;
	value = std::strtod(valueString.c_str(), &parseEnd);

	if (parseEnd == valueString.c_str()) // no chars were converted into a number
		return (false);
	if (*parseEnd != '\0') // reject extra chars after the numeric value
		return (false);
	return (true);
}

// find exact date or closest lower date inside the database
// YYYY-MM-DD strings are ordered chronologically inside the map
bool	BitcoinExchange::findRateForDate(const std::string &date, double &rate) const{
	std::map<std::string, double>::const_iterator	it;

	// lower_bound returns the first date >= to the requested date
	it = _database.lower_bound(date);

	if (it != _database.end() && it->first == date){
		rate = it->second;
		return (true);
	}
	// there is no earlier rate if the requested date comes before the first entry
	if (it == _database.begin())
		return (false);
	// no exact match: move back to the closest earlier date
	if (it == _database.end() || it->first != date)
		--it;
	rate = it->second;
	return (true);
}

// read one input line, validate it, and print result or error
void	BitcoinExchange::processLine(const std::string &line) const{
	std::size_t	separatorPos;
	std::string	dateString;
	std::string	valueString;
	double		value;
	double		rate;

	separatorPos = line.find('|');
	if (separatorPos == std::string::npos){
		std::cout << "Error: bad input => " << line << std::endl;
		return;
	}

	dateString = trimSpaces(line.substr(0, separatorPos));
	valueString = trimSpaces(line.substr(separatorPos + 1));

	if (!isValidDate(dateString)){
		std::cout << "Error: bad input => " << line << std::endl;
		return;
	}
	if (!parseValue(valueString, value)){
		std::cout << "Error: bad input => " << line << std::endl;
		return;
	}
	if (value < 0){
		std::cout << "Error: not a positive number." << std::endl;
		return;
	}
	if (value > 1000){
		std::cout << "Error: too large a number." << std::endl;
		return;
	}
	if (!findRateForDate(dateString, rate)){
		std::cout << "Error: no exchange rate available for date => "
			<< dateString << std::endl;
		return;
	}

	std::cout << dateString << " => " << valueString
		<< " = " << (value * rate) << std::endl;
}

// load provided csv database into a std::map
void	BitcoinExchange::loadDatabase(const std::string &databaseFile){
	std::ifstream	file(databaseFile.c_str());
	std::string		line;
	std::size_t		commaPos;
	std::string		dateString;
	std::string		rateString;
	double			rate;

	if (!file)
		throw std::runtime_error("Error: could not open database file.");

	while (std::getline(file, line)){
		if (line.empty() || line == "date,exchange_rate")
			continue;

		commaPos = line.find(',');
		if (commaPos == std::string::npos)
			continue;

		dateString = trimSpaces(line.substr(0, commaPos));
		rateString = trimSpaces(line.substr(commaPos + 1));

		if (!isValidDate(dateString))
			continue;
		if (!parseValue(rateString, rate))
			continue;

		_database[dateString] = rate;
	}
}

// read input file and process each line after the header
void	BitcoinExchange::processInput(const std::string &inputFile) const{
	std::ifstream	file(inputFile.c_str());
	std::string		line;
	bool			firstLine;

	if (!file)
		throw std::runtime_error("Error: could not open file.");

	firstLine = true; // only treat first non-empty line as a possible header
	while (std::getline(file, line)){
		if (line.empty())
			continue;
		// skip the header when it's present
		if (firstLine && trimSpaces(line) == "date | value"){
			firstLine = false;
			continue;
		}
		firstLine = false;
		processLine(line);
	}
}
