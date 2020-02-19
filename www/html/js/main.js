let localActuator = {};

function init() {
  makeActuatorTable()
  setInterval(() => {
    makeActuatorTable()
  }, 10000)
}

function getActuator(id) {
  return 	$.ajax({
		method:"GET",
		url: '/cgi-bin/api/actuator.cgi?' + id
	})
		.done(data => {
      localActuator = _.cloneDeep(data.data[0])
      return data
    })
    .fail(err => {
      console.error(err);
      return null;
    })  
}

function makeActuatorTable() {
  getActuators()
  .then((data) => {
    let table = '';
    $.each(data.data, (index, actuator) => {
      table +=`<tr>`
      table +=`<th onclick="getModalActuator(${actuator.actuatorid})" scope="row">${actuator.actuatorid}</th>`
      table +=`<td onclick="getModalActuator(${actuator.actuatorid})">${actuator.actuatorname}</td>`
      table +=`<td onclick="getModalActuator(${actuator.actuatorid})">${actuator.value}</td>`
      table +=`<td onclick="getModalActuator(${actuator.actuatorid})">${actuator.type}</td>`
      table +=`<td onclick="getModalActuator(${actuator.actuatorid})">${actuator.iopin === '1' ? 'I/O' : 'PWM'}</td>`
      table +=`<td onclick="getModalActuator(${actuator.actuatorid})">${actuator.arduinoid}</td>`
			table +=`<td><button style="margin-right: 4px" class="btn btn-danger" onclick="deleteActuator(${actuator.actuatorid})"><i class="fas fa-trash-alt"></i> Delete</button>`
      table +=`<button style="margin-right: 4px" class="btn btn-primary" onclick="editActuator(${actuator.actuatorid})"><i class="fas fa-pencil-alt"></i> Edit</button>`
      table +=`<button class="btn btn-primary" onclick="settingsActuator(${actuator.actuatorid})"><i class="fas fa-wrench"></i> Settings</button></td>`
      table +='</tr>'
    })
    $("#actuatorTable").html(table)
  })
}

function getActuators() {
  return $.ajax({
    method: "GET",
    url: "/cgi-bin/api/actuator.cgi"
  })
    .done(data => {
      return data
    })
    .fail(err => {
      console.error(err)
      throw err
    })
}

function deleteActuator(id) {
  $.ajax({
    method: "DELETE",
    url: "/cgi-bin/api/actuator.cgi?"+id
  })
    .done(() => {
      makeActuatorTable()
    })
}