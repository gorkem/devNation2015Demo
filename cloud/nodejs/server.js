#!/bin/env node

var mosca = require('mosca');
var ipaddress = process.env.OPENSHIFT_NODEJS_IP || "127.0.0.1";
var port = process.env.OPENSHIFT_NODEJS_PORT || 8080;


console.log('Starting server on %s port %s',ipaddress, port );
var server = new mosca.Server({
  port:port,
  host:ipaddress,
  onlyHttp:true,
  http: {
        host:ipaddress,
        port:port,
        bundle: true,
        static: './'
  }
}, function(error){console.log('error');});


server.on('ready', setup);
// fired when the mqtt server is ready
function setup() {
console.log('Mosca server is up and running');
}
// fired whena  client is connected
server.on('clientConnected', function(client) {
console.log('client connected', client.id);
});
// fired when a message is received
server.on('published', function(packet, client) {
console.log('Published : ', packet.payload);
});
// fired when a client subscribes to a topic
server.on('subscribed', function(topic, client) {
console.log('subscribed : ', topic);
});
// fired when a client subscribes to a topic
server.on('unsubscribed', function(topic, client) {
console.log('unsubscribed : ', topic);
});
// fired when a client is disconnecting
server.on('clientDisconnecting', function(client) {
console.log('clientDisconnecting : ', client.id);
});
// fired when a client is disconnected
server.on('clientDisconnected', function(client) {
console.log('clientDisconnected : ', client.id);
});