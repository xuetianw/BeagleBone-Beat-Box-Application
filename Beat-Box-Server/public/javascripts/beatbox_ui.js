"use strict";
// Client-side interactions with the browser.

// Make connection to server when web page is fully loaded.
const socket = io()
$(document).ready(function() {

    window.setInterval(function() {sendRequest()}, 1000);
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
});

function sendPrimeCommand(message) {
	socket.emit('prime', message);
};

function sendRequest() {
    socket.emit('prime', "get volume\n");
    socket.emit('prime', "get bpm\n");
    socket.emit('prime', "current mode\n");

}

socket.on('commandReply', function(result) {
    console.log(result);
    if (result.includes("current volume is ")) {
        var temp = result.substring("current volume is ".length, result.length);
        $('#volumeid').val(temp)
    } else if( result.includes("current BPM is")) {
        var temp = result.substring("current BPM is ".length, result.length);
        $('#BPMid').val(temp)
    } else if( result.includes("current mode is ")) {
        var temp = result.substring("current mode is ".length, result.length);
        $('#modeid').html(temp)
    }
});