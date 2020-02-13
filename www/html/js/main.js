function init() {
  makeActuatorTable()
}

setInterval(() => {
  makeActuatorTable()
}, 10000)

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
      table +='<tr>'
      table +=`<th scope="row">${actuator.actuatorid}</th>`
      table +=`<td>${actuator.actuatorname}</td>`
      table +=`<td>${actuator.type}</td>`
      table +=`<td>${actuator.value}</td>`
      table +=`<td>${actuator.arduinoid}</td>`
      table +=`<td>${actuator.arduinovalueid}</td>`
      table +=`<td><button class="btn btn-danger" onclick="deleteActuator(${actuator.actuatorid})"><i class="fas fa-trash-alt"></i> Delete</button></td>`
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