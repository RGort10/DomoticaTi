#include "domoticaTi.h"

void getLoginNeeds(char** env, char* remoteAddress, char* userAgent) {
  getRemoteAddress(env, remoteAddress);
  getUserAgent(env, userAgent);
}

int searchLoginSession(char* id, char** env, char* username) {
  char remoteAddress[100];
  char userAgent[250];
  char query[1000];

  getLoginNeeds(env, remoteAddress, userAgent);

  removeBadCharacters(remoteAddress);
  removeBadCharacters(userAgent);

  sprintf(query, "SELECT count(*) FROM usersessions WHERE sessioncookie = %d AND useragent = '%s' AND remoteaddress = '%s' AND time > NOW() - INTERVAL 24 HOUR AND logout = false", atoi(id), userAgent, remoteAddress);
  int records = countRecords(query);
  
  if(records > 0) {
    sprintf(query, "SELECT accesslevel, username FROM user WHERE userid = ALL (SELECT userid FROM usersessions WHERE sessioncookie = %d AND useragent = '%s' AND remoteaddress = '%s' AND time > UNIX_TIMESTAMP() - 84600000 AND logout = false)", atoi(id), userAgent, remoteAddress);
    int actuatorid = getUser(query, username);
    return actuatorid;
  }
  return records;
}

int searchLogin(char* env[], char* username) {
  int index = 0, loggedIn = 0;
  while(env[index] != NULL) {
    if(strncmp(env[index], "HTTP_COOKIE=SESSIONID=", 22) == 0) {
      char* sessionID = malloc(15);
      memcpy(sessionID, env[index]+22, 10);
      loggedIn = searchLoginSession(sessionID, env, username);
      break;
    }
    index++;
  }
  if(loggedIn < 1) {
    printf("STATUS: 401\n");
    exit(0);
  } else {
    return loggedIn;
  }
}