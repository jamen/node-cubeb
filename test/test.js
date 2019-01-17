const cubeb = require('../dist/cubeb.js')
const sigfaultHandler = require('segfault-handler')

sigfaultHandler.registerHandler('crash.log')

const context = cubeb.init("yeet")

// console.log(cubeb.getMaxChannelCount(context))

// console.log(cubeb.getMinLatency(context, { channels: 2, sampleRate: 44100, type: 'int16' }))

console.log(cubeb.getMinLatency(context, null))