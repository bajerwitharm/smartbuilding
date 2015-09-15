var io = require('socket.io').listen(instance);
io.sockets.on('connection', function(socket) {
    console.log('Connection to the node.js socket')
});