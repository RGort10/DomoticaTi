function saveActuator() {
	const actuatorBody = {
		actuatorname: 	$('#modalNewActuator-actuatorname').val(),
		type: 					$('#modalNewActuator-type').val(),
		arduinoid: 			$('#modalNewActuator-arduinoid').val(),
		arduinocomponentid: $('#modalNewActuator-arduinocomponentid').val()
	}

	if (1===1){//validateActuator(actuatorBody) || 1 === 1) {
		$.ajax({
			method: 'POST',
			url: '/cgi-bin/api/actuator.cgi',
			data: JSON.stringify(actuatorBody)
		})
			.done(() => {
				$('#modalNewActuator').modal('toggle')
				emptyNewActuatorForm()
				makeActuatorTable()
			})
			.fail((err) => {
				if(err.status === 400) {
					$.each(err.responseJSON, (key, value) => {
						if(value === false) {
							$(`#modalNewActuator-${key}`).addClass('is-invalid')
							$(`#modalNewActuator-${key}`).removeClass('is-valid')
						} else {
							$(`#modalNewActuator-${key}`).addClass('is-valid')
							$(`#modalNewActuator-${key}`).removeClass('is-invalid')
						}
					})
				}
			})
	}
}

function emptyNewActuatorForm() {
	$(`#modalNewActuator-actuatorname`).removeClass('is-valid')
	$(`#modalNewActuator-type`).removeClass('is-valid')
	$(`#modalNewActuator-arduinoid`).removeClass('is-valid')
	$(`#modalNewActuator-arduinocomponentid`).removeClass('is-valid')
	$(`#modalNewActuator-actuatorname`).removeClass('is-invalid')
	$(`#modalNewActuator-type`).removeClass('is-invalid')
	$(`#modalNewActuator-arduinoid`).removeClass('is-invalid')
	$(`#modalNewActuator-arduinocomponentid`).removeClass('is-invalid')
	$(`#modalNewActuator-actuatorname`).val(null)
	$(`#modalNewActuator-type`).val(null)
	$(`#modalNewActuator-arduinoid`).val(null)
	$(`#modalNewActuator-arduinocomponentid`).val(null)
}