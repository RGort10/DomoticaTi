#include "tables.h"
#include "../library/domoticaTi.h"
#include <mariadb/mysql.h>


int validateActuator(struct actuator);
void createInsertQueryActuator(char*, struct actuator);
void getSensorBody(char*);

int CONTENT_SIZE = 0;

int main(int argc, const char* argv[], char* env[]) {


  char METHOD[10];
  CONTENT_SIZE = getContentSize(env);

  /*Get Method from request*/
  getMethod(METHOD, env);

  if(argc == 1) {

    if(strcmp(METHOD, "GET") == 0) {
      selectQueryJSON("SELECT * FROM sensorhistory");
    } else {
      errorResponse(400, "check request url");
    }
  } else {
    errorResponse(400, "check request url");
  }

  return 0;
}

void getSensorBody(char* data) {
  fgets(data, CONTENT_SIZE, stdin);
  removeBadCharacters(data);
}