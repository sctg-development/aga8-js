# 🧮 Sonic Nozzle Vue Demo

A modern Vue.js application demonstrating real-time flow rate through a sonic nozzle using the AGA8 equations for gas properties and ISO 9300:2022 for nozzle flow calculations.

[![Live Demo](https://img.shields.io/badge/demo-live-brightgreen.svg)](https://sonic.lasersmart.work/)
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

Try the calculator at [sonic.lasersmart.work](https://sonic.lasersmart.work/)

<img width="1235" alt="image" src="https://github.com/user-attachments/assets/3289e062-f111-47ed-beda-6480bef9c95a" />


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
