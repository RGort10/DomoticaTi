function saveSettingsActuator() {
  const actuatorBody = _.cloneDeep(localActuator)
  actuatorBody.iopin = $(`#modalSettingsActuator-iopin`).prop("checked") ? '1' : '0' // +0 for true to 1 and false to 0
  actuatorBody.minimumvalue = $(`#modalSettingsActuator-minimumvalue`).val()
  actuatorBody.maximumvalue = $(`#modalSettingsActuator-maximumvalue`).val()
  actuatorBody.value = actuatorBody.value == actuatorBody.maximumvalue ? actuatorBody.maximumvalue : actuatorBody.minimumvalue

  $.ajax({
    method: 'PUT',
    url: '/cgi-bin/api/actuator.cgi?settings+' + actuatorBody.actuatorid,
    data: JSON.stringify(actuatorBody)
  })
    .done(() => {
      emptySettingsActuatorForm()
      $('#modalSettingsActuator').modal('toggle')
      getModalActuator(actuatorBody.actuatorid)
      makeActuatorTable()
    })
    .fail((err) => {
      if(err.status === 400) {
        $.each(err.responseJSON, (key, value) => {
          if(value === false) {
            $(`#modalSettingsActuator-${key}`).addClass('is-invalid')
            $(`#modalSettingsActuator-${key}`).removeClass('is-valid')
          } else {
            $(`#modalSettingsActuator-${key}`).addClass('is-valid')
            $(`#modalSettingsActuator-${key}`).removeClass('is-invalid')
          }
        })
      }
    })
}

function settingsActuator(id) {
  this.getActuator(id)
    .then(data => {
      const actuator = data.data[0]
      console.log(actuator)
      $('#modalSettingsActuator').modal('toggle')
			$(`#modalSettingsActuator-iopin`).prop("checked", actuator.iopin == '1')
			$(`#modalSettingsActuator-minimumvalue`).val(actuator.minimumvalue)
			$(`#modalSettingsActuator-maximumvalue`).val(actuator.maximumvalue)
		})
}

function emptySettingsActuatorForm() {
	$(`#modalSettingsActuator-iopin`).removeClass('is-valid')
	$(`#modalSettingsActuator-minimumvalue`).removeClass('is-valid')
	$(`#modalSettingsActuator-maximumvalue`).removeClass('is-valid')
	$(`#modalSettingsActuator-iopin`).removeClass('is-invalid')
	$(`#modalSettingsActuator-minimumvalue`).removeClass('is-invalid')
	$(`#modalSettingsActuator-maximumvalue`).removeClass('is-invalid')
	$(`#modalSettingsActuator-iopin`).prop("checked", false)
	$(`#modalSettingsActuator-minimumvalue`).val(null)
	$(`#modalSettingsActuator-maximumvalue`).val(null)
}