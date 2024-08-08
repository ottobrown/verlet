#pragma once

#include "raymath.h"

static const Vector2 V2NULL = {0.0f, 0.0f};

/// Add two vectors
#define v2add(v, u) Vector2Add(v, u)
/// Subtract two vectors
#define v2sub(v, u) Vector2Subtract(v, u)
/// Dot product of two vectors
#define v2dot(v, u) Vector2DotProduct(v, u)
/// Scale a vector
#define v2scl(v, a) Vector2Scale(v, a)
/// Get length of a vector
#define v2len(v) Vector2Length(v)
/// Get squared length of a vector
#define v2lsq(v) Vector2LengthSqr(v)
/// Get distance between two vectors
#define v2dst(v, u) Vector2Distance(v, u)
/// Get squared distance between two vectors
#define v2dsq(v, u) Vector2Distance(v, u)
/// Get the norm of a vector
#define v2nrm(v) Vector2Normalize(v)
