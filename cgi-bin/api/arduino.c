#include "tables.h"
#include "../library/domoticaTi.h"
#include <mariadb/mysql.h>


int validateArduino(struct arduino);
void createInsertQueryArduino(char*, struct arduino);
void createUpdateQueryArduino(char*, struct arduino);
struct arduino readArduinoJSON();

int CONTENT_SIZE = 0;

int main(int argc, const char* argv[], char* env[]) {

  searchLogin(env);

  char METHOD[10];
  CONTENT_SIZE = getContentSize(env);

  /*Get Method from request*/
  getMethod(METHOD, env);

  if(argc == 1) {

    if(strcmp(METHOD, "GET") == 0) {
      selectQueryJSON("SELECT * FROM arduino");

    } else if(strcmp(METHOD, "POST") == 0) {
      struct arduino arduino;
      arduino = readArduinoJSON();

      if(validateArduino(arduino) > 0) {
        char* query = malloc(200);
        createInsertQueryArduino(query, arduino);
        executeQuery(query);
      } else {
        errorResponse(400, "validation failed.");
      }

    } else {  
      errorResponse(400, "check request url");
    }
  } else if(argc == 2) { // one data
    if(strcmp(METHOD, "DELETE") == 0) {

      const char* arduinoid = argv[1];

      if(atoi(arduinoid) != 0 && strlen(arduinoid) > 0) {
        char* query = malloc(150);
        sprintf(query, "DELETE FROM arduino WHERE arduinoid=%d", atoi(arduinoid));
        executeQuery(query);

      } else {
        errorResponse(400, "validation vailed");
      }

    } else if(strcmp(METHOD, "GET") == 0) {
      const char* arduinoid = argv[1];
      if(atoi(arduinoid) != 0 && strlen(arduinoid) > 0) {
        char* query = malloc(150);
        sprintf(query, "SELECT * FROM arduino WHERE arduinoid = %d", atoi(arduinoid));
        selectQueryJSON(query);
      }

    } else if(strcmp(METHOD, "PUT") == 0) {
      struct arduino arduino = readArduinoJSON();

      const char* arduinoid = argv[1];
      if(atoi(arduinoid) != 0 && strlen(arduinoid) > 0 && validateArduino(arduino) > 0) {
        arduino.arduinoid = atoi(arduinoid);
        char* query = malloc(200);
        createUpdateQueryArduino(query, arduino);
        executeQuery(query);
      }
    } else {
      errorResponse(400, "check request url");
    }
  } else if(argc > 2) { // one redirect + 1 data
    if(strcmp(argv[1], "last") == 0) {
      if(strcmp(METHOD, "GET") == 0) {
        const char* arduinoid = argv[2];
        if(atoi(arduinoid) != 0 && strlen(arduinoid) > 0) {
          char* query = malloc(150);
          sprintf(query, "SELECT * FROM arduinohistory WHERE arduinoid = %d ORDER BY date DESC, time DESC LIMIT 25", atoi(arduinoid));
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

int validateArduino(struct arduino arduino) {
  short validation = 1, index = 0;
  char response[ARDUINO_FIELDS][2][50];
  for(index = 0; index < ARDUINO_FIELDS; index++) {
    strncpy(response[index][0], ARDUINO_FIELD_NAMES[index], 50);
    strncpy(response[index][1], "true", 50);
  }
  
  if(strlen(arduino.arduinoname) == 0) {
    strncpy(response[1][1], "false", 50);
    validation--;
  }

  if(strlen(arduino.staticip) == 0) {
    strncpy(response[2][1], "false", 50);
    validation--;
  }


  if(validation < 1) {
    printf("STATUS: 400 \ncontent-type: application/json\n\n{");
    for (index = 1; index < ARDUINO_FIELDS; index++) {
      printf("\"%s\":%s%c", response[index][0], response[index][1], index+1 < ARDUINO_FIELDS ? ',' : ' ');
    }
    printf("}");
    exit(0);
  }

  return 1;
}

void createInsertQueryArduino(char* query, struct arduino arduino) {
  strcpy(query, "INSERT INTO arduino (staticip, arduinoname) VALUES('");

  strcat(query, arduino.staticip);
  strcat(query, "','");
  strcat(query, arduino.arduinoname);
  strcat(query, "')");
}

void createUpdateQueryArduino(char* query, struct arduino arduino) {
  strcpy(query, "UPDATE arduino SET ");
  strcat(query, "staticip = '");
  strcat(query, arduino.staticip);
  strcat(query, "', arduinoname = '");
  strcat(query, arduino.arduinoname);
  strcat(query, "' WHERE arduinoid = ");
  sprintf(query, "%s%d", query, arduino.arduinoid);
}

struct arduino readArduinoJSON() {
  struct arduino newArduino = {0,"",""}; //define all elements so we can validate the struct
  
  char* data = malloc(CONTENT_SIZE+10);
  int index;

  fgets(data, CONTENT_SIZE, stdin);
  for(index = 0; index < ARDUINO_FIELDS; index++) {
    char* dataPointer = strstr(data, ARDUINO_FIELD_NAMES[index]);
    if(dataPointer != NULL) {

      dataPointer = dataPointer + 3 + strlen(ARDUINO_FIELD_NAMES[index]); //+3 for: ": "
      char* dataPointerEnd = strchr(dataPointer, '"');

      int dataSize = dataPointerEnd - dataPointer;

      char* retrievedData = malloc(dataSize+1);
      strncpy(retrievedData, dataPointer, dataSize);
      retrievedData[dataSize] = '\0';
      removeBadCharacters(retrievedData);

      switch (index) { // starts with 1 because user can't make there own id.
      case 1:
        memcpy(newArduino.arduinoname, retrievedData, ARDUINO_FIELD_ARDUINONAME_SIZE);
        break;
      
      case 2:
        memcpy(newArduino.staticip, retrievedData, ARDUINO_FIELD_STATICIP_SIZE);
        break;
      
      
      default:
        break;
      }
    }
  }

  return newArduino;
}