#!/usr/bin/env node

var exec = require('child_process').exec;
var plist = require('plist');

var threshold = 10;

exec('/usr/sbin/ioreg -arc AppleSmartBattery', function(err, stdout, stderr) {
	if (err) return console.log('Error', err);
	var obj = plist.parse(stdout.toString())[0];
	// console.log(obj);

	var externalConnected = obj.ExternalConnected;
	var fullyCharged = obj.FullyCharged;
	var charging = obj.IsCharging;
	var level = (obj.CurrentCapacity * 100 / obj.MaxCapacity).toFixed(0); // 0 digits after the decimal point

	if (externalConnected) {
		if (fullyCharged) console.log('✔');
		else console.log('▲ %d%', level);
	} else {
		if (level < threshold) console.log('▼ %d%!', level);
		else console.log('▼ %d%', level);
	}

	//console.log('Ext %s full %s charging %s level %d', externalConnected, fullyCharged, charging, level);
});
