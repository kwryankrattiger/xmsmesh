#pragma once
//------------------------------------------------------------------------------
/// \file
/// \ingroup meshing
/// \copyright (C) Copyright Aquaveo 2018. Distributed under FreeBSD License
/// (See accompanying file LICENSE or https://aqaveo.com/bsd/license.txt)
//------------------------------------------------------------------------------

//----- Included files ---------------------------------------------------------

// 3. Standard library headers

// 4. External library headers

#include <boost/shared_ptr.hpp>

// 5. Shared code headers

#include <xmscore/misc/base_macros.h> // for XM_DISALLOW_COPY_AND_ASSIGN

//----- Forward declarations ---------------------------------------------------

//----- Namespace declaration --------------------------------------------------

namespace xms
{
class MeMultiPolyMesherIo;

//----- Constants / Enumerations -----------------------------------------------

//----- Structs / Classes ------------------------------------------------------

//----- Function prototypes ----------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
class MeMultiPolyMesher
{
public:
  static boost::shared_ptr<MeMultiPolyMesher> New();
  /// \cond

  virtual bool MeshIt(MeMultiPolyMesherIo& a_io) = 0;
  virtual void CheckForIntersections(const MeMultiPolyMesherIo& a_io,
                                     std::string& a_errors) const = 0;
  virtual ~MeMultiPolyMesher() {}

  /// \endcond
protected:
  MeMultiPolyMesher() {}

private:
  XM_DISALLOW_COPY_AND_ASSIGN(MeMultiPolyMesher)
}; // MeMultiPolyMesher

} // namespace xms
