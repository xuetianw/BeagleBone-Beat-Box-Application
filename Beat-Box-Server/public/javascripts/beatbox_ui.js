"use strict";
// Client-side interactions with the browser.
var INITIALSECOND;
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
    socket.emit('proc', "get time");
});

function sendPrimeCommand(message) {
	socket.emit('prime', message);
}

function sendRequest() {
    socket.emit('prime', "get volume\n");
    socket.emit('prime', "get bpm\n");
    socket.emit('prime', "current mode\n");
    socket.emit('proc', 'uptime');
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
    // console.log("testest");



    // var newDiv = $('<div></div>').text(result);
    // $('#messages').append(newDiv);
    // $('#messages').scrollTop($('#messages').prop('scrollHeight'));
});

socket.on('disconnect', function() {
    $('#error-box').show();
})


// Handle data coming back from the server
socket.on('fileContents', function(result) {
    var fileName = result.fileName;
    var contents = result.contents;
//		console.log("fileContenst callback: fileName " + fileName
//				+ ", contents: " + contents);

    var domObj;
    switch(fileName) {
        case 'version':
            domObj = $('#versionId');
            break;
        case 'uptime':
            domObj = $('#status');
            break;
        case 'cpuinfo':
            domObj = $('#cpuinfoId');
            break;
        case 'cmdline':
            domObj = $('#cmdlineId');
            break;
        default:
            console.log("Unknown DOM object: " + fileName);
            return;
    }
    // Make linefeeds into <br> tag.
    // contents = replaceAll(contents, "\n", "<br/>");
    // console.log(contents.substring(0, 5))
    var uptmeInSeconds = parseInt(contents) - INITIALSECOND;
    var minutes, hours, secconds
    if (uptmeInSeconds > 60) {
        minutes = uptmeInSeconds / 60;
        secconds = uptmeInSeconds % 60

        if (minutes > 60) {
            hours = minutes / 60;
            minutes = minutes % 60
        }

    } else {
        hours = 0;
        minutes = 0;
        secconds = uptmeInSeconds
    }


    domObj.html("devices up for : \n" + "<br/>"+ hours.toString() + ":" + minutes.toString() + " : " + secconds.toString() + " H : M: S");
});


socket.on("initialtime", function(result) {
    INITIALSECOND = parseInt((result.contents).substring(0, 5));
    // console.log(INITIALSECOND)
})