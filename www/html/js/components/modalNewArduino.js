function saveArduino() {
	const arduinoBody = {
		arduinoname: 	      $('#modalNewArduino-arduinoname').val(),
		staticip: 					    $('#modalNewArduino-staticip').val(),
	}

  $.ajax({
    method: 'POST',
    url: '/cgi-bin/api/arduino.cgi',
    data: JSON.stringify(arduinoBody)
  })
    .done(() => {
      $('#modalNewArduino').modal('toggle')
      emptyNewArduinoForm()
      makeArduinoTable()
    })
    .fail((err) => {
      if(err.status === 400) {
        $.each(err.responseJSON, (key, value) => {
          if(value === false) {
            $(`#modalNewArduino-${key}`).addClass('is-invalid')
            $(`#modalNewArduino-${key}`).removeClass('is-valid')
          } else {
            $(`#modalNewArduino-${key}`).addClass('is-valid')
            $(`#modalNewArduino-${key}`).removeClass('is-invalid')
          }
        })
      }
    })
}

function emptyNewArduinoForm() {
	$(`#modalNewArduino-arduinoname`).removeClass('is-valid')
	$(`#modalNewArduino-staticip`).removeClass('is-valid')
	$(`#modalNewArduino-arduinoname`).removeClass('is-invalid')
	$(`#modalNewArduino-staticip`).removeClass('is-invalid')
	$(`#modalNewArduino-arduinoname`).val(null)
	$(`#modalNewArduino-staticip`).val(null)
}