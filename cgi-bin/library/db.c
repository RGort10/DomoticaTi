#include "domoticaTi.h"

void executeQuery(char* query) {
  MYSQL *con = mysql_init(NULL);

  if (con == NULL) {
      errorResponse(500, "mysql_init() failed");
  }  
  
  if (mysql_real_connect(con, "localhost", "domoticati", "domoticati", "domoticati", 0, NULL, 0) == NULL) {
      finish_with_error(con);
  }    
  
  if (mysql_query(con, query)) {  
      finish_with_error(con);
  }
  
  mysql_close(con);
  printf("Content-Type: application/json\n\n");
  printf("{\"done\": true}");
}

int executeQueryNoOutput(char* query) {
  MYSQL *con = mysql_init(NULL);

  if (con == NULL) {
      errorResponse(500, "mysql_init() failed");
  }  
  
  if (mysql_real_connect(con, "localhost", "domoticati", "domoticati", "domoticati", 0, NULL, 0) == NULL) {
      finish_with_error(con);
  }    
  
  if (mysql_query(con, query)) {  
      finish_with_error(con);
  }
  
  mysql_close(con);
  return 1;
}

int countRecords(char* query) {
  int fieldCounter = 0, i = 0;
  MYSQL *con = mysql_init(NULL);

  if (con == NULL) {
      errorResponse(500, "mysql_init() failed");
  }  

  if (mysql_real_connect(con, "localhost", "domoticati", "domoticati", "domoticati", 0, NULL, 0) == NULL) {
      finish_with_error(con);
  }    

  if (mysql_query(con, query)) {  
      finish_with_error(con);
  }

  MYSQL_RES *result = mysql_store_result(con);

  if (result == NULL) {
      finish_with_error(con);
  }  

  MYSQL_ROW row;
  MYSQL_FIELD *field;

  row = mysql_fetch_row(result);
  
  mysql_free_result(result);
  mysql_close(con);

  return atoi(row[0]);
}

void getOneRecordOneColumn(char* query, char* result) {
  int fieldCounter = 0, i = 0;
  MYSQL *con = mysql_init(NULL);

  if (con == NULL) {
      errorResponse(500, "mysql_init() failed");
  }  

  if (mysql_real_connect(con, "localhost", "domoticati", "domoticati", "domoticati", 0, NULL, 0) == NULL) {
      finish_with_error(con);
  }    

  if (mysql_query(con, query)) {  
      finish_with_error(con);
  }

  MYSQL_RES *result = mysql_store_result(con);

  if (result == NULL) {
      finish_with_error(con);
  }  

  MYSQL_ROW row;
  MYSQL_FIELD *field;

  row = mysql_fetch_row(result);
  sprintf(result, "%s", row[0]);
  
  mysql_free_result(result);
  mysql_close(con);
}

int getUser(char* query, char* username) {
  int fieldCounter = 0, i = 0;
  MYSQL *con = mysql_init(NULL);

  if (con == NULL) {
      errorResponse(500, "mysql_init() failed");
  }  

  if (mysql_real_connect(con, "localhost", "domoticati", "domoticati", "domoticati", 0, NULL, 0) == NULL) {
      finish_with_error(con);
  }    

  if (mysql_query(con, query)) {  
      finish_with_error(con);
  }

  MYSQL_RES *result = mysql_store_result(con);

  if (result == NULL) {
      finish_with_error(con);
  }  

  MYSQL_ROW row;
  MYSQL_FIELD *field;

  row = mysql_fetch_row(result);
  
  mysql_free_result(result);
  mysql_close(con);

  strcpy(username, row[1]);

  return atoi(row[0]);
}