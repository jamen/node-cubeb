const { Cubeb, CubebStream } = require('../dist/cubeb.js')
// const sigfaultHandler = require('segfault-handler')

// sigfaultHandler.registerHandler('crash.log')

console.log(Cubeb)

const cubeb = new Cubeb("yeet")

console.log(cubeb)

// console.log(cubeb.getMaxChannelCount())

// console.log(cubeb.getBackendId())

// const options = {
//     channels: 2,
//     rate: 44100,
//     format: Cubeb.CUBEB_SAMPLE_FLOAT32LE,
//     layout: Cubeb.CUBEB_LAYOUT_STEREO
// }

// console.log(options)

// console.log(cubeb.getMinLatency(options))

// console.log(cubeb.getPreferredSampleRate())

console.log(new CubebStream(cubeb, 'test_stream', ))