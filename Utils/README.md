# Utilities

## Before usage

1. Make sure [NodeJS](https://nodejs.org) is installed in your system and available in the Path
2. Locate `Utils` folder (Considering you are in the root of the library)

```
cd Utils
```

3. Install necessary dependencies using npm:

```
npm install
```

## Contents:

- [Static file generator](#static-file-generator)

---

## Static file generator

Generates header file using source one. Tries to minify and gzip if `.js` or `.css`.
The generated header file contains:

- `${variable name}_HASH` - file hash. Supposed to be used as filename in the module source
- `${variable name}_LENGTH` - file size. Supposed to be used as length in the module source
- `${variable name}` - actual file. Supposed to be used as file in the module source

${variable name} is generated from the source file. The script tries to transform filename `camelCase` to the `SCREAMING_SNAKE_CASE` naming convention. For example: `myScript.js` is going to be `MY_SCRIPT_JS`

### Usage

Considering you are in the root of the library

```
node Utils/generateStaticFile.js path/to/your/file.js
```

---
