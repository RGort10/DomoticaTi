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
			$(`#modalActuator-iopin`).html(actuator.auto === '1' ? 'On' : 'Off')
			$(`#modalActuator-maximumvalue`).html(actuator.maximumvalue)
      $(`#modalActuator-minimumvalue`).html(actuator.minimumvalue)
      $('#modalActuator-deleteActuator').attr('onclick', `deleteActuator(${actuator.actuatorid})`)
      $('#modalActuator-editActuator').attr('onclick', `editActuator(${actuator.actuatorid})`)
      $('#modalActuator-settingsActuator').attr('onclick', `settingsActuator(${actuator.actuatorid})`)
		})
}
