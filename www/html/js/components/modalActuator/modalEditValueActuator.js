function editValueActuator(id) {
  getActuator(id)
  .then(() => {
    if(localActuator.iopin === '1') {
      $('#modalEditValueActuator-io').css('display', 'block')      
      $('#modalEditValueActuator-pwm').css('display', 'none')
      $(`#modalEditValueActuator-iovalue`).prop("checked", localActuator.value === localActuator.maximumvalue)      
    } else {
      $('#modalEditValueActuator-io').css('display', 'none')      
      $('#modalEditValueActuator-pwm').css('display', 'block')
      $('#modalEditValueActuator-pwmSlider').attr('min', localActuator.minimumvalue)
      $('#modalEditValueActuator-pwmInput').attr('min', localActuator.minimumvalue)
      $('#modalEditValueActuator-pwmSlider').attr('max', localActuator.maximumvalue)
      $('#modalEditValueActuator-pwmInput').attr('max', localActuator.maximumvalue)
      $('#modalEditValueActuator-pwmSlider').attr('value', localActuator.value)
      $('#modalEditValueActuator-pwmInput').attr('value', localActuator.value)
      $('#modalEditValueActuator-pwmSlider').val(localActuator.value)
      $('#modalEditValueActuator-pwmInput').val(localActuator.value)
    }
    $('#modalEditValueActuator-errorMessage').html('')
    $('#modalEditValueActuator').modal('toggle')
  })
}

function syncPWMValue(inputNumber){
  switch(inputNumber) {
    case 0: 
      $('#modalEditValueActuator-pwmInput').val(
        $('#modalEditValueActuator-pwmSlider').val()
      )
      break

    case 1: 
      $('#modalEditValueActuator-pwmSlider').val(
        $('#modalEditValueActuator-pwmInput').val()
      )
      break
  }
}

function saveEditValueActuator() {
  if(localActuator.iopin === '1') {
    localActuator.value = $(`#modalEditValueActuator-iovalue`).prop("checked") ? localActuator.maximumvalue : localActuator.minimumvalue
  } else {
    if($('#modalEditValueActuator-pwmSlider').val() === $('#modalEditValueActuator-pwmInput').val()) {
      localActuator.value = $('#modalEditValueActuator-pwmSlider').val()
    } else {
      $('#modalEditValueActuator-errorMessage').addClass('btn-outline-danger')
      $('#modalEditValueActuator-errorMessage').html(`Please check everything! The two values don't match`)
      throw 'No matching values'
    }
  }

  $.ajax({
    method: 'PUT',
    url: '/cgi-bin/api/actuator.cgi?value+' + localActuator.actuatorid,
    data: JSON.stringify(localActuator)
  })
    .done(() => {
      $('#modalEditValueActuator').modal('toggle')
      makeActuatorTable()
    })
    .fail((err) => {
      console.log(err)
    })
}