# AGA8-js Project

This project uses Emscripten to compile C++ code and expose NIST Aga8 functions as TypeScript functions.

## Project structure

- `src/cpp/GERG2008.cpp` : From <https://github.com/usnistgov/AGA8/blob/master/AGA8CODE/C/GERG2008.cpp> (AGA8 code).
- `src/cpp/GERG2008.h` : From <https://github.com/usnistgov/AGA8/blob/master/AGA8CODE/C/GERG2008.h> (AGA8 code).
- `src/cpp/Detail.cpp` : From <https://github.com/usnistgov/AGA8/blob/master/AGA8CODE/C/Detail.cpp> (AGA8 code).
- `src/cpp/Detail.h` : From <https://github.com/usnistgov/AGA8/blob/master/AGA8CODE/C/Detail.h> (AGA8 code).
- `src/cpp/Gross.cpp` : From <https://github.com/usnistgov/AGA8/blob/master/AGA8CODE/C/Gross.cpp> (AGA8 code).
- `src/cpp/Gross.h` : From <https://github.com/usnistgov/AGA8/blob/master/AGA8CODE/C/Gross.h> (AGA8 code).
- `src/cpp/bindings.cpp` : Binds C++ functions to TypeScript functions via Emscripten.

## Compilation

To compile the project, use CMake with Emscripten:

```bash
npm i
npm run build
```

## Test execution

To run the tests, use the test framework configured in `package.json` :

```bash
npm test
```

## License

This project is licensed under the AGPL3 License - see the [LICENSE](LICENSE.md) file for details.  
All NIST AGA8 code is licensed on its own license (see code).  
