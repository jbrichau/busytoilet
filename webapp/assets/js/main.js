var socket = io();

var App = function() {
    return this.init();
};

App.prototype.init = function() {
    return this
        .setupHandlers()
        .createChildren()
        .enable();
};

App.prototype.setupHandlers = function() {
    this.onOpenMessageHandler = this.onOpenMessage.bind(this);
    this.onCloseMessageHandler = this.onCloseMessage.bind(this);
    this.onOfflineMessageHandler = this.onOfflineMessage.bind(this);
    this.onErrorMessageHandler = this.onErrorMessage.bind(this);
    this.batteryLevelHandler = this.batteryLevelHandler.bind(this);
    this.lightLeftOnHandler = this.lightLeftOnHandler.bind(this);

    return this;
};

App.prototype.createChildren = function() {
    this.box = document.querySelector('.box');
    this.icon = document.querySelector('.icon');
    this.battery = document.querySelector('.battery');
    this.light = document.querySelector('.light');

    return this;
};

App.prototype.enable = function() {
    socket.connect();

    socket.on('open', this.onOpenMessageHandler);
    socket.on('close', this.onCloseMessageHandler);
    socket.on('offline', this.onOfflineMessageHandler);
    socket.on('error', this.onErrorMessageHandler);
    socket.on('battery', this.batteryLevelHandler);
    socket.on('light-left-on', this.lightLeftOnHandler);

    return this;
};

App.prototype.onOpenMessage = function() {
    this.box.classList.remove('box_isActive');
    this.icon.classList.remove('icon_isActive');
    this.icon.classList.remove('icon_isOffline');

    this.box.classList.add('box_isInActive');
    this.icon.classList.add('icon_isInActive');

    return this;
};

App.prototype.onCloseMessage = function() {
  this.box.classList.remove('box_isInActive');
  this.icon.classList.remove('icon_isInActive');
  this.icon.classList.remove('icon_isOffline');

  this.box.classList.add('box_isActive');
  this.icon.classList.add('icon_isActive');

  return this;
};

App.prototype.onOfflineMessage = function() {
  this.icon.classList.remove('icon_isInActive');
  this.icon.classList.remove('icon_isActive');

  this.icon.classList.add('icon_isOffline');

  return this;
};

App.prototype.onErrorMessage = function(result) {
  console.error('Server returned an error: ', result);

  return this;
};

App.prototype.lightLeftOnHandler = function(result) {
  if(result)
    this.light.style.display = "block"
  else
    this.light.style.display = "none"
}

App.prototype.batteryLevelHandler = function(result) {
  var className = 'battery ';
  switch(Math.round(result / 25)) {
    case 0:
      className = className + 'battery-0';
      break;
    case 1:
      className = className + 'battery-1';
      break;
    case 2:
      className = className + 'battery-2';
      break;
    case 3:
      className = className + 'battery-3';
      break;
    case 4:
      className = className + 'battery-4';
      break;
  }
  this.battery.className = className;
}

this.app = new App();
