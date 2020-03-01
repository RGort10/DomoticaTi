function login() {
  const loginBody = {
    email: $('#loginEmail').val(),
    password: $('#loginPassword').val()
  }
  $.ajax({
    method: "POST",
    url: "/cgi-bin/api/login.cgi",
    data: JSON.stringify(loginBody)
  })
    .done(() => {
      window.location.assign("/sensor.html");
    })
}

function logout() {
  $.ajax({
    method: "GET",
    url: "/cgi-bin/api/login.cgi?logout"
  })
    .done(() => {
      window.location.assign("/login.html");
    })
}

let oldXHROpen = window.XMLHttpRequest.prototype.open;window.XMLHttpRequest.prototype.open = function(method, url, async, user, password) {
  // do something with the method, url and etc.
  this.addEventListener('load', function() {
   // do something with the response text
   if(this.status === 401) {
    window.location.assign('/login.html')
   }
  });            
  return oldXHROpen.apply(this, arguments);
}
