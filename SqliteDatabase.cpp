#include "SqliteDatabase.h"
#include <algorithm>

SqliteDatabase::SqliteDatabase()
	: m_db(nullptr)
{
	open();
}

SqliteDatabase::~SqliteDatabase()
{
	close();
}

bool SqliteDatabase::open()
{

	std::string dbFileName = "triviaDB.sqlite";
	int file_exist = _access(dbFileName.c_str(), 0);
	int res = sqlite3_open(dbFileName.c_str(), &m_db);
	if (res != SQLITE_OK) {
		m_db = nullptr;
		std::cout << "Failed to open DB" << std::endl;
		return false;
	}

	if (file_exist != 0) {
		//USERS
		const char* sqlStatement = R"(CREATE TABLE USERS (
			USERNAME TEXT PRIMARY KEY NOT NULL,
			PASSWORD TEXT NOT NULL,
			EMAIL TEXT NOT NULL
			);
		)";
		char* errMessage = nullptr;
		int res = sqlite3_exec(m_db, sqlStatement, nullptr, nullptr, &errMessage);
		if (res != SQLITE_OK)
			return false;

		// QUESTIONS
		const char* sqlQuestionsStatement = R"(CREATE TABLE IF NOT EXISTS QUESTIONS (
		QUESTION_ID INTEGER PRIMARY KEY AUTOINCREMENT,
		QUESTION TEXT NOT NULL,
		CORRECT_ANS TEXT NOT NULL,
		ANS2 TEXT NOT NULL,
		ANS3 TEXT NOT NULL,
		ANS4 TEXT NOT NULL
		);)";
		char* errMessage2 = nullptr;
		if (sqlite3_exec(m_db, sqlQuestionsStatement, nullptr, nullptr, &errMessage2) != SQLITE_OK) {
			std::cerr << "Failed to create QUESTIONS table: " << errMessage2 << std::endl;
			sqlite3_free(errMessage2);
			return false;
		}	

		const char* sqlStatsStatement = R"(CREATE TABLE IF NOT EXISTS STATISTICS (
    USERNAME TEXT PRIMARY KEY,
    GAMES_PLAYED INTEGER DEFAULT 0,
    TOTAL_ANSWERS INTEGER DEFAULT 0,
    CORRECT_ANSWERS INTEGER DEFAULT 0,
    AVERAGE_ANSWER_TIME REAL DEFAULT 0.0
    );
)";

		char* errMessage3 = nullptr;
		if (sqlite3_exec(m_db, sqlStatsStatement, nullptr, nullptr, &errMessage3) != SQLITE_OK) {
			std::cerr << "Failed to create STATISTICS table: " << errMessage3 << std::endl;
			sqlite3_free(errMessage3);
			return false;
		}
	}

	return true;
}
bool SqliteDatabase::close()
{
	if (m_db != nullptr) {
		sqlite3_close(m_db);
		m_db = nullptr;
	}
	return true;
}
bool SqliteDatabase::doesUserExist(const std::string& username)
{
	sqlite3_stmt* stmt = nullptr;

	std::string sql = "SELECT 1 FROM USERS WHERE USERNAME = ? ;";

	if (sqlite3_prepare_v2(m_db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
		std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(m_db) << std::endl;
	}

	if (sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC)!=SQLITE_OK)
	{
		sqlite3_finalize(stmt);
		throw std::runtime_error("Bind username failed: " + std::string(sqlite3_errmsg(m_db)));
	}

	while (sqlite3_step(stmt) == SQLITE_ROW) {
		if (sqlite3_column_int(stmt, 0))
		{
			sqlite3_finalize(stmt);
			return true;
		}

	}

	sqlite3_finalize(stmt);
	return false;
}
bool SqliteDatabase::doesPasswordMatch(const std::string& username, const std::string& password)
{
	sqlite3_stmt* stmt = nullptr;

	std::string sql = "SELECT 1 FROM USERS WHERE USERNAME = ? AND PASSWORD = ? ;";

	if (sqlite3_prepare_v2(m_db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
		throw std::runtime_error("Failed to prepare statement: " + std::string(sqlite3_errmsg(m_db)));
	}

	if (sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC) != SQLITE_OK) {
		sqlite3_finalize(stmt);
		throw std::runtime_error("Bind username failed: " + std::string(sqlite3_errmsg(m_db)));
	}

	if (sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_STATIC) != SQLITE_OK) {
		sqlite3_finalize(stmt);
		throw std::runtime_error("Bind password failed: " + std::string(sqlite3_errmsg(m_db)));
	}

	while (sqlite3_step(stmt) == SQLITE_ROW) {
		if (sqlite3_column_int(stmt, 0))
		{
			sqlite3_finalize(stmt);
			return true;
		}

	}

	sqlite3_finalize(stmt);
	return false;
}
bool SqliteDatabase::addNewUser(const std::string& username, const std::string& password, const std::string& email)
{
	std::string sql = "INSERT INTO USERS (USERNAME, PASSWORD, EMAIL) VALUES (?, ?, ?);";
	sqlite3_stmt* stmt = nullptr;

	if (sqlite3_prepare_v2(m_db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
		std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(m_db) << std::endl;
		throw std::runtime_error("Failed to prepare statment: " + std::string(sqlite3_errmsg(m_db)));
	}

	if (sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC) != SQLITE_OK) {
		sqlite3_finalize(stmt);
		throw std::runtime_error("Bind username failed: " + std::string(sqlite3_errmsg(m_db)));
	}

	if (sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_STATIC) != SQLITE_OK) {
		sqlite3_finalize(stmt);
		throw std::runtime_error("Bind password failed: " + std::string(sqlite3_errmsg(m_db)));
	}

	if (sqlite3_bind_text(stmt, 3, email.c_str(), -1, SQLITE_STATIC) != SQLITE_OK) {
		sqlite3_finalize(stmt);
		throw std::runtime_error("Bind email failed: " + std::string(sqlite3_errmsg(m_db)));
	}

	if (sqlite3_step(stmt) != SQLITE_DONE) {
		std::cerr << "Insert failed: " << sqlite3_errmsg(m_db) << std::endl;
		sqlite3_finalize(stmt);
		return false;
	}

	sqlite3_finalize(stmt);
	return true;
}


std::list<Question> SqliteDatabase::getQuestions(int count)
{
	std::list<Question> questions;
	std::string sql = "SELECT QUESTION, CORRECT_ANS, ANS2, ANS3, ANS4 FROM QUESTIONS ORDER BY RANDOM() LIMIT ?;";
	sqlite3_stmt* stmt = nullptr;

	if (sqlite3_prepare_v2(m_db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
		throw std::runtime_error("Failed to prepare questions statement");
	}

	sqlite3_bind_int(stmt, 1, count);

	while (sqlite3_step(stmt) == SQLITE_ROW) {
		std::string qText = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
		std::string correct = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
		std::string ans2 = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
		std::string ans3 = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
		std::string ans4 = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));

		std::vector<std::string> answers = { correct, ans2, ans3, ans4 };
		questions.push_back(Question(qText, answers));
	}

	sqlite3_finalize(stmt);
	return questions;
}

float SqliteDatabase::getPlayerAverageAnswerTime(const std::string& username)
{
	float avgTime = 0.0f;
	std::string sql = "SELECT AVERAGE_ANSWER_TIME FROM STATISTICS WHERE USERNAME = ?;";
	sqlite3_stmt* stmt = nullptr;

	if (sqlite3_prepare_v2(m_db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
		sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
		if (sqlite3_step(stmt) == SQLITE_ROW) {
			avgTime = static_cast<float>(sqlite3_column_double(stmt, 0));
		}
	}
	sqlite3_finalize(stmt);
	return avgTime;
}

int SqliteDatabase::getNumOfCorrectAnswers(const std::string& username)
{
	int correctAnswers = 0;
	std::string sql = "SELECT CORRECT_ANSWERS FROM STATISTICS WHERE USERNAME = ?;";
	sqlite3_stmt* stmt = nullptr;

	if (sqlite3_prepare_v2(m_db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
		sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
		if (sqlite3_step(stmt) == SQLITE_ROW) {
			correctAnswers = sqlite3_column_int(stmt, 0);
		}
	}
	sqlite3_finalize(stmt);
	return correctAnswers;
}

int SqliteDatabase::getNumOfTotalAnswers(const std::string& username)
{
	int totalAnswers = 0;
	std::string sql = "SELECT TOTAL_ANSWERS FROM STATISTICS WHERE USERNAME = ?;";
	sqlite3_stmt* stmt = nullptr;

	if (sqlite3_prepare_v2(m_db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
		sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
		if (sqlite3_step(stmt) == SQLITE_ROW) {
			totalAnswers = sqlite3_column_int(stmt, 0);
		}
	}
	sqlite3_finalize(stmt);
	return totalAnswers;
}

int SqliteDatabase::getNumOfPlayerGames(const std::string& username)
{
	int gamesPlayed = 0;
	std::string sql = "SELECT GAMES_PLAYED FROM STATISTICS WHERE USERNAME = ?;";
	sqlite3_stmt* stmt = nullptr;

	if (sqlite3_prepare_v2(m_db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
		sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
		if (sqlite3_step(stmt) == SQLITE_ROW) {
			gamesPlayed = sqlite3_column_int(stmt, 0);
		}
	}
	sqlite3_finalize(stmt);
	return gamesPlayed;
}

void SqliteDatabase::initUserStatistics(const std::string& username)
{
	std::string sql = "INSERT INTO STATISTICS (USERNAME, GAMES_PLAYED, TOTAL_ANSWERS, CORRECT_ANSWERS, AVERAGE_ANSWER_TIME) VALUES (?, 0, 0, 0, 0.0);";
	sqlite3_stmt* stmt = nullptr;

	if (sqlite3_prepare_v2(m_db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
		sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);

		if (sqlite3_step(stmt) != SQLITE_DONE) {
			std::cerr << "Failed to initialize statistics for user: " << username << std::endl;
		}
	}
	else {
		std::cerr << "Failed to prepare initUserStatistics statement" << std::endl;
	}

	sqlite3_finalize(stmt);
}

float SqliteDatabase::getPlayerScore(const std::string& username)
{
	float score = 0.0f; 
	std::string sql = "SELECT CORRECT_ANSWERS, AVERAGE_ANSWER_TIME FROM STATISTICS WHERE USERNAME = ?;";
	sqlite3_stmt* stmt = nullptr;

	if (sqlite3_prepare_v2(m_db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
		sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);

		if (sqlite3_step(stmt) == SQLITE_ROW) {
			int correctAnswers = sqlite3_column_int(stmt, 0);
			float avgTime = static_cast<float>(sqlite3_column_double(stmt, 1));

			if (avgTime > 0) {
				score = (correctAnswers * 1000) / avgTime;
			}
		}
	}
	sqlite3_finalize(stmt);
	return score;
}

std::vector<std::string> SqliteDatabase::getHighScores()
{
	std::vector<std::pair<std::string, float>> playerScores;
	std::string sql = "SELECT USERNAME FROM STATISTICS;";
	sqlite3_stmt* stmt = nullptr;
	// 1. First, we retrieve all usernames and their scores
	if (sqlite3_prepare_v2(m_db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
		while (sqlite3_step(stmt) == SQLITE_ROW) {
			std::string username = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
			float score = getPlayerScore(username); 

			playerScores.push_back({ username, score });
		}
	}
	sqlite3_finalize(stmt);

	// 2. Then, we sort the players by their scores in descending order
	std::sort(playerScores.begin(), playerScores.end(),
		[](const std::pair<std::string, float>& a, const std::pair<std::string, float>& b) {
			return a.second > b.second;
		});

	// 3. Finally, we format the top 5 scores for display 
	std::vector<std::string> topScores;
	for (size_t i = 0; i < playerScores.size() && i < 5; ++i) {
		// Format: "Username: Score"
		std::string formattedScore = playerScores[i].first + ": " + std::to_string(playerScores[i].second);
		topScores.push_back(formattedScore);
	}

	return topScores;
}