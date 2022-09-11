const fs = require('fs')
const crypto = require('crypto')
const zlib = require('zlib')
const babel = require('@babel/core')
const { minify: minifyCSS } = require('csso')
const { extname } = require('path')

if (!process.argv[2] || !process.argv[2].trim())
  throw new Error(`Source file hasn't been specified`)

const from = process.argv[2].trim()
const to = generateOutputFileName(from)

switch (extname(from)) {
  case '.js':
    processFile(
      fs.readFileSync(from, { encoding: 'utf-8' }),
      data => babel.transformSync(data, { minified: true }).code,
      gzip
    )
    break
  case '.css':
    processFile(
      fs.readFileSync(from, { encoding: 'utf-8' }),
      data => minifyCSS(data).css,
      gzip
    )
    break
  case '.svg':
    processFile(fs.readFileSync(from, { encoding: 'utf-8' }), gzip)
  default:
    processFile(fs.readFileSync(from))
    break
}

function generateOutputFileName(inputFileName) {
  return inputFileName + '.h'
}

function generateCPPVariableName(inputFileName) {
  return inputFileName
    .split('/')
    .pop()
    .replaceAll(/[A-Z]*[A-Z]/g, str => `_${str.trim().toUpperCase()}`)
    .replaceAll(/[. :\\\-()]/g, () => '_')
    .replace(/^_*/, () => '')
    .replaceAll(/_{2,}/g, () => '_')
    .toUpperCase()
}

function writeToTemplate(varName, data) {
  const encodedData = Array.from(data).map(
    (value, index) =>
      (index % 5 === 0 ? '\n\t' : '') +
      '0x' +
      value.toString(16).padStart(2, '0')
  )
  return `#pragma once

#include <Arduino.h>

#define ${varName}_HASH "${crypto.createHash('md5').update(data).digest('hex')}"
#define ${varName}_LENGTH ${encodedData.length}
const uint8_t ${varName}[] PROGMEM = {${encodedData.join(', ')}\n};
`
}

function processFile(data, ...mappers) {
  fs.writeFileSync(
    to,
    writeToTemplate(
      generateCPPVariableName(from),
      mappers.reduce((data, mapper) => mapper(data), data)
    )
  )
}

function gzip(data) {
  return zlib.gzipSync(data, { level: 9 })
}
