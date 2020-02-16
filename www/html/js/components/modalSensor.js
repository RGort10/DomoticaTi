function getSensor(id) {
	$.ajax({
		method: "GET",
		url: '/cgi-bin/api/sensor.cgi?' + id
	})
		.done(data => {
			const actuator = data.data[0]
			$('#modalActuator').modal('toggle')
			$(`#modalActuator-actuatorname`).html(actuator.actuatorname)
			$(`#modalActuator-value`).html(actuator.value)
			$(`#modalActuator-type`).html(actuator.type)
			$(`#modalActuator-arduinoid`).html(actuator.arduinoid)
			$(`#modalActuator-arduinocomponentid`).html(actuator.arduinovalueid)
		})
}


function getSensorHistory() {
  setTimeout(() => {
    getSensorHistory()
  }, 10000)
  $.ajax({
		method: "GET",
		url: '/cgi-bin/api/sensorHistory.cgi'
  })
    .done(data => {
      let table = '';
      $.each(data.data, (index, record) => {
        table +=`<tr>`
        table +=`<td>${moment(record.date + '', 'YYYYMMDD').format('DD-MM-YYYY')}</th>`
        table +=`<td>${moment(record.time + '', 'HHmmss').format('HH:mm:ss')}</td>`
        table +=`<td>${record.value}</td>`
        table +='</tr>'
      })
      $("#modalSensorHistory").html(table)
    })
}