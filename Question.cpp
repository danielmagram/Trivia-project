#include "Question.h"
#include <algorithm> 

Question::Question(std::string question, std::vector<std::string> answers)
    : m_question(question), m_answers(answers)
{
    if (!m_answers.empty()) {
        m_correctAnswer = m_answers[0];
    }
    else {
        m_correctAnswer = "";
    }

    std::random_shuffle(m_answers.begin(), m_answers.end());
}

std::string Question::getQuestion() const
{
    return m_question;
}

std::vector<std::string> Question::getAnswers() const
{
    return m_answers;
}

std::string Question::getCorrectAnswer() const
{
    return m_correctAnswer;
}