# AGA8 GERG-2008, GROSS and DETAIL Gas Properties Calculator

[![npm version](https://badge.fury.io/js/@sctg%2Faga8-js.svg)](https://www.npmjs.com/package/@sctg/aga8-js)
[![Build Status](https://github.com/sctg-development/aga8-js/actions/workflows/build.yaml/badge.svg)](https://github.com/sctg-development/aga8-js/actions/workflows/build.yaml)
[![License](https://img.shields.io/badge/License-AGPL%20v3-blue.svg)](https://www.gnu.org/licenses/agpl-3.0)
[![Documentation](https://img.shields.io/badge/docs-latest-brightgreen.svg)](https://sctg-development.github.io/aga8-js/)

High-performance WebAssembly implementation of the GERG-2008 equation of state and AGA8 methods for natural gas properties calculations. This package provides JavaScript/TypeScript bindings to the industry-standard algorithms for precise gas property calculations.

## 🌟 Features

- **Three Calculation Methods**:
  - 📊 GERG-2008 (Reference equations for high-accuracy calculations)
  - 🔍 Detail (AGA8 Part 1 method for precise gas mixtures)
  - 📈 Gross (AGA8 Part 2 method for simplified calculations)
  
- **Complete Gas Support**:
  - 🧪 Full support for all 21 gas components defined by AGA8
  - 🎯 High-precision calculations for natural gas mixtures
  - 🌡️ Wide range of temperature and pressure conditions

- **Technical Excellence**:
  - ⚡ WebAssembly-powered for near-native performance
  - 📦 Tree-shakeable npm package
  - 💪 TypeScript type definitions included
  - 📚 Comprehensive [API documentation](https://sctg-development.github.io/aga8-js/)

## 🚀 Live Demo

Try it now at [Lasersmart AGA8](https://aga8.lasersmart.work/)!

[<img width="1164" alt="AGA8 Calculator Demo" src="https://github.com/user-attachments/assets/76c1deaa-9519-4bb4-916b-22e31a6eb06b" />](https://aga8.lasersmart.work/)

View the demo source code in our [Vue.js implementation](https://github.com/sctg-development/aga8-js/tree/main/src/aga8-vue).

## 📦 Installation

```bash
npm install @sctg/aga8-js
```

## 💻 Quick Start

```typescript
import AGA8wasm, { type GasMixture } from '@sctg/aga8-js';

// Initialize AGA8 module
const AGA8 = await AGA8wasm();
AGA8.SetupGERG();

// Define gas mixture (94% methane, 5% CO2, 1% helium)
const mixture: GasMixture = {
    methane: 0.94,
    nitrogen: 0,
    carbon_dioxide: 0.05,
    ethane: 0,
    propane: 0,
    isobutane: 0,
    n_butane: 0,
    isopentane: 0,
    n_pentane: 0,
    n_hexane: 0,
    n_heptane: 0,
    n_octane: 0,
    n_nonane: 0,
    n_decane: 0,
    hydrogen: 0,
    oxygen: 0,
    carbon_monoxide: 0,
    water: 0,
    hydrogen_sulfide: 0,
    helium: 0.01,
    argon: 0
}

// Calculate properties
const molarMass = AGA8.MolarMassGERG(mixture);
const { D: density } = AGA8.DensityGERG(0, 400, 50000, mixture);
const properties = AGA8.PropertiesGERG(400, density, mixture);

console.log('Results:', {
  molarMass: `${molarMass.toFixed(4)} g/mol`,
  density: `${density.toFixed(4)} mol/L`,
  compressibility: properties.Z.toFixed(6),
  soundSpeed: `${properties.W.toFixed(2)} m/s`
});
```

## Samples

- [sonic-nozzle-flow.ts](src/examples/sonic-nozzle-flow.ts): Calculate the flow rate through a sonic nozzle
- [gas-properties-calculations.ts](src/examples/gas-properties-calculations.ts): Calculate gas properties for a mixture using GERG-2008

## 📚 Documentation

- [API Reference](https://sctg-development.github.io/aga8-js/)
- [GERG-2008 Method](https://sctg-development.github.io/aga8-js/GERG2008_8h.html)
- [Detail Method](https://sctg-development.github.io/aga8-js/Detail_8h.html)
- [Gross Method](https://sctg-development.github.io/aga8-js/Gross_8h.html)

## 🛠️ Development

### Prerequisites

- Node.js ≥ 22.0.0
- Emscripten SDK ≥ 4.0.0
- CMake ≥ 3.10

### Building from Source

```bash
# Emscripten SDK installation
# Clone repository
git clone https://github.com/emscripten-core/emsdk.git
cd emsdk
# Download and install the 4.0.1 SDK
./emsdk install 4.0.1
./emsdk activate 4.0.1
source ./emsdk_env.sh
```

```bash
# WebAssembly module build
# Clone repository
git clone https://github.com/sctg-development/aga8-js.git
cd aga8-js

# Install dependencies
npm install

# Build WebAssembly module
npm run build

# Run tests
npm test
```

## 🤝 Contributing

We welcome contributions!

## 📄 License

- **AGA8-JS**: GNU Affero General Public License v3.0
- **Original AGA8 Implementation**: Public Domain (NIST)

## 🙏 Acknowledgments

- Original AGA8 implementation by the National Institute of Standards and Technology (NIST)
- WebAssembly port, AGA8 modifications and maintenance by Ronan LE MEILLAT

## 📧 Support

- Create an [Issue](https://github.com/sctg-development/aga8-js/issues)
