function getArduino(id) {
	$.ajax({
		method: "GET",
		url: '/cgi-bin/api/arduino.cgi?' + id
	})
		.done(data => {
			const arduino = data.data[0]
			$('#modalArduino').modal('toggle')
			$(`#modalArduino-arduinoname`).html(arduino.arduinoname)
			$(`#modalArduino-staticip`).html(arduino.staticip)
		})
}
