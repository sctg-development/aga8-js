# AGA8 Gas Properties Calculator

[![npm version](https://badge.fury.io/js/@sctg%2Faga8-js.svg)](https://www.npmjs.com/package/@sctg/aga8-js)
[![Build Status](https://github.com/sctg-development/aga8-js/actions/workflows/build.yaml/badge.svg)](https://github.com/sctg-development/aga8-js/actions/workflows/build.yaml)
[![License](https://img.shields.io/badge/License-AGPL%20v3-blue.svg)](https://www.gnu.org/licenses/agpl-3.0)

WebAssembly bindings for [AGA8](https://github.com/usnistgov/AGA8) gas properties calculation methods (Detail, GERG-2008, and Gross).

## Features

- 📊 Supports Detail, GERG-2008, and Gross calculation methods
- 🧪 All 22 NIST gas components support
- 🚀 High-performance WebAssembly implementation
- 📦 Easy to use npm package
- 🔍 TypeScript type definitions included

## Installation

```bash
npm install @sctg/aga8-js
```

## Quick Start

```typescript
import AGA8wasm, { gazMixtureInMolePercent } from '@sctg/aga8-js';

// Initialize module
const AGA8 = await AGA8wasm();
AGA8.SetupGERG();

// Define gas mixture (94% methane, 5% CO2, 1% helium)
const mixture: gazMixtureInMolePercent = [
    0,     // PLACEHOLDER
    0.94,  // METHANE
    0,     // NITROGEN
    0.05,  // CO2
    0,     // ETHANE
    // ... other components
    0.01,  // HELIUM
    0      // ARGON
];

// Calculate properties
const molarMass = AGA8.MolarMassGERG(mixture);
const density = AGA8.DensityGERG(0, 400, 50000, mixture);
const properties = AGA8.PropertiesGERG(400, density.D, mixture);
```

## API Documentation

### Gas Components

The gas mixture array must contain 22 elements in the following order:

1. PLACEHOLDER (must be 0)
2. METHANE
3. NITROGEN
4. CARBON_DIOXIDE
5. ETHANE
6. PROPANE
7. ISOBUTANE
8. N_BUTANE
9. ISOPENTANE
10. N_PENTANE
11. N_HEXANE
12. N_HEPTANE
13. N_OCTANE
14. N_NONANE
15. N_DECANE
16. HYDROGEN
17. OXYGEN
18. CARBON_MONOXIDE
19. WATER
20. HYDROGEN_SULFIDE
21. HELIUM
22. ARGON

The sum of all components must be equal to 1.

### Available Methods

- `SetupDetail()`, `SetupGERG()`, `SetupGross()`: Initialize calculation methods
- `MolarMassDetail()`, `MolarMassGERG()`, `MolarMassGross()`: Calculate molar mass
- `DensityDetail()`, `DensityGERG()`, `DensityGross()`: Calculate density
- `PropertiesDetail()`, `PropertiesGERG()`: Calculate thermodynamic properties
- Others see <https://pages.nist.gov/AGA8/>

## Development

### Prerequisites

- Node.js >= 22
- Emscripten SDK >= 4.0
- CMake >= 3.10

### Setup

```bash
# Clone repository
git clone https://github.com/sctg-development/aga8-js.git
cd aga8-js

# Install dependencies
npm install

# Build WebAssembly module
npm run build
```

### Testing

```bash
npm test
```

## Contributing

Contributions are welcome! Please feel free to submit a Pull Request.

## License

This project is licensed under the GNU Affero General Public License v3.0 - see the LICENSE file for details.  
NISt AGA8 is published by the National Institute of Standards and Technology (NIST) and use its own license (see the code).

## Credits

- Original AGA8 implementation by NIST
- WebAssembly port by Ronan LE MEILLAT for SCTG and LaserSmart
