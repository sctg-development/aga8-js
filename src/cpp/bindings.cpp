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

EMSCRIPTEN_DECLARE_VAL_TYPE(gazMixtureInMolePercent);
EMSCRIPTEN_DECLARE_VAL_TYPE(xGrsArray);

using namespace emscripten;
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

struct PressureResult
{
    double P;
    double Z;
};
struct DensityResult
{
    double D;
    int ierr;
    std::string herr;
};

struct PropertiesDetailResult
{
    double P, Z, dPdD, d2PdD2, d2PdTD, dPdT;
    double U, H, S, Cv, Cp, W, G, JT, Kappa;
};

struct PropertiesGERGResult
{
    double P, Z, dPdD, d2PdD2, d2PdTD, dPdT;
    double U, H, S, Cv, Cp, W, G, JT, Kappa, A;
};

struct PressureGrossResult
{
    double P;
    double Z;
    int ierr;
    std::string herr;
};
struct GrossHvResult
{
    val xGrs;
    double HN;
    double HCH;
};

struct GrossInputsResult
{
    val xGrs;
    double Gr;
    double HN;
    double HCH;
    int ierr;
    std::string herr;
};

struct BmixResult
{
    double B;
    double C;
    int ierr;
    std::string herr;
};

struct GrossMethod1Result
{
   val xGrs;
    double Mm;
    double HCH;
    double HN;
    int ierr;
    std::string herr;
};

struct GrossMethod2Result
{
    val xGrs;
    double Hv;
    double Mm;
    double HCH;
    double HN;
    int ierr;
    std::string herr;
};

// Helper function to convert a JavaScript array to a C++ vector
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
double MolarMassDetail_wrapper(gazMixtureInMolePercent x_array)
{
    std::vector<double> x = array_to_vector(x_array);
    double Mm = 0;
    MolarMassDetail(x, Mm);
    return Mm;
}

PressureResult PressureDetail_wrapper(double T, double D, gazMixtureInMolePercent x_array)
{
    std::vector<double> x = array_to_vector(x_array);
    double P = 0, Z = 0;
    PressureDetail(T, D, x, P, Z);
    PressureResult result = {P, Z};
    return result;
}

DensityResult DensityDetail_wrapper(double T, double P, gazMixtureInMolePercent x_array)
{
    std::vector<double> x = array_to_vector(x_array);
    double D = 0;
    int ierr = 0;
    std::string herr;

    DensityDetail(T, P, x, D, ierr, herr);

    DensityResult result = {D, ierr, herr};
    return result;
}

PropertiesDetailResult PropertiesDetail_wrapper(double T, double D, gazMixtureInMolePercent x_array)
{
    std::vector<double> x = array_to_vector(x_array);
    double P = 0, Z = 0, dPdD = 0, d2PdD2 = 0, d2PdTD = 0, dPdT = 0;
    double U = 0, H = 0, S = 0, Cv = 0, Cp = 0, W = 0, G = 0, JT = 0, Kappa = 0;

    PropertiesDetail(T, D, x, P, Z, dPdD, d2PdD2, d2PdTD, dPdT,
                     U, H, S, Cv, Cp, W, G, JT, Kappa);

    PropertiesDetailResult result = {P, Z, dPdD, d2PdD2, d2PdTD, dPdT, U, H, S, Cv, Cp, W, G, JT, Kappa};

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

PressureResult PressureGERG_wrapper(double T, double D, gazMixtureInMolePercent x_array)
{
    std::vector<double> x = array_to_vector(x_array);
    double P = 0, Z = 0;
    PressureGERG(T, D, x, P, Z);

    PressureResult result = {P, Z};
    return result;
}

DensityResult DensityGERG_wrapper(int iflag, double T, double P, gazMixtureInMolePercent x_array)
{
    std::vector<double> x = array_to_vector(x_array);
    double D = 0;
    int ierr = 0;
    std::string herr;

    DensityGERG(iflag, T, P, x, D, ierr, herr);

    DensityResult result = {D, ierr, herr};
    return result;
}

PropertiesGERGResult PropertiesGERG_wrapper(double T, double D, gazMixtureInMolePercent x_array)
{
    std::vector<double> x = array_to_vector(x_array);
    double P = 0, Z = 0, dPdD = 0, d2PdD2 = 0, d2PdTD = 0, dPdT = 0;
    double U = 0, H = 0, S = 0, Cv = 0, Cp = 0, W = 0, G = 0, JT = 0, Kappa = 0, A = 0;

    PropertiesGERG(T, D, x, P, Z, dPdD, d2PdD2, d2PdTD, dPdT,
                   U, H, S, Cv, Cp, W, G, JT, Kappa, A);

    PropertiesGERGResult result = {P, Z, dPdD, d2PdD2, d2PdTD, dPdT, U, H, S, Cv, Cp, W, G, JT, Kappa, A};
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

PressureGrossResult PressureGross_wrapper(double T, double D, xGrsArray xGrs_array, double HCH)
{
    std::vector<double> xGrs = array_to_vector(xGrs_array);
    double P = 0, Z = 0;
    int ierr = 0;
    std::string herr;

    PressureGross(T, D, xGrs, HCH, P, Z, ierr, herr);

    PressureGrossResult result = {P, Z, ierr, herr};
    return result;
}

DensityResult DensityGross_wrapper(double T, double P, xGrsArray xGrs_array, double HCH)
{
    std::vector<double> xGrs = array_to_vector(xGrs_array);
    double D = 0;
    int ierr = 0;
    std::string herr;

    DensityGross(T, P, xGrs, HCH, D, ierr, herr);

    DensityResult result = {D, ierr, herr};
    return result;
}

GrossHvResult GrossHv_wrapper(gazMixtureInMolePercent x_array)
{
    std::vector<double> x = array_to_vector(x_array);
    std::vector<double> xGrs(x.size());
    double HN = 0, HCH = 0;

    GrossHv(x, xGrs, HN, HCH);
    GrossHvResult result = {vector_to_array(xGrs), HN, HCH};
    return result;
}

GrossInputsResult GrossInputs_wrapper(double T, double P, gazMixtureInMolePercent x_array)
{
    std::vector<double> x = array_to_vector(x_array);
    std::vector<double> xGrs(x.size());
    double Gr = 0, HN = 0, HCH = 0;
    int ierr = 0;
    std::string herr;

    GrossInputs(T, P, x, xGrs, Gr, HN, HCH, ierr, herr);

    GrossInputsResult result = {vector_to_array(xGrs), Gr, HN, HCH, ierr, herr};
    return result;
}

BmixResult Bmix_wrapper(double T, xGrsArray xGrs_array, double HCH)
{
    std::vector<double> xGrs = array_to_vector(xGrs_array);
    double B = 0, C = 0;
    int ierr = 0;
    std::string herr;

    Bmix(T, xGrs, HCH, B, C, ierr, herr);

    BmixResult result = {B, C, ierr, herr};
    return result;
}

GrossMethod1Result GrossMethod1_wrapper(double Th, double Td, double Pd, xGrsArray xGrs_array, double Gr, double Hv)
{
    std::vector<double> xGrs = array_to_vector(xGrs_array);
    double Mm = 0, HCH = 0, HN = 0;
    int ierr = 0;
    std::string herr;

    GrossMethod1(Th, Td, Pd, xGrs, Gr, Hv, Mm, HCH, HN, ierr, herr);

    GrossMethod1Result result = {vector_to_array(xGrs), Mm, HCH, HN, ierr, herr};
    return result;
}

GrossMethod2Result GrossMethod2_wrapper(double Th, double Td, double Pd, xGrsArray xGrs_array, double Gr)
{
    std::vector<double> xGrs = array_to_vector(xGrs_array);
    double Hv = 0, Mm = 0, HCH = 0, HN = 0;
    int ierr = 0;
    std::string herr;

    GrossMethod2(Th, Td, Pd, xGrs, Gr, Hv, Mm, HCH, HN, ierr, herr);

    GrossMethod2Result result = {vector_to_array(xGrs), Hv, Mm, HCH, HN, ierr, herr};
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

    register_type<gazMixtureInMolePercent>("gazMixtureInMolePercent");
    register_type<xGrsArray>("xGrsArray");
    register_vector<double>("VectorDouble");

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
        .field("Kappa", &PropertiesDetailResult::Kappa);

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
        .field("A", &PropertiesGERGResult::A);

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