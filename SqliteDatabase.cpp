#include "SqliteDatabase.h"


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
bool SqliteDatabase::doesUserExist(std::string username)
{
	sqlite3_stmt* stmt = nullptr;

	std::string sql = "SELECT 1 FROM USERS WHERE USERNAME = ? ;";

	if (sqlite3_prepare_v2(m_db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
		std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(m_db) << std::endl;
	}

	if (sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_TRANSIENT))
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
bool SqliteDatabase::doesPasswordMatch(std::string username, std::string password)
{
	sqlite3_stmt* stmt = nullptr;

	std::string sql = "SELECT 1 FROM USERS WHERE USERNAME = ? AND PASSWORD = ? ;";

	if (sqlite3_prepare_v2(m_db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
		std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(m_db) << std::endl;
	}

	if (sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_TRANSIENT) != SQLITE_OK) {
		sqlite3_finalize(stmt);
		throw std::runtime_error("Bind username failed: " + std::string(sqlite3_errmsg(m_db)));
	}

	if (sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_TRANSIENT) != SQLITE_OK) {
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
bool SqliteDatabase::addNewUser(std::string username, std::string password, std::string email)
{
	std::string sql = "INSERT INTO USERS (USERNAME, PASSWORD, EMAIL) VALUES (?, ?, ?);";
	sqlite3_stmt* stmt = nullptr;

	if (sqlite3_prepare_v2(m_db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
		std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(m_db) << std::endl;
		throw std::runtime_error("Failed to prepare statment: " + std::string(sqlite3_errmsg(m_db)));
	}

	if (sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_TRANSIENT) != SQLITE_OK) {
		sqlite3_finalize(stmt);
		throw std::runtime_error("Bind username failed: " + std::string(sqlite3_errmsg(m_db)));
	}

	if (sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_TRANSIENT) != SQLITE_OK) {
		sqlite3_finalize(stmt);
		throw std::runtime_error("Bind password failed: " + std::string(sqlite3_errmsg(m_db)));
	}

	if (sqlite3_bind_text(stmt, 3, email.c_str(), -1, SQLITE_TRANSIENT) != SQLITE_OK) {
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
