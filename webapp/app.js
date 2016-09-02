'use strict';

var config = require('./config');
var server = require('./server');
var superagent = require('superagent');
var io = require('socket.io').listen(server);
var port = process.env.PORT || 9100;

server.listen(port);

var App = function() {
    return this.init();
};

App.prototype.init = function() {
    this.bathStatusUrl = 'https://api.particle.io/v1/devices/'+config.deviceId+'/roomOccupied?access_token='+config.accessToken;
    this.lightLeftOnUrl = 'https://api.particle.io/v1/devices/'+config.deviceId+'/lightLeftOn?access_token='+config.accessToken;
    this.batteryStatusUrl = 'https://api.particle.io/v1/devices/'+config.deviceId+'/soc?access_token='+config.accessToken;
    return this.startLoop();
};

App.prototype.startLoop = function() {
    setInterval(this.getRoomStatus.bind(this), 2000);
    setInterval(this.getLightLeftOnStatus.bind(this), 60000);
    setInterval(this.getBatteryStatus.bind(this),60000);
    return this;
};

App.prototype.getRoomStatus = function() {
    superagent
        .get(this.bathStatusUrl)
        .on('error', function(err) {
            io.emit('error', err);
        })
        .end(function(err,res) {
            if (err || !res.ok) {
              if (err.status == 408)
                io.emit('offline', res);
              else
                io.emit('error', res);
              return this;
             }

            if (res.body.result === false) {
                io.emit('open');
                return this;
            }

            if (res.body.result === true) {
                io.emit('close');
                return this;
            }

        });
    return this;
};

App.prototype.getBatteryStatus = function() {
    superagent
        .get(this.batteryStatusUrl)
        .end(function(err,res) {
            if (err || !res.ok)
              return this;
            io.emit('battery',res.body.result);
        });
    return this;
};

App.prototype.getLightLeftOnStatus = function() {
    superagent
        .get(this.lightLeftOnUrl)
        .end(function(err,res) {
            if (err || !res.ok)
              return this;
            io.emit('light-left-on',res.body.result);
        });
    return this;
};

module.exports = new App();
