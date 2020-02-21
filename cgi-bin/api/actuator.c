#include "tables.h"
#include "../library/domoticaTi.h"
#include <mariadb/mysql.h>


int validateActuator(struct actuator);
void createInsertQueryActuator(char*, struct actuator);
void createUpdateQueryActuator(char*, struct actuator);
struct actuator readActuatorJSON();
int validateActuatorSettings(struct actuator);
void createUpdateQueryActuatorSettings(char*, struct actuator);

int CONTENT_SIZE = 0;

int main(int argc, const char* argv[], char* env[]) {
  searchLogin(env);

  char METHOD[10];
  CONTENT_SIZE = getContentSize(env);

  /*Get Method from request*/
  getMethod(METHOD, env);

  if(argc == 1) {

    if(strcmp(METHOD, "GET") == 0) {
      selectQueryJSON("SELECT * FROM actuator");

    } else if(strcmp(METHOD, "POST") == 0) {
      struct actuator actuator;
      actuator = readActuatorJSON();

      if(validateActuator(actuator) > 0) {
        char* query = malloc(200);
        createInsertQueryActuator(query, actuator);
        executeQuery(query);
      } else {
        errorResponse(400, "validation failed.");
      }

    } else {
      errorResponse(400, "check request url");
    }
  } else if(argc == 2) { // one data
    if(strcmp(METHOD, "DELETE") == 0) {

      const char* actuatorid = argv[1];

      if(atoi(actuatorid) != 0 && strlen(actuatorid) > 0) {
        char* query = malloc(150);
        sprintf(query, "DELETE FROM actuator WHERE actuatorid=%d", atoi(actuatorid));
        executeQuery(query);

      } else {
        errorResponse(400, "validation vailed");
      }

    } else if(strcmp(METHOD, "GET") == 0) {
			const char* actuatorid = argv[1];
      if(atoi(actuatorid) != 0 && strlen(actuatorid) > 0) {
				char* query = malloc(150);
        sprintf(query, "SELECT * FROM actuator WHERE actuatorid = %d", atoi(actuatorid));
				selectQueryJSON(query);
			} else {
        errorResponse(400, "validation vailed");
      }

		} else if(strcmp(METHOD, "PUT") == 0) {
      struct actuator actuator = readActuatorJSON();

			const char* actuatorid = argv[1];
      if(atoi(actuatorid) != 0 && strlen(actuatorid) > 0 && validateActuator(actuator) > 0) {
				actuator.actuatorid = atoi(actuatorid);
				char* query = malloc(200);
        createUpdateQueryActuator(query, actuator);
        executeQuery(query);
			} else {
        errorResponse(400, "validation vailed");
      }
    } else {
      errorResponse(400, "check request url");
    }
  } else if(argc > 2) { // one redirect + 1 data
    if(strcmp(METHOD, "PUT") == 0 && strcmp(argv[1], "settings") == 0) {
      struct actuator actuator = readActuatorJSON();

			const char* actuatorid = argv[2];
      if(atoi(actuatorid) != 0 && strlen(actuatorid) > 0 && validateActuator(actuator) > 0 && validateActuatorSettings(actuator) > 0) {
				actuator.actuatorid = atoi(actuatorid);
				char* query = malloc(300);
        createUpdateQueryActuatorSettings(query, actuator);
        executeQuery(query);
			} else {
        errorResponse(400, "validation vailed");
      }
    } else if(strcmp(METHOD, "PUT") == 0 && strcmp(argv[1], "value") == 0) {
      struct actuator actuator = readActuatorJSON();

			const char* actuatorid = argv[2];
      if(atoi(actuatorid) != 0 && strlen(actuatorid) > 0 && validateActuator(actuator) > 0 && validateActuatorSettings(actuator) > 0) {
				actuator.actuatorid = atoi(actuatorid);
				char* query = malloc(300);
        sprintf(query, "UPDATE actuator SET value = %d WHERE actuatorid = %d", actuator.value, actuator.actuatorid);
        executeQuery(query);
			} else {
        errorResponse(400, "validation vailed");
      }
    } else {
      errorResponse(404, "url Not Found. Please check all parameters");
    }
  } else {
    errorResponse(404, "URL Not found. Please check all parameters");
  }

  return 0;
}

int validateActuator(struct actuator actuator) {
  short validation = 1, index = 0;
  char response[ACTUATOR_FIELDS][2][50];
  for(index = 0; index < ACTUATOR_FIELDS; index++) {
    strncpy(response[index][0], ACTUATOR_FIELD_NAMES[index], 50);
    strncpy(response[index][1], "true", 50);
  }
  
  if(actuator.arduinoid > 0) { //uncomment following piece when arduino table is active.
    char* query = malloc(100);
    sprintf(query, "SELECT count(*) FROM arduino WHERE arduinoid = %d", actuator.arduinoid);
    if(!countRecords(query)) {
      strncpy(response[1][1], "false", 50);
      validation--;
    }
  } else {
    strncpy(response[1][1], "false", 50);
    validation--;
  }

  if(strlen(actuator.type) == 0) {
    strncpy(response[3][1], "false", 50);
    validation--;
  }

  if(strlen(actuator.arduinovalueid) != 3) {
    strncpy(response[4][1], "false", 50);
    validation--;
  }

  if(strlen(actuator.actuatorname) == 0) {
    strncpy(response[5][1], "false", 50);
    validation--;
  }

  if(validation < 1) {
    printf("STATUS: 400 \ncontent-type: application/json\n\n{");
    for (index = 1; index < ACTUATOR_FIELDS; index++) {
      printf("\"%s\":%s%c", response[index][0], response[index][1], index+1 < ACTUATOR_FIELDS ? ',' : ' ');
    }
    printf("}");
    exit(0);
  }

  return 1;
}

int validateActuatorSettings(struct actuator actuator) {
  short validation = 1, index = 0;
  char response[ACTUATOR_FIELDS][2][50];
  for(index = 0; index < ACTUATOR_FIELDS; index++) {
    strncpy(response[index][0], ACTUATOR_FIELD_NAMES[index], 50);
    strncpy(response[index][1], "true", 50);
  }
  if(! (actuator.iopin == 1 || actuator.iopin == 0)) {
    strncpy(response[6][1], "false", 50);
    validation--;
  }

  if(actuator.maximumvalue <= actuator.minimumvalue) {
    strncpy(response[7][1], "false", 50);
    strncpy(response[8][1], "false", 50);
    validation--;
  }

  if(actuator.value > actuator.maximumvalue || actuator.value < actuator.minimumvalue) {
    strncpy(response[2][1], "false", 50);
    strncpy(response[7][1], "false", 50);
    strncpy(response[8][1], "false", 50);
    validation--;
  }

  if(validation < 1) {
    printf("STATUS: 400 \ncontent-type: application/json\n\n{");
    for (index = 0; index < ACTUATOR_FIELDS; index++) {
      printf("\"%s\":%s%c", response[index][0], response[index][1], index+1 < ACTUATOR_FIELDS ? ',' : ' ');
    }
    printf("}");
    exit(0);
  }

  return validation;
}

void createInsertQueryActuator(char* query, struct actuator actuator) {
  strcpy(query, "INSERT INTO actuator (arduinoid, type, arduinovalueid, actuatorname) VALUES(");

  sprintf(query, "%s%d", query, actuator.arduinoid);
  strcat(query, ",'");
  strcat(query, actuator.type);
  strcat(query, "','");
  strcat(query, actuator.arduinovalueid);
  strcat(query, "','");
  strcat(query, actuator.actuatorname);
  strcat(query, "')");
}

void createUpdateQueryActuator(char* query, struct actuator actuator) {
	strcpy(query, "UPDATE actuator SET arduinoid = ");

  sprintf(query, "%s%d", query, actuator.arduinoid);
  strcat(query, ", type = '");
  strcat(query, actuator.type);
  strcat(query, "', arduinovalueid = '");
  strcat(query, actuator.arduinovalueid);
  strcat(query, "', actuatorname = '");
  strcat(query, actuator.actuatorname);
  strcat(query, "' WHERE actuatorid = ");
	sprintf(query, "%s%d", query, actuator.actuatorid);
}

void createUpdateQueryActuatorSettings(char* query, struct actuator actuator) {
	strcpy(query, "UPDATE actuator SET iopin = ");

  sprintf(query, "%s%d", query, actuator.iopin);
  strcat(query, ", minimumvalue = ");
  sprintf(query, "%s%d", query, actuator.minimumvalue);
  strcat(query, ", maximumvalue = ");
  sprintf(query, "%s%d", query, actuator.maximumvalue);
  strcat(query, ", value = ");
  sprintf(query, "%s%d", query, actuator.value);
  strcat(query, " WHERE actuatorid = ");
	sprintf(query, "%s%d", query, actuator.actuatorid);
}


struct actuator readActuatorJSON() {
  struct actuator newActuator = {-1,-1,0,"","","", 0, 0, 0}; //define all elements so we can validate the struct
  
  char* data = malloc(CONTENT_SIZE+10);
  int index;

  fgets(data, CONTENT_SIZE, stdin);
  for(index = 0; index < ACTUATOR_FIELDS; index++) {
    char searchString[50];
    sprintf(searchString, "\"%s\"", ACTUATOR_FIELD_NAMES[index]);
    char* dataPointer = strstr(data, searchString);
    if(dataPointer != NULL) {
      dataPointer = dataPointer + 4 + strlen(ACTUATOR_FIELD_NAMES[index]); //+3 for: ": "
      char* dataPointerEnd = strchr(dataPointer, '"');

      int dataSize = dataPointerEnd - dataPointer;

      char* retrievedData = malloc(dataSize+1);
      strncpy(retrievedData, dataPointer, dataSize);
      retrievedData[dataSize] = '\0';
      removeBadCharacters(retrievedData);
      switch (index) { // starts with 1 because user can't make there own id.
      case 1:
        newActuator.arduinoid = atoi(retrievedData);
        break;

      case 2:
        newActuator.value = atoi(retrievedData);
        break;
      
      case 3:
        strncpy(newActuator.type, retrievedData, ACTUATOR_FIELD_TYPE_SIZE);
        break;
      
      case 4:
        strncpy(newActuator.arduinovalueid, retrievedData, ACTUATOR_FIELD_ARDUINOVALUEID_SIZE);
        break;
      
      case 5:
        memcpy(newActuator.actuatorname, retrievedData, ACTUATOR_FIELD_ACTUATORNAME_SIZE);
        break;
      
      case 6:
        newActuator.iopin = atoi(retrievedData);
        break;
      
      case 7:
        newActuator.minimumvalue = atoi(retrievedData);
        break;
      
      case 8:
        newActuator.maximumvalue = atoi(retrievedData);
        break;
      
      default:
        break;
      }
    }
  }

  return newActuator;
}