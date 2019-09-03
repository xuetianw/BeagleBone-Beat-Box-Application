// After creating package.json, install modules:
//   $ npm install
//
// Launch server with:
//   $ node server.js
const path = require('path')
const http = require('http')
const express = require('express')
const socketio = require('socket.io')
const Filter = require('bad-words')

const app = express()
const server = http.createServer(app)
const io = socketio(server)

var dgram = require('dgram');


const port = process.env.PORT || 8080
const publicDirectoryPath = path.join(__dirname, '/public')

app.use(express.static(publicDirectoryPath))

io.on('connection', (socket) => {
	console.log('New WebSocket connection')

    socket.on('prime', function(data) {
        console.log('prime command: ' + data);

        // Info for connecting to the local process via UDP
        var PORT = 8088;
        var HOST = '127.0.0.1';
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
            console.log("UDP Client: message Rx" + remote.address + ':' + remote.port +' - ' + message);

            var reply = message.toString('utf8')
            socket.emit('commandReply', reply);

            client.close();

        });
        client.on("UDP Client: close", function() {
            console.log("closed");
        });
        client.on("UDP Client: error", function(err) {
            console.log("error: ",err);
        });
    });

})

server.listen(port, () => {
	console.log(`Server is up on port ${port}!`)
})


