const fs = require('fs/promises')
const crypto = require('crypto')
const zlib = require('zlib')

// Usage: node gzipForPROGMEM.js yourScript.js

fs.readFile(process.argv[2]).then(data => {
  compressed = zlib.gzipSync(data, { level: 9 })
  console.log('Array length:', compressed.length)
  res = Array.from(compressed)
    .map(v => '0x' + v.toString(16))
    .join()
  fs.writeFile(
    crypto.createHash('md5').update(compressed).digest('hex'),
    res
  )
})
