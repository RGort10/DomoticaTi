function saveEditActuator() {
	const actuatorBody = {
		actuatorname: 	$('#modalEditActuator-actuatorname').val(),
		type: 					$('#modalEditActuator-type').val(),
		arduinoid: 			$('#modalEditActuator-arduinoid').val(),
		arduinovalueid: $('#modalEditActuator-arduinovalueid').val()
	}

	if (1===1){//validateActuator(actuatorBody) || 1 === 1) {
		$.ajax({
			method: 'POST',
			url: '/cgi-bin/api/actuator.cgi',
			data: JSON.stringify(actuatorBody)
		})
			.done(() => {
				emptyNewActuatorForm()
				$('modalEditActuator').modal('toggle')
				makeActuatorTable()
			})
			.fail((err) => {
				if(err.status === 400) {
					$.each(err.responseJSON, (key, value) => {
						if(value === false) {
							$(`#modalEditActuator-${key}`).addClass('is-invalid')
							$(`#modalEditActuator-${key}`).removeClass('is-valid')
						} else {
							$(`#modalEditActuator-${key}`).addClass('is-valid')
							$(`#modalEditActuator-${key}`).removeClass('is-invalid')
						}
					})
				}
			})
	}
}

function emptyEditActuatorForm() {
	$(`#modalEditActuator-actuatorname`).removeClass('is-valid')
	$(`#modalEditActuator-type`).removeClass('is-valid')
	$(`#modalEditActuator-arduinoid`).removeClass('is-valid')
	$(`#modalEditActuator-arduinovalueid`).removeClass('is-valid')
	$(`#modalEditActuator-actuatorname`).removeClass('is-invalid')
	$(`#modalEditActuator-type`).removeClass('is-invalid')
	$(`#modalEditActuator-arduinoid`).removeClass('is-invalid')
	$(`#modalEditActuator-arduinovalueid`).removeClass('is-invalid')
	$(`#modalEditActuator-actuatorname`).val(null)
	$(`#modalEditActuator-type`).val(null)
	$(`#modalEditActuator-arduinoid`).val(null)
	$(`#modalEditActuator-arduinovalueid`).val(null)
}