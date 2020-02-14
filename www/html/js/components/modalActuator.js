function getActuator(id) {
	$.ajax({
		method: "GET",
		url: '/cgi-bin/api/actuator.cgi?' + id
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
