function initSensor() {
  makeSensorTable()
  setInterval(() => {
    makeSensorTable()
    console.log(document.cookie);
  }, 3000)
}



function makeSensorTable() {
  getSensors()
  .then((data) => {
    let table = '';
    $.each(data.data, (index, sensor) => {
      table +=`<tr>`
      table +=`<th onclick="getSensor(${sensor.sensorid})" scope="row">${sensor.sensorid}</th>`
      table +=`<td onclick="getSensor(${sensor.sensorid})">${sensor.sensorname}</td>`
      table +=`<td onclick="getSensor(${sensor.sensorid})">${sensor.arduinoid}</td>`
      table +=`<td onclick="getSensor(${sensor.sensorid})">${sensor.arduinocomponentid}</td>`
      table +=`<td onclick="getSensor(${sensor.sensorid})">${sensor.unit || 'none'}</td>`
			table +=`<td><button style="margin-right: 4px" class="btn btn-danger" onclick="deleteSensor(${sensor.sensorid})"><i class="fas fa-trash-alt"></i> Delete</button>`
			table +=`<button class="btn btn-primary" onclick="editSensor(${sensor.sensorid})"><i class="fas fa-pencil-alt"></i> Edit</button></td>`
      table +='</tr>'
    })
    $("#sensorTable").html(table)
  })
}

function getSensors() {
  return $.ajax({
    method: "GET",
    url: "/cgi-bin/api/sensor.cgi"
  })
    .done(data => {
      return data
    })
    .fail(err => {
      console.error(err)
      throw err
    })
}