function saveSensor() {
	const sensorBody = {
		sensorname: 	      $('#modalNewSensor-sensorname').val(),
		unit: 					    $('#modalNewSensor-unit').val(),
		arduinoid: 			    $('#modalNewSensor-arduinoid').val(),
		arduinocomponentid: $('#modalNewSensor-arduinocomponentid').val()
	}

  $.ajax({
    method: 'POST',
    url: '/cgi-bin/api/sensor.cgi',
    data: JSON.stringify(sensorBody)
  })
    .done(() => {
      $('#modalNewSensor').modal('toggle')
      emptyNewSensorForm()
      makeSensorTable()
    })
    .fail((err) => {
      if(err.status === 400) {
        $.each(err.responseJSON, (key, value) => {
          if(value === false) {
            $(`#modalNewSensor-${key}`).addClass('is-invalid')
            $(`#modalNewSensor-${key}`).removeClass('is-valid')
          } else {
            $(`#modalNewSensor-${key}`).addClass('is-valid')
            $(`#modalNewSensor-${key}`).removeClass('is-invalid')
          }
        })
      }
    })
}

function emptyNewSensorForm() {
	$(`#modalNewSensor-sensorname`).removeClass('is-valid')
	$(`#modalNewSensor-unit`).removeClass('is-valid')
	$(`#modalNewSensor-arduinoid`).removeClass('is-valid')
	$(`#modalNewSensor-arduinocomponentid`).removeClass('is-valid')
	$(`#modalNewSensor-sensorname`).removeClass('is-invalid')
	$(`#modalNewSensor-unit`).removeClass('is-invalid')
	$(`#modalNewSensor-arduinoid`).removeClass('is-invalid')
	$(`#modalNewSensor-arduinocomponentid`).removeClass('is-invalid')
	$(`#modalNewSensor-sensorname`).val(null)
	$(`#modalNewSensor-unit`).val(null)
	$(`#modalNewSensor-arduinoid`).val(null)
	$(`#modalNewSensor-arduinocomponentid`).val(null)
}