/*
This software was developed by employees of the National Institute of Standards and Technology (NIST), 
an agency of the Federal Government and is being made available as a public service. Pursuant to title 17 
United States Code Section 105, works of NIST employees are not subject to copyright protection in the 
United States. This software may be subject to foreign copyright. Permission in the United States and in 
foreign countries, to the extent that NIST may hold copyright, to use, copy, modify, create derivative works, 
and distribute this software and its documentation without fee is hereby granted on a non-exclusive basis, 
provided that this notice and disclaimer of warranty appears in all copies.

THE SOFTWARE IS PROVIDED 'AS IS' WITHOUT ANY WARRANTY OF ANY KIND, EITHER EXPRESSED, IMPLIED, OR STATUTORY, 
INCLUDING, BUT NOT LIMITED TO, ANY WARRANTY THAT THE SOFTWARE WILL CONFORM TO SPECIFICATIONS, ANY IMPLIED 
WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, AND FREEDOM FROM INFRINGEMENT, AND ANY 
WARRANTY THAT THE DOCUMENTATION WILL CONFORM TO THE SOFTWARE, OR ANY WARRANTY THAT THE SOFTWARE WILL BE 
ERROR FREE. IN NO EVENT SHALL NIST BE LIABLE FOR ANY DAMAGES, INCLUDING, BUT NOT LIMITED TO, DIRECT, INDIRECT, 
SPECIAL OR CONSEQUENTIAL DAMAGES, ARISING OUT OF, RESULTING FROM, OR IN ANY WAY CONNECTED WITH THIS SOFTWARE, 
WHETHER OR NOT BASED UPON WARRANTY, CONTRACT, TORT, OR OTHERWISE, WHETHER OR NOT INJURY WAS SUSTAINED BY 
PERSONS OR PROPERTY OR OTHERWISE, AND WHETHER OR NOT LOSS WAS SUSTAINED FROM, OR AROSE OUT OF THE RESULTS OF, 
OR USE OF, THE SOFTWARE OR SERVICES PROVIDED HEREUNDER.
*/
#ifndef AGA8Gross_H_
#define AGA8Gross_H_

#include <vector>
#include <string>

void MolarMassGross(const std::vector<double> &x, double &Mm);
void PressureGross(const double T, const double D, const std::vector<double> &xGrs, const double HCH, double &P, double &Z, int &ierr, std::string &herr);
void DensityGross(const double T, const double P, const std::vector<double> &xGrs, const double HCH, double &D, int &ierr, std::string &herr);
void GrossHv(const std::vector<double> &x, std::vector<double> &xGrs, double &HN, double &HCH);
void GrossInputs(const double T, const double P, const std::vector<double> &x, std::vector<double> &xGrs, double &Gr, double &HN, double &HCH, int &ierr, std::string &herr);
void Bmix(const double T, const std::vector<double> &xGrs, const double HCH, double &B, double &C, int &ierr, std::string &herr);
void GrossMethod1(const double Th, const double Td, const double Pd, std::vector<double> &xGrs, const double Gr, const double Hv, double & Mm, double &HCH, double &HN, int &ierr, std::string &herr);
void GrossMethod2(const double Th, const double Td, const double Pd, std::vector<double> &xGrs, const double Gr, double &Hv, double &Mm, double &HCH, double &HN, int &ierr, std::string &herr);
void SetupGross();

#endif
