function saveEditSensor() {
  const sensorBody = {
    sensorid:   $('#modalEditSensor-sensorid').html(),
    sensorname:   $('#modalEditSensor-sensorname').val(),
    unit:           $('#modalEditSensor-unit').val(),
    arduinoid:       $('#modalEditSensor-arduinoid').val(),
    arduinocomponentid: $('#modalEditSensor-arduinocomponentid').val()
  }

  if (1===1){//validateSensor(sensorBody) || 1 === 1) {
    $.ajax({
      method: 'PUT',
      url: '/cgi-bin/api/sensor.cgi?' + sensorBody.sensorid,
      data: JSON.stringify(sensorBody)
    })
      .done(() => {
        $('#modalEditSensor').modal('toggle')
        emptyEditSensorForm()
        makeSensorTable()
      })
      .fail((err) => {
        if(err.status === 400) {
          $.each(err.responseJSON, (key, value) => {
            if(value === false) {
              $(`#modalEditSensor-${key}`).addClass('is-invalid')
              $(`#modalEditSensor-${key}`).removeClass('is-valid')
            } else {
              $(`#modalEditSensor-${key}`).addClass('is-valid')
              $(`#modalEditSensor-${key}`).removeClass('is-invalid')
            }
          })
        }
      })
  }
}

function editSensor(id) {
  $.ajax({
    method: "GET",
    url: '/cgi-bin/api/sensor.cgi?' + id
  })
    .done(data => {
      const sensor = data.data[0]
      $('#modalEditSensor').modal('toggle')
      $(`#modalEditSensor-sensorid`).html(sensor.sensorid)
      $(`#modalEditSensor-sensorname`).val(sensor.sensorname)
      $(`#modalEditSensor-unit`).val(sensor.unit === ' ' ? 'none' : sensor.unit)
      $(`#modalEditSensor-arduinoid`).val(sensor.arduinoid)
      $(`#modalEditSensor-arduinocomponentid`).val(sensor.arduinocomponentid)
    })
}

function emptyEditSensorForm() {
  $(`#modalEditSensor-sensorname`).removeClass('is-valid')
  $(`#modalEditSensor-unit`).removeClass('is-valid')
  $(`#modalEditSensor-arduinoid`).removeClass('is-valid')
  $(`#modalEditSensor-arduinocomponentid`).removeClass('is-valid')
  $(`#modalEditSensor-sensorname`).removeClass('is-invalid')
  $(`#modalEditSensor-unit`).removeClass('is-invalid')
  $(`#modalEditSensor-arduinoid`).removeClass('is-invalid')
  $(`#modalEditSensor-arduinocomponentid`).removeClass('is-invalid')
  $(`#modalEditSensor-sensorname`).val(null)
  $(`#modalEditSensor-unit`).val(null)
  $(`#modalEditSensor-arduinoid`).val(null)
  $(`#modalEditSensor-arduinocomponentid`).val(null)
}