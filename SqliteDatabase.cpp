#include "SqliteDatabase.h"


bool SqliteDatabase::open()
{

	std::string dbFileName = "triviaDB.sqlite";
	int file_exist = _access(dbFileName.c_str(), 0);
	int res = sqlite3_open(dbFileName.c_str(), &db);
	if (res != SQLITE_OK) {
		db = nullptr;
		std::cout << "Failed to open DB" << std::endl;
		return false;
	}

	if (file_exist != 0) {
		//USERS
		const char* sqlStatement = R"(CREATE TABLE USERS (
			USERNAME TEXT PRIMARY KEY AUTOINCREMENT NOT NULL,
			PASSWORD TEXT NOT NULL,
			EMAIL TEXT NOT NULL
			);
		)";
		char* errMessage = nullptr;
		int res = sqlite3_exec(db, sqlStatement, nullptr, nullptr, &errMessage);
		if (res != SQLITE_OK)
			return false;
	}
	return true;
}
bool SqliteDatabase::close()
{
	if (db != nullptr) {
		sqlite3_close(db);
		db = nullptr;
	}
}
bool SqliteDatabase::doesUserExist(std::string username)
{
	sqlite3_stmt* stmt = nullptr;

	std::string sql = "SELECT 1 FROM USERS WHERE USERNAME = ? ;";

	if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
		std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
	}

	if (sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_TRANSIENT))
	{
		sqlite3_finalize(stmt);
		throw std::runtime_error("Bind username failed: " + std::string(sqlite3_errmsg(db)));
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

	if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
		std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
	}

	if (sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_TRANSIENT) != SQLITE_OK) {
		sqlite3_finalize(stmt);
		throw std::runtime_error("Bind username failed: " + std::string(sqlite3_errmsg(db)));
	}

	if (sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_TRANSIENT) != SQLITE_OK) {
		sqlite3_finalize(stmt);
		throw std::runtime_error("Bind password failed: " + std::string(sqlite3_errmsg(db)));
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

	if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
		std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
	}

	if (sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_TRANSIENT) != SQLITE_OK) {
		sqlite3_finalize(stmt);
		throw std::runtime_error("Bind username failed: " + std::string(sqlite3_errmsg(db)));
	}

	if (sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_TRANSIENT) != SQLITE_OK) {
		sqlite3_finalize(stmt);
		throw std::runtime_error("Bind password failed: " + std::string(sqlite3_errmsg(db)));
	}

	if (sqlite3_bind_text(stmt, 3, email.c_str(), -1, SQLITE_TRANSIENT) != SQLITE_OK) {
		sqlite3_finalize(stmt);
		throw std::runtime_error("Bind email failed: " + std::string(sqlite3_errmsg(db)));
	}

	if (sqlite3_step(stmt) != SQLITE_DONE) {
		std::cerr << "Insert failed: " << sqlite3_errmsg(db) << std::endl;
		sqlite3_finalize(stmt);
		return false;
	}

	sqlite3_finalize(stmt);
	return true;
}
