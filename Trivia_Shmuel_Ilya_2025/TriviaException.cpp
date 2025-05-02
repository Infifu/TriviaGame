#include "TriviaException.h"

TriviaException::TriviaException(const char* msg) : m_message(msg)
{
}

const char* TriviaException::what() const throw()
{
	return m_message.c_str();
}
