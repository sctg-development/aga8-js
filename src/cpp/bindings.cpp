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

// The compositions in the gazMixtureInMolePercent array use the following order and must be sent as mole fractions:
//     0 - PLACEHOLDER
//     1 - Methane
//     2 - Nitrogen
//     3 - Carbon dioxide
//     4 - Ethane
//     5 - Propane
//     6 - Isobutane
//     7 - n-Butane
//     8 - Isopentane
//     9 - n-Pentane
//    10 - n-Hexane
//    11 - n-Heptane
//    12 - n-Octane
//    13 - n-Nonane
//    14 - n-Decane
//    15 - Hydrogen
//    16 - Oxygen
//    17 - Carbon monoxide
//    18 - Water
//    19 - Hydrogen sulfide
//    20 - Helium
//    21 - Argon
//
// For example, a mixture (in moles) of 94% methane, 5% CO2, and 1% helium would be (in mole fractions):
// gazMixtureInMolePercent(1)=0.94, gazMixtureInMolePercent(3)=0.05, gazMixtureInMolePercent(20)=0.01
enum class GasComponent
{
    PLACEHOLDER = 0,
    METHANE = 1,
    NITROGEN = 2,
    CARBON_DIOXIDE = 3,
    ETHANE = 4,
    PROPANE = 5,
    ISOBUTANE = 6,
    N_BUTANE = 7,
    ISOPENTANE = 8,
    N_PENTANE = 9,
    N_HEXANE = 10,
    N_HEPTANE = 11,
    N_OCTANE = 12,
    N_NONANE = 13,
    N_DECANE = 14,
    HYDROGEN = 15,
    OXYGEN = 16,
    CARBON_MONOXIDE = 17,
    WATER = 18,
    HYDROGEN_SULFIDE = 19,
    HELIUM = 20,
    ARGON = 21
};
EMSCRIPTEN_DECLARE_VAL_TYPE(gazMixtureInMolePercent);

using namespace emscripten;

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

double MolarMassDetail_wrapper(gazMixtureInMolePercent x_array)
{
    std::vector<double> x = array_to_vector(x_array);
    double Mm = 0;
    MolarMassDetail(x, Mm);
    return Mm;
}

val PressureDetail_wrapper(double T, double D, gazMixtureInMolePercent x_array)
{
    std::vector<double> x = array_to_vector(x_array);
    double P = 0, Z = 0;
    PressureDetail(T, D, x, P, Z);

    val result = val::object();
    result.set("P", P);
    result.set("Z", Z);
    return result;
}

val DensityDetail_wrapper(double T, double P, gazMixtureInMolePercent x_array)
{
    std::vector<double> x = array_to_vector(x_array);
    double D = 0;
    int ierr = 0;
    std::string herr;

    DensityDetail(T, P, x, D, ierr, herr);

    val result = val::object();
    result.set("D", D);
    result.set("ierr", ierr);
    result.set("herr", herr);
    return result;
}

val PropertiesDetail_wrapper(double T, double D, gazMixtureInMolePercent x_array)
{
    std::vector<double> x = array_to_vector(x_array);
    double P = 0, Z = 0, dPdD = 0, d2PdD2 = 0, d2PdTD = 0, dPdT = 0;
    double U = 0, H = 0, S = 0, Cv = 0, Cp = 0, W = 0, G = 0, JT = 0, Kappa = 0;

    PropertiesDetail(T, D, x, P, Z, dPdD, d2PdD2, d2PdTD, dPdT,
                     U, H, S, Cv, Cp, W, G, JT, Kappa);

    val result = val::object();
    result.set("P", P);
    result.set("Z", Z);
    result.set("dPdD", dPdD);
    result.set("d2PdD2", d2PdD2);
    result.set("d2PdTD", d2PdTD);
    result.set("dPdT", dPdT);
    result.set("U", U);
    result.set("H", H);
    result.set("S", S);
    result.set("Cv", Cv);
    result.set("Cp", Cp);
    result.set("W", W);
    result.set("G", G);
    result.set("JT", JT);
    result.set("Kappa", Kappa);
    return result;
}

// GERG wrappers
double MolarMassGERG_wrapper(gazMixtureInMolePercent x_array)
{
    std::vector<double> x = array_to_vector(x_array);
    double Mm = 0;
    MolarMassGERG(x, Mm);
    return Mm;
}

val PressureGERG_wrapper(double T, double D, gazMixtureInMolePercent x_array)
{
    std::vector<double> x = array_to_vector(x_array);
    double P = 0, Z = 0;
    PressureGERG(T, D, x, P, Z);

    val result = val::object();
    result.set("P", P);
    result.set("Z", Z);
    return result;
}

val DensityGERG_wrapper(int iflag, double T, double P, gazMixtureInMolePercent x_array)
{
    std::vector<double> x = array_to_vector(x_array);
    double D = 0;
    int ierr = 0;
    std::string herr;

    DensityGERG(iflag, T, P, x, D, ierr, herr);

    val result = val::object();
    result.set("D", D);
    result.set("ierr", ierr);
    result.set("herr", herr);
    return result;
}

val PropertiesGERG_wrapper(double T, double D, gazMixtureInMolePercent x_array)
{
    std::vector<double> x = array_to_vector(x_array);
    double P = 0, Z = 0, dPdD = 0, d2PdD2 = 0, d2PdTD = 0, dPdT = 0;
    double U = 0, H = 0, S = 0, Cv = 0, Cp = 0, W = 0, G = 0, JT = 0, Kappa = 0, A = 0;

    PropertiesGERG(T, D, x, P, Z, dPdD, d2PdD2, d2PdTD, dPdT,
                   U, H, S, Cv, Cp, W, G, JT, Kappa, A);

    val result = val::object();
    result.set("P", P);
    result.set("Z", Z);
    result.set("dPdD", dPdD);
    result.set("d2PdD2", d2PdD2);
    result.set("d2PdTD", d2PdTD);
    result.set("dPdT", dPdT);
    result.set("U", U);
    result.set("H", H);
    result.set("S", S);
    result.set("Cv", Cv);
    result.set("Cp", Cp);
    result.set("W", W);
    result.set("G", G);
    result.set("JT", JT);
    result.set("Kappa", Kappa);
    result.set("A", A);
    return result;
}

// Gross wrappers
double MolarMassGross_wrapper(gazMixtureInMolePercent x_array)
{
    std::vector<double> x = array_to_vector(x_array);
    double Mm = 0;
    MolarMassGross(x, Mm);
    return Mm;
}

val PressureGross_wrapper(double T, double D, val xGrs_array, double HCH)
{
    std::vector<double> xGrs = array_to_vector(xGrs_array);
    double P = 0, Z = 0;
    int ierr = 0;
    std::string herr;

    PressureGross(T, D, xGrs, HCH, P, Z, ierr, herr);

    val result = val::object();
    result.set("P", P);
    result.set("Z", Z);
    result.set("ierr", ierr);
    result.set("herr", herr);
    return result;
}

val DensityGross_wrapper(double T, double P, val xGrs_array, double HCH)
{
    std::vector<double> xGrs = array_to_vector(xGrs_array);
    double D = 0;
    int ierr = 0;
    std::string herr;

    DensityGross(T, P, xGrs, HCH, D, ierr, herr);

    val result = val::object();
    result.set("D", D);
    result.set("ierr", ierr);
    result.set("herr", herr);
    return result;
}

val GrossHv_wrapper(gazMixtureInMolePercent x_array)
{
    std::vector<double> x = array_to_vector(x_array);
    std::vector<double> xGrs(x.size());
    double HN = 0, HCH = 0;

    GrossHv(x, xGrs, HN, HCH);

    val result = val::object();
    result.set("xGrs", val::array(xGrs));
    result.set("HN", HN);
    result.set("HCH", HCH);
    return result;
}

val GrossInputs_wrapper(double T, double P, gazMixtureInMolePercent x_array)
{
    std::vector<double> x = array_to_vector(x_array);
    std::vector<double> xGrs(x.size());
    double Gr = 0, HN = 0, HCH = 0;
    int ierr = 0;
    std::string herr;

    GrossInputs(T, P, x, xGrs, Gr, HN, HCH, ierr, herr);

    val result = val::object();
    result.set("xGrs", val::array(xGrs));
    result.set("Gr", Gr);
    result.set("HN", HN);
    result.set("HCH", HCH);
    result.set("ierr", ierr);
    result.set("herr", herr);
    return result;
}

val Bmix_wrapper(double T, val xGrs_array, double HCH)
{
    std::vector<double> xGrs = array_to_vector(xGrs_array);
    double B = 0, C = 0;
    int ierr = 0;
    std::string herr;

    Bmix(T, xGrs, HCH, B, C, ierr, herr);

    val result = val::object();
    result.set("B", B);
    result.set("C", C);
    result.set("ierr", ierr);
    result.set("herr", herr);
    return result;
}

val GrossMethod1_wrapper(double Th, double Td, double Pd, val xGrs_array, double Gr, double Hv)
{
    std::vector<double> xGrs = array_to_vector(xGrs_array);
    double Mm = 0, HCH = 0, HN = 0;
    int ierr = 0;
    std::string herr;

    GrossMethod1(Th, Td, Pd, xGrs, Gr, Hv, Mm, HCH, HN, ierr, herr);

    val result = val::object();
    result.set("xGrs", val::array(xGrs));
    result.set("Mm", Mm);
    result.set("HCH", HCH);
    result.set("HN", HN);
    result.set("ierr", ierr);
    result.set("herr", herr);
    return result;
}

val GrossMethod2_wrapper(double Th, double Td, double Pd, val xGrs_array, double Gr)
{
    std::vector<double> xGrs = array_to_vector(xGrs_array);
    double Hv = 0, Mm = 0, HCH = 0, HN = 0;
    int ierr = 0;
    std::string herr;

    GrossMethod2(Th, Td, Pd, xGrs, Gr, Hv, Mm, HCH, HN, ierr, herr);

    val result = val::object();
    result.set("xGrs", val::array(xGrs));
    result.set("Hv", Hv);
    result.set("Mm", Mm);
    result.set("HCH", HCH);
    result.set("HN", HN);
    result.set("ierr", ierr);
    result.set("herr", herr);
    return result;
}

EMSCRIPTEN_BINDINGS(AGA8_module)
{
    enum_<GasComponent>("GasComponent")
        .value("PLACEHOLDER", GasComponent::PLACEHOLDER)
        .value("METHANE", GasComponent::METHANE)
        .value("NITROGEN", GasComponent::NITROGEN)
        .value("CARBON_DIOXIDE", GasComponent::CARBON_DIOXIDE)
        .value("ETHANE", GasComponent::ETHANE)
        .value("PROPANE", GasComponent::PROPANE)
        .value("ISOBUTANE", GasComponent::ISOBUTANE)
        .value("N_BUTANE", GasComponent::N_BUTANE)
        .value("ISOPENTANE", GasComponent::ISOPENTANE)
        .value("N_PENTANE", GasComponent::N_PENTANE)
        .value("N_HEXANE", GasComponent::N_HEXANE)
        .value("N_HEPTANE", GasComponent::N_HEPTANE)
        .value("N_OCTANE", GasComponent::N_OCTANE)
        .value("N_NONANE", GasComponent::N_NONANE)
        .value("N_DECANE", GasComponent::N_DECANE)
        .value("HYDROGEN", GasComponent::HYDROGEN)
        .value("OXYGEN", GasComponent::OXYGEN)
        .value("CARBON_MONOXIDE", GasComponent::CARBON_MONOXIDE)
        .value("WATER", GasComponent::WATER)
        .value("HYDROGEN_SULFIDE", GasComponent::HYDROGEN_SULFIDE)
        .value("HELIUM", GasComponent::HELIUM)
        .value("ARGON", GasComponent::ARGON);

    register_type<gazMixtureInMolePercent>("number[]");

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