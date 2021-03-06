

function saveEditActuator() {
	const actuatorBody = {
		actuatorid: 	$('#modalEditActuator-actuatorid').html(),
		actuatorname: 	$('#modalEditActuator-actuatorname').val(),
		type: 					$('#modalEditActuator-type').val(),
		arduinoid: 			$('#modalEditActuator-arduinoid').val(),
		arduinocomponentid: $('#modalEditActuator-arduinocomponentid').val()
	}

  $.ajax({
    method: 'PUT',
    url: '/cgi-bin/api/actuator.cgi?' + actuatorBody.actuatorid,
    data: JSON.stringify(actuatorBody)
  })
    .done(() => {
      emptyEditActuatorForm()
      $('#modalEditActuator').modal('toggle')
      getModalActuator(actuatorBody.actuatorid)
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

function editActuator(id) {
  getActuator(id)
    .then(data => {
      const actuator = data.data[0]
			$('#modalEditActuator').modal('toggle')
			$(`#modalEditActuator-actuatorid`).html(actuator.actuatorid)
			$(`#modalEditActuator-actuatorname`).val(actuator.actuatorname)
			$(`#modalEditActuator-type`).val(actuator.type)
			$(`#modalEditActuator-arduinoid`).val(actuator.arduinoid)
			$(`#modalEditActuator-arduinocomponentid`).val(actuator.arduinocomponentid)
		})
}

function emptyEditActuatorForm() {
	$(`#modalEditActuator-actuatorname`).removeClass('is-valid')
	$(`#modalEditActuator-type`).removeClass('is-valid')
	$(`#modalEditActuator-arduinoid`).removeClass('is-valid')
	$(`#modalEditActuator-arduinocomponentid`).removeClass('is-valid')
	$(`#modalEditActuator-actuatorname`).removeClass('is-invalid')
	$(`#modalEditActuator-type`).removeClass('is-invalid')
	$(`#modalEditActuator-arduinoid`).removeClass('is-invalid')
	$(`#modalEditActuator-arduinocomponentid`).removeClass('is-invalid')
	$(`#modalEditActuator-actuatorname`).val(null)
	$(`#modalEditActuator-type`).val(null)
	$(`#modalEditActuator-arduinoid`).val(null)
	$(`#modalEditActuator-arduinocomponentid`).val(null)
}