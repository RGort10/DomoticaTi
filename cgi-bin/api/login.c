#include "tables.h"
#include "../library/domoticaTi.h"
#include <mariadb/mysql.h>
#include <time.h>
#include <stdlib.h>

void getLoginNeeds(char** env, char* remoteAddress, char* userAgent);
void getLogin(char* email, char* password);


long searchLoginCookie(char* env[]);

int CONTENT_SIZE = 0;

int main(int argc, const char* argv[], char* env[]) {

  char METHOD[10];
  CONTENT_SIZE = getContentSize(env);

  /*Get Method from request*/
  getMethod(METHOD, env);

  srand(time(NULL));

  /*Get Method from request*/

  if(argc == 1) {
    if(strcmp(METHOD, "POST") == 0) {
      char remoteAddress[100];
      char userAgent[250];
      char query[3000];

      char email[255];
      char password[255];
      getLogin(email, password);
      if(strlen(email) > 3 && strlen(password) > 1) {
        sprintf(query, "SELECT count(*) FROM user WHERE email = '%s' AND password = '%s'", email, password);

        if(countRecords(query)) {
          unsigned int sessionID = (unsigned int)rand();

          sprintf(query, "SELECT userid FROM user WHERE email = '%s' AND password = '%s'", email, password);
          int userid = countRecords(query); // Dit is vies. TODO Make dedicated function
          getLoginNeeds(env, remoteAddress, userAgent);

          sprintf(query, "INSERT INTO usersessions (userid, sessioncookie, remoteaddress, useragent, time) VALUES (%d, %d, '%s', '%s', UNIX_TIMESTAMP())", userid, sessionID, remoteAddress, userAgent);
          if(executeQueryNoOutput(query)) {
            printf("STATUS: 200\nSet-Cookie: SESSIONID=%d\n", sessionID);
          } else {
            errorResponse(500, "Error logging in.");
          }
        } else {
          errorResponse(418, "Bad User");
        }
      } else {
        errorResponse(400, "Please check everything!!");
      }
      
    } else {
      errorResponse(400, "check request method");
    }
    
  } else if(argc == 2) {
    if(strcmp(argv[1], "logout") == 0) {
      char remoteAddress[100];
      char userAgent[250];
      char query[3000];
      getLoginNeeds(env, remoteAddress, userAgent);
      unsigned long cookie = searchLoginCookie(env);
      if(strlen(remoteAddress) > 1 && strlen(userAgent) > 1 && cookie) {
        sprintf(query, "UPDATE usersessions SET logout = true WHERE sessioncookie = %ld AND useragent = '%s' AND remoteAddress = '%s'", cookie, userAgent, remoteAddress);
        executeQuery(query);
      } else {
        errorResponse(500, "something went wrong with data");
      }
    } else {
      errorResponse(400, "bad url");
    }
  } else {
    errorResponse(400, "check request url");
  }

  return 0;
}

long searchLoginCookie(char* env[]) {
  int index = 0, loggedIn = 0;
  while(env[index] != NULL) {
    if(strncmp(env[index], "HTTP_COOKIE=SESSIONID=", 22) == 0) {
      char* sessionID = malloc(15);
      memcpy(sessionID, env[index]+22, 10);
      return atoi(sessionID);
      break;
    }
    index++;
  }
  return -1;
}

void getLoginNeeds(char** env, char* remoteAddress, char* userAgent) {
  int index = 0;
  while(env[index] != NULL) {
    if(strncmp(env[index], "REMOTE_ADDR=", 12) == 0) {
      strncpy(remoteAddress, env[index]+12, 99);
      removeBadCharacters(remoteAddress);
    } else if (strncmp(env[index], "HTTP_USER_AGENT=", 16) == 0) {
      strncpy(userAgent, env[index]+16, 249);
      removeBadCharacters(userAgent);
    }
    index++;
  }
}

void getLogin(char* email, char* password) {
  char* data = malloc(CONTENT_SIZE+10);
  int index;

  char dataFields[2][30] = {"email","password"};

  fgets(data, CONTENT_SIZE, stdin);
  for(index = 0; index < 2; index++) {
    char* dataPointer = strstr(data, dataFields[index]);
    if(dataPointer != NULL) {

      dataPointer = dataPointer + 3 + strlen(dataFields[index]); //+3 for: ": "
      char* dataPointerEnd = strchr(dataPointer, '"');

      int dataSize = dataPointerEnd - dataPointer;

      char* retrievedData = malloc(dataSize+1);
      strncpy(retrievedData, dataPointer, dataSize);
      retrievedData[dataSize] = '\0';
      removeBadCharacters(retrievedData);

      switch (index) { // starts with 1 because user can't make there own id.
      case 0:
        strncpy(email, retrievedData, 254);
        break;

      case 1:
        strncpy(password, retrievedData, 254);
        break;
      
      default:
        break;
      }
    }
  }
}