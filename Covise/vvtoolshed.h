//****************************************************************************
// Project Affiliation: Virvo (Virtual Reality Volume Renderer)
// Copyright:           (c) 2002 Juergen Schulze-Doebold. All rights reserved.
// Author's E-Mail:     schulze@hlrs.de
// Institution:         University of Stuttgart, Supercomputing Center (HLRS)
//****************************************************************************

#ifndef _VVTOOLSHED_H_
#define _VVTOOLSHED_H_

#include <stdio.h>

//============================================================================
// Constant Definitions
//============================================================================

const float TS_PI = 3.1415926535897932384626433832795028841971693993751058f; ///< compiler independent definition for pi

//============================================================================
// Type Declarations
//============================================================================

typedef unsigned char   uchar;      ///< abbreviation for unsigned char
typedef unsigned short  ushort;     ///< abbreviation for unsigned short
typedef unsigned int    uint;       ///< abbreviation for unsigned int
typedef unsigned long   ulong;      ///< abbreviation for unsigned long
typedef signed   char   schar;      ///< abbreviation for signed char
typedef signed   short  sshort;     ///< abbreviation for signed short
typedef signed   int    sint;       ///< abbreviation for signed int
typedef signed   long   slong;      ///< abbreviation for signed long

//============================================================================
// Function Templates
//============================================================================

/// @return the maximum of two values
template <class C> inline C ts_max(const C a, const C b)   
{
  return (a < b) ? b : a;
}

/// @return the maximum of three values
template <class C> inline C ts_max(const C a, const C b, const C c)   
{
  return ts_max(a, ts_max(b, c));
}

/// @return the minimum of two values
template <class C> inline C ts_min(const C a, const C b)
{
  return (a > b) ? b : a;
}

/// @return the minimum of three values
template <class C> inline C ts_min(const C a, const C b, const C c)
{
  return ts_min(a, ts_min(b, c));
}

/// @return the absolute value of a value
template <class C> inline C ts_abs(const C a)        
{
  return (a < 0) ? (-a) : a;
}

/// @return the sign (-1 or 1) of a value
template <class C> inline C ts_sgn(const C a)
{
  return (a < 0) ? (-1) : 1;
}

/// @return the sign or zero(-1, 0, 1)
template <class C> inline C ts_zsgn(const C a)
{
  return (a < 0) ? (-1) : (a > 0 ? 1 : 0);
}

/// @return the result of value a clamped between left and right
template <class C> inline C ts_clamp(const C a, const C left, const C right) 
{
  if (a < left)  return left;
  if (a > right) return right;
  return a;
}

/// Swaps the values a and b
template <class C> inline void ts_swap(C a, C b)
{
  C bak = a;
  a     = b;
  b     = bak;
}

/// @param a a value
/// @return the square of a
template <class C> inline C ts_sqr(C a)
{
  return a * a;
}


//============================================================================
// Class Definitions
//============================================================================

/** Collection of miscellaneous tools. 
    Consists of static helper functions which are project independent.
    @author Juergen Schulze-Doebold
    
    <B>Terminology for extraction functions:</B><BR>
    Example: c: \ test \ vfview.exe
    <UL>
      <LI>Pathname  = c: \ test \ vfview.exe
      <LI>Dirname   = c: \ test \ 
      <LI>Extension = exe
      <LI>Filename  = vfview.exe
      <LI>Basename  = vfview
    </UL>
*/
class vvToolshed
{
  private:
    static int progressSteps;     ///< total number of progress steps

  public:
    enum EndianType     /// endianness
    {
      LITTLE_END,       ///< little endian: low-order byte is stored first
      BIG_END           ///< big endian: hight-order byte is stored first
    };

    static int     strCompare(const char*, const char*);
    static int     strCompare(const char*, const char*, int n);
    static bool    isSuffix(const char*, const char*);
    static void    HSBtoRGB(float*, float*, float*);
    static void    HSBtoRGB(float, float, float, float*, float*, float*);
    static void    RGBtoHSB(float*, float*, float*);
    static void    RGBtoHSB(float, float, float, float*, float*, float*);
    static void    strcpyTail(char*, const char*, char);
    static void    strcpyHead(char*, const char*, char);
    static void    strTrim(char*);
    static void    extractFilename(char*, const char*);
    static void    extractDirname(char*, const char*);
    static void    extractExtension(char*, const char*);
    static void    extractBasename(char*, const char*);
    static void    extractBasePath(char*, const char*);
    static void    replaceExtension(char*, const char*, const char*);
    static bool    increaseFilename(char*);
    static void    draw3DLine(int, int, int, int, int, int, uchar, uchar*, int, int, int);
    static void    draw2DLine(int, int, int, int, uint, uchar*, int, int, int);
    static int     getTextureSize(int);
    static bool    isFile(const char*);
    static long    getFileSize(const char*);
    static void    getMinMax(const float*, int, float*, float*);
    static void    getMinMax(const uchar*, int, int*, int*);
    static void    getMinMax16bitBE(const uchar*, int, int*, int*);
    static void    getMinMaxAlpha(const uchar*, int, int*, int*);
    static void    getMinMaxIgnore(const float*, int, float, float*, float*);
    static void    convertUChar2Float(const uchar*, float*, int);
    static void    convertFloat2UChar(const float*, uchar*, int);
    static void    convertFloat2UCharClamp(const float*, uchar*, int, float, float);
    static void    convertFloat2ShortClamp(const float*, uchar*, int, float, float);
    static void    convertFloat2UCharClampZero(const float*, uchar*, int, float, float, float);
    static int     getLargestPrimeFactor(int);
    static int     round(float);
    static void    initProgress(int);
    static void    printProgress(int);
    static int     encodeRLE(uchar*, uchar*, int, int, int);
    static int     decodeRLE(uchar*, uchar*, int, int, int);
    static int     encodeRLEFast(uchar*, uchar*, int, int);
    static int     decodeRLEFast(uchar*, uchar*, int, int);
    static int     getNumProcessors();
    static void    makeColorBoardTexture(int, int, float, uchar*);
    static void    convertXY2HS(float, float, float*, float*);
    static void    convertHS2XY(float, float, float*, float*);
    static uchar   read8(FILE*);
    static int     write8(FILE*, uchar);
    static ushort  read16BE(FILE*);
    static ushort  read16LE(FILE*);
    static int     write16BE(FILE*, ushort);
    static int     write16LE(FILE*, ushort);
    static ulong   read32BE(FILE*);
    static ulong   read32LE(FILE*);
    static int     write32BE(FILE*, ulong);
    static int     write32LE(FILE*, ulong);
    static float   readFloat(FILE*);
    static int     writeFloat(FILE*, float);
    static uchar   read8(uchar*);
    static int     write8(uchar*, uchar);
    static ushort  read16BE(uchar*);
    static ushort  read16LE(uchar*);
    static int     write16BE(uchar*, ushort);
    static int     write16LE(uchar*, ushort);
    static ulong   read32BE(uchar*);
    static ulong   read32LE(uchar*);
    static int     write32BE(uchar*, ulong);
    static int     write32LE(uchar*, ulong);
    static float   readFloat(uchar*);
    static int     writeFloat(uchar*, float);
    static void    makeArraySystemIndependent(int, float*);
    static void    makeArraySystemDependent(int, float*);
    static EndianType getEndianness();
    static void    sleep(int);
};

#endif

//============================================================================
// End of File
//============================================================================
