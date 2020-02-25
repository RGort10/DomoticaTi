#include <curl/curl.h>
#include <mariadb/mysql.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


#define ACCESLEVEL_DELETE 11

/**
 * Error Response
*/

void finish_with_error(MYSQL*);
void errorResponse(int, char*);


/**
 * db_JSON
*/

int selectQueryJSON(char*);


/**
 * database
*/

void executeQuery(char*);
int executeQueryNoOutput(char*);
void getOneRecordOneColumn(char*, char*);
int countRecords(char* query);
int getUser(char* query, char* username);

/**
 * httpFunctions
*/

void getMethod(char[], char**);
int getContentSize(char**);
void getRemoteAddress(char**, char*);
void getUserAgent(char**, char*);

/**
 * validation
*/

void removeBadCharacters(char*);


/**
 * Login
*/

void getLoginNeeds(char**, char*, char*);
int searchLoginSession(char*, char**, char*);
int searchLogin(char**, char*);

/**
 * LibcURL 
*/

int pingArduino(char*, char*);