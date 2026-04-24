#pragma once
#include <string>
#include <vector>

class Question
{
public:
    Question(std::string question, std::vector<std::string> answers);

    std::string getQuestion() const;
    std::vector<std::string> getAnswers() const;
    std::string getCorrectAnswer() const;

private:
    std::string m_question;
	std::vector<std::string> m_answers; // The first answer in the vector is the correct answer in getAnswers. I will return the answers in random order, but the correct answer will always be the first one in the vector.
};