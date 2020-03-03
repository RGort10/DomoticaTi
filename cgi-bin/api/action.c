#include "tables.h"
#include "../library/domoticaTi.h"
#include <mariadb/mysql.h>


int validateCronJob(struct cronjob);
void createInsertQueryCronJob(char*, struct cronjob);
struct cronjob readCronJobJSON();

int CONTENT_SIZE = 0;

int main(int argc, const char* argv[], char* env[]) {
  char username[100];
  //const int acceslevel = searchLogin(env, username);


  char METHOD[10];
  CONTENT_SIZE = getContentSize(env);

  /*Get Method from request*/
  getMethod(METHOD, env);

  if(argc == 1) {

    if(strcmp(METHOD, "GET") == 0) {
      selectQueryJSON("SELECT * FROM cronjob");

    } else if(strcmp(METHOD, "POST") == 0) {
      struct cronjob cron;
      cron = readCronJobJSON();
      if(validateCronJob(cron) > 0) {
        char query[500];
        createInsertQueryCronJob(query, cron);
        executeQuery(query);
      } else {
        errorResponse(400, "validation failed.");
      }

    } else {  
      errorResponse(400, "check request url");
    }
  } else if(argc == 2) { // one data
    /*if(strcmp(METHOD, "DELETE") == 0) {

      const char* actionid = argv[1];

      if(atoi(actionid) != 0 && strlen(actionid) > 0) {
        char* query = malloc(150);
        sprintf(query, "DELETE FROM action WHERE actionid=%d", atoi(actionid));
        executeQuery(query);

      } else {
        errorResponse(400, "validation vailed");
      }

    } else*/ if(strcmp(METHOD, "GET") == 0) {
      const char* actionid = argv[1];
      if(atoi(actionid) != 0 && strlen(actionid) > 0) {
        char* query = malloc(150);
        sprintf(query, "SELECT * FROM cronjob WHERE actuatorid = %d", atoi(actionid));
        selectQueryJSON(query);
      }

    } /*else if(strcmp(METHOD, "PUT") == 0) {
      struct action action = readActionJSON();

      const char* actionid = argv[1];
      if(atoi(actionid) != 0 && strlen(actionid) > 0 && validateAction(action) > 0) {
        action.actionid = atoi(actionid);
        char* query = malloc(200);
        createUpdateQueryAction(query, action);
        executeQuery(query);
      }
    }*/ else {
      errorResponse(400, "check request url");
    }
  } /*else if(argc > 2) { // one redirect + 1 data
    if(strcmp(argv[1], "last") == 0) {
      if(strcmp(METHOD, "GET") == 0) {
        const char* actionid = argv[2];
        if(atoi(actionid) != 0 && strlen(actionid) > 0) {
          char* query = malloc(150);
          sprintf(query, "SELECT * FROM actionhistory WHERE actionid = %d ORDER BY date DESC, time DESC LIMIT 25", atoi(actionid));
          selectQueryJSON(query);
        }
      } else {
        errorResponse(400, "check request method");
      }
    } else {
      errorResponse(404, "url Not Found. Please check all parameters");
    }
  }*/ else {
    errorResponse(404, "URL Not found. Please check all parameters");
  }

  return 0;
}

int validateCronJob(struct cronjob cron) {
  short validation = 1, index = 0;
  char response[CRONJOB_FIELDS][2][50];
  for(index = 0; index < CRONJOB_FIELDS; index++) {
    strncpy(response[index][0], CRONJOB_FIELD_NAMES[index], 50);
    strncpy(response[index][1], "true", 50);
  }
  
  /*if(cron.actuatorid > 0) { //uncomment following piece when arduino table is active.
    char* query = malloc(500);
    sprintf(query, "SELECT count(*) FROM actuator WHERE actuatorid = %d", cron.actuatorid);
    if(!countRecords(query)) {
      strncpy(response[1][1], "false", 50);
      validation--;
    } else {
      char* minimumValue;
      char* maximumValue;
      sprintf(query, "SELECT minimumvalue, maximumvalue FROM actuator WHERE actuatorid = %d", cron.actuatorid);
      getOneRecordTwoColumns(query, minimumValue, maximumValue);
      if(cron.value < (char)atoi(minimumValue) || cron.value > (char)atoi(maximumValue)) {
        strncpy(response[2][1], "false", 50);
        validation--;
      }
    }
  } else {
    strncpy(response[1][1], "false", 50);
    validation--;
  }*/

  if(cron.day < 1) {
    strncpy(response[3][1], "false", 50);
    validation--;
  }

  if(cron.hour < 0 || cron.hour > 23) {
    strncpy(response[4][1], "false", 50);
    validation--;
  }

  if(cron.minute < 0 || cron.minute > 55 || cron.minute % 5 != 0) {
    strncpy(response[5][1], "false", 50);
    validation--;
  }


  if(validation < 1) {
    printf("STATUS: 400 \ncontent-type: application/json\n\n{");
    for (index = 1; index < CRONJOB_FIELDS; index++) {
      printf("\"%s\":%s%c", response[index][0], response[index][1], index+1 < CRONJOB_FIELDS ? ',' : ' ');
    }
    printf("}");
    exit(0);
  }

  return 1;
}

void createInsertQueryCronJob(char* query, struct cronjob cron) {
  strcpy(query, "INSERT INTO cronjob (actuatorid, value, day, hour, minute) VALUES(");
  
  sprintf(query, "%s%d,", query, cron.actuatorid);
  sprintf(query, "%s%d,", query, cron.value);
  sprintf(query, "%s%hhu,", query, cron.day);
  sprintf(query, "%s%hhi,", query, cron.hour);
  sprintf(query, "%s%hhi)", query, cron.minute);
}

struct cronjob readCronJobJSON() {
  struct cronjob newCron = {0,0,0,0,0,0}; //define all elements so we can validate the struct
  
  char* data = malloc(CONTENT_SIZE+10);
  char index;
  unsigned char dayValue = 0;

  fgets(data, CONTENT_SIZE, stdin);
  for(index = 0; index < CRONJOB_FIELDS; index++) {
    char searchString[50];
    sprintf(searchString, "\"%s\"", CRONJOB_FIELD_NAMES[index]);
    char* dataPointer = strstr(data, searchString);
    if(dataPointer != NULL) {
        dataPointer = dataPointer + 4 + strlen(CRONJOB_FIELD_NAMES[index]); //+3 for: ": "
        char* dataPointerEnd;
        if(index != 3) {
          dataPointerEnd = strchr(dataPointer, '"');
        } else {
          dataPointerEnd = strchr(dataPointer, ']'); // Array instead of string
        }

        int dataSize = dataPointerEnd - dataPointer;
        char* retrievedData = malloc(dataSize+1);
        strncpy(retrievedData, dataPointer, dataSize);
        retrievedData[dataSize] = '\0';

      if(index != 3) {
        removeBadCharacters(retrievedData);

      } else { // Array
        int dayIndex;
        for(dayIndex = 0; dayIndex < 7; dayIndex++) {
          char searchStringDay[50];
          sprintf(searchStringDay, "\"%s\"", CRONJOB_FIELD_DAY_OPTIONS[dayIndex]);
          char* dataPointerDay = strstr(retrievedData, searchStringDay);
          if(dataPointerDay != NULL) {
            switch (dayIndex) {
            case 0:
              dayValue += 0b00000010;
              break;

            case 1:
              dayValue += 0b00000100;
              break;

            case 2:
              dayValue += 0b00001000;
              break;

            case 3:
              dayValue += 0b00010000;
              break;

            case 4:
              dayValue += 0b00100000;
              break;

            case 5:
              dayValue += 0b01000000;
              break;

            case 6:
              dayValue += 0b10000000;
              break;

            default:
              break;
            }
          }
        }
      }
      
      switch (index) { // starts with 1 because user can't make there own id.
        case 1:
          newCron.actuatorid = atoi(retrievedData);
          break;

        case 2:
          newCron.value = atoi(retrievedData);
          break;
        
        case 3:
          newCron.day = dayValue;
          break;
        
        case 4:
          newCron.hour = (unsigned char)atoi(retrievedData);
          break;
        
        case 5:
          newCron.minute = (unsigned char)atoi(retrievedData);
          break;

        
        default:
          break;
      }
    }
  }

  return newCron;
}