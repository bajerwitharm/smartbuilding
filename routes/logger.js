module.exports.initLogger = function() {
	var server = net.createServer(function (stream) {
        stream.setEncoding('utf8');
        stream.addListener("data", function (datagram) {
            var datas = datagram.split("\n");
            for (var data in datas) {
              data = datas[data];
              if (data.length == 0) {
                continue;
              }
              write_message(data);
              sockets = sio.sockets.sockets;
              for (var id in sockets) {
                var socket = sockets[id];
                socket.get('regex', function (err, regex) {
                    if (!regex || data.search(regex) != -1) {
                      socket.emit('logs', [data]);
                    }
                  });
                    }
            }
          });
      }); 
    server.listen(514, '0.0.0.0');
}
    