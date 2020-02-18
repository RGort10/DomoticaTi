function initArduino() {
  makeArduinoTable()
}

setInterval(() => {
  makeArduinoTable()
}, 10000)

function makeArduinoTable() {
  getArduinos()
  .then((data) => {
    let table = '';
    $.each(data.data, (index, arduino) => {
      table +=`<tr>`
      table +=`<th onclick="getArduino(${arduino.arduinoid})" scope="row">${arduino.arduinoid}</th>`
      table +=`<td onclick="getArduino(${arduino.arduinoid})">${arduino.arduinoname}</td>`
      table +=`<td onclick="getArduino(${arduino.arduinoid})">${arduino.staticip}</td>`
			table +=`<td><button style="margin-right: 4px" class="btn btn-danger" onclick="deleteArduino(${arduino.arduinoid})"><i class="fas fa-trash-alt"></i> Delete</button>`
			table +=`<button class="btn btn-primary" onclick="editArduino(${arduino.arduinoid})"><i class="fas fa-pencil-alt"></i> Edit</button></td>`
      table +='</tr>'
    })
    $("#arduinoTable").html(table)
  })
}

function getArduinos() {
  return $.ajax({
    method: "GET",
    url: "/cgi-bin/api/arduino.cgi"
  })
    .done(data => {
      return data
    })
    .fail(err => {
      console.error(err)
      throw err
    })
}

function deleteArduino(id) {
  $.ajax({
    method: "DELETE",
    url: "/cgi-bin/api/arduino.cgi?"+id
  })
    .done(() => {
      makeArduinoTable()
    })
}