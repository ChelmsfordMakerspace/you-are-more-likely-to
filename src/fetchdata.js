function HTTPGET(url) {
	var req = new XMLHttpRequest();
	req.open("GET", url, false);
	req.send(null);
	return req.responseText;
}

function fetchWeather(position) {
	//Get weather info
	var response = HTTPGET("http://192.168.1.192/api?" +
    "lat=" + position.coords.latitude + "&lon=" + position.coords.longitude, true);
		
	//Convert to JSON
	var json = JSON.parse(response);
		
	//Extract the data
	var fact = json.fact;
	var error = json.error;
	var times = Math.round(json.timesmorelikely);
	
	//Console output to check all is working.
	console.log("Fact: " + fact);
	console.log("Times: " + times);
	console.log("Error: " + error);
	
	//Construct a key-value dictionary	
	var dict = {"FACT": fact, "TIMES": times};
	
	//Send data to watch for display
	Pebble.sendAppMessage(dict);
}

function getLocation(pos) {
  navigator.geolocation.getCurrentPosition(fetchWeather);
}

Pebble.addEventListener("ready",
  function(e) {
	//App is ready to receive JS messages
	console.log("READY TO GO");
	getLocation();
  }
);

Pebble.addEventListener("appmessage",
  function(e) {
	//Watch wants new data!
	console.log("Watch data recieved");
	getLocation();
  }
);