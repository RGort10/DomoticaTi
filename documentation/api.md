## api documentation

RESPONSE is ALWAYS excluded from errors. 

# actuator
  # get
    1: 
      url: actuator.cgi
      response: all actuators
    
    2:
      url: actuator.cgi?'id'
      response: actuator with actuatorid == 'id'
    
    3: 
      url: actuator.cgi?last+'id'
      response: last 25 records of actuator with actuatorid == 'id'

  # post
    1: 
      url: actuator.cgi
      data: JSON Object
      response: error / {done: true}
      error: JSON Object of key: true/false

  # put
    1: 
      url: actuator.cgi?'id'
      data: JSON Object
      response: error / {done: true}
      error: JSON Object of key: true/false
    
    2: 
      url: actuator.cgi?settings+'id'
      data: JSON Object
      response: error / {done: true}
      error: JSON Object of key: true/false

    3: 
      url: actuator.cgi?value+'id'
      data: JSON Object
      response: error / {done: true}
      error: JSON Object of key: true/false

  # delete
    1: 
      url: actuator.cgi?'id'
      response: error / {done: true}
