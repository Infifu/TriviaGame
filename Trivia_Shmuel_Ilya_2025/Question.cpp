#include "Question.h"

Question::Question(const std::string& questionText,const std::vector<std::string>& possibleAnswers,int correctAnswerIndex): m_question(questionText), m_possibleAnswers(possibleAnswers), m_correctAnswerId(correctAnswerIndex) {}

std::string Question::getQuestion() const 
{
    return m_question;
}

std::vector<std::string> Question::getPossibleAnswers() const 
{
    return m_possibleAnswers;
}

int Question::getCorrectAnswerId() const 
{
    return m_correctAnswerId;
}
