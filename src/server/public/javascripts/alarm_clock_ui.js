"use strict";
// Client-side interactions with the browser.

// Make connection to server when web page is fully loaded.
var socket = io.connect();
$(document).ready(function() {
    $('#error-box').css('display', 'block');
    $('#error-box').hide();
    // Get uptime of BBG
    // const uptimeInterval = window.setInterval(function() {
    //     //Request Uptime
    //     socket.emit('bbg','uptime');
    //     //Timer, if not cleared, then Node Server is down
    //     var errorTimer = setTimeout(function() {
    //         handleError("Lost Connection to Node Server, Restart Server and Refresh the page");
    //     }, 1000);
    //     socket.on('server', function(){
    //         // Tells client that server is still running
    //         clearTimeout(errorTimer);
    //     });
    // }, 1000);
    // // Get Data From beagleBone
    // const getDataInterval = window.setInterval(function() {
    //     //Request Data
    //     socket.emit('bbg','get');
    // }, 200);
    //Buttons That send commands
	$('#modeNone').click(function(){
        socket.emit('bbg','mode ' + 'none');
	});
	$('#modeRock1').click(function(){
        socket.emit('bbg','mode ' + 'standard');
	});
	$('#modeRock2').click(function(){
        socket.emit('bbg','mode ' + 'beatTwo');
	});
	$('#volumeDown').click(function(){
		var volume = parseInt($('#volumeid').val());
        volume -= 5;
        if(volume >= 0){
            socket.emit('bbg','volume ' + volume.toString());
        }
	});
    $('#volumeUp').click(function(){
        var volume = parseInt($('#volumeid').val());
        volume += 5;
        if(volume <= 100){
            socket.emit('bbg','volume ' + volume.toString());
        }
	});
    $('#tempoDown').click(function(){
        var tempo = parseInt($('#tempoid').val());
        tempo -= 5;
        if(tempo > 0){
            socket.emit('bbg','tempo ' + tempo.toString());
        }
	});
    $('#tempoUp').click(function(){
        var tempo = parseInt($('#tempoid').val());
        tempo += 5;
        socket.emit('bbg','tempo ' + tempo.toString());
	});
    $('#hhatSound').click(function(){
        socket.emit('bbg', 'play ' + 'hhat');
	});
    $('#snareSound').click(function(){
        socket.emit('bbg', 'play ' + 'snare');
	});
    $('#baseSound').click(function(){
        socket.emit('bbg', 'play ' + 'base');
	});
    //Error Handling
    socket.on('error',function(msg){
        handleError(msg);
    });
    function handleError(msg){
        clearInterval(uptimeInterval);
        clearInterval(getDataInterval);
        console.log(msg);
        $('#error-text').html(msg);
        $('#error-box').show();
        var unhidetimer = setTimeout(function(){
            $('#error-box').hide();
        }, 10000);
    };
    // Update time
    // socket.on('uptime', function(status) {
    //     // Update the uptime
    //     $('#status').html(status);
    // });
    // socket.on('data', function(msg){
    //     // Set data from BBG
    //     var mode = msg.split(' ')[0]
    //     var volume = msg.split(' ')[1]
    //     var tempo = msg.split(' ')[2]
    //     $('#modeid').html(mode);
    //     $('#volumeid').val(volume);
    //     $('#tempoid').val(tempo);
    // });
});