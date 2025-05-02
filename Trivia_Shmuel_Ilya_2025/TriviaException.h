#pragma once
#include <exception>
#include <iostream>
#include <string>

class TriviaException : public _exception
{
private:
	std::string m_message;
public:

	TriviaException(const char* msg);
	const char* what() const throw();
};

