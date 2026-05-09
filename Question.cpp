#include "Question.h"
#include <algorithm> 

Question::Question(std::string question, std::vector<std::string> answers)
    : m_question(question), m_answers(answers)
{
}

std::string Question::getQuestion() const
{
    return m_question;
}


std::vector<std::string> Question::getAnswers() const
{
	std::vector<std::string> shuffledAnswers = m_answers;
	std::random_shuffle(shuffledAnswers.begin(), shuffledAnswers.end());
    return shuffledAnswers;
}

std::string Question::getCorrectAnswer() const
{
    if (!m_answers.empty())
    {
        return m_answers[0];
    }
    return ""; 
}