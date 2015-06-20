/*eslint-env node */
"use strict";

var BlePeribherals = require('./ble/blePeripherals');

var peripherals = new BlePeribherals.BlePeripherals();

peripherals.on('sensorDiscovered', function(sensor){
	sensor.poll(2000, function(data){
		console.log(data.readFloatLE(0));
	});
});