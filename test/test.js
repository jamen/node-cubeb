const { Cubeb } = require('../dist/cubeb.js')
// const sigfaultHandler = require('segfault-handler')

// sigfaultHandler.registerHandler('crash.log')

console.log(Cubeb)

const cubeb = new Cubeb("yeet")

// console.log(cubeb.getMaxChannelCount())

// console.log(cubeb.getBackendId())

console.log(cubeb.getMinLatency({ channels: 2, sampleRate: 44100, type: 'int16' }))

cubeb.destroy();