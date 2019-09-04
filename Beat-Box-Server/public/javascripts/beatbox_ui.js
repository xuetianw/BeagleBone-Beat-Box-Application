"use strict";
// Client-side interactions with the browser.

// Make connection to server when web page is fully loaded.
const socket = io()
$(document).ready(function() {
	$('#modeNothing').click(function(){
		sendPrimeCommand("none");
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
		sendPrimeCommand("increase volume");
	});
	$('#PCMDown').click(function(){
		sendPrimeCommand("decrease BPM\n");
	});
	$('#PCMUp').click(function(){
		sendPrimeCommand("increase BPM\n");
	});

	socket.on('commandReply', function(result) {
		var newDiv = $('<div></div>').text(result);
		$('#messages').append(newDiv);
		$('#messages').scrollTop($('#messages').prop('scrollHeight'));
	});
	
});

function sendPrimeCommand(message) {
	socket.emit('prime', message);
};