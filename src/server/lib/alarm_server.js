"use strict";
/*
 * Respond to commands over a websocket to relay UDP commands to a local program
 */

var socketio = require('socket.io');
var io;

var dgram = require('dgram');

exports.listen = function(server) {
	io = socketio.listen(server);
	io.set('log level 1');

	io.sockets.on('connection', function(socket) {
		handleCommand(socket);
	});
};

function handleCommand(socket) {
	// Pased string of comamnd to relay
	socket.on('bbg', function(data) {
		// Info for connecting to the local process via UDP
		var errorTimer = setTimeout(function() {
            socket.emit("error","Lost Connection to BBG, Restart Server and Refresh the page");
        }, 1000);
		if(data != "uptime"){
			clearTimeout(errorTimer);
		}else{
			socket.emit('server','response');
		}
		var PORT = 12345;
		var HOST = '127.0.0.2';
		var buffer = new Buffer(data);
		var client = dgram.createSocket('udp4');
		client.send(buffer, 0, buffer.length, PORT, HOST, function(err, bytes) {
			if (err) 
				throw err;
			console.log('UDP message sent to ' + HOST +':'+ PORT);
		});
		client.on('listening', function () {
			var address = client.address();
			console.log('UDP Client: listening on ' + address.address + ":" + address.port);
		});
		// Handle an incoming message over the UDP from the local application.
		client.on('message', function (message, remote) {
			clearTimeout(errorTimer);
			console.log("UDP Client: message Rx" + remote.address + ':' + remote.port +' - ' + message);
			var reply = message.toString('utf8')
			var command = reply.split(' ')[0];
			// https://stackoverflow.com/questions/6871403/how-can-i-delete-the-first-word-from-a-line
			// For remove of first token
			socket.emit(command, reply.split(' ').slice(1).join(' '));
			client.close();

		});
		client.on("UDP Client: close", function() {
			console.log("closed");
		});
		client.on("UDP Client: error", function(err) {
			console.log("error: ",err);
		});
	});
};