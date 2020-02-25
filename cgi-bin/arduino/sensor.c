#include "../api/tables.h"
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

  if(argc == 2) {

    if(strcmp(METHOD, "POST") == 0 && strlen(argv[1]) == 3) {
      char* body = malloc(CONTENT_SIZE + 10);
      char* query = malloc(CONTENT_SIZE + 150);
      char arduinoComponentID[4];
      char remoteAddress[100];
      strcpy(arduinoComponentID, argv[1]);
      removeBadCharacters(arduinoComponentID);
      getRemoteAddress(env, remoteAddress);
      sprintf(query, "SELECT sensorid FROM sensor WHERE arduinocomponentid = '%s' AND arduinoid = ALL (SELECT arduinoid from arduino WHERE staticip = '%s')", argv[1], remoteAddress);
      int sensorID = countRecords(query);
      getSensorBody(body);
      sprintf(query, "INSERT INTO sensorhistory (sensorid, value, date, time) VALUES (%d, '%s', CURDATE() + 0, CURTIME() + 0)", sensorID, body);
      executeQuery(query);
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