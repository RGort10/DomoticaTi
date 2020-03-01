#include "domoticaTi.h"

void getMethod(char method[], char **env) {
  int index = 0;
  while(env[index] != NULL) {
    if(strncmp(env[index], "REQUEST_METHOD=", 15) == 0) {
      memcpy(method, env[index]+15, 9);
    }
    index++;
  }
}

int getContentSize(char **env) {
  int index = 0;
  while(env[index] != NULL) {
    if(strncmp(env[index], "CONTENT_LENGTH=", 15) == 0){
      if(strlen(env[index]) > 15) {
        return atoi(env[index]+15);
      } else {
        return 0;
      }
    }
    index++;
  }
  return 0;
}

int getSessionCookie(char **env) {
  int index = 0;
  while(env[index] != NULL) {
    if(strncmp(env[index], "HTTP_COOKIE=SESSIONID=", 22) == 0){
      if(strlen(env[index]) > 22) {
        return atoi(env[index]+22);
      } else {
        return -1;
      }
    }
    index++;
  }
  return -1;
}

void getRemoteAddress(char** env, char* remoteAddress) {
 int index = 0;
  while(env[index] != NULL) {
    if(strncmp(env[index], "REMOTE_ADDR=", 12) == 0) {
      strncpy(remoteAddress, env[index]+12, 99);
      break;
    }
    index++;
  }
}

void getUserAgent(char** env, char* userAgent) {
 int index = 0;
  while(env[index] != NULL) {
    if(strncmp(env[index], "HTTP_USER_AGENT=", 16) == 0) {
      strncpy(userAgent, env[index]+16, 249);
      break;
    }
    index++;
  }
}