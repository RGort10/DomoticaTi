function saveEditArduino() {
  const arduinoBody = {
    arduinoid:    $('#modalEditArduino-arduinoid').html(),
    arduinoname:  $('#modalEditArduino-arduinoname').val(),
    staticip:     $('#modalEditArduino-staticip').val(),
  }

  if (1===1){//validateArduino(arduinoBody) || 1 === 1) {
    $.ajax({
      method: 'PUT',
      url: '/cgi-bin/api/arduino.cgi?' + arduinoBody.arduinoid,
      data: JSON.stringify(arduinoBody)
    })
      .done(() => {
        $('#modalEditArduino').modal('toggle')
        emptyEditArduinoForm()
        makeArduinoTable()
      })
      .fail((err) => {
        if(err.status === 400) {
          $.each(err.responseJSON, (key, value) => {
            if(value === false) {
              $(`#modalEditArduino-${key}`).addClass('is-invalid')
              $(`#modalEditArduino-${key}`).removeClass('is-valid')
            } else {
              $(`#modalEditArduino-${key}`).addClass('is-valid')
              $(`#modalEditArduino-${key}`).removeClass('is-invalid')
            }
          })
        }
      })
  }
}

function editArduino(id) {
  $.ajax({
    method: "GET",
    url: '/cgi-bin/api/arduino.cgi?' + id
  })
    .done(data => {
      const arduino = data.data[0]
      $('#modalEditArduino').modal('toggle')
      $(`#modalEditArduino-arduinoid`).html(arduino.arduinoid)
      $(`#modalEditArduino-arduinoname`).val(arduino.arduinoname)
      $(`#modalEditArduino-staticip`).val(arduino.staticip)
    })
}

function emptyEditArduinoForm() {
  $(`#modalEditArduino-arduinoname`).removeClass('is-valid')
	$(`#modalEditArduino-staticip`).removeClass('is-valid')
	$(`#modalEditArduino-arduinoname`).removeClass('is-invalid')
	$(`#modalEditArduino-staticip`).removeClass('is-invalid')
	$(`#modalEditArduino-arduinoname`).val(null)
	$(`#modalEditArduino-staticip`).val(null)
}