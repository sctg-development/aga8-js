# 🧮 AGA8 Vue Demo

A modern Vue.js application demonstrating real-time gas property calculations using the AGA8 DETAIL, GERG-2008, and GROSS equations of state. All calculations are performed client-side using WebAssembly.

[![Live Demo](https://img.shields.io/badge/demo-live-brightgreen.svg)](https://aga8.lasersmart.work/)
[![Vue.js](https://img.shields.io/badge/vue.js-v3.5-42b883.svg)](https://vuejs.org/)
[![TailwindCSS](https://img.shields.io/badge/tailwindcss-v4.0-38bdf8.svg)](https://tailwindcss.com/)
[![TypeScript](https://img.shields.io/badge/typescript-v5.7-3178c6.svg)](https://www.typescriptlang.org/)

## ✨ Features

- 🚀 Real-time calculations using WebAssembly
- 🎯 High-precision gas property computations
- 📱 Responsive design with TailwindCSS
- 🧪 Support for 21 different gas components
- 📋 Easy-to-use copy-to-clipboard functionality
- 🔢 LaTeX math rendering for equations
- 💾 No server-side calculations required

## 🌟 Live Demo

Try the calculator at [aga8.lasersmart.work](https://aga8.lasersmart.work/)

[<img width="821" alt="AGA8 Calculator Interface" src="https://github.com/user-attachments/assets/148f537c-e6ca-43c7-8b41-c50f770c64e8" />](https://aga8.lasersmart.work/)

## 🛠️ Development Setup

### Prerequisites

- Node.js ≥ 22.0.0
- npm ≥ 10.0.0

### Installation

```bash
# Clone the repository
git clone https://github.com/sctg-development/aga8-js.git

# Navigate to the Vue demo directory
cd aga8-js/src/aga8-vue

# Install dependencies
npm install
```

### Development Commands

```bash
# Start development server with hot-reload
npm run dev

# Build for production
npm run build

# Preview production build
npm run preview

# Lint and fix files
npm run lint
```

## 📚 Technical Details

The demo uses:
- Vue.js 3 with Composition API
- TypeScript for type safety
- TailwindCSS for styling
- Temml for LaTeX math rendering
- [@sctg/aga8-js](https://www.npmjs.com/package/@sctg/aga8-js) for calculations

### Supported Calculations

- Compressibility factor (Z)
- Density
- Pressure
- Internal energy
- Enthalpy
- Entropy
- Heat capacities (Cv, Cp)
- Speed of sound
- Joule-Thomson coefficient
- And many more...

## 📖 About AGA8

The AGA8 equations are industry-standard methods for calculating thermodynamic properties of natural gas mixtures. This implementation includes:

- **DETAIL**: High-precision calculations (AGA Report No. 8, Part 1)
- **GERG-2008**: Reference equations for high-accuracy calculations

## 📄 License

GNU Affero General Public License v3.0 - see the [LICENSE](../../LICENSE.md) file for details.

## 🙏 Credits

- Original AGA8 implementation by NIST
- WebAssembly port by Ronan LE MEILLAT
- Vue.js demo implementation by SCTG Development Team
