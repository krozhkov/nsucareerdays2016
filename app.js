
console.log('start http client.');

var settings = [
	{ pin: 5, resource: 'light01' },
	{ pin: 2, resource: 'light02' },
	{ pin: 0, resource: 'light03' }
];
var powerPin = 4;

function initPins(settings) {
	settings.forEach(function (setting) {
		GPIO.setmode(setting.pin, 0, 0);
		GPIO.write(setting.pin, false);
	});
	GPIO.setmode(powerPin, 0, 0);
	GPIO.write(powerPin, true);
}

function changedCallback(cb) {
	console.log('sending request...', cb, Wifi.status());
	if (cb == 2) {
		console.log('wait 2 seconds and send request.');
		setTimeout(sendRequest, 2000);
	}
};

function sendRequest() {
	console.log('sending request...');
	Http.request({
		hostname: 'ngurestexample.us-east-1.elasticbeanstalk.com',
		port: 80,
		path: '/',
		method: 'GET'
	},
	function(response) {
		console.log('receive response.');
		var states = JSON.parse(response.body);
		console.log('response body:', states);
		updateLightStates(states);
		setTimeout(sendRequest, 1000);
	})
	.end()
	.setTimeout(5000, function() {
		console.log('abort request.');
		this.abort();
		setTimeout(sendRequest, 10000);
	});
}

function updateLightStates(states) {
	settings.forEach(function (setting) {
		var state = !!states[setting.resource];
		GPIO.write(setting.pin, state);
	});
}

initPins(settings);
Wifi.changed(changedCallback);
