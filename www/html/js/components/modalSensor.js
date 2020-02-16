function getSensor(id) {
	$.ajax({
		method: "GET",
		url: '/cgi-bin/api/sensor.cgi?' + id
	})
		.done(data => {
			const sensor = data.data[0]
			$(`#modalSensor-sensorname`).html(sensor.sensorname)
			$(`#modalSensor-unit`).html(sensor.unit == " " ? 'none' : sensor.unit)
			$(`#modalSensor-arduinoid`).html(sensor.arduinoid)
			$(`#modalSensor-arduinocomponentid`).html(sensor.arduinocomponentid)
      $('#modalSensor').modal('toggle')
      getSensorHistory(id)
		})
}


function getSensorHistory(id) {
  $.ajax({
		method: "GET",
		url: '/cgi-bin/api/sensor.cgi?last+' + id
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