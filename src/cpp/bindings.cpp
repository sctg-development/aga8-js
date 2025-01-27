/**
 *  * \mainpage AGA 8 , GROSS and DETAIL Equations of State
 * This is the introduction.
 * @includedoc README.md
 * @file bindings.cpp
 * @brief Emscripten bindings for the AGA8 gas calculation library
 *
 * This file provides WebAssembly bindings for the AGA8 gas calculation library,
 * enabling its use in JavaScript environments. It implements three main calculation methods:
 * - Detail method (high accuracy)
 * - GERG-2008 method (reference equations)
 * - Gross method (simplified calculations)
 *
 * The gas composition is represented using a 21-component system where:
 * - Index 1: Methane
 * - Index 2: Nitrogen
 * - Index 3: Carbon dioxide
 * - Index 4-14: Various hydrocarbons (Ethane through n-Decane)
 * - Index 15-21: Other components (Hydrogen, Oxygen, CO, Water, H2S, Helium, Argon)
 *
 * @note All compositions must be provided as mole fractions (not percentages)
 * @note Temperature inputs are in Kelvin
 * @note Pressure inputs are in kPa
 * @note Density inputs are in mol/l (mol/dm³)
 *
 * Example mixture (94% methane, 5% CO2, 1% helium):
 * gasMixtureInMolePercent(1) = 0.94  // Methane
 * gasMixtureInMolePercent(3) = 0.05  // CO2
 * gasMixtureInMolePercent(20) = 0.01 // Helium
 *
 * @copyright (C) 2025 Ronan LE MEILLAT
 * @license GNU Affero General Public License v3.0
 */
/*
 * Copyright (C) 2025 Ronan LE MEILLAT
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 */
#include <emscripten/bind.h>
#include <emscripten/val.h>
#include "Detail.h"
#include "GERG2008.h"
#include "Gross.h"

using namespace emscripten;

/**
 * @brief Structure to hold gas mixture composition in mole percent
 * @details Contains mole fractions for each gas component in the mixture sum must be 1
 * @var methane Methane mole fraction
 * @var nitrogen Nitrogen mole fraction
 * @var carbon_dioxide Carbon dioxide mole fraction
 * @var ethane Ethane mole fraction
 * @var propane Propane mole fraction
 * @var isobutane Isobutane mole fraction
 * @var n_butane n-Butane mole fraction
 * @var isopentane Isopentane mole fraction
 * @var n_pentane n-Pentane mole fraction
 * @var n_hexane n-Hexane mole fraction
 * @var n_heptane n-Heptane mole fraction
 * @var n_octane n-Octane mole fraction
 * @var n_nonane n-Nonane mole fraction
 * @var n_decane n-Decane mole fraction
 * @var hydrogen Hydrogen mole fraction
 * @var oxygen Oxygen mole fraction
 * @var carbon_monoxide Carbon monoxide mole fraction
 * @var water Water mole fraction
 * @var hydrogen_sulfide Hydrogen sulfide mole fraction
 * @var helium Helium mole fraction
 * @var argon Argon mole fraction
 */
struct gasMixture
{
    double methane;          /**< Methane mole fraction */
    double nitrogen;         /**< Nitrogen mole fraction */
    double carbon_dioxide;   /**< Carbon dioxide mole fraction */
    double ethane;           /**< Ethane mole fraction */
    double propane;          /**< Propane mole fraction */
    double isobutane;        /**< Isobutane mole fraction */
    double n_butane;         /**< n-Butane mole fraction */
    double isopentane;       /**< Isopentane mole fraction */
    double n_pentane;        /**< n-Pentane mole fraction */
    double n_hexane;         /**< n-Hexane mole fraction */
    double n_heptane;        /**< n-Heptane mole fraction */
    double n_octane;         /**< n-Octane mole fraction */
    double n_nonane;         /**< n-Nonane mole fraction */
    double n_decane;         /**< n-Decane mole fraction */
    double hydrogen;         /**< Hydrogen mole fraction */
    double oxygen;           /**< Oxygen mole fraction */
    double carbon_monoxide;  /**< Carbon monoxide mole fraction */
    double water;            /**< Water mole fraction */
    double hydrogen_sulfide; /**< Hydrogen sulfide mole fraction */
    double helium;           /**< Helium mole fraction */
    double argon;            /**< Argon mole fraction */
};

/**
 * @brief Structure to hold pressure and compressibility factor values
 * @details Contains pressure (P) and compressibility factor (Z) as double precision values
 * @var P Pressure value
 * @var Z Compressibility factor value
 */
struct PressureResult
{
    double P; /**< Pressure value (kPa) */
    double Z; /**< Compressibility factor value */
};

/**
 * @struct DensityResult
 * @brief Structure to hold the results of density calculations
 *
 * @var D Density in mol/L
 * @var ierr Error flag (0 = success, non-zero = error)
 * @var herr Error message string describing the error if ierr is non-zero
 */
struct DensityResult
{
    double D;         /**< D Density in mol/L */
    int ierr;         /**< ierr Error flag (0 = success, non-zero = error) */
    std::string herr; /**< herr Error message string describing the error if ierr is non-zero */
};

/**
 * @brief Structure containing thermodynamic properties for DETAIL calculations
 * @details Contains the following properties:
 * @var P Pressure
 * @var Z Compressibility factor
 * @var dPdD Derivative of pressure with respect to density
 * @var d2PdD2 Second derivative of pressure with respect to density
 * @var d2PdTD Mixed derivative of pressure with respect to temperature and density
 * @var dPdT Derivative of pressure with respect to temperature
 * @var U Internal energy
 * @var H Enthalpy
 * @var S Entropy
 * @var Cv Isochoric heat capacity
 * @var Cp Isobaric heat capacity
 * @var W Speed of sound
 * @var G Gibbs free energy
 * @var JT Joule-Thomson coefficient
 * @var Kappa Isothermal compressibility
 * @var Cf Critical flow factor [-]
 *
 * @see PropertiesDetail For the underlying calculation implementation
 */
struct PropertiesDetailResult
{
    double P;      /**< Pressure in kPa */
    double Z;      /**< Compressibility factor */
    double dPdD;   /**< Derivative of pressure with respect to density */
    double d2PdD2; /**< Second derivative of pressure with respect to density */
    double d2PdTD; /**< Mixed derivative of pressure with respect to temperature and density */
    double dPdT;   /**< Derivative of pressure with respect to temperature */
    double U;      /**< Internal energy */
    double H;      /**< Enthalpy */
    double S;      /**< Entropy */
    double Cv;     /**< Isochoric heat capacity */
    double Cp;     /**< Isobaric heat capacity */
    double W;      /**< Speed of sound */
    double G;      /**< Gibbs free energy */
    double JT;     /**< Joule-Thomson coefficient */
    double Kappa;  /**< Isothermal compressibility */
    double Cf;     /**< Critical flow factor */
};

/**
 * @brief Structure containing thermodynamic properties for GERG-2008 calculations
 *
 * @param P Pressure [kPa]
 * @param Z Compressibility factor [-]
 * @param dPdD First partial derivative of pressure with respect to density [(Pa⋅m³)/kg]
 * @param d2PdD2 Second partial derivative of pressure with respect to density [(Pa⋅m⁶)/kg²]
 * @param d2PdTD Second partial derivative of pressure with respect to temperature and density [(Pa⋅m³)/(kg⋅K)]
 * @param dPdT First partial derivative of pressure with respect to temperature [Pa/K]
 * @param U Internal energy [J/kg]
 * @param H Enthalpy [J/kg]
 * @param S Entropy [J/(kg⋅K)]
 * @param Cv Isochoric heat capacity [J/(kg⋅K)]
 * @param Cp Isobaric heat capacity [J/(kg⋅K)]
 * @param W Speed of sound [m/s]
 * @param G Gibbs free energy [J/kg]
 * @param JT Joule-Thomson coefficient [K/Pa]
 * @param Kappa Isothermal compressibility [1/Pa]
 * @param A Helmholtz free energy [J/kg]
 * @param Cf Critical flow factor [-]
 *
 * @see PropertiesGERG For the underlying calculation implementation
 */
struct PropertiesGERGResult
{
    double P;      /**< Pressure in kPa */
    double Z;      /**< Compressibility factor */
    double dPdD;   /**< Derivative of pressure with respect to density [(Pa⋅m³)/kg]*/
    double d2PdD2; /**< Second derivative of pressure with respect to density [(Pa⋅m⁶)/kg²]*/
    double d2PdTD; /**< Mixed derivative of pressure with respect to temperature and density */
    double dPdT;   /**< Derivative of pressure with respect to temperature [Pa/K]*/
    double U;      /**< Internal energy [J/kg]*/
    double H;      /**< Enthalpy [J/kg]*/
    double S;      /**< Entropy [J/(kg⋅K)]*/
    double Cv;     /**< Isochoric heat capacity [J/(kg⋅K)]*/
    double Cp;     /**< Isobaric heat capacity [J/(kg⋅K)]*/
    double W;      /**< Speed of sound [m/s]*/
    double G;      /**< Gibbs free energy [J/kg]*/
    double JT;     /**< Joule-Thomson coefficient [K/Pa]*/
    double Kappa;  /**< Isothermal compressibility */
    double A;      /**< Helmholtz free energy [J/kg]*/
    double Cf;     /**< Critical flow factor */
};

/**
 * @brief Structure containing the composition of the equivalent hydrocarbon, nitrogen, and CO2 (mole fractions)
 *
 * @var hydrocarbon Mole fraction of the equivalent hydrocarbon
 * @var nitrogen Mole fraction of nitrogen
 * @var carbon_dioxide Mole fraction of carbon dioxide
 */
struct xGrs
{
    double hydrocarbon;    /**< Mole fraction of the equivalent hydrocarbon */
    double nitrogen;       /**< Mole fraction of nitrogen */
    double carbon_dioxide; /**< Mole fraction of carbon dioxide */
};

/**
 * @brief Structure containing computation results
 *
 * @var P Pressure value (kPa)
 * @var Z Compressibility factor
 * @var ierr Error code (0 = success, non-zero = error)
 * @var herr Error message string
 */
struct PressureGrossResult
{
    double P;         /**< Pressure value (kPa) */
    double Z;         /**< Compressibility factor */
    int ierr;         /**< Error code (0 = success, non-zero = error) */
    std::string herr; /**< Error message string */
};

/**
 * @brief Structure containing computation results
 *
 * @var D Density value (mol/L)
 * @var ierr Error code (0 = success, non-zero = error)
 * @var herr Error message string
 *
 * @see GrossHv For the underlying use implementation
 */
struct GrossHvResult
{
    xGrs xGrs;  /**< Compositions of the equivalent hydrocarbon, nitrogen, and CO2 (mole fractions) */
    double HN;  /**< Molar ideal gross heating value of the mixture (kJ/mol) at 298.15 K*/
    double HCH; /**< Molar ideal gross heating value of the equivalent hydrocarbon (kJ/mol) at 298.15 K */
};

struct GrossInputsResult
{
    xGrs xGrs;        /**< Compositions of the equivalent hydrocarbon, nitrogen, and CO2 (mole fractions) */
    double Gr;        /**< Relative density at T and P */
    double HN;        /**< Molar ideal gross heating value of the mixture (kJ/mol) at 298.15 K */
    double HCH;       /**< Molar ideal gross heating value of the equivalent hydrocarbon (kJ/mol) at 298.15 K */
    int ierr;         /**< Error number (0 indicates no error) */
    std::string herr; /**< Error message if ierr is not equal to zero */
};

struct BmixResult
{
    double B;         /**< Second virial coefficient (dm^3/mol) */
    double C;         /**< Third virial coefficient (dm^6/mol^2) */
    int ierr;         /**< Error number (0 indicates no error) */
    std::string herr; /**< Error message if ierr is not equal to zero */
};

struct GrossMethod1Result
{
    xGrs xGrs;        /**< Compositions of the equivalent hydrocarbon, nitrogen, and CO2 (mole fractions) */
    double Mm;        /**< Molar mass (g/mol) */
    double HCH;       /**< Molar ideal gross heating value of the equivalent hydrocarbon (kJ/mol) at 298.15 K */
    double HN;        /**< Molar ideal gross heating value of the mixture (kJ/mol) at 298.15 K */
    int ierr;         /**< Error number (0 indicates no error) */
    std::string herr; /**< Error message if ierr is not equal to zero */
};

struct GrossMethod2Result
{
    xGrs xGrs;        /**< Compositions of the equivalent hydrocarbon, nitrogen, and CO2 (mole fractions) */
    double Hv;        /**< Volumetric ideal gross heating value (MJ/m^3) at Th */
    double Mm;        /**< Molar mass (g/mol) */
    double HCH;       /**< Molar ideal gross heating value of the equivalent hydrocarbon (kJ/mol) at 298.15 K */
    double HN;        /**< Molar ideal gross heating value of the mixture (kJ/mol) at 298.15 K */
    int ierr;         /**< Error number (0 indicates no error) */
    std::string herr; /**< Error message if ierr is not equal to zero */
};

// Helper function to convert a JavaScript object to a C++ struct
/**
 * @brief Converts a JavaScript gasMixture Object to a C++ struct
 * @param js_object JavaScript gasMixture object
 */
std::vector<double> gasMixture_to_vector(gasMixture js_object)
{
    std::vector<double> result(22);
    result[0] = 0;
    result[1] = js_object.methane;
    result[2] = js_object.nitrogen;
    result[3] = js_object.carbon_dioxide;
    result[4] = js_object.ethane;
    result[5] = js_object.propane;
    result[6] = js_object.isobutane;
    result[7] = js_object.n_butane;
    result[8] = js_object.isopentane;
    result[9] = js_object.n_pentane;
    result[10] = js_object.n_hexane;
    result[11] = js_object.n_heptane;
    result[12] = js_object.n_octane;
    result[13] = js_object.n_nonane;
    result[14] = js_object.n_decane;
    result[15] = js_object.hydrogen;
    result[16] = js_object.oxygen;
    result[17] = js_object.carbon_monoxide;
    result[18] = js_object.water;
    result[19] = js_object.hydrogen_sulfide;
    result[20] = js_object.helium;
    result[21] = js_object.argon;
    return result;
}

// Helper function to convert a JavaScript xGr object to a C++ struct
/**
 * @brief Converts a JavaScript xGrs Object to a C++ struct
 * @param js_object JavaScript xGrs object
 */
std::vector<double> xGrs_to_vector(xGrs js_object)
{
    std::vector<double> result(3);
    result[0] = 0;
    result[1] = js_object.hydrocarbon;
    result[2] = js_object.nitrogen;
    result[3] = js_object.carbon_dioxide;
    return result;
}

// Helper function to convert a vector to a xGrs JavaScript object
/**
 * @brief Converts a C++ vector to a JavaScript xGrs object
 * @param vec C++ vector containing the xGrs values
 */
xGrs vector_to_xGrs(const std::vector<double> &vec)
{
    xGrs result;
    result.hydrocarbon = vec[1];
    result.nitrogen = vec[2];
    result.carbon_dioxide = vec[3];
    return result;
}

// Helper function to convert a JavaScript array to a C++ vector
/**
 * @brief Converts a JavaScript array to a C++ vector of doubles
 *
 * @param js_array JavaScript array object passed as an emscripten::val
 * @return std::vector<double> C++ vector containing the converted values
 *
 * @details This function takes a JavaScript array passed through emscripten's val
 * interface and converts it to a C++ std::vector<double>. Each element in the
 * JavaScript array is converted to a double value.
 */
std::vector<double> array_to_vector(const val &js_array)
{
    auto length = js_array["length"].as<unsigned>();
    std::vector<double> result(length);
    for (unsigned i = 0; i < length; ++i)
    {
        result[i] = js_array[i].as<double>();
    }
    return result;
}

// Helper function to convert a C++ vector to a JavaScript array
/**
 * @brief Converts a C++ vector of doubles to a JavaScript array
 *
 * @param vec The input vector containing double values to convert
 * @return val A JavaScript array containing the values from the input vector
 *
 * This function takes a C++ vector of doubles and creates a new JavaScript array,
 * copying each element from the vector to the corresponding index in the JavaScript array.
 * The function is typically used for interoperability between C++ and JavaScript/WebAssembly.
 */
val vector_to_array(const std::vector<double> &vec)
{
    val result = val::array();
    for (size_t i = 0; i < vec.size(); ++i)
    {
        result.set(i, vec[i]);
    }
    return result;
}

// Detail wrappers
/**
 * @brief Calculates the molar mass of a gas mixture given mole percentages
 *
 * This function wraps the MolarMassDetail calculation for use with external bindings.
 * It converts the input array to a vector before calculating the molar mass.
 *
 * @param x_array Gas mixture composition in mole percentages
 * @return double Molar mass of the gas mixture in kg/mol
 *
 * @see MolarMassDetail For the underlying calculation implementation
 */
double MolarMassDetail_wrapper(gasMixture x_array)
{
    std::vector<double> x = gasMixture_to_vector(x_array);
    double Mm = 0;
    MolarMassDetail(x, Mm);
    return Mm;
}

/**
 * @brief Wraps the PressureDetail function to calculate pressure and compressibility factor
 *
 * @param T Temperature in Kelvin
 * @param D Density in mol/l
 * @param x_array Gas mixture composition in mole percent
 * @return PressureResult Struct containing:
 *         - P: Pressure in kPa
 *         - Z: Compressibility factor (dimensionless)
 *
 * This wrapper function converts the input array to a vector and handles the pressure
 * calculation using PressureDetail, returning the results in a convenient struct.
 * @see PressureDetail For the underlying calculation implementation
 */
PressureResult PressureDetail_wrapper(double T, double D, gasMixture x_array)
{
    std::vector<double> x = gasMixture_to_vector(x_array);
    double P = 0, Z = 0;
    PressureDetail(T, D, x, P, Z);
    PressureResult result = {P, Z};
    return result;
}

/**
 * @brief Calculates the density of a gas mixture given temperature, pressure and composition
 *
 * This function wraps the DensityDetail calculation for easier interface handling.
 *
 * @param T Temperature in K
 * @param P Pressure in kPa
 * @param x_array Gas mixture composition in mole percent
 * @return DensityResult struct containing:
 *         - D: Density in kg/m³
 *         - ierr: Error code (0 = successful)
 *         - herr: Error message
 * @see DensityDetail For the underlying calculation implementation
 */
DensityResult DensityDetail_wrapper(double T, double P, gasMixture x_array)
{
    std::vector<double> x = gasMixture_to_vector(x_array);
    double D = 0;
    int ierr = 0;
    std::string herr;

    DensityDetail(T, P, x, D, ierr, herr);

    DensityResult result = {D, ierr, herr};
    return result;
}

/**
 * @brief Wrapper function to calculate detailed thermodynamic properties of a gas mixture
 *
 * @param T Temperature [K]
 * @param D Density [mol/l]
 * @param x_array Gas mixture composition in mole percent
 *
 * @return PropertiesDetailResult struct containing:
 *   - P: Pressure [kPa]
 *   - Z: Compressibility factor [-]
 *   - dPdD: Pressure derivative with respect to density [(kPa·l)/mol]
 *   - d2PdD2: Second pressure derivative with respect to density [(kPa·l²)/mol²]
 *   - d2PdTD: Mixed pressure derivative with respect to temperature and density [(kPa·l)/(mol·K)]
 *   - dPdT: Pressure derivative with respect to temperature [kPa/K]
 *   - U: Internal energy [J/mol]
 *   - H: Enthalpy [J/mol]
 *   - S: Entropy [J/(mol·K)]
 *   - Cv: Isochoric heat capacity [J/(mol·K)]
 *   - Cp: Isobaric heat capacity [J/(mol·K)]
 *   - W: Speed of sound [m/s]
 *   - G: Gibbs energy [J/mol]
 *   - JT: Joule-Thomson coefficient [K/kPa]
 *   - Kappa: Isentropic exponent [-]
 *   - Cf: Critical flow factor [-]
 * @see PropertiesDetail For the underlying calculation implementation
 */
PropertiesDetailResult PropertiesDetail_wrapper(double T, double D, gasMixture x_array)
{
    std::vector<double> x = gasMixture_to_vector(x_array);
    double P = 0, Z = 0, dPdD = 0, d2PdD2 = 0, d2PdTD = 0, dPdT = 0;
    double U = 0, H = 0, S = 0, Cv = 0, Cp = 0, W = 0, G = 0, JT = 0, Kappa = 0, Cf = 0;

    PropertiesDetail(T, D, x, P, Z, dPdD, d2PdD2, d2PdTD, dPdT,
                     U, H, S, Cv, Cp, W, G, JT, Kappa, Cf);

    PropertiesDetailResult result = {P, Z, dPdD, d2PdD2, d2PdTD, dPdT, U, H, S, Cv, Cp, W, G, JT, Kappa, Cf};

    return result;
}

// GERG wrappers
/**
 * @brief Calculates the molar mass of a gas mixture using GERG-2008 equation of state
 *
 * @param x_array Gas mixture composition in mole percent
 * @return double Molar mass of the mixture in kg/mol
 *
 * This function serves as a wrapper for the MolarMassGERG calculation.
 * It converts the input array to a vector format before performing the calculation.
 *
 * @see MolarMassGERG For the underlying calculation implementation
 */
double MolarMassGERG_wrapper(gasMixture x_array)
{
    std::vector<double> x = gasMixture_to_vector(x_array);
    double Mm = 0;
    MolarMassGERG(x, Mm);
    return Mm;
}

/**
 * @brief Calculates pressure and compressibility factor using GERG-2008 equation of state
 *
 * @param T Temperature in K
 * @param D Density in mol/l (or mol/dm³)
 * @param x_array Gas mixture composition in mole percent
 * @return PressureResult Struct containing:
 *         - P: Pressure in kPa
 *         - Z: Compressibility factor (dimensionless)
 *
 * This function is a wrapper around the PressureGERG function that handles
 * the conversion between array and vector representations of the gas mixture composition.
 *
 * @see PressureGERG For the underlying calculation implementation
 */
PressureResult PressureGERG_wrapper(double T, double D, gasMixture x_array)
{
    std::vector<double> x = gasMixture_to_vector(x_array);
    double P = 0, Z = 0;
    PressureGERG(T, D, x, P, Z);

    PressureResult result = {P, Z};
    return result;
}

/**
 * @brief Wrapper function to calculate density using GERG-2008 equation of state
 *
 * @param iflag Flag to specify calculation path (1: density from pressure/temperature, 2: pressure from density/temperature)
 * @param T Temperature [K]
 * @param P Pressure [kPa]
 * @param x_array Array containing mole fractions of components in percent
 * @return DensityResult Struct containing:
 *         - D: Density [kg/m³]
 *         - ierr: Error flag (0: successful, non-zero: error occurred)
 *         - herr: Error message string
 *
 * @see DensityGERG For the underlying calculation implementation
 */
DensityResult DensityGERG_wrapper(int iflag, double T, double P, gasMixture x_array)
{
    std::vector<double> x = gasMixture_to_vector(x_array);
    double D = 0;
    int ierr = 0;
    std::string herr;

    DensityGERG(iflag, T, P, x, D, ierr, herr);

    DensityResult result = {D, ierr, herr};
    return result;
}

/**
 * @brief Calculates thermodynamic properties using GERG-2008 equation of state
 *
 * @param T Temperature [K]
 * @param D Density [kg/m³]
 * @param x_array Array of mole fractions [%] of the mixture components
 *
 * @return PropertiesGERGResult struct containing:
 *   - P: Pressure [kPa]
 *   - Z: Compressibility factor [-]
 *   - dPdD: Partial derivative of pressure with respect to density [(kPa)/(kg/m³)]
 *   - d2PdD2: Second partial derivative of pressure with respect to density [(kPa)/(kg/m³)²]
 *   - d2PdTD: Mixed partial derivative of pressure with respect to temperature and density [(kPa)/(K·kg/m³)]
 *   - dPdT: Partial derivative of pressure with respect to temperature [kPa/K]
 *   - U: Internal energy [J/kg]
 *   - H: Enthalpy [J/kg]
 *   - S: Entropy [J/(kg·K)]
 *   - Cv: Isochoric heat capacity [J/(kg·K)]
 *   - Cp: Isobaric heat capacity [J/(kg·K)]
 *   - W: Speed of sound [m/s]
 *   - G: Gibbs energy [J/kg]
 *   - JT: Joule-Thomson coefficient [K/kPa]
 *   - Kappa: Isentropic exponent [-]
 *   - A: Helmholtz energy [J/kg]
 *   - Cf: Critical flow factor [-]
 *
 * @see PropertiesGERG For the underlying calculation implementation
 */
PropertiesGERGResult PropertiesGERG_wrapper(double T, double D, gasMixture x_array)
{
    std::vector<double> x = gasMixture_to_vector(x_array);
    double P = 0, Z = 0, dPdD = 0, d2PdD2 = 0, d2PdTD = 0, dPdT = 0;
    double U = 0, H = 0, S = 0, Cv = 0, Cp = 0, W = 0, G = 0, JT = 0, Kappa = 0, A = 0, Cf = 0;

    PropertiesGERG(T, D, x, P, Z, dPdD, d2PdD2, d2PdTD, dPdT,
                   U, H, S, Cv, Cp, W, G, JT, Kappa, A, Cf);

    PropertiesGERGResult result = {P, Z, dPdD, d2PdD2, d2PdTD, dPdT, U, H, S, Cv, Cp, W, G, JT, Kappa, A, Cf};
    return result;
}

// Gross wrappers
/**
 * @brief Calculates the gross molar mass of a gas mixture
 *
 * This function is a wrapper to MolarMassGross that converts a gas mixture composition from array to vector
 * and calculates its gross molar mass.
 *
 * @param x_array Gas mixture composition in mole percent
 * @return double Gross molar mass of the mixture in g/mol
 *
 * @see MolarMassGross For the underlying calculation implementation
 */
double MolarMassGross_wrapper(gasMixture x_array)
{
    std::vector<double> x = gasMixture_to_vector(x_array);
    double Mm = 0;
    MolarMassGross(x, Mm);
    return Mm;
}

/**
 * @brief Wrapper function for PressureGross calculation
 *
 * This function wraps the PressureGross calculation to provide a simplified interface
 * for computing pressure and compressibility factor for natural gas mixtures.
 *
 * @param T Temperature in K
 * @param D Density in mol/l
 * @param xGrs_object Array containing mole fractions of components
 * @param HCH Gross heating value in MJ/m³
 *
 * @return PressureGrossResult struct containing:
 *         - P: Pressure in kPa
 *         - Z: Compressibility factor
 *         - ierr: Error code (0 = success)
 *         - herr: Error message
 *
 * @see PressureGross For the underlying calculation implementation
 */
PressureGrossResult PressureGross_wrapper(double T, double D, xGrs xGrs_object, double HCH)
{
    std::vector<double> xGrs = xGrs_to_vector(xGrs_object);
    double P = 0, Z = 0;
    int ierr = 0;
    std::string herr;

    PressureGross(T, D, xGrs, HCH, P, Z, ierr, herr);

    PressureGrossResult result = {P, Z, ierr, herr};
    return result;
}

/**
 * @brief Wrapper function for DensityGross calculation
 *
 * This function calculates the density of a natural gas mixture using the GERG-2008 equation of state
 *
 * @param T Temperature [K]
 * @param P Pressure [kPa]
 * @param xGrs_object Array containing mole fractions of the mixture components
 * @param HCH Heating value [MJ/m³]
 * @return DensityResult struct containing:
 *         - D: Density [kg/m³]
 *         - ierr: Error code (0 = successful)
 *         - herr: Error message string
 *
 * @see DensityGross For the underlying calculation implementation
 * @note This is a wrapper function that converts the input array to a vector before calling the main DensityGross function
 */
DensityResult DensityGross_wrapper(double T, double P, xGrs xGrs_object, double HCH)
{
    std::vector<double> xGrs = xGrs_to_vector(xGrs_object);
    double D = 0;
    int ierr = 0;
    std::string herr;

    DensityGross(T, P, xGrs, HCH, D, ierr, herr);

    DensityResult result = {D, ierr, herr};
    return result;
}

/**
 * @brief Wrapper function to calculate gross heating values for a gas mixture
 *
 * @param x_array Gas mixture composition in mole fraction
 * @return GrossHvResult Structure containing:
 *         - xGrs: Vector of calculated gross heating values for each component
 *         - HN: Normal gross heating value of the mixture
 *         - HCH: Chinese gross heating value of the mixture
 *
 * This function serves as a wrapper around the GrossHv calculation, converting
 * between array and vector data structures while maintaining the computational
 * logic of the original GERG-2008 implementation.
 *
 * @see GrossHv For the underlying calculation implementation
 */
GrossHvResult GrossHv_wrapper(gasMixture x_array)
{
    std::vector<double> x = gasMixture_to_vector(x_array);
    std::vector<double> xGrs(x.size());
    double HN = 0, HCH = 0;

    GrossHv(x, xGrs, HN, HCH);
    GrossHvResult result = {vector_to_xGrs(xGrs), HN, HCH};
    return result;
}

/**
 * @brief Wrapper function for GrossInputs calculation
 *
 * This function wraps the GrossInputs calculation to handle array conversions and provide a structured result
 *
 * @param T Temperature value
 * @param P Pressure value
 * @param x_array Gas mixture composition in mole percent
 * @return GrossInputsResult Structure containing:
 *         - xGrs: Vector of gross mole fractions
 *         - Gr: Relative density
 *         - HN: Heating value (normal conditions) [MJ/m³]
 *         - HCH: Heating value (combustion conditions) [MJ/m³]
 *         - ierr: Error code (0 = success)
 *         - herr: Error message
 * @see GrossInputs For the underlying calculation implementation
 */
GrossInputsResult GrossInputs_wrapper(double T, double P, gasMixture x_array)
{
    std::vector<double> x = gasMixture_to_vector(x_array);
    std::vector<double> xGrs(x.size());
    double Gr = 0, HN = 0, HCH = 0;
    int ierr = 0;
    std::string herr;

    GrossInputs(T, P, x, xGrs, Gr, HN, HCH, ierr, herr);

    GrossInputsResult result = {vector_to_xGrs(xGrs), Gr, HN, HCH, ierr, herr};
    return result;
}

/**
 * @brief Calculates mixture second and third virial coefficients using the GERG-2008 equation of state
 *
 * @param T Temperature [K]
 * @param xGrs_object Array of mole fractions for each component in the mixture
 * @param HCH Parameter for handling hydrocarbon mixtures
 * @return BmixResult Structure containing:
 *         - B: Second virial coefficient [cm³/mol]
 *         - C: Third virial coefficient [(cm³/mol)²]
 *         - ierr: Error code (0 = successful)
 *         - herr: Error message string
 *
 * This function serves as a wrapper for the Bmix calculation, converting array inputs
 * to the required vector format and returning results in a structured format.
 *
 * @see Bmix For the underlying calculation implementation
 */
BmixResult Bmix_wrapper(double T, xGrs xGrs_object, double HCH)
{
    std::vector<double> xGrs = xGrs_to_vector(xGrs_object);
    double B = 0, C = 0;
    int ierr = 0;
    std::string herr;

    Bmix(T, xGrs, HCH, B, C, ierr, herr);

    BmixResult result = {B, C, ierr, herr};
    return result;
}

/**
 * @brief Wrapper function for GrossMethod1 calculation method
 *
 * @param Th Temperature high (T1) [K]
 * @param Td Temperature difference (T1-T2) [K]
 * @param Pd Pressure drop (P1-P2) [Pa]
 * @param xGrs_object Array of mole fractions for the mixture components
 * @param Gr Mass flow rate [kg/s]
 * @param Hv Heating value [J/kg]
 *
 * @return GrossMethod1Result Structure containing:
 *         - Updated mole fractions array
 *         - Mm (Molar mass) [kg/kmol]
 *         - HCH (Heat capacity [J/(kg·K)])
 *         - HN (Heat value normalized)
 *         - ierr (Error code, 0 = success)
 *         - herr (Error message string)
 *
 * @see GrossMethod1 For the underlying calculation implementation
 */
GrossMethod1Result GrossMethod1_wrapper(double Th, double Td, double Pd, xGrs xGrs_object, double Gr, double Hv)
{
    std::vector<double> xGrs = xGrs_to_vector(xGrs_object);
    double Mm = 0, HCH = 0, HN = 0;
    int ierr = 0;
    std::string herr;

    GrossMethod1(Th, Td, Pd, xGrs, Gr, Hv, Mm, HCH, HN, ierr, herr);

    GrossMethod1Result result = {vector_to_xGrs(xGrs), Mm, HCH, HN, ierr, herr};
    return result;
}

/**
 * @brief Wrapper function for the GrossMethod2 calculation method
 *
 * @param Th Temperature at higher pressure (K)
 * @param Td Temperature at lower pressure (K)
 * @param Pd Pressure at lower temperature (kPa)
 * @param xGrs_object Array containing molar composition of the gas mixture
 * @param Gr Relative density of the gas mixture
 *
 * @return GrossMethod2Result struct containing:
 *         - Updated molar composition array
 *         - Hv: Heating value
 *         - Mm: Molar mass
 *         - HCH: Hydrocarbon content
 *         - HN: Number related to heating value
 *         - ierr: Error code (0 = success)
 *         - herr: Error message string
 *
 * @see GrossMethod2 For the underlying calculation implementation
 */
GrossMethod2Result GrossMethod2_wrapper(double Th, double Td, double Pd, xGrs xGrs_object, double Gr)
{
    std::vector<double> xGrs = xGrs_to_vector(xGrs_object);
    double Hv = 0, Mm = 0, HCH = 0, HN = 0;
    int ierr = 0;
    std::string herr;

    GrossMethod2(Th, Td, Pd, xGrs, Gr, Hv, Mm, HCH, HN, ierr, herr);

    GrossMethod2Result result = {vector_to_xGrs(xGrs), Hv, Mm, HCH, HN, ierr, herr};
    return result;
}

/**
 * @brief Emscripten bindings for the AGA8 gas calculation module
 *
 * These bindings expose the AGA8 gas calculation functionality to JavaScript/WebAssembly.
 * It includes:
 *
 * Enums:
 * - GasComponent: Enumeration of different gas components (methane, nitrogen, etc.)
 *
 * Types:
 * - gasMixtureInMolePercent: Gas mixture composition in mole percent
 * - xGrsArray: Array for gross method calculations
 * - VectorDouble: Vector of double precision numbers
 *
 * Value Objects:
 * - PressureResult: Pressure calculation results (P, Z)
 * - DensityResult: Density calculation results (D, error info)
 * - PropertiesDetailResult: Detailed gas properties results
 * - PropertiesGERGResult: GERG-2008 properties calculation results
 * - PressureGrossResult: Gross method pressure calculation results
 * - GrossHvResult: Heating value calculation results
 * - GrossInputsResult: Gross method input processing results
 * - BmixResult: Binary mixture calculation results
 * - GrossMethod1Result: Results for gross characterization method 1
 * - GrossMethod2Result: Results for gross characterization method 2
 *
 * Functions:
 * Detail Methods:
 * - SetupDetail: Initialize detail calculation method
 * - MolarMassDetail: Calculate molar mass using detail method
 * - PressureDetail: Calculate pressure using detail method
 * - DensityDetail: Calculate density using detail method
 * - PropertiesDetail: Calculate detailed properties
 *
 * GERG Methods:
 * - SetupGERG: Initialize GERG-2008 calculation method
 * - MolarMassGERG: Calculate molar mass using GERG-2008
 * - PressureGERG: Calculate pressure using GERG-2008
 * - DensityGERG: Calculate density using GERG-2008
 * - PropertiesGERG: Calculate properties using GERG-2008
 *
 * Gross Methods:
 * - SetupGross: Initialize gross calculation method
 * - MolarMassGross: Calculate molar mass using gross method
 * - PressureGross: Calculate pressure using gross method
 * - DensityGross: Calculate density using gross method
 * - GrossHv: Calculate heating value
 * - GrossInputs: Process inputs for gross calculations
 * - Bmix: Calculate binary mixture properties
 * - GrossMethod1: Perform gross characterization method 1
 * - GrossMethod2: Perform gross characterization method 2
 */
EMSCRIPTEN_BINDINGS(AGA8_module)
{
    register_vector<double>("VectorDouble");

    value_object<gasMixture>("GasMixture")
        .field("methane", &gasMixture::methane)
        .field("nitrogen", &gasMixture::nitrogen)
        .field("carbon_dioxide", &gasMixture::carbon_dioxide)
        .field("ethane", &gasMixture::ethane)
        .field("propane", &gasMixture::propane)
        .field("isobutane", &gasMixture::isobutane)
        .field("n_butane", &gasMixture::n_butane)
        .field("isopentane", &gasMixture::isopentane)
        .field("n_pentane", &gasMixture::n_pentane)
        .field("n_hexane", &gasMixture::n_hexane)
        .field("n_heptane", &gasMixture::n_heptane)
        .field("n_octane", &gasMixture::n_octane)
        .field("n_nonane", &gasMixture::n_nonane)
        .field("n_decane", &gasMixture::n_decane)
        .field("hydrogen", &gasMixture::hydrogen)
        .field("oxygen", &gasMixture::oxygen)
        .field("carbon_monoxide", &gasMixture::carbon_monoxide)
        .field("water", &gasMixture::water)
        .field("hydrogen_sulfide", &gasMixture::hydrogen_sulfide)
        .field("helium", &gasMixture::helium)
        .field("argon", &gasMixture::argon);

    value_object<xGrs>("xGrs")
        .field("hydrocarbon", &xGrs::hydrocarbon)
        .field("nitrogen", &xGrs::nitrogen)
        .field("carbon_dioxide", &xGrs::carbon_dioxide);

    value_object<PressureResult>("PressureResult")
        .field("P", &PressureResult::P)
        .field("Z", &PressureResult::Z);

    value_object<DensityResult>("DensityResult")
        .field("D", &DensityResult::D)
        .field("ierr", &DensityResult::ierr)
        .field("herr", &DensityResult::herr);

    value_object<PropertiesDetailResult>("PropertiesDetailResult")
        .field("P", &PropertiesDetailResult::P)
        .field("Z", &PropertiesDetailResult::Z)
        .field("dPdD", &PropertiesDetailResult::dPdD)
        .field("d2PdD2", &PropertiesDetailResult::d2PdD2)
        .field("d2PdTD", &PropertiesDetailResult::d2PdTD)
        .field("dPdT", &PropertiesDetailResult::dPdT)
        .field("U", &PropertiesDetailResult::U)
        .field("H", &PropertiesDetailResult::H)
        .field("S", &PropertiesDetailResult::S)
        .field("Cv", &PropertiesDetailResult::Cv)
        .field("Cp", &PropertiesDetailResult::Cp)
        .field("W", &PropertiesDetailResult::W)
        .field("G", &PropertiesDetailResult::G)
        .field("JT", &PropertiesDetailResult::JT)
        .field("Kappa", &PropertiesDetailResult::Kappa)
        .field("Cf", &PropertiesDetailResult::Cf);

    value_object<PropertiesGERGResult>("PropertiesGERGResult")
        .field("P", &PropertiesGERGResult::P)
        .field("Z", &PropertiesGERGResult::Z)
        .field("dPdD", &PropertiesGERGResult::dPdD)
        .field("d2PdD2", &PropertiesGERGResult::d2PdD2)
        .field("d2PdTD", &PropertiesGERGResult::d2PdTD)
        .field("dPdT", &PropertiesGERGResult::dPdT)
        .field("U", &PropertiesGERGResult::U)
        .field("H", &PropertiesGERGResult::H)
        .field("S", &PropertiesGERGResult::S)
        .field("Cv", &PropertiesGERGResult::Cv)
        .field("Cp", &PropertiesGERGResult::Cp)
        .field("W", &PropertiesGERGResult::W)
        .field("G", &PropertiesGERGResult::G)
        .field("JT", &PropertiesGERGResult::JT)
        .field("Kappa", &PropertiesGERGResult::Kappa)
        .field("A", &PropertiesGERGResult::A)
        .field("Cf", &PropertiesGERGResult::Cf);

    value_object<PressureGrossResult>("PressureGrossResult")
        .field("P", &PressureGrossResult::P)
        .field("Z", &PressureGrossResult::Z)
        .field("ierr", &PressureGrossResult::ierr)
        .field("herr", &PressureGrossResult::herr);

    value_object<GrossHvResult>("GrossHvResult")
        .field("xGrs", &GrossHvResult::xGrs)
        .field("HN", &GrossHvResult::HN)
        .field("HCH", &GrossHvResult::HCH);

    value_object<GrossInputsResult>("GrossInputsResult")
        .field("xGrs", &GrossInputsResult::xGrs)
        .field("Gr", &GrossInputsResult::Gr)
        .field("HN", &GrossInputsResult::HN)
        .field("HCH", &GrossInputsResult::HCH)
        .field("ierr", &GrossInputsResult::ierr)
        .field("herr", &GrossInputsResult::herr);

    value_object<BmixResult>("BmixResult")
        .field("B", &BmixResult::B)
        .field("C", &BmixResult::C)
        .field("ierr", &BmixResult::ierr)
        .field("herr", &BmixResult::herr);

    value_object<GrossMethod1Result>("GrossMethod1Result")
        .field("xGrs", &GrossMethod1Result::xGrs)
        .field("Mm", &GrossMethod1Result::Mm)
        .field("HCH", &GrossMethod1Result::HCH)
        .field("HN", &GrossMethod1Result::HN)
        .field("ierr", &GrossMethod1Result::ierr)
        .field("herr", &GrossMethod1Result::herr);

    value_object<GrossMethod2Result>("GrossMethod2Result")
        .field("xGrs", &GrossMethod2Result::xGrs)
        .field("Hv", &GrossMethod2Result::Hv)
        .field("Mm", &GrossMethod2Result::Mm)
        .field("HCH", &GrossMethod2Result::HCH)
        .field("HN", &GrossMethod2Result::HN)
        .field("ierr", &GrossMethod2Result::ierr)
        .field("herr", &GrossMethod2Result::herr);

    // Detail bindings
    function("SetupDetail", &SetupDetail);
    function("MolarMassDetail", &MolarMassDetail_wrapper);
    function("PressureDetail", &PressureDetail_wrapper);
    function("DensityDetail", &DensityDetail_wrapper);
    function("PropertiesDetail", &PropertiesDetail_wrapper);

    // GERG bindings
    function("SetupGERG", &SetupGERG);
    function("MolarMassGERG", &MolarMassGERG_wrapper);
    function("PressureGERG", &PressureGERG_wrapper);
    function("DensityGERG", &DensityGERG_wrapper);
    function("PropertiesGERG", &PropertiesGERG_wrapper);

    // Gross bindings
    function("SetupGross", &SetupGross);
    function("MolarMassGross", &MolarMassGross_wrapper);
    function("PressureGross", &PressureGross_wrapper);
    function("DensityGross", &DensityGross_wrapper);
    function("GrossHv", &GrossHv_wrapper);
    function("GrossInputs", &GrossInputs_wrapper);
    function("Bmix", &Bmix_wrapper);
    function("GrossMethod1", &GrossMethod1_wrapper);
    function("GrossMethod2", &GrossMethod2_wrapper);
}