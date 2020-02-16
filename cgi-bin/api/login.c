#include "tables.h"
#include "../library/domoticaTi.h"
#include <mariadb/mysql.h>


int CONTENT_SIZE = 0;

int main(int argc, const char* argv[], char* env[]) {



  /*Get Method from request*/

  if(argc == 1) {

      printf("STATUS: 200\nSet-Cookie: SESSIONID=232223\n\n");
    
  } else {
    errorResponse(400, "check request url");
  }

  return 0;
}
