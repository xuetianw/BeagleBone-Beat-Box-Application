"use strict";
// Client-side interactions with the browser.

// Make connection to server when web page is fully loaded.
const socket = io()
$(document).ready(function() {
	$('#modeNone').click(function(){
		sendPrimeCommand("none\n");
	});
	$('#modeRock1').click(function(){
		sendPrimeCommand("rock 1\n");
	});
	$('#modeRock2').click(function(){
		sendPrimeCommand("rock 2\n");
	});
	$('#volumeDown').click(function(){
		sendPrimeCommand("decrease volume\n");
	});
    $('#volumeUp').click(function(){
        sendPrimeCommand("increase volume\n");
    });
    $('#BPMown').click(function(){
        sendPrimeCommand("decrease BPM\n");
    });
    $('#BPMUp').click(function(){
        sendPrimeCommand("increase BPM\n");
    });
    $('#Hi-Hat').click(function(){
        sendPrimeCommand("hi-hat\n");
    });
    $('#Snare').click(function(){
        sendPrimeCommand("snare\n");
    });
    $('#Base').click(function(){
        sendPrimeCommand("base\n");
    });

	socket.on('commandReply', function(result) {
		var newDiv = $('<div></div>').text(result);
		$('#messages').append(newDiv);
		$('#messages').scrollTop($('#messages').prop('scrollHeight'));
	});
    window.setInterval(function() {sendRequest('uptime')}, 1000);

});

function sendPrimeCommand(message) {
	socket.emit('prime', message);
};