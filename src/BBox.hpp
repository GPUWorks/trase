/*

Copyright (c) 2005-2016, University of Oxford.
All rights reserved.

University of Oxford means the Chancellor, Masters and Scholars of the
University of Oxford, having an administrative office at Wellington
Square, Oxford OX1 2JD, UK.

This file is part of trase.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
 * Redistributions of source code must retain the above copyright notice,
   this list of conditions and the following disclaimer.
 * Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.
 * Neither the name of the University of Oxford nor the names of its
   contributors may be used to endorse or promote products derived from this
   software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/

#ifndef BBOX_H_
#define BBOX_H_

#include "Vector.hpp"
#include <algorithm>
#include <limits>

namespace trase {

///
/// @brief Contains the minimum and maximum extents of a hypercube in @p D
/// dimensional space
///
/// @tparam D the number of spatial dimensions
///
template <typename T, int N> struct bbox {
  typedef Vector<T, N> vector_t;

  ///
  /// @brief minimum point in the box (i.e. lower left corner for D=2)
  ///
  vector_t bmin;

  ///
  /// @brief maximum point in the box (i.e. upper right corner for D=2)
  ///
  vector_t bmax;

  inline bbox()
      : bmin(vector_t::Constant(std::numeric_limits<T>::max())),
        bmax(vector_t::Constant(-std::numeric_limits<T>::max())) {}

  inline bbox(const vector_t &p)
      : bmin(p), bmax(p + std::numeric_limits<double>::epsilon()) {}

  inline bbox(const vector_t &min, const vector_t &max)
      : bmin(min), bmax(max) {}

  inline bbox(const double *min, const double *max) {
    for (int i = 0; i < N; ++i) {
      bmin[i] = min[i];
      bmax[i] = max[i];
    }
  }

  vector_t delta() const { return bmax - bmin; }
  const vector_t &min() const { return bmin; }
  const vector_t &max() const { return bmax; }

  ///
  /// @return increase the bounding box to cover both boxes
  ///
  inline bbox &operator+=(const bbox &arg) {
    for (int i = 0; i < N; ++i) {
      bmin[i] = std::min(bmin[i], arg.bmin[i]);
      bmax[i] = std::max(bmax[i], arg.bmax[i]);
    }
    return *this;
  }

  ///
  /// @return the bounding box covering both input boxes
  ///
  inline bbox operator+(const bbox &arg) {
    bbox bounds = *this;
    bounds += arg;
    return bounds;
  }

  ///
  /// @return translate the bounding box
  ///
  inline bbox &operator+=(const vector_t &arg) {
    bmin += arg;
    bmax += arg;
    return *this;
  }

  ///
  /// @return translate the bounding box
  ///
  inline bbox operator+(const vector_t &arg) {
    bbox bounds = *this;
    bounds += arg;
    return bounds;
  }

  ///
  /// @return scale the bounding box
  ///
  inline bbox &operator*=(const vector_t &arg) {
    bmin = 0.5 * ((bmax * (1 - arg) + bmin * (1 + arg)));
    bmax = 0.5 * ((bmax * (1 + arg) + bmin * (1 - arg)));
    return *this;
  }

  ///
  /// @return scale the bounding box
  ///
  inline bbox operator*(const vector_t &arg) {
    bbox bounds = *this;
    bounds *= arg;
    return bounds;
  }

  ///
  /// @return true if lhs box is within rhs box
  ///
  inline bool operator<(const bbox &arg) {
    bbox bounds;
    bool within = true;
    for (int i = 0; i < N; ++i) {
      within |= bmin[i] >= arg.bmin[i];
      within |= bmax[i] < arg.bmax[i];
    }
    return within;
  }

  ///
  /// @return true if lhs box is the same or within rhs box
  ///
  inline bool operator<=(const bbox &arg) {
    bbox bounds;
    bool within = true;
    for (int i = 0; i < N; ++i) {
      within |= bmin[i] >= arg.bmin[i];
      within |= bmax[i] <= arg.bmax[i];
    }
    return within;
  }

  ///
  /// @return true if box has no volume
  ///
  inline bool is_empty() {
    for (int i = 0; i < N; ++i) {
      if (bmax[i] < bmin[i] + 3 * std::numeric_limits<double>::epsilon())
        return true;
    }
    return false;
  }
};

///
/// @brief print bbox to a stream
///
/// @tparam N the number of spatial dimensions
/// @param out the stream
/// @param b the box to print
///
template <typename T, int N>
std::ostream &operator<<(std::ostream &out, const bbox<T, N> &b) {
  return out << "bbox(" << b.bmin << "<->" << b.bmax << ")";
}

typedef bbox<float, 2> bfloat2_t;
typedef bbox<float, 1> bfloat1_t;

} // namespace trase

#endif