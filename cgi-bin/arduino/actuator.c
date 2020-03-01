#include "../library/domoticaTi.h"

void getArduinoBody(char*);

int CONTENT_SIZE = 0;

int main(int argc, const char* argv[], char* env[]) {


  char METHOD[10];
  CONTENT_SIZE = getContentSize(env);

  /*Get Method from request*/
  getMethod(METHOD, env);

  if(argc == 1) {

    if(strcmp(METHOD, "POST") == 0) {
      char ipAddress[100];
      char* body = malloc(CONTENT_SIZE + 10);
      char query[2000];
      getArduinoBody(body);
      getRemoteAddress(env, ipAddress);

      sprintf(query, "INSERT INTO sensorhistory (sensorid, value, date, time) VALUES (SELECT sensorid FROM sensor WHERE arduinocomponentid = '%s' AND arduinoid = ALL (SELECT arduinoid FROM arduino WHERE staticip = '%s')), '%s', CURDATE() + 0, CURTIME() + 0)", "");

      sprintf(query, "INSERT INTO sensorhistory (sensorid, value, date, time) VALUES (2, '%s', CURDATE() + 0, CURTIME() + 0)", body);
      executeQuery(query);
    } else {
      errorResponse(400, "check request url");
    }
  } else if(argc == 2) {
    if(strcmp(METHOD, "GET") == 0) {
      if(strlen(argv[1]) == 3) {
        char query[250];
        char value[50];
        char remoteAddress[100];
        getRemoteAddress(env, remoteAddress);
        sprintf(query, "SELECT value FROM actuator WHERE arduinocomponentid = '%s' AND arduinoid = ALL (SELECT arduinoid from arduino WHERE staticip = '%s')", argv[1], remoteAddress);
        getOneRecordOneColumn(query, value);
        printf("content-type: text/plain\n\n%s%s", argv[1], value);
      } else {
        errorResponse(400, "check request url");
      }
    } else {
      errorResponse(400, "check request url");
    }
  } else {
    errorResponse(400, "check request url");
  }

  return 0;
}

void getArduinoBody(char* data) {
  fgets(data, CONTENT_SIZE, stdin);
  removeBadCharacters(data);
}