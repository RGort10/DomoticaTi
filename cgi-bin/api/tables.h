const char ACTUATOR_FIELD_NAMES[][30] = {"actuatorid", "arduinoid", "value", "type", "arduinocomponentid", "actuatorname", "iopin", "minimumvalue", "maximumvalue"};

#define ACTUATOR_FIELDS 9
#define ACTUATOR_FIELD_TYPE_SIZE 30
#define ACTUATOR_FIELD_arduinocomponentid_SIZE 5
#define ACTUATOR_FIELD_ACTUATORNAME_SIZE 100

#define INIT_ACTUATOR_TABLE = "CREATE TABLE actuator \
(actuatorid int unsigned NOT NULL auto_increment, \
arduinoid int unsigned not null, \
value int, \
type varchar(30), \
arduinocomponentid varchar(5) not null, \
actuatorname varchar(100) not null, \
PRIMARY KEY (actuatorid), \
FOREIGN KEY (arduinoid) REFERENCES arduino(arduinoid));"

struct actuator
{
  unsigned int actuatorid;
  unsigned int arduinoid;
  int value;
  char type[ACTUATOR_FIELD_TYPE_SIZE];
  char arduinocomponentid[ACTUATOR_FIELD_arduinocomponentid_SIZE];
  char actuatorname[ACTUATOR_FIELD_ACTUATORNAME_SIZE];
  int iopin;
  int minimumvalue;
  int maximumvalue;
};

const char SENSOR_FIELD_NAMES[][30] = {"sensorid", "sensorname", "unit", "arduinocomponentid", "arduinoid"};

#define SENSOR_FIELDS 5
#define SENSOR_FIELD_UNIT_SIZE 5
#define SENSOR_FIELD_ARDUINOCOMPONENTID_SIZE 5
#define SENSOR_FIELD_SENSORNAME_SIZE 50

struct sensor
{
  unsigned int sensorid;
  char sensorname[SENSOR_FIELD_SENSORNAME_SIZE];
  char unit[SENSOR_FIELD_UNIT_SIZE];
  char arduinocomponentid[SENSOR_FIELD_ARDUINOCOMPONENTID_SIZE];
  unsigned int arduinoid;
};


const char ARDUINO_FIELD_NAMES[][30] = {"arduinoid", "arduinoname", "staticip"};

#define ARDUINO_FIELDS 3
#define ARDUINO_FIELD_ARDUINONAME_SIZE 100
#define ARDUINO_FIELD_STATICIP_SIZE 30

struct arduino
{
  unsigned int arduinoid;
  char arduinoname[ARDUINO_FIELD_ARDUINONAME_SIZE];
  char staticip[ARDUINO_FIELD_STATICIP_SIZE];
};