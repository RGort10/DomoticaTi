#include <mariadb/mysql.h>
#include <stdio.h>
#include <stdlib.h>

void finish_with_error(MYSQL *con)
{
  fprintf(stderr, "%s\n", mysql_error(con));
  mysql_close(con);
  exit(1);        
}

int main(int argc, char **argv)
{
  MYSQL *con = mysql_init(NULL);
  
  if (con == NULL) 
  {
      fprintf(stderr, "%s\n", mysql_error(con));
      exit(1);
  }  

  if (mysql_real_connect(con, "localhost", "domoticati", "domoticati", "domoticati", 0, NULL, 0) == NULL) 
  {
      finish_with_error(con);
  }    
  
  if (mysql_query(con, "CREATE TABLE IF NOT EXISTS arduino(arduinoid INT UNSIGNED PRIMARY KEY NOT NULL AUTO_INCREMENT,\
staticip VARCHAR(40) NOT NULL DEFAULT '',\
arduinoname VARCHAR(100) NOT NULL DEFAULT '')")) {      
      finish_with_error(con);
  }

  if (mysql_query(con, "CREATE TABLE IF NOT EXISTS actuator(actuatorid INT UNSIGNED NOT NULL AUTO_INCREMENT PRIMARY KEY,\
arduinoid INT UNSIGNED NOT NULL,\
actuatorname VARCHAR(100) NOT NULL DEFAULT '',\
arduinocomponentid varchar(5) NOT NULL DEFAULT '',\
type VARCHAR(30) NOT NULL DEFAULT '',\
iopin bool NOT NULL DEFAULT 1,\
auto bool NOT NULL DEFAULT 0,\
value INT NOT NULL DEFAULT 0,\
minimumvalue INT NOT NULL DEFAULT 0,\
maximumvalue INT NOT NULL DEFAULT 255,\
FOREIGN KEY(arduinoid) REFERENCES arduino(arduinoid))")) {      
      finish_with_error(con);
  }

 if (mysql_query(con, "CREATE TABLE IF NOT EXISTS actuatorhistory(actuatorhistoryid INT UNSIGNED NOT NULL AUTO_INCREMENT PRIMARY KEY,\
actuatorid INT UNSIGNED NOT NULL,\
value VARCHAR(50) NOT NULL DEFAULT '',\
bywho VARCHAR(30) NOT NULL DEFAULT '',\
date INT NOT NULL DEFAULT 0,\
time INT NOT NULL DEFAULT 0,\
FOREIGN KEY(actuatorid) REFERENCES actuator(actuatorid))")) {      
      finish_with_error(con);
  }

if (mysql_query(con, "CREATE TABLE IF NOT EXISTS sensor(sensorid INT UNSIGNED NOT NULL AUTO_INCREMENT PRIMARY KEY,\
arduinoid INT UNSIGNED NOT NULL,\
sensorname VARCHAR(100) NOT NULL DEFAULT '',\
arduinocomponentid varchar(5) NOT NULL DEFAULT '',\
unit varchar(10) NOT NULL DEFAULT '',\
FOREIGN KEY(arduinoid) REFERENCES arduino(arduinoid))")) {      
      finish_with_error(con);
  }

 if (mysql_query(con, "CREATE TABLE IF NOT EXISTS sensorhistory(sensorhistoryid INT UNSIGNED NOT NULL AUTO_INCREMENT PRIMARY KEY,\
sensorid INT UNSIGNED NOT NULL,\
value VARCHAR(50) NOT NULL DEFAULT '',\
date INT NOT NULL DEFAULT 0,\
time INT NOT NULL DEFAULT 0,\
FOREIGN KEY(sensorid) REFERENCES sensor(sensorid))")) {      
      finish_with_error(con);
  }
 if (mysql_query(con, "CREATE TABLE IF NOT EXISTS user(userid INT UNSIGNED NOT NULL AUTO_INCREMENT PRIMARY KEY,\
username VARCHAR(255) NOT NULL DEFAULT '',\
email VARCHAR(255) NOT NULL DEFAULT '',\
password VARCHAR(255) NOT NULL DEFAULT '',\
keyvalue VARCHAR(50) NOT NULL DEFAULT '',\
accesslevel INT NOT NULL DEFAULT 0)")) {
       finish_with_error(con);
  }

 if (mysql_query(con, "CREATE TABLE IF NOT EXISTS usersessions(sessionid INT UNSIGNED NOT NULL AUTO_INCREMENT PRIMARY KEY,\
userid INT UNSIGNED NOT NULL,\
useragent VARCHAR(255) NOT NULL DEFAULT '',\
remoteaddress VARCHAR(100) NOT NULL DEFAULT '',\
time TIMESTAMP DEFAULT CURRENT_TIMESTAMP NOT NULL,\
logout bool NOT NULL DEFAULT 0,\
sessioncookie INT NOT NULL DEFAULT 0,\
FOREIGN KEY(userid) REFERENCES user(userid))")) {      
       finish_with_error(con);
  }
  

  mysql_close(con);
  exit(0);
}