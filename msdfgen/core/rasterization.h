
#pragma once

#include "Vector2.h"
#include "Scanline.h"
#include "Shape.h"
#include "BitmapRef.hpp"

namespace msdfgen {

/// Rasterizes the shape into a monochrome bitmap.
void rasterize(const BitmapRef<float, 1> &output, const Shape &shape, const Vector2 &scale, const Vector2 &translate, FillRule fillRule = FILL_NONZERO);
/// Fixes the sign of the input signed distance field, so that it matches the shape's rasterized fill.
void distanceSignCorrection(const BitmapRef<float, 1> &sdf, const Shape &shape, const Vector2 &scale, const Vector2 &translate, FillRule fillRule = FILL_NONZERO);
void distanceSignCorrection(const BitmapRef<float, 3> &sdf, const Shape &shape, const Vector2 &scale, const Vector2 &translate, FillRule fillRule = FILL_NONZERO);

}
