#include "tables.h"
#include "../library/domoticaTi.h"
#include <mariadb/mysql.h>


int validateSensor(struct sensor);
void createInsertQuerySensor(char*, struct sensor);
void createUpdateQuerySensor(char*, struct sensor);
struct sensor readSensorJSON();

int CONTENT_SIZE = 0;

int main(int argc, const char* argv[], char* env[]) {
  char username[100];
  const int acceslevel = searchLogin(env, username);


  char METHOD[10];
  CONTENT_SIZE = getContentSize(env);

  /*Get Method from request*/
  getMethod(METHOD, env);

  if(argc == 1) {

    if(strcmp(METHOD, "GET") == 0) {
      selectQueryJSON("SELECT * FROM sensor");

    } else if(strcmp(METHOD, "POST") == 0) {
      struct sensor sensor;
      sensor = readSensorJSON();

      if(validateSensor(sensor) > 0) {
        char* query = malloc(200);
        createInsertQuerySensor(query, sensor);
        executeQuery(query);
      } else {
        errorResponse(400, "validation failed.");
      }

    } else {  
      errorResponse(400, "check request url");
    }
  } else if(argc == 2) { // one data
    if(strcmp(METHOD, "DELETE") == 0) {

      const char* sensorid = argv[1];

      if(atoi(sensorid) != 0 && strlen(sensorid) > 0) {
        char* query = malloc(150);
        sprintf(query, "DELETE FROM sensor WHERE sensorid=%d", atoi(sensorid));
        executeQuery(query);

      } else {
        errorResponse(400, "validation vailed");
      }

    } else if(strcmp(METHOD, "GET") == 0) {
      const char* sensorid = argv[1];
      if(atoi(sensorid) != 0 && strlen(sensorid) > 0) {
        char* query = malloc(150);
        sprintf(query, "SELECT * FROM sensor WHERE sensorid = %d", atoi(sensorid));
        selectQueryJSON(query);
      }

    } else if(strcmp(METHOD, "PUT") == 0) {
      struct sensor sensor = readSensorJSON();

      const char* sensorid = argv[1];
      if(atoi(sensorid) != 0 && strlen(sensorid) > 0 && validateSensor(sensor) > 0) {
        sensor.sensorid = atoi(sensorid);
        char* query = malloc(200);
        createUpdateQuerySensor(query, sensor);
        executeQuery(query);
      }
    } else {
      errorResponse(400, "check request url");
    }
  } else if(argc > 2) { // one redirect + 1 data
    if(strcmp(argv[1], "last") == 0) {
      if(strcmp(METHOD, "GET") == 0) {
        const char* sensorid = argv[2];
        if(atoi(sensorid) != 0 && strlen(sensorid) > 0) {
          char* query = malloc(150);
          sprintf(query, "SELECT * FROM sensorhistory WHERE sensorid = %d ORDER BY date DESC, time DESC LIMIT 25", atoi(sensorid));
          selectQueryJSON(query);
        }
      } else {
        errorResponse(400, "check request method");
      }
    } else {
      errorResponse(404, "url Not Found. Please check all parameters");
    }
  } else {
    errorResponse(404, "URL Not found. Please check all parameters");
  }

  return 0;
}

int validateSensor(struct sensor sensor) {
  short validation = 1, index = 0;
  char response[SENSOR_FIELDS][2][50];
  for(index = 0; index < SENSOR_FIELDS; index++) {
    strncpy(response[index][0], SENSOR_FIELD_NAMES[index], 50);
    strncpy(response[index][1], "true", 50);
  }
  
  if(strlen(sensor.sensorname) == 0) {
    strncpy(response[1][1], "false", 50);
    validation--;
  }

  if(strlen(sensor.unit) == 0) {
    strncpy(response[2][1], "false", 50);
    validation--;
  }

  if(strlen(sensor.arduinocomponentid) != 3) {
    strncpy(response[3][1], "false", 50);
    validation--;
  }

  if(sensor.arduinoid > 0) {
    char* query = malloc(100);
    sprintf(query, "SELECT count(*) FROM arduino WHERE arduinoid = %d", sensor.arduinoid);
    if(!countRecords(query)) {
      strncpy(response[4][1], "false", 50);
      validation--;
    }
  } else {
    strncpy(response[4][1], "false", 50);
    validation--;
  }

  if(validation < 1) {
    printf("STATUS: 400 \ncontent-type: application/json\n\n{");
    for (index = 1; index < SENSOR_FIELDS; index++) {
      printf("\"%s\":%s%c", response[index][0], response[index][1], index+1 < SENSOR_FIELDS ? ',' : ' ');
    }
    printf("}");
    exit(0);
  }

  return 1;
}

void createInsertQuerySensor(char* query, struct sensor sensor) {
  strcpy(query, "INSERT INTO sensor (arduinoid, unit, arduinocomponentid, sensorname) VALUES(");

  sprintf(query, "%s%d", query, sensor.arduinoid);
  strcat(query, ",'");
  strcat(query, sensor.unit);
  strcat(query, "','");
  strcat(query, sensor.arduinocomponentid);
  strcat(query, "','");
  strcat(query, sensor.sensorname);
  strcat(query, "')");
}

void createUpdateQuerySensor(char* query, struct sensor sensor) {
  strcpy(query, "UPDATE sensor SET arduinoid = ");

  sprintf(query, "%s%d", query, sensor.arduinoid);
  strcat(query, ", unit = '");
  strcat(query, sensor.unit);
  strcat(query, "', arduinocomponentid = '");
  strcat(query, sensor.arduinocomponentid);
  strcat(query, "', sensorname = '");
  strcat(query, sensor.sensorname);
  strcat(query, "' WHERE sensorid = ");
  sprintf(query, "%s%d", query, sensor.sensorid);
}

struct sensor readSensorJSON() {
  struct sensor newSensor = {-1,"","","",-1}; //define all elements so we can validate the struct
  
  char* data = malloc(CONTENT_SIZE+10);
  int index;

  fgets(data, CONTENT_SIZE, stdin);
  for(index = 0; index < SENSOR_FIELDS; index++) {
    char* dataPointer = strstr(data, SENSOR_FIELD_NAMES[index]);
    if(dataPointer != NULL) {

      dataPointer = dataPointer + 3 + strlen(SENSOR_FIELD_NAMES[index]); //+3 for: ": "
      char* dataPointerEnd = strchr(dataPointer, '"');

      int dataSize = dataPointerEnd - dataPointer;

      char* retrievedData = malloc(dataSize+1);
      strncpy(retrievedData, dataPointer, dataSize);
      retrievedData[dataSize] = '\0';
      removeBadCharacters(retrievedData);

      switch (index) { // starts with 1 because user can't make there own id.
      case 1:
        memcpy(newSensor.sensorname, retrievedData, SENSOR_FIELD_SENSORNAME_SIZE);
        break;
      
      case 2:
        strncpy(newSensor.unit, strcmp(retrievedData, "none") != 0 ? retrievedData : " ", SENSOR_FIELD_UNIT_SIZE);
        break;
      
      case 3:
        strncpy(newSensor.arduinocomponentid, retrievedData, SENSOR_FIELD_ARDUINOCOMPONENTID_SIZE);
        break;
      
      case 4:
        newSensor.arduinoid = atoi(retrievedData);
        break;
      
      default:
        break;
      }
    }
  }

  return newSensor;
}