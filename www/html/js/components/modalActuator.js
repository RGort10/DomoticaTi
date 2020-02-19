function getModalActuator(id) {
  getActuator(id)
		.then(data => {
			const actuator = data.data[0]
			$('#modalActuator').modal('toggle')
			$(`#modalActuator-actuatorname`).html(actuator.actuatorname)
			$(`#modalActuator-value`).html(actuator.value)
			$(`#modalActuator-type`).html(actuator.type)
			$(`#modalActuator-arduinoid`).html(actuator.arduinoid)
			$(`#modalActuator-arduinocomponentid`).html(actuator.arduinovalueid)
			$(`#modalActuator-iopin`).html(actuator.iopin === '1' ? 'I/O' : 'PWM')
			$(`#modalActuator-maximumvalue`).html(actuator.maximumvalue)
			$(`#modalActuator-minimumvalue`).html(actuator.minimumvalue)
		})
}
