# 🧮 Sonic Nozzle Vue Demo

A modern Vue.js application demonstrating real-time flow rate through a sonic nozzle using the AGA8 equations for gas properties and ISO 9300:2022 for nozzle flow calculations.

[![Live Demo](https://img.shields.io/badge/demo-live-brightgreen.svg)](https://sonic.lasersmart.work/)
[![Vue.js](https://img.shields.io/badge/vue.js-v3.5-42b883.svg)](https://vuejs.org/)
[![TailwindCSS](https://img.shields.io/badge/tailwindcss-v4.0-38bdf8.svg)](https://tailwindcss.com/)
[![TypeScript](https://img.shields.io/badge/typescript-v5.7-3178c6.svg)](https://www.typescriptlang.org/)

## ✨ Features

### Calculations

- 🚀 Real-time computations via WebAssembly
- 🎯 High-precision gas properties using DETAIL method
- 🧪 GERG-2008 equation of state support
- 💨 ISO 9300:2022 nozzle flow calculations

### Components

- 🧪 Support for 21 different gas components
- 📊 Real-time flow visualization
- 🔢 LaTeX math rendering
- 📱 Responsive design with TailwindCSS

### User Experience

- 📋 Copy-to-clipboard functionality
- 💾 Client-side only calculations
- 📈 Interactive pressure-flow graphs
- 📊 Excel worksheet export with:
  - Automatic formulas for mass flow rate calculation
  - Integrated nozzle diameter computation for a target mass flow rate
  - Complete gas mixture composition report
  - Configurable target mass flow rate
  - Temperature and pressure ranges

## 🌟 Live Demo

Try the calculator at [sonic.lasersmart.work](https://sonic.lasersmart.work/)

<img width="1235" alt="Sonic Nozzle Calculator Demo" src="/public/demo-screenshot.png" />

## 🛠️ Development Setup

### Prerequisites

- Node.js ≥ 22.0.0
- npm ≥ 10.0.0

### Installation

```bash
# Clone the repository
git clone https://github.com/sctg-development/aga8-js.git

# Navigate to the Vue demo directory
cd aga8-js/src/aga8-soinc-nozzle-vue

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

## Computations and formulas

```math
\begin{aligned}
R\space is\space the\space gas\space constant \newline
M\space is\space the\space molecular\space weight \newline
D\space is\space the\space diameter\space of\space the\space nozzle \newline  
A\space is\space the\space cross-sectional\space area\space of\space the\space nozzle \newline
C_d\space is\space the\space discharge\space coefficient \newline
R_{e_{nt}}\space is\space the\space Reynolds\space number\space of\space the\space nozzle \newline
R_s = \frac{R}{M} \newline
C_d=a-\frac{b}{R_{e_{nt}}^n} \newline
A=\pi \cdot \left( \frac{D}{2} \right)^2\newline
C^*_p = \sqrt{\kappa\left(\frac{ 2}{\kappa + 1}\right)^{\frac{\kappa + 1}{\kappa - 1}}}\newline
Q=A \cdot C_d \cdot C^*\frac{P_{in}}{\sqrt{R_s \cdot T_{in}}} \newline
\end{aligned}
```

## About ISO 9300:2022

ISO 9300:2022 specifies the method for calculating the flow-rate of gas through a sonic nozzle. The method is based on the AGA Report No. 8, Part 1. The method is applicable to the flow of natural gas and other gases mixture composed of the 21 components listed in the standard.

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
