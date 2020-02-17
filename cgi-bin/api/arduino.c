#include "tables.h"
#include "../library/domoticaTi.h"
#include <mariadb/mysql.h>


int validateActuator(struct actuator);
void createInsertQueryActuator(char*, struct actuator);
void createUpdateQueryActuator(char*, struct actuator);
struct actuator readActuatorJSON();

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
    } else {
      errorResponse(400, "Check request url!");
    }
  } else {
    errorResponse(400, "Check request url!");
  }
  return 0;
}