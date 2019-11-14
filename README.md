# Msdfgen Library for Wasm32

This repository contains a copy of 
[msdfgen library](https://github.com/Chlumsky/msdfgen) (commit 
[86acea](https://github.com/Chlumsky/msdfgen/tree/86acea7835269a9817f772eea6d7465cb080cdfe)) 
with its freetype dependency, adapted to webassebly. The changes 
includes CMakeScripts, 
[exported C API](https://github.com/luna/msdfgen-wasm/blob/wip/ao/msdf-wasm/msdfgen/wasm/msdfgen_c.h),
and other required adjustements.

See [this instruction](https://emscripten.org/docs/porting/connecting_cpp_and_javascript/Interacting-with-code.html)
how to use exported functions in JavaScript.

# Build

You need to have emscripten SDK and cmake installed:

```bash
mkdir build
cd build
emconfigure cmake .. # -DCMAKE_BUILD_TYPE=RELEASE
emmake make
```

The final output is `msdfgen_wasm.js` file located in `build/msdfgen/`

