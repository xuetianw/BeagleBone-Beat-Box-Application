"use strict";
// Client-side interactions with the browser.

// Make connection to server when web page is fully loaded.
var socket = io.connect("http://localhost:3042");
$(document).ready(function() {
	$('#modeNothing').click(function(){
		sendPrimeCommand("MUSIC1");
	});
	$('#modeRock1').click(function(){
		sendPrimeCommand("MUSIC2");
	});
	$('#modeRock2').click(function(){
		sendPrimeCommand("MUSIC3");
	});
	$('#btnLast').click(function(){
		sendPrimeCommand("last 5");
	});
	$('#btnStop').click(function(){
		sendPrimeCommand("stop");
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