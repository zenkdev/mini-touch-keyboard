const SerialPort = require('serialport');
const Readline = require('@serialport/parser-readline');
var ks = require('node-key-sender');

const port = new SerialPort('COM6', { baudRate: 115200 });

const parser = new Readline();
port.pipe(parser);

parser.on('data', line => {
  console.log(`> ${line}`);

  const arrCommands = line.split(':').map(command => command.split('+'));
  console.log(arrCommands);

  let batch = ks.startBatch();
  for (let n = 0; n < arrCommands.length; n += 1) {
    batch = batch.batchTypeCombination(arrCommands[n]);
  }

  batch.sendBatch().then(
    () => {
      port.write('DONE\n');
    },
    reason => {
      console.error(reason);
    },
  );
});
// port.write('KEYBOARD POWER ON\n');
console.log('> KEYBOARD ONLINE');
