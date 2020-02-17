function init() {
  makeActuatorTable()
  setInterval(() => {
    makeActuatorTable()
  }, 10000)
}



function makeActuatorTable() {
  getActuators()
  .then((data) => {
    let table;
    table = '<table class="table table-hover"><thead class="thead-dark"><tr>'
    table +='<th scope="col">ID</th>'
    table +='<th scope="col">Name</th>'
    table +='<th scope="col">Type</th>'
    table +='<th scope="col">Value</th>'
    table +='<th scope="col">Arduino ID</th>'
    table +='<th scope="col">Arduino Component ID</th>'
    table +='<th scope="col">Actions</th>'
    table +='</tr></thead><tbody>'
    $.each(data.data, (index, actuator) => {
      table +=`<tr>`
      table +=`<th onclick="getActuator(${actuator.actuatorid})" scope="row">${actuator.actuatorid}</th>`
      table +=`<td onclick="getActuator(${actuator.actuatorid})">${actuator.actuatorname}</td>`
      table +=`<td onclick="getActuator(${actuator.actuatorid})">${actuator.type}</td>`
      table +=`<td onclick="getActuator(${actuator.actuatorid})">${actuator.value}</td>`
      table +=`<td onclick="getActuator(${actuator.actuatorid})">${actuator.arduinoid}</td>`
      table +=`<td onclick="getActuator(${actuator.actuatorid})">${actuator.arduinovalueid}</td>`
			table +=`<td><button style="margin-right: 4px" class="btn btn-danger" onclick="deleteActuator(${actuator.actuatorid})"><i class="fas fa-trash-alt"></i> Delete</button>`
			table +=`<button class="btn btn-primary" onclick="editActuator(${actuator.actuatorid})"><i class="fas fa-pencil-alt"></i> Edit</button></td>`
      table +='</tr>'
    })
    table += '</tbody></table>'
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