const SerialPort = require('serialport');
const Readline = require('@serialport/parser-readline');
var ks = require('node-key-sender');

const port = new SerialPort('COM6', { baudRate: 115200 });

const parser = new Readline();
port.pipe(parser);

parser.on('data', line => {
  console.log(`> ${line}`);

  const parts = line.split('+');
  ks.sendCombination(parts);
});
// port.write('ROBOT POWER ON\n');
console.log('> ROBOT ONLINE');
