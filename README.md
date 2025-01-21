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

## Sample usage

```typescript
import AGA8wasm, { gazMixtureInMolePercent } from './dist/aga8';

async function calculateGERGProperties() {
    // Initialize GERG-2008 module
    const AGA8 = await AGA8wasm();
    AGA8.SetupGERG();

    // Gas mixture composition (mole fractions, must sum to 1)
    const gasMixture:gazMixtureInMolePercent = [
        0,        // [0] PLACEHOLDER (must be 0)
        0.77824,  // [1] METHANE
        0.02,     // [2] NITROGEN
        0.06,     // [3] CARBON DIOXIDE
        0.08,     // [4] ETHANE
        0.03,     // [5] PROPANE
        0.0015,   // [6] ISOBUTANE
        0.003,    // [7] N-BUTANE
        0.0005,   // [8] ISOPENTANE
        0.00165,  // [9] N-PENTANE
        0.00215,  // [10] N-HEXANE
        0.00088,  // [11] N-HEPTANE
        0.00024,  // [12] N-OCTANE
        0.00015,  // [13] N-NONANE
        0.00009,  // [14] N-DECANE
        0.004,    // [15] HYDROGEN
        0.005,    // [16] OXYGEN
        0.002,    // [17] CARBON MONOXIDE
        0.0001,   // [18] WATER
        0.0025,   // [19] HYDROGEN SULFIDE
        0.007,    // [20] HELIUM
        0.001     // [21] ARGON
    ];

    // Operating conditions
    const temperature = 400;   // Kelvin
    const pressure = 50000;    // kPa

    // Property calculations
    const molarMass = AGA8.MolarMassGERG(gasMixture);
    const density = AGA8.DensityGERG(0, temperature, pressure, gasMixture);
    const properties = AGA8.PropertiesGERG(temperature, density.D, gasMixture);

    // Output results
    console.log('GERG-2008 Gas Properties:');
    console.log(`Molar mass: ${molarMass.toFixed(6)} g/mol`);
    console.log(`Density: ${density.D.toFixed(6)} mol/l`);
    console.log(`Pressure: ${properties.P.toFixed(2)} kPa`);
    console.log(`Compressibility factor: ${properties.Z.toFixed(6)}`);
    console.log(`Internal energy: ${properties.U.toFixed(2)} J/mol`);
    console.log(`Enthalpy: ${properties.H.toFixed(2)} J/mol`);
}

calculateGERGProperties().catch(console.error);
```

You can run this simply with `npx tsx sample.ts`.

## License

This project is licensed under the AGPL3 License - see the [LICENSE](LICENSE.md) file for details.  
All NIST AGA8 code is licensed on its own license (see code).  
