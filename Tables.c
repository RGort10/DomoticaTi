#include <mysql.h>
#include <stdio.h>

int main(){
MYSQL *conn;/*Connection*/
MYSQL_RES *res;/*Result*/
MYSQL_ROW *row;/*Row*/

  char *server = "localhost";
  char *user = "root";
  char *password = "";
  char *database = "domotica";

conn = mysql_init(NULL);

  if(!mysql_real_connect(conn, server, user , password, database, 0, NULL, 0)){
  printf("Failed to connect MySQL Server %s. Error %s\n", server, mysql_error(conn));
  return 0;
}

  if(mysql_query(conn, "show tables")){
  printf("Failed to execure query: %s\n", mysql_error(conn));
  return 0;
}
res = mysql_use_result(conn);

  printf("MYSQL Tables in domotica database:\n");
  while((row = mysql_fetch_row(res))!= NULL){
  printf("%s \n", row[0]);
}
 
  mysql_free_result(res);
  /*CREATING tables*/

  if(mysql_query(conn, "CREATE TABLE IF NOT EXISTS Arduino(ArduinoID INT PRIMARY KEY AUTO_INCREMENT,\
static_ip_address VARCHAR(20) NOT NULL,\
ArduinoName VARCHAR(50) NOT NULL)"))
{
  printf("Failed to execute table create Arduino. Error %s\n", mysql_error(conn));
  return 0;
}

  if(mysql_query(conn, "CREATE TABLE IF NOT EXISTS sensor(sensorid INT unsigned PRIMARY KEY AUTO_INCREMENT,\
arduinoid INT unsigned NOT NULL,\
sensorname VARCHAR(50) NOT NULL,\
unit VARCHAR(5) NOT NULL,\
arduinocomponentid varchar(5) NOT NULL,\
FOREIGN KEY(arduinoid) REFERENCES Arduino(ArduinoID))"))
{
  printf("Failed to execute table Sensor . Error %s\n", mysql_error(conn));
  return 0;
}
  if(mysql_query(conn, "CREATE TABLE IF NOT EXISTS SensorHistory(SensorID INT NOT NULL,\
Date TIMESTAMP NOT NULL,\
Value INT NOT NULL,\
PRIMARY KEY( Date, SensorID),\
FOREIGN KEY(SensorID) REFERENCES Sensor(SensorID))"))
{
  printf("Failed to execute table create senHistory. Error %s\n", mysql_error(conn));
  return 0;
}

  if(mysql_query(conn, "CREATE TABLE IF NOT EXISTS User(UserID INT PRIMARY KEY AUTO_INCREMENT,\
Name VARCHAR(50) NOT NULL)"))
{
  printf("Failed to execute table create User. Error %s\n", mysql_error(conn));
  return 0;
}  
/*zie password*/
  if(mysql_query(conn, "CREATE TABLE IF NOT EXISTS Login(Username VARCHAR(50) PRIMARY KEY NOT NULL,\
Password VARCHAR(50) NOT NULL,\
UserID INT NOT NULL,\
ResetKey VARCHAR(100),\
FOREIGN KEY(UserID) REFERENCES User(UserID))"))  
{
  printf("Failed to execute table create Login. Error %s\n", mysql_error(conn));
  return 0;
}



 

  if(mysql_query(conn, "CREATE TABLE IF NOT EXISTS Actuator(ActuatorID INT NOT NULL AUTO_INCREMENT PRIMARY KEY,\
ArduinoID INT NOT NULL,\
ActuatorName VARCHAR(50) NOT NULL,\
Arduino_valueID INT NOT NULL,\
Eenheid VARCHAR(20) NOT NULL,\
FOREIGN KEY(ArduinoID) REFERENCES Arduino(ArduinoID))"))
{
  printf("Failed to execute table create Actuator. Error %s\n", mysql_error(conn));
  return 0;
}

 if(mysql_query(conn, "CREATE TABLE IF NOT EXISTS ActuatorHistory(ActuatorID INT AUTO_INCREMENT NOT NULL,\
Value VARCHAR(50) NOT NULL,\
Date TIMESTAMP NOT NULL,\
PRIMARY KEY(ActuatorID, Date),\
FOREIGN KEY(ActuatorID) REFERENCES Actuator(ActuatorID))"))
{
  printf("Failed to execute table create ActuatorHistory. Error %s\n", mysql_error(conn));
  return 0;
}

  /*instering*/
  if(mysql_query(conn, "INSERT INTO Sensor (ArduinoID, SensorName, Eenheid, Arduino_valueID)VALUES( 1, 'TEMPERATUUR', 'C', 2)")){
  printf("Failed to execute insert query: %s\n", mysql_error(conn));
  return 0;
}
  /*selecting*/
  if(mysql_query(conn, "select * from Sensor")){
  printf("Failed to exectur querys. Error: %s\n", mysql_error(conn));
  return 0;
}
 
res = mysql_store_result(conn);
  if(res == NULL){
  return 0;
}

  int columns = mysql_num_fields(res);

  int i=0;

  printf("Entries in the table Sensor:\n");
  while(row = mysql_fetch_row(res)){
  for(i=0; i< columns; i++){
  printf("%s ", row[i] ? row[i] : "NULL");
}
  printf("\n");
}
  mysql_free_result(res);
  mysql_close(conn);

  return 1;
}




