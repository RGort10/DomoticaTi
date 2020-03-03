#include "../cgi-bin/library/domoticaTi.h"
#include <math.h>

int main(int argc, char const *argv[])
{
  time_t rawtime;
  struct tm * timeinfo;

  time (&rawtime);
  timeinfo = localtime (&rawtime);
  printf ("ActionTime at %d -> %d:%d\n", timeinfo->tm_wday, timeinfo->tm_hour, (timeinfo->tm_min / 5) * 5);
  int weekday = timeinfo->tm_wday;
  if (weekday == 0) {
    weekday = 7;
  }

  char query[300];
  sprintf(query, "SELECT actuatorid, value FROM cronjob WHERE (day & %d) > 0 AND minute = %d AND hour = %d", (int)pow((double)2, (double)weekday), (timeinfo->tm_min / 5) * 5, timeinfo->tm_hour);

  return selectQueryJSON(query);
}

int selectQueryJSON(char* query) {

  MYSQL *con = mysql_init(NULL);
  
  if (con == NULL)
  {
      fprintf(stderr, "mysql_init() failed\n");
      exit(1);
  }  
  
  if (mysql_real_connect(con, "localhost", "domoticati", "domoticati", "domoticati", 0, NULL, 0) == NULL) 
  {
      finish_with_error(con);
  }    
  
  if (mysql_query(con, query)) 
  {
      finish_with_error(con);
  }
  
  MYSQL_RES *result = mysql_store_result(con);
  
  if (result == NULL) 
  {
      finish_with_error(con);
  }

  MYSQL_ROW row;
  
  while ((row = mysql_fetch_row(result))) 
  { 
    char ipAddressArduino[100];
    char arduinoComponentId[10];
    char query[400];
    sprintf(query, "SELECT staticip FROM arduino WHERE arduinoid= ALL(SELECT arduinoid FROM actuator WHERE actuatorid = %s)", row[0]);
    getOneRecordOneColumn(query, ipAddressArduino);

    if(pingArduino(ipAddressArduino) > 0) {
      sprintf(query, "UPDATE actuator SET value = %s WHERE actuatorid = %s", row[1], row[0]);
      executeQueryNoOutput(query);

      sprintf(query, "SELECT arduinocomponentid from actuator where actuatorid = %s", row[0]);
      getOneRecordOneColumn(query, arduinoComponentId);
      
      pingArduinoComponent(ipAddressArduino, arduinoComponentId);
    }    
  }
  
  mysql_free_result(result);
  mysql_close(con);
  return 1;
}