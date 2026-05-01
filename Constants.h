#pragma once

enum class RequestCode : unsigned char {
    ERROR_CODE = 40,
    LOGIN = 202,
    SIGNUP = 33,
    LOGOUT = 203,
    JOIN_ROOM = 100,
    CREATE_ROOM = 101,
    GET_ROOMS = 102,
    GET_PLAYERS = 105,
    GET_HIGHSCORE = 140,
    GET_PERSONAL_STATS = 150
};


enum class Status : unsigned int {
    SUCCESS = 1,
    GENERIC_ERROR = 0,
    DB_ERROR = 2,
    WRONG_PARAMETERS = 3,
    ROOM_FULL = 4,
    ROOM_NOT_FOUND = 5,
	EMPTY_VALUE = 6,
	USER_EXISTS = 7,
	USER_ALREADY_LOGGED_IN = 8,
};