#pragma once
//------------------------------------------------------------------------------
/// \file
/// \brief Utilities related to a VTK unstructured grid (from
///        shared1\UGridUtils.cpp)
/// \ingroup ugrid
/// \copyright (C) Copyright Aquaveo 2018. Distributed under FreeBSD License
/// (See accompanying file LICENSE or https://aqaveo.com/bsd/license.txt)
//------------------------------------------------------------------------------

//----- Included files ---------------------------------------------------------
#include <string>
#include <vector>

#include <xmscore/stl/set.h>
#include <xmscore/stl/vector.h>
#include <xmscore/misc/boost_defines.h> // for BSHP
#include <xmscore/misc/DynBitset.h>     // for DynBitset

//----- Forward declarations ---------------------------------------------------
class vtkUnstructuredGrid;

//----- Namespace declaration --------------------------------------------------
namespace xms
{
//----- Forward declarations ---------------------------------------------------
class TrTin;

//----- Constants / Enumerations -----------------------------------------------

//----- Structs / Classes ------------------------------------------------------

//----- Function prototypes ----------------------------------------------------
void meSizeFunctionFromDepth(const VecDbl& a_depths,
                             VecDbl& a_size,
                             double a_minSize,
                             double a_maxSize);
void meSmoothSizeFunction(BSHP<TrTin> a_tin,
                          const VecFlt& a_sizes,
                          double a_sizeRatio,
                          double a_minSize,
                          int a_anchorType,
                          const DynBitset& a_ptFlags,
                          VecFlt& a_smoothSize);
void meSmoothElevBySlope(BSHP<TrTin> a_tin,
                         const VecFlt& a_sizes,
                         double a_maxSize,
                         int a_anchorType,
                         const DynBitset& a_ptFlags,
                         VecFlt& a_smoothSize);
void meModifyMessageWithPolygonId(int a_polyId, std::string& a_msg);
} // namespace xms
