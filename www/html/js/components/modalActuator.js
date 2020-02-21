function getModalActuator(id) {
  console.log('hallo')
  getActuator(id)
		.then(data => {
			const actuator = data.data[0]
			$('#modalActuator').modal('show')
			$(`#modalActuator-actuatorname`).html(actuator.actuatorname)
			$(`#modalActuator-value`).html(actuator.value)
			$(`#modalActuator-type`).html(actuator.type)
			$(`#modalActuator-arduinoid`).html(actuator.arduinoid)
			$(`#modalActuator-arduinocomponentid`).html(actuator.arduinovalueid)
			$(`#modalActuator-iopin`).html(actuator.iopin === '1' ? 'I/O' : 'PWM')
			$(`#modalActuator-iopin`).html(actuator.auto === '1' ? 'On' : 'Off')
			$(`#modalActuator-maximumvalue`).html(actuator.maximumvalue)
      $(`#modalActuator-minimumvalue`).html(actuator.minimumvalue)
      $('#modalActuator-deleteActuator').attr('onclick', `deleteActuator(${actuator.actuatorid})`)
      $('#modalActuator-editActuator').attr('onclick', `editActuator(${actuator.actuatorid})`)
      $('#modalActuator-settingsActuator').attr('onclick', `settingsActuator(${actuator.actuatorid})`)
      getActuatorHistory(id)
		})
}

function getActuatorHistory(id) {
  $.ajax({
		method: "GET",
		url: '/cgi-bin/api/actuator.cgi?last+' + id
  })
    .done(data => {
      let table = '';
      $.each(data.data, (index, record) => {
        table +=`<tr>`
        table +=`<td>${moment(record.date + '', 'YYYYMMDD').format('DD-MM-YYYY')}</th>`
        table +=`<td>${moment(record.time + '', 'HHmmss').format('HH:mm:ss')}</td>`
        table +=`<td>${record.value}</td>`
        table +=`<td>${record.bywho}</td>`
        table +='</tr>'
      })
      $("#modalActuatorHistory").html(table)
    })
}