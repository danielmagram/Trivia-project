#pragma once
#include <string>
#include <vector>

class Question {
private:
    std::string m_question;
    std::vector<std::string> m_answers;
    std::string m_correctAnswer; 
public:
    Question(std::string question, std::vector<std::string> answers);
    std::string getQuestion() const;
    std::vector<std::string> getAnswers() const;
    std::string getCorrectAnswer() const;
};