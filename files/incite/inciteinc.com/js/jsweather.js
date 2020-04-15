$(document).ready(function() {
  
  var setWeather = function(dta) {
    var temp = Math.round(dta.main.temp);
    var weather = dta.weather[0];
    var icon;

    if (weather.description === 'clear sky') {
      if (weather.icon.substr(-1, 1) === 'n') {
        icon = 'icon-weather_night';
      } else {
        icon = 'icon-weather_sunny';
      }
    }
    else if (weather.description === 'few clouds') {
      icon = 'icon-weather_partlycloudy'; 
    }
    else if (weather.description === 'scattered clouds') {
      icon = 'icon-weather_cloudy';
    }
    else if (weather.description === 'broken clouds') {
      icon = 'icon-weather_partlycloudy';
    }
    else if (weather.description === 'shower rain') {
      icon = 'icon-weather_rain';
    }
    else if (weather.description === 'rain') {
      icon = 'icon-weather_rain';
    }
    else if (weather.description.indexOf('rain') != -1) {
      icon = 'icon-weather_rain';
    }
    else if (weather.description === 'thunderstorm') {
      icon = 'icon-weather_rain';
    }
    else if (weather.description === 'snow') {
      icon = 'icon-weather_snow';
    }
    else if (weather.description === 'mist') {
      icon = 'icon-weather_cloudy';
    }

    $('.temp').text( temp + 'º F' );
    $('[icon-weather]')[0].className = icon;
    // $('#ip').val( dta.userIp );
    // $('#user-agent').val( navigator.userAgent );
  }

  var setTime = function() {
    var date = new Date();
    var hours = date.getHours();
    var minutes = date.getMinutes();
    var ampm = hours >= 12 ? 'PM' : 'AM';
    hours = hours % 12;
    hours = hours ? hours : 12; // the hour '0' should be '12'
    minutes = minutes < 10 ? '0'+minutes : minutes;
    var strTime = hours + ':' + minutes + ' ' + ampm;

    $('.time').text(strTime);

  }

  setInterval(setTime, 1000);

  var x = document.querySelector('.weather')
  function getLocation() {

    if (navigator.geolocation) {
      navigator.geolocation.getCurrentPosition(showPosition);

    } else {
      x.innerHTML = "Geolocation is not supported by this browser.";
    }
  }
  getLocation();

  function showPosition(position) {
    var lat = position.coords.latitude;
    var lon = position.coords.longitude;

    $.ajax({
      url: 'https://weather-api.joystickinteractive.com/?lat=' + lat + '&lon=' + lon + '&units=imperial',
    }).done(function(data) {

      var temp = data.main.temp;

      setWeather(data);
    });

    // x.innerHTML = "Latitude: " + position.coords.latitude +
    // "<br>Longitude: " + position.coords.longitude;
  }

});



