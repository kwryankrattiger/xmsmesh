//------------------------------------------------------------------------------
/// \file
/// \brief
/// \copyright (C) Copyright Aquaveo 2018. Distributed under the xmsng
///  Software License, Version 1.0. (See accompanying file
///  LICENSE_1_0.txt or copy at http://www.aquaveo.com/xmsng/LICENSE_1_0.txt)
//------------------------------------------------------------------------------

//----- Included files ---------------------------------------------------------
#include <pybind11/pybind11.h>
#include <iostream>
#include <sstream>
#include <pybind11/numpy.h>
#include <boost/shared_ptr.hpp>

#include <xmsinterp/interpolate/InterpLinear.h>
#include <xmsmesh/meshing/MeMultiPolyMesherIo.h>

//----- Namespace declaration --------------------------------------------------
namespace py = pybind11;

//----- Python Interface -------------------------------------------------------
PYBIND11_DECLARE_HOLDER_TYPE(T, boost::shared_ptr<T>);

void initMeMultiPolyMesherIo(py::module &m) {
    py::class_<xms::MeMultiPolyMesherIo, boost::shared_ptr<xms::MeMultiPolyMesherIo>> polyMesherIo(m, "MeMultiPolyMesherIo");


    polyMesherIo.def(py::init<>());
    // ---------------------------------------------------------------------------
    // function: check_topology
    // ---------------------------------------------------------------------------
    const char* check_topology_doc = R"pydoc(
        Optional. If true, checks polygon input topology for errors.
    )pydoc";
    polyMesherIo.def_readwrite("check_topology", &xms::MeMultiPolyMesherIo::m_checkTopology,check_topology_doc);
    // ---------------------------------------------------------------------------
    // function: return_cell_polygons
    // ---------------------------------------------------------------------------
    const char* return_cell_polygons_doc = R"pydoc(
        If true, returns the polygon index of each cell.
    )pydoc";
    polyMesherIo.def_readwrite("return_cell_polygons", 
        &xms::MeMultiPolyMesherIo::m_returnCellPolygons, 
        return_cell_polygons_doc);
    // ---------------------------------------------------------------------------
    // function: points
    // ---------------------------------------------------------------------------
    const char* points_doc = R"pydoc(
        The points of the resulting mesh.
    )pydoc";
    polyMesherIo.def_property("points",
        [](xms::MeMultiPolyMesherIo &self) -> py::iterable {
            xms::VecPt3d &vec_pts = self.m_points;
            py::array_t<double, py::array::c_style> ret_points({(int)vec_pts.size(), 3});
            auto r = ret_points.mutable_unchecked<2>();
            int i = 0;
            for (ssize_t i = 0; i < r.shape(0); i++) {
                r(i, 0) = vec_pts[i].x;
                r(i, 1) = vec_pts[i].y;
                r(i, 2) = vec_pts[i].z;
            }
            return ret_points;
        },
        [](xms::MeMultiPolyMesherIo &self, py::iterable out_poly) {
                self.m_points.clear();
                self.m_points.reserve(py::len(out_poly));
                for (auto item : out_poly) {
                if(!py::isinstance<py::iterable>(item)) {
                    throw py::type_error("First arg must be a n-tuple of 3-tuples");
                }
                py::tuple tuple = item.cast<py::tuple>();
                if (py::len(tuple) != 3) {
                    throw py::type_error("Input points must be 3-tuples");
                } else {
                    xms::Pt3d point(tuple[0].cast<double>(), tuple[1].cast<double>(), tuple[2].cast<double>());
                    self.m_points.push_back(point);
                }
                }
        },points_doc);
    // ---------------------------------------------------------------------------
    // function: cells
    // ---------------------------------------------------------------------------
    const char* cells_doc = R"pydoc(
        The cells of the resulting mesh, as a stream.
    )pydoc";
    polyMesherIo.def_property("cells",
            [](xms::MeMultiPolyMesherIo &self) -> py::iterable {
                return py::array(self.m_cells.size(), self.m_cells.data());
            },
            [](xms::MeMultiPolyMesherIo &self, py::iterable cells) {
                 xms::VecInt &vecInt = self.m_cells;
                 vecInt.clear();
                 vecInt.reserve(py::len(cells));
                 for (auto item : cells) {
                    vecInt.push_back(item.cast<int>());
                 }
            },
            cells_doc);
    // ---------------------------------------------------------------------------
    // function: cell_polygons
    // ---------------------------------------------------------------------------
    const char* cell_polygons_doc = R"pydoc(
        Polygon index of each cell.
    )pydoc";
    polyMesherIo.def_property("cell_polygons",
            [](xms::MeMultiPolyMesherIo &self) -> py::iterable {
                return py::array(self.m_cellPolygons.size(), self.m_cellPolygons.data());
            },
            [](xms::MeMultiPolyMesherIo &self, py::iterable cell_polygons) {
                 xms::VecInt &vecInt = self.m_cellPolygons;
                 vecInt.clear();
                 vecInt.reserve(py::len(cell_polygons));
                 for (auto item : cell_polygons) {
                    vecInt.push_back(item.cast<int>());
                 }
            },cell_polygons_doc);
    // ---------------------------------------------------------------------------
    // function: poly_inputs
    // ---------------------------------------------------------------------------
    const char* poly_inputs_doc = R"pydoc(
        Required (but some data is optional). Inputs for each polygon.
    )pydoc";
    polyMesherIo.def_property("poly_inputs",
            [](xms::MeMultiPolyMesherIo &self) -> py::iterable {
                py::tuple ret_tuple(self.m_polys.size());
                for (int i = 0; i < self.m_polys.size(); i++) {
                    ret_tuple[i] = self.m_polys[i];
                }
                return ret_tuple;
            },
            [](xms::MeMultiPolyMesherIo &self, py::iterable polys) {
                 std::vector<xms::MePolyInput> &vecPolys = self.m_polys;
                 vecPolys.clear();
                 vecPolys.reserve(py::len(polys));
                 for (auto item : polys) {
                    vecPolys.push_back(item.cast<xms::MePolyInput>());
                 }
            },poly_inputs_doc);
    // ---------------------------------------------------------------------------
    // function: refine_points
    // ---------------------------------------------------------------------------
    const char* refine_points_doc = R"pydoc(
        Optional. Refine points.
    )pydoc";
    polyMesherIo.def_property("refine_points",
            [](xms::MeMultiPolyMesherIo &self) -> py::iterable {
                py::tuple ret_tuple(self.m_refPts.size());
                for (int i = 0; i < self.m_refPts.size(); i++) {
                    ret_tuple[i] = self.m_refPts[i];
                }
                return ret_tuple;
            },
            [](xms::MeMultiPolyMesherIo &self, py::iterable refine_points) {
                 std::vector<xms::MeRefinePoint> &vecRefinePoints = self.m_refPts;
                 vecRefinePoints.clear();
                 vecRefinePoints.reserve(py::len(refine_points));
                 for (auto item : refine_points) {
                    vecRefinePoints.push_back(item.cast<xms::MeRefinePoint>());
                 }
            },refine_points_doc);
}

void initMePolyInput(py::module &m) {
    py::class_<xms::MePolyInput, boost::shared_ptr<xms::MePolyInput>> polyInput(m, "MePolyInput");

    polyInput.def(py::init<>([](py::iterable out_poly, py::iterable inside_polys, double bias,
                           boost::shared_ptr<xms::InterpBase> &size_function,
                           py::iterable poly_corners, boost::shared_ptr<xms::InterpBase> &elev_function) {
            xms::VecPt3d vec_out_poly;
            for (auto item : out_poly) {
              if(!py::isinstance<py::iterable>(item)) {
                throw py::type_error("First arg (out_poly) must be a n-tuple of 3-tuples");
              }
              py::tuple tuple = item.cast<py::tuple>();
              if (py::len(tuple) != 3) {
                throw py::type_error("Input points must be 3-tuples");
              } else {
                xms::Pt3d point(tuple[0].cast<double>(), tuple[1].cast<double>(), tuple[2].cast<double>());
                vec_out_poly.push_back(point);
              }
            }
            xms::VecPt3d2d vec_inside_polys;
            vec_inside_polys.reserve(py::len(inside_polys));
            for (auto poly : inside_polys) {
                if (!py::isinstance<py::iterable>(poly)) {
                    throw py::type_error("Arg (inside_polys) must be an n-tuple of @n-tuples of 3-tuples");
                }
                xms::VecPt3d vec_poly;
                vec_poly.reserve(py::len(poly));
                for (auto p : poly) {
                    if (!py::isinstance<py::iterable>(poly)) {
                        throw py::type_error("Arg (inside_polys) must be an n-tuple of @n-tuples of 3-tuples");
                    }
                    py::tuple pt = p.cast<py::tuple>();
                    if (py::len(pt) != 3) {
                        throw py::type_error("Arg (inside_polys) must be an n-tuple of n-tuples of @3-tuples");
                    } else {
                        xms::Pt3d point(pt[0].cast<double>(), pt[1].cast<double>(), pt[2].cast<double>());
                        vec_poly.push_back(point);
                    }
                }
                vec_inside_polys.push_back(vec_poly);
            }
            xms::VecInt vec_poly_corners(py::len(poly_corners));
            int k = 0;
            for (auto item : poly_corners) {
              vec_poly_corners.at(k) = item.cast<int>();
              k++;
            }
            return new xms::MePolyInput(vec_out_poly, vec_inside_polys, bias, size_function,
                                        vec_poly_corners, elev_function);
        }));
    polyInput.def(py::init<>([]() {
            return new xms::MePolyInput();
    }));
    // ---------------------------------------------------------------------------
    // function: outside_poly
    // ---------------------------------------------------------------------------
    const char* outside_poly_doc = R"pydoc(
        Required. Outer polygons. Clockwise. 1st pt != last.
    )pydoc";
    polyInput.def_property("outside_poly",
            [](xms::MePolyInput &self) -> py::iterable {
                xms::VecPt3d &vec_pts = self.m_outPoly;
                py::array_t<double, py::array::c_style> ret_points({(int)vec_pts.size(), 3});
                auto r = ret_points.mutable_unchecked<2>();
                int i = 0;
                for (ssize_t i = 0; i < r.shape(0); i++) {
                  r(i, 0) = vec_pts[i].x;
                  r(i, 1) = vec_pts[i].y;
                  r(i, 2) = vec_pts[i].z;
                }
                return ret_points;
            },
            [](xms::MePolyInput &self, py::iterable out_poly) {
                 self.m_outPoly.clear();
                 self.m_outPoly.reserve(py::len(out_poly));
                 for (auto item : out_poly) {
                   if(!py::isinstance<py::iterable>(item)) {
                     throw py::type_error("First arg must be a n-tuple of 3-tuples");
                   }
                   py::tuple tuple = item.cast<py::tuple>();
                   if (py::len(tuple) != 3) {
                     throw py::type_error("Input points must be 3-tuples");
                   } else {
                     xms::Pt3d point(tuple[0].cast<double>(), tuple[1].cast<double>(), tuple[2].cast<double>());
                     self.m_outPoly.push_back(point);
                   }
                 }
            },outside_poly_doc);
    // ---------------------------------------------------------------------------
    // function: inside_polys
    // ---------------------------------------------------------------------------
    const char* inside_polys_doc = R"pydoc(
        Optional. Inner polygons (holes). Counter clockwise. 1st pt != last.
    )pydoc";
    polyInput.def_property("inside_polys",
            [](xms::MePolyInput &self) -> py::iterable {
                //xms::VecPt3d2d &vec_inside_polys = self.m_insidePolys;
                py::tuple py_inside_polys(self.m_insidePolys.size());
                for (int i = 0; i < self.m_insidePolys.size(); i++) {
                    auto inside_poly = self.m_insidePolys[i];
                    py::array_t<double, py::array::c_style> poly_points({(int)inside_poly.size(), 3});
                    auto r = poly_points.mutable_unchecked<2>();
                    for (ssize_t i = 0; i < r.shape(0); i++) {
                      r(i, 0) = inside_poly[i].x;
                      r(i, 1) = inside_poly[i].y;
                      r(i, 2) = inside_poly[i].z;
                    }
                    py_inside_polys[i] = poly_points;
                }
                return py_inside_polys;
            },
            [](xms::MePolyInput &self, py::iterable inside_polys) {
                self.m_insidePolys.clear();
                self.m_insidePolys.reserve(py::len(inside_polys));
                for (auto poly : inside_polys) {
                    if (!py::isinstance<py::iterable>(poly)) {
                        throw py::type_error("Arg (inside_polys) must be an n-tuple of @n-tuples of 3-tuples");
                    }
                    xms::VecPt3d vec_poly;
                    vec_poly.reserve(py::len(poly));
                    for (auto p : poly) {
                        if (!py::isinstance<py::iterable>(poly)) {
                            throw py::type_error("Arg (inside_polys) must be an n-tuple of @n-tuples of 3-tuples");
                        }
                        py::tuple pt = p.cast<py::tuple>();
                        if (py::len(pt) != 3) {
                            throw py::type_error("Arg (inside_polys) must be an n-tuple of n-tuples of @3-tuples");
                        } else {
                            xms::Pt3d point(pt[0].cast<double>(), pt[1].cast<double>(), pt[2].cast<double>());
                            vec_poly.push_back(point);
                        }
                    }
                    self.m_insidePolys.push_back(vec_poly);
                }
            },inside_polys_doc);
    // -------------------------------------------------------------------------
    // function: poly_corners
    // -------------------------------------------------------------------------
    const char* poly_corners_doc = R"pydoc(
        Optional. Corner nodes for creating meshes using the patch algorithm. 
        3 per outer poly (not 4 - outer poly index point [0] is assumed to be 
        a corner
    )pydoc";
    polyInput.def_property("poly_corners",
            [](xms::MePolyInput &self) -> py::iterable {
                return py::array(self.m_polyCorners.size(), self.m_polyCorners.data());
            },
            [](xms::MePolyInput &self, py::iterable poly_corners) {
                 self.m_polyCorners.clear();
                 for (auto item : poly_corners) {
                    self.m_polyCorners.push_back(item.cast<int>());
                 }
            },poly_corners_doc);
    // -------------------------------------------------------------------------
    // function: bound_pts_to_remove
    // -------------------------------------------------------------------------
    const char* bound_pts_to_remove_doc = R"pydoc(
        Optional. Outer boundary locations to remove after the paving 
        process. Used by the ugAutoCorrectCells class.
    )pydoc";
    polyInput.def_property("bound_pts_to_remove",
            [](xms::MePolyInput &self) -> py::iterable {
                xms::VecPt3d &vec_pts = self.m_boundPtsToRemove;
                py::array_t<double, py::array::c_style> ret_points({(int)vec_pts.size(), 3});
                auto r = ret_points.mutable_unchecked<2>();
                int i = 0;
                for (ssize_t i = 0; i < r.shape(0); i++) {
                  r(i, 0) = vec_pts[i].x;
                  r(i, 1) = vec_pts[i].y;
                  r(i, 2) = vec_pts[i].z;
                }
                return ret_points;
            },
            [](xms::MePolyInput &self, py::iterable out_poly) {
                 self.m_boundPtsToRemove.clear();
                 self.m_boundPtsToRemove.reserve(py::len(out_poly));
                 for (auto item : out_poly) {
                   if(!py::isinstance<py::iterable>(item)) {
                     throw py::type_error("First arg must be a n-tuple of 3-tuples");
                   }
                   py::tuple tuple = item.cast<py::tuple>();
                   if (py::len(tuple) != 3) {
                     throw py::type_error("Input points must be 3-tuples");
                   } else {
                     xms::Pt3d point(tuple[0].cast<double>(), tuple[1].cast<double>(), tuple[2].cast<double>());
                     self.m_boundPtsToRemove.push_back(point);
                   }
                 }
            },bound_pts_to_remove_doc);
    // -------------------------------------------------------------------------
    // function: bias
    // -------------------------------------------------------------------------
    const char* bias_doc = R"pydoc(
       Optional. Factor for transitioning between areas of high refinement to 
       less refinement.
    )pydoc";
    polyInput.def_readwrite("bias", &xms::MePolyInput::m_bias, bias_doc);
    // -------------------------------------------------------------------------
    // function: size_function
    // -------------------------------------------------------------------------
    const char* size_function_doc = R"pydoc(
            Optional. Size function for scalar paving.
    )pydoc";
    polyInput.def_readwrite("size_function", &xms::MePolyInput::m_sizeFunction,
            size_function_doc);
    // -------------------------------------------------------------------------
    // function: elev_function
    // -------------------------------------------------------------------------
    const char* elev_function_doc = R"pydoc(
        Optional. Elevation function for interpolating z coordinate of mesh 
        points.
    )pydoc";
    polyInput.def_readwrite("elev_function", &xms::MePolyInput::m_elevFunction,
        elev_function_doc);
    // -------------------------------------------------------------------------
    // function: const_size_function
    // -------------------------------------------------------------------------
    const char* const_size_function_doc = R"pydoc(
        Optional. Transition factor for constant size function.
    )pydoc";
    polyInput.def_readwrite("const_size_function", &xms::MePolyInput::m_constSizeFunction,
        const_size_function_doc);
    // -------------------------------------------------------------------------
    // function: const_size_bias
    // -------------------------------------------------------------------------
    const char* const_size_bias_doc = R"pydoc(
        Optional. Transition factor for constant size function.
    )pydoc";
    polyInput.def_readwrite("const_size_bias", &xms::MePolyInput::m_constSizeBias,
        const_size_bias_doc);
    // -------------------------------------------------------------------------
    // function: remove_internal_four_triangle_pts
    // -------------------------------------------------------------------------
    const char* remove_internal_four_triangle_pts_doc = R"pydoc(
        Optional. Remove internal points that are only connected to 4 cells. 
        Used by the ugAutoCorrectCells class
    )pydoc";
    polyInput.def_readwrite("remove_internal_four_triangle_pts", &xms::MePolyInput::m_removeInternalFourTrianglePts,
            remove_internal_four_triangle_pts_doc);
    // -------------------------------------------------------------------------
    // function: poly_id
    // -------------------------------------------------------------------------
    const char* poly_id_doc = R"pydoc(
        Optional. Set when needed. Can be useful for classes who need an ID.
    )pydoc";
    polyInput.def_readwrite("poly_id", &xms::MePolyInput::m_polyId,
        poly_id_doc);
    // -------------------------------------------------------------------------
    // function: seed_points
    // -------------------------------------------------------------------------
    const char* seed_points_doc = R"pydoc(
        Optional array of seed points. If the user has some methodology for 
        creating points inside the polygon then those points can be specified 
        here. If these points are specified then the paving is not performed.
        These points will not be used if the meshing option is patch.
    )pydoc";
    polyInput.def_property("seed_points",
            [](xms::MePolyInput &self) -> py::iterable {
                xms::VecPt3d &vec_pts = self.m_seedPoints;
                py::array_t<double, py::array::c_style> ret_points({(int)vec_pts.size(), 3});
                auto r = ret_points.mutable_unchecked<2>();
                int i = 0;
                for (ssize_t i = 0; i < r.shape(0); i++) {
                  r(i, 0) = vec_pts[i].x;
                  r(i, 1) = vec_pts[i].y;
                  r(i, 2) = vec_pts[i].z;
                }
                return ret_points;
            },
            [](xms::MePolyInput &self, py::iterable seed_points) {
                 self.m_seedPoints.clear();
                 self.m_seedPoints.reserve(py::len(seed_points));
                 for (auto item : seed_points) {
                   if(!py::isinstance<py::iterable>(item)) {
                     throw py::type_error("First arg must be a n-tuple of 3-tuples");
                   }
                   py::tuple tuple = item.cast<py::tuple>();
                   if (py::len(tuple) != 3) {
                     throw py::type_error("Input points must be 3-tuples");
                   } else {
                     xms::Pt3d point(tuple[0].cast<double>(), tuple[1].cast<double>(), tuple[2].cast<double>());
                     self.m_seedPoints.push_back(point);
                   }
                 }                
            }, seed_points_doc);
    // -------------------------------------------------------------------------
    // function: __str__
    // -------------------------------------------------------------------------
    const char* __str__doc = R"pydoc(
        outputs contents as string

        Returns:
            str: Contents as a string
    )pydoc";
    polyInput.def("__str__", [](xms::MePolyInput &self) {
             std::string szf = self.m_sizeFunction == nullptr ? "none" : self.m_sizeFunction->ToString();
             std::string elevf = self.m_elevFunction == nullptr ? "none" : self.m_elevFunction->ToString();
             std::stringstream ss;
             ss <<  "bias: " << self.m_bias << std::endl <<
                    "size_func: " << szf << std::endl <<
                    "elev_func: " << elevf << std::endl <<
                    "const_size_function: " << self.m_constSizeFunction << std::endl <<
                    "const_size_bias: " << self.m_constSizeBias << std::endl <<
                    "outside_poly size: " << self.m_outPoly.size() << std::endl <<
                    "inside_polys size: " << self.m_insidePolys.size() << std::endl;
             return ss.str();
        },__str__doc)
    ;
}

void initMeRefinePoint(py::module &m) {
    py::class_<xms::MeRefinePoint, boost::shared_ptr<xms::MeRefinePoint>> refinePoint(m, "MeRefinePoint");

    refinePoint.def(py::init<>([](py::tuple pt, double size, bool create_mesh_point) {
            if(py::len(pt) != 3) {
                throw py::type_error("Input point should be a 3-tuple");
            }
            xms::Pt3d point(pt[0].cast<double>(), pt[1].cast<double>(), pt[2].cast<double>());
            return new xms::MeRefinePoint(point, size, create_mesh_point);
        }));
    // -------------------------------------------------------------------------
    // function: point
    // -------------------------------------------------------------------------
    const char* point_doc = R"pydoc(
        Location of refine point or hard points. Hard points are points that 
        must be included in the final mesh but have no user specified size 
        associated with them

    )pydoc";
    refinePoint.def_property("point",
          [](xms::MeRefinePoint &self) -> py::tuple {
            return py::make_tuple(self.m_pt.x, self.m_pt.y, self.m_pt.z);
          },
          [](xms::MeRefinePoint &self, py::tuple pt) {
            if(py::len(pt) != 3) {
              throw py::type_error("Input point should be a 3-tuple");
            } else {
              xms::Pt3d point(pt[0].cast<double>(), pt[1].cast<double>(), pt[2].cast<double>());
              self.m_pt = point;
            }
          },point_doc);
    // -------------------------------------------------------------------------
    // function: size
    // -------------------------------------------------------------------------
    const char* size_doc = R"pydoc(
        Element size at the refine point. A negative value indicates a hard 
        point.
    )pydoc";
    refinePoint.def_readwrite("size", &xms::MeRefinePoint::m_size,size_doc);
    // -------------------------------------------------------------------------
    // function: create_mesh_point
    // -------------------------------------------------------------------------
    const char* create_mesh_point_doc = R"pydoc(
        Should a mesh node/point be created at the refine point.
    )pydoc";
    refinePoint.def_readwrite("create_mesh_point", &xms::MeRefinePoint::m_createMeshPoint,
      create_mesh_point_doc);
    // -------------------------------------------------------------------------
    // function: __str__
    // -------------------------------------------------------------------------
    const char* __str___doc = R"pydoc(
        Returns contents as string

        Returns:
            str: contents as a string
    )pydoc";
    refinePoint.def("__str__", [](xms::MeRefinePoint &self) {
            std::stringstream ss;
            ss << "point: (" << self.m_pt << ")" <<  std::endl <<
                   "size: " << self.m_size <<  std::endl <<
                   "create_mesh_point: " << self.m_createMeshPoint << std::endl;
            return ss.str();
        }, __str___doc);
}
