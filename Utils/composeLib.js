const {
  existsSync,
  lstatSync,
  readdirSync,
  readFileSync,
  writeFileSync,
} = require('fs')
const {
  join,
  basename,
  extname,
  normalize,
  resolve,
} = require('path')

const composerConfig = {
  excludeDirs: ['Mocks'],
}

const collectFiles = path => {
  return readdirSync(path)
    .map(itemName => {
      const filePath = join(path, itemName)
      if (lstatSync(filePath).isDirectory()) {
        if (composerConfig.excludeDirs.includes(basename(filePath))) return []
        else return collectFiles(filePath)
      } else
        return extname(itemName).match(/^\.(h|cpp|hpp|c)$/)
          ? normalize(filePath)
          : []
    })
    .flat(Infinity)
}

const getDependencyMap = filePaths => {
  const map = {}

  filePaths.forEach(filePath => {
    map[filePath] = []
    readFileSync(filePath, { encoding: 'utf-8' })
      .match(/(?<=#include ").+(?=")/gm)
      ?.map(include => normalize(include))
      .forEach(include =>
        map[filePath].push(
          filePaths.find(f => {
            return f.includes(
              include.match(/\.{0,}\.[\/\\]/)
                ? resolve(filePath, '../', include)
                : include
            )
          })
        )
      )
  })

  Object.entries(map).forEach(([k, v]) => (map[k] = v.filter(v => !!v)))

  return map
}

const buildDependencySatisfiedChain = dependsOn => {
  let fileList = Object.keys(dependsOn)

  let orderedHeaders = []
  while (fileList.length) {
    fileList = fileList.filter(file => {
      const dependenciesMet = dependsOn[file].every(dependency =>
        orderedHeaders.includes(dependency)
      )

      if (dependenciesMet) orderedHeaders.push(file)

      return !dependenciesMet
    })
  }

  return orderedHeaders
}

const assemble = includeOrder => {
  const composedFiles = []
  const regexHeaderMatcher = `(${includeOrder
    .map(header => `(.*${basename(header)})`)
    .join('|')})`

  includeOrder.forEach(filePath => {
    const contents = readFileSync(filePath, { encoding: 'utf8' })
      .replace(new RegExp(`^.*#include "${regexHeaderMatcher}".*$`, 'gm'), '')
      .replace(/^.*#pragma once.*$/gm, '')
    composedFiles.push(contents)
  })

  return composedFiles.join('\n')
}

let path = process.argv[2]?.trim()

if (!path) throw new Error(`Source dir hasn't been specified`)

if (!existsSync(path)) throw new Error(`Path does not exist`)

path = resolve(path)

if (!lstatSync(path).isDirectory())
  throw new Error(
    `You provided a path to the file. You need to provide the root of the library`
  )

console.log('Collecting files')

const fileList = collectFiles(path)

console.log('Building dependency chain')
const dependencyMap = getDependencyMap(fileList)

console.log('Defining include order to satisfy dependencies')
const includeOrder = buildDependencySatisfiedChain(dependencyMap)

console.log('Assembling')
const writeReadyFile = assemble(includeOrder)
writeFileSync(resolve(path, 'composedLibrary.h'), writeReadyFile)

console.log('Done')
