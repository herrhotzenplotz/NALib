
#ifndef NA_MATH_CONSTANTS_INCLUDED
#define NA_MATH_CONSTANTS_INCLUDED
#ifdef __cplusplus
  extern "C"{
#endif

#include <float.h>
#include <math.h>
#include "../NABase.h"

// Fundamental mathematical constants. All macros are defined as float, double
// and long double with the corresponding suffix.
//
// Note that there usually exist constants in math.h like M_PI but they
// are not standardized and so far, the author has not found a difference
// in computation.
//
// Note that the constants are written with approximately 30 digits which is
// more than enough even for long double.
//
// Note that all values are computed with maple like this:
// Digits := 30;
// evalf( Pi );

// The famous pi constant
#define NA_PIf             3.14159265358979323846264338328f
#define NA_PI              3.14159265358979323846264338328
#define NA_PIl             3.14159265358979323846264338328L

// 2 times Pi. The full circumference of the unit circle.
#define NA_PI2f            6.28318530717958647692528676656f
#define NA_PI2             6.28318530717958647692528676656
#define NA_PI2l            6.28318530717958647692528676656L

// The half of Pi. Corresponds to an angle of 90 Degrees
#define NA_PI_HALFf        1.57079632679489661923132169164f
#define NA_PI_HALF         1.57079632679489661923132169164
#define NA_PI_HALFl        1.57079632679489661923132169164L

// The Euler constant
#define NA_Ef              2.71828182845904523536028747135f
#define NA_E               2.71828182845904523536028747135
#define NA_El              2.71828182845904523536028747135L

// Square root of 2.
#define NA_SQRT2f          1.41421356237309504880168872421f
#define NA_SQRT2           1.41421356237309504880168872421
#define NA_SQRT2l          1.41421356237309504880168872421L

// Square root of 3.
#define NA_SQRT3f          1.73205080756887729352744634151f
#define NA_SQRT3           1.73205080756887729352744634151
#define NA_SQRT3l          1.73205080756887729352744634151L

// Square root of 2 pi
#define NA_SQRTPI2f        2.50662827463100050241576528481f
#define NA_SQRTPI2         2.50662827463100050241576528481
#define NA_SQRTPI2l        2.50662827463100050241576528481L

// Natural Logarithm of 2.
// Used on systems which do not define log2 natively.
#define NA_INV_LOGOF2f     1.44269504088896340735992468100f
#define NA_INV_LOGOF2      1.44269504088896340735992468100
#define NA_INV_LOGOF2l     1.44269504088896340735992468100L

// Natural Logarithm of 10.
// Used on systems which do not defined log10 natively.
#define NA_INV_LOGOF10f    0.434294481903251827651128918917f
#define NA_INV_LOGOF10     0.434294481903251827651128918917
#define NA_INV_LOGOF10l    0.434294481903251827651128918917L

// The smaller part of the golden ratio
#define NA_GOLD_RATIOf     0.38196601125010515179541316563f
#define NA_GOLD_RATIO      0.38196601125010515179541316563
#define NA_GOLD_RATIOl     0.38196601125010515179541316563L

// The bigger part of the golden ratio
#define NA_GOLD_RATIO_BIGf 0.618033988749894848204586834370f
#define NA_GOLD_RATIO_BIG  0.618033988749894848204586834370
#define NA_GOLD_RATIO_BIGl 0.618033988749894848204586834370L

// The conversion factor between degrees and radiants
#define NA_DEGREESf        0.0174532925199432957692369076849f
#define NA_DEGREES         0.0174532925199432957692369076849
#define NA_DEGREESl        0.0174532925199432957692369076849L

// The conversion factor between radiants and degrees = 1/NA_DEGREES
#define NA_DEG_PER_RADf    57.2957795130823208767981548141f
#define NA_DEG_PER_RAD     57.2957795130823208767981548141
#define NA_DEG_PER_RADl    57.2957795130823208767981548141L

#define NA_THIRDf          0.33333333333333333333333333333f
#define NA_THIRD           0.33333333333333333333333333333
#define NA_THIRDl          0.33333333333333333333333333333L

#define NA_ARBITRARY_BUT_CAREFULLY_CHOSEN_VALUE 42



// The following constants are constants from physics, chemistry, mathematics,
// all mixed together. All constants are provided only as double constants.
// These constants are directly from NIST:
// http://physics.nist.gov/constants
//
// See Configuration.h for the definition of NA_NIST_CODATA_YEAR


// Universal Constants
// - VAC_ELEC_PERMET    Vacuum electric permittivity, Z_0 in [Ohm]
// - VAC_MAG_PERMET     Vacuum magnetic permeability, epsilon_0 in [Fm-1]
// - MAG_CONST          magnetic constant, mu_0 in [NA-2], = 4*PI*10^-7
// - GRAV               Newtonian constant of gravitation, G in [m3kg-1s-2]
// - GRAV_HBARC         Newtonian constant of gravitation over h-bar c,
//                      G/(h_bar c) in [(GeV/c2)-2]
// - PLANCK             Planck constant, h in [Js]
// - PLANCK_EVS         Planck constant in eV s, h in [eVs]
// - PLANCK_REDUCED     Reduced Planck constant, h_bar in [Js]
// - PLANCK_REDUCED_EVS Reduced Planck constant in eV s, h_bar in [eVs]
// - PLANCK_REDUCED_C   Reduced Planck constant times c in MeV fm, h_bar c
//                      in [MeV fm].
// - PLANCK_LEN         Planck length, l_P in [m]
// - PLANCK_MASS        Planck mass, m_P in [kg]
// - PLANCK_MASS_EQU    Planck mass energy equivalent in GeV, m_P c2 in [GeV]
// - PLANCK_TEMP        Planck temperature, T_P in [K]
// - PLANCK_TIME        Planck time, t_P in [s]
// - LIGHT_SPEED_VAC    speed of light in vacuum, c_0 in [ms-1]
//
// - NA_ZERO_DEGREES_CELSIUS    273.15 Kelvin.

// Note that with the new SI system of 2018, some constants changed their
// name. Please use the new macro names starting with 2018:
//
// NA_CHAR_IMP_VAC     -> NA_VAC_ELEC_PERMET
// NA_ELEC_CONST       -> NA_VAC_ELEC_PERMET
// PLANCK_2PI          -> PLANCK_REDUCED
// PLANCK_2PI_EVS      -> PLANCK_REDUCED_EVS
// PLANCK_2PIC         -> PLANCK_REDUCED_C
// NA_PLANCK_MASS_GEV  -> PLANCK_MASS_EQU

#if (NA_NIST_CODATA_YEAR == 2006)
  #define NA_LIGHT_SPEED_VAC     299792458.
  #define NA_CHAR_IMP_VAC        376.730313461
  #define NA_ELEC_CONST          8.854187817e-12
  #define NA_MAG_CONST           12.566370614e-7
  #define NA_GRAV                6.67428e-11
  #define NA_GRAV_HBARC          6.70881e-39
  #define NA_PLANCK              6.62606896e-34
  #define NA_PLANCK_EVS          4.13566733e-15
  #define NA_PLANCK_2PI          1.054571628e-34
  #define NA_PLANCK_2PI_EVS      6.58211899e-16
  #define NA_PLANCK_2PIC         197.3269631
  #define NA_PLANCK_LEN          1.616252e-35
  #define NA_PLANCK_MASS         2.17644e-8
  #define NA_PLANCK_MASS_GEV     1.220892e19
  #define NA_PLANCK_TEMP         1.416785e32
  #define NA_PLANCK_TIME         5.39124e-44
#elif (NA_NIST_CODATA_YEAR == 2010)
  #define NA_LIGHT_SPEED_VAC     299792458.         // same as 2006
  #define NA_CHAR_IMP_VAC        376.730313461      // same as 2006
  #define NA_ELEC_CONST          8.854187817e-12    // same as 2006
  #define NA_MAG_CONST           12.566370614e-7    // same as 2006
  #define NA_GRAV                6.67384e-11
  #define NA_GRAV_HBARC          6.70837e-39
  #define NA_PLANCK              6.62606957e-34
  #define NA_PLANCK_EVS          4.135667516e-15
  #define NA_PLANCK_2PI          1.054571726e-34
  #define NA_PLANCK_2PI_EVS      6.58211928e-16
  #define NA_PLANCK_2PIC         197.3269718
  #define NA_PLANCK_LEN          1.616199e-35
  #define NA_PLANCK_MASS         2.17651e-8
  #define NA_PLANCK_MASS_GEV     1.220932e19
  #define NA_PLANCK_TEMP         1.416833e32
  #define NA_PLANCK_TIME         5.39106e-44
#elif (NA_NIST_CODATA_YEAR == 2014)
  #define NA_LIGHT_SPEED_VAC     299792458.         // same as 2010
  #define NA_CHAR_IMP_VAC        376.730313461      // same as 2010
  #define NA_ELEC_CONST          8.854187817e-12    // same as 2010
  #define NA_MAG_CONST           12.566370614e-7    // same as 2010
  #define NA_GRAV                6.67408e-11
  #define NA_GRAV_HBARC          6.70861e-39
  #define NA_PLANCK              6.626070040e-34
  #define NA_PLANCK_EVS          4.135667662e-15
  #define NA_PLANCK_2PI          1.054571800e-34
  #define NA_PLANCK_2PI_EVS      6.582119514e-16
  #define NA_PLANCK_2PIC         197.3269788
  #define NA_PLANCK_LEN          1.616229e-35
  #define NA_PLANCK_MASS         2.176470e-8
  #define NA_PLANCK_MASS_GEV     1.220910e19
  #define NA_PLANCK_TEMP         1.416808e32
  #define NA_PLANCK_TIME         5.39116e-44
#elif (NA_NIST_CODATA_YEAR == 2018)
  #define NA_LIGHT_SPEED_VAC     299792458.         // same as 2014
  #define NA_CHAR_IMP_VAC        376.730313668
  #define NA_VAC_ELEC_PERMET     8.8541878128e-12   // was NA_ELEC_CONST
  #define NA_VAC_MAG_PERMET      1.25663706212e-6   // was NA_MAG_CONST
  #define NA_GRAV                6.67430e-11
  #define NA_GRAV_HBARC          6.70883e-39
  #define NA_PLANCK              6.62607015e-34
  #define NA_PLANCK_EVS          4.135667696e-15
  #define NA_PLANCK_REDUCED      1.054571817e-34    // was NA_PLANCK_2PI
  #define NA_PLANCK_REDUCED_EVS  6.582119569e-16    // was NA_PLANCK_2PI_EVS
  #define NA_PLANCK_REDUCED_C    197.3269804        // was NA_PLANCK_2PIC
  #define NA_PLANCK_LEN          1.616255e-35
  #define NA_PLANCK_MASS         2.176434e-8
  #define NA_PLANCK_MASS_EQU     1.220890e19        // was NA_PLANCK_MASS_GEV
  #define NA_PLANCK_TEMP         1.416784e32
  #define NA_PLANCK_TIME         5.391247e-44
#endif

#define NA_ZERO_DEGREES_CELSIUS  273.15

// Electromagnetic constants
// - BOHR               Bohr magneton, mu_B in [JT-1]
// - BOHR_EVT           Bohr magneton in eV/T, mu_B in [eVT-1]
// - BOHR_HZT           Bohr magneton in Hz/T, mu_B/h in [HzT-1]
// - BOHR_MTESLA        Bohr magneton in inverse meters per tesla, mu_B/hc
//                      in [m-1T-1]
// - BOHR_KT            Bohr magneton in K/T, mu_B/k in [KT-1]
// - COND_QUANT         conductance quantum, G_0 in [S]
// - COND_QUANT_INV     inverse of conductance quantum, G_0-1 in [Ohm]
// - E_CHARGE           elementary charge, e in [C]
// - E_CHARGE_H         elementary charge over h, e/h in [AJ-1]
// - JOSEPHSON          Josephson constant, K_J in [HzV-1]
// - MAG_FLUX_QUANT     magnetic flux quantum, Phi_0 in [Wb]
// - NUC_MAG            nuclear magneton, mu_N in [JT-1]
// - NUC_MAG_EVT        nuclear magneton in eV/T, mu_N in [eVT-1]
// - NUC_MAG_MTESLA     nuclear magneton in inverse meters per tesla, mu_N/hc
//                      in [m-1T-1]
// - NUC_MAG_KT         nuclear magneton in K/T, mu_N/k in [KT-1]
// - NUC_MAG_MHZT       nuclear magneton in MHz/T, mu_N/h in [MHzT-1]
// - VON_KLITZING       von Klitzing constant, R_K in [Ohm]

#if (NA_NIST_CODATA_YEAR == 2006)
  #define NA_BOHR                927.400915e-26
  #define NA_BOHR_EVT            5.7883817555e-5
  #define NA_BOHR_HZT            13.99624604e9
  #define NA_BOHR_MTESLA         46.6864515
  #define NA_BOHR_KT             0.6717131
  #define NA_COND_QUANT          7.7480917004e-5
  #define NA_COND_QUANT_INV      12906.4037787
  #define NA_E_CHARGE            1.602176487e-19
  #define NA_E_CHARGE_H          2.417989454e14
  #define NA_JOSEPHSON           483597.891e9
  #define NA_MAG_FLUX_QUANT      2.067833667e-15
  #define NA_NUC_MAG             5.05078324e-27
  #define NA_NUC_MAG_EVT         3.1524512326e-8
  #define NA_NUC_MAG_MTESLA      2.542623616e-2
  #define NA_NUC_MAG_KT          3.6582637e-4
  #define NA_NUC_MAG_MHZT        7.62259384
  #define NA_VON_KLITZING        25812.807557
#elif (NA_NIST_CODATA_YEAR == 2010)
  #define NA_BOHR                927.400968e-26
  #define NA_BOHR_EVT            5.7883818066e-5
  #define NA_BOHR_HZT            13.99624555e9
  #define NA_BOHR_MTESLA         46.6864498
  #define NA_BOHR_KT             0.67171388
  #define NA_COND_QUANT          7.7480917346e-5
  #define NA_COND_QUANT_INV      12906.4037217
  #define NA_E_CHARGE            1.602176565e-19
  #define NA_E_CHARGE_H          2.417989348e14
  #define NA_JOSEPHSON           483597.870e9
  #define NA_MAG_FLUX_QUANT      2.067833758e-15
  #define NA_NUC_MAG             5.05078353e-27
  #define NA_NUC_MAG_EVT         3.1524512605e-8
  #define NA_NUC_MAG_MTESLA      2.542623527e-2
  #define NA_NUC_MAG_KT          3.6582682e-4
  #define NA_NUC_MAG_MHZT        7.62259357
  #define NA_VON_KLITZING        25812.807
#elif (NA_NIST_CODATA_YEAR == 2014)
  #define NA_BOHR                927.4009994e-26
  #define NA_BOHR_EVT            5.7883818012e-5
  #define NA_BOHR_HZT            13.996245042e9
  #define NA_BOHR_MTESLA         46.68644814
  #define NA_BOHR_KT             0.67171405
  #define NA_COND_QUANT          7.7480917310e-5
  #define NA_COND_QUANT_INV      12906.4037278
  #define NA_E_CHARGE            1.6021766208e-19
  #define NA_E_CHARGE_H          2.417989262e14
  #define NA_JOSEPHSON           483597.8525e9
  #define NA_MAG_FLUX_QUANT      2.067833831e-15
  #define NA_NUC_MAG             5.050783699e-27
  #define NA_NUC_MAG_EVT         3.1524512550e-8
  #define NA_NUC_MAG_MTESLA      2.542623432e-2
  #define NA_NUC_MAG_KT          3.6582690e-4
  #define NA_NUC_MAG_MHZT        7.622593285
  #define NA_VON_KLITZING        25812.8074555
#elif (NA_NIST_CODATA_YEAR == 2018)
  #define NA_BOHR                9.2740100783e-24
  #define NA_BOHR_EVT            5.7883818060e-5
  #define NA_BOHR_HZT            1.39962449361e10
  #define NA_BOHR_MTESLA         46.686447783 
  #define NA_BOHR_KT             0.67171381563 
  #define NA_COND_QUANT          7.748091729e-5
  #define NA_COND_QUANT_INV      12906.40372
  #define NA_E_CHARGE            1.602176634e-19
  #define NA_E_CHARGE_H          2.417989242e14
  #define NA_JOSEPHSON           483597.8484e9
  #define NA_MAG_FLUX_QUANT      2.067833848e-15
  #define NA_NUC_MAG             5.0507837461e-27
  #define NA_NUC_MAG_EVT         3.15245125844e-8
  #define NA_NUC_MAG_MTESLA      2.54262341353e-2
  #define NA_NUC_MAG_KT          3.6582677756e-4
  #define NA_NUC_MAG_MHZT        7.6225932291
  #define NA_VON_KLITZING        25812.80745
#endif

// Physico-chemical constants
// - ATOM_MASS          atomic mass constant, m_u in [kg]
// - ATOM_MASS_ENERGY   atomic mass constant energy equivalent, m_uc2 in [J]
// - ATOM_MASS_MEV      atomic mass constant energy equivalent in MeV, m_uc2
//                      in [MeV]
// - AVOGADRO           Avogadro constant, N_A or L in [mol-1]
// - BOLTZMANN          Boltzmann constant, k in [JK-1]
// - BOLTZMANN_EVK      Boltzmann constant in eV/K, k in [eVK-1]
// - BOLTZMANN_HZK      Boltzmann constant in Hz/K, k/h in [HzK-1]
// - BOLTZMANN_MK       Boltzmann constant in inverse meters per kelvin, k/hc
//                      in [m-1K-1]
// - FARADAY            Faraday constant, F in [Cmol-1]
// - FIRST_RAD          first radiation constant, c_1 in [Wm2]
// - FIRST_RAD_SPECT    first radiation constant for spectral radiance, c_1L
//                      in [Wm2sr-1]
// - SECOND_RAD         second radiation constant, c_2 in [mK]
//                      = PLANCK*LIGHT_SPEED_VAC/BOLTZMANN
// - LOSCHMIDT_100      Loschmidt constant (273.15 K, 100 kPa), n_0 in [m-3]
// - LOSCHMIDT_101      Loschmidt constant (273.15 K, 101.325 kPa), n_0 in [m-3]
// - MOL_GAS            molar gas constant, R in [Jmol-1K-1]
// - MOL_PLANCK         molar Planck constant, N_Ah in [Jsmol-1]
// - MOL_PLANCK_C       molar Planck constant times c, N_Ahc in [Jm*mol-1]
//                      Macro removed since 2018
// - MOL_IDEAL_GAS_100  molar volume of ideal gas (273.15 K, 100 kPa), V_m
//                      in [m3mol-1]
// - MOL_IDEAL_GAS_101  molar volume of ideal gas (273.15 K, 101.325 kPa), V_m
//                      in [m3mol-1]
// - SACKUR_TETRODE_100 Sackur-Tetrode constant (1 K, 100 kPa), S_0/R, no unit
// - SACKUR_TETRODE_101 Sackur-Tetrode constant (1 K, 101.325 kPa), S_0/R,
//                      no unit
// - STEF_BOLTZ         Stefan-Boltzmann constant, sigma in [Wm-2K-4]
// - WIEN_FREQ          Wien frequency displacement law constant, b prime
//                      in [HzK-1]
// - WIEN_WAVE          Wien wavelength displacement law constant, b in [mK]

#if (NA_NIST_CODATA_YEAR == 2006)
  #define NA_ATOM_MASS           1.660538782e-27
  #define NA_ATOM_MASS_ENERGY    1.492417830e-10
  #define NA_ATOM_MASS_MEV       931.494028
  #define NA_AVOGADRO            6.02214179e23
  #define NA_BOLTZMANN           1.3806504e-23
  #define NA_BOLTZMANN_EVK       8.617343e-5
  #define NA_BOLTZMANN_HZK       2.0836644e10
  #define NA_BOLTZMANN_MK        69.50356
  #define NA_FARADAY             96485.3399
  #define NA_FIRST_RAD           3.74177118e-16
  #define NA_FIRST_RAD_SPECT     1.191042759e-16
  #define NA_SECOND_RAD          1.4387752e-2
  #define NA_LOSCHMIDT_101       2.6867774e25
  #define NA_MOL_GAS             8.314472
  #define NA_MOL_PLANCK          3.9903126821e-10
  #define NA_MOL_PLANCK_C        0.11962656472
  #define NA_MOL_IDEAL_GAS_100   22.710981e-3
  #define NA_MOL_IDEAL_GAS_101   22.413996e-3
  #define NA_SACKUR_TETRODE_100  -1.1517047
  #define NA_SACKUR_TETRODE_101  -1.1648677
  #define NA_STEF_BOLTZ          5.670400e-8
  #define NA_WIEN_FREQ           5.878933e10
  #define NA_WIEN_WAVE           2.8977685e-3
#elif (NA_NIST_CODATA_YEAR == 2010)
  #define NA_ATOM_MASS           1.660538921e-27
  #define NA_ATOM_MASS_ENERGY    1.492417954e-10
  #define NA_ATOM_MASS_MEV       931.494061
  #define NA_AVOGADRO            6.02214129e23
  #define NA_BOLTZMANN           1.3806488e-23
  #define NA_BOLTZMANN_EVK       8.6173324e-5
  #define NA_BOLTZMANN_HZK       2.0836618e10
  #define NA_BOLTZMANN_MK        69.503476
  #define NA_FARADAY             96485.3365
  #define NA_FIRST_RAD           3.74177153e-16
  #define NA_FIRST_RAD_SPECT     1.191042869e-16
  #define NA_SECOND_RAD          1.4387770e-2
  #define NA_LOSCHMIDT_100       2.6516462e25
  #define NA_LOSCHMIDT_101       2.6867805e25
  #define NA_MOL_GAS             8.3144621
  #define NA_MOL_PLANCK          3.9903127176e-10
  #define NA_MOL_PLANCK_C        0.119626565779
  #define NA_MOL_IDEAL_GAS_100   22.710953e-3
  #define NA_MOL_IDEAL_GAS_101   22.413968e-3
  #define NA_SACKUR_TETRODE_100  -1.1517078
  #define NA_SACKUR_TETRODE_101  -1.1648708
  #define NA_STEF_BOLTZ          5.670373e-8
  #define NA_WIEN_FREQ           5.8789254e10
  #define NA_WIEN_WAVE           2.8977721e-3
#elif (NA_NIST_CODATA_YEAR == 2014)
  #define NA_ATOM_MASS           1.660539040e-27
  #define NA_ATOM_MASS_ENERGY    1.492418062e-10
  #define NA_ATOM_MASS_MEV       931.4940954
  #define NA_AVOGADRO            6.022140857e23
  #define NA_BOLTZMANN           1.38064852e-23
  #define NA_BOLTZMANN_EVK       8.6173303e-5
  #define NA_BOLTZMANN_HZK       2.0836612e10
  #define NA_BOLTZMANN_MK        69.503457
  #define NA_FARADAY             96485.33289
  #define NA_FIRST_RAD           3.741771790e-16
  #define NA_FIRST_RAD_SPECT     1.191042953e-16
  #define NA_SECOND_RAD          1.43877736e-2
  #define NA_LOSCHMIDT_100       2.6516467e25
  #define NA_LOSCHMIDT_101       2.6867811e25
  #define NA_MOL_GAS             8.3144598
  #define NA_MOL_PLANCK          3.9903127110e-10
  #define NA_MOL_PLANCK_C        0.119626565582
  #define NA_MOL_IDEAL_GAS_100   22.710947e-3
  #define NA_MOL_IDEAL_GAS_101   22.413962e-3
  #define NA_SACKUR_TETRODE_100  -1.1517084
  #define NA_SACKUR_TETRODE_101  -1.1648714
  #define NA_STEF_BOLTZ          5.670367e-8
  #define NA_WIEN_FREQ           5.8789238e10
  #define NA_WIEN_WAVE           2.8977729e-3
#elif (NA_NIST_CODATA_YEAR == 2018)
  #define NA_ATOM_MASS           1.66053906660e-27
  #define NA_ATOM_MASS_ENERGY    1.49241808560e-10
  #define NA_ATOM_MASS_MEV       931.49410242
  #define NA_AVOGADRO            6.02214076e23
  #define NA_BOLTZMANN           1.380649e-23
  #define NA_BOLTZMANN_EVK       8.617333262e-5
  #define NA_BOLTZMANN_HZK       2.083661912e10
  #define NA_BOLTZMANN_MK        69.50348004
  #define NA_FARADAY             96485.33212
  #define NA_FIRST_RAD           3.741771852e-16
  #define NA_FIRST_RAD_SPECT     1.191042972e-16
  #define NA_SECOND_RAD          1.438776877e-2
  #define NA_LOSCHMIDT_100       2.651645804e25
  #define NA_LOSCHMIDT_101       2.686780111e25
  #define NA_MOL_GAS             8.314462618
  #define NA_MOL_PLANCK          3.990312712e-10
  #define NA_MOL_IDEAL_GAS_100   22.71095464e-3
  #define NA_MOL_IDEAL_GAS_101   22.41396954e-3
  #define NA_SACKUR_TETRODE_100  -1.15170753706
  #define NA_SACKUR_TETRODE_101  -1.16487052358
  #define NA_STEF_BOLTZ          5.670374419e-8
  #define NA_WIEN_FREQ           5.878925757e10
  #define NA_WIEN_WAVE           2.897771955e-3
#endif

// Mathematical constants
// - PRIME_MERSENNE_2       Mersenne Prime 2^2 -1
// - PRIME_MERSENNE_3       Mersenne Prime 2^3 -1
// - PRIME_MERSENNE_5       Mersenne Prime 2^5 -1
// - PRIME_MERSENNE_7       Mersenne Prime 2^7 -1
// - PRIME_MERSENNE_13      Mersenne Prime 2^13-1
// - PRIME_MERSENNE_17      Mersenne Prime 2^17-1
// - PRIME_MERSENNE_19      Mersenne Prime 2^19-1
// - PRIME_MERSENNE_31      Mersenne Prime 2^31-1
// - PRIME_MERSENNE_61      Mersenne Prime 2^61-1
// - PRIME_FERMAT_1         Fermat Prime 2^1 +1
// - PRIME_FERMAT_2         Fermat Prime 2^2 +1
// - PRIME_FERMAT_4         Fermat Prime 2^4 +1
// - PRIME_FERMAT_8         Fermat Prime 2^8 +1
// - PRIME_FERMAT_16        Fermat Prime 2^16+1
// - PRIME_FERMAT_32        Fermat Prime 2^32+1
// - PRIME_BEFORE_2_16      Largest prime before 2^16
#define NA_PRIME_MERSENNE_2    3
#define NA_PRIME_MERSENNE_3    7
#define NA_PRIME_MERSENNE_5    31
#define NA_PRIME_MERSENNE_7    127
#define NA_PRIME_MERSENNE_13   8191
#define NA_PRIME_MERSENNE_17   131071
#define NA_PRIME_MERSENNE_19   524287
#define NA_PRIME_MERSENNE_31   2147483647
#define NA_PRIME_MERSENNE_61   2305843009213693951LL
#define NA_PRIME_FERMAT_0      3
#define NA_PRIME_FERMAT_2      5
#define NA_PRIME_FERMAT_4      17
#define NA_PRIME_FERMAT_8      257
#define NA_PRIME_FERMAT_16     65537
#define NA_PRIME_FERMAT_32     4294967297LL
#define NA_PRIME_BEFORE_2_16   65521


#ifdef __cplusplus
  } // extern "C"
#endif
#endif // NA_MATH_CONSTANTS_INCLUDED



// This is free and unencumbered software released into the public domain.

// Anyone is free to copy, modify, publish, use, compile, sell, or
// distribute this software, either in source code form or as a compiled
// binary, for any purpose, commercial or non-commercial, and by any
// means.

// In jurisdictions that recognize copyright laws, the author or authors
// of this software dedicate any and all copyright interest in the
// software to the public domain. We make this dedication for the benefit
// of the public at large and to the detriment of our heirs and
// successors. We intend this dedication to be an overt act of
// relinquishment in perpetuity of all present and future rights to this
// software under copyright law.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
// OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
// OTHER DEALINGS IN THE SOFTWARE.

// For more information, please refer to <http://unlicense.org/>
