
console.log('start program.');

var ledPin = 5;
var state = false;
 
GPIO.setmode(ledPin, 0, 0);
GPIO.write(ledPin, state);

loop();

function loop() {
	state = !state;
	GPIO.write(ledPin, state);
	console.log("level:", state);
	setTimeout(loop, 2000);
}
