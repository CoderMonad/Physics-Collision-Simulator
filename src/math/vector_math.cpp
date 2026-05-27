/*****************************************************************************/
#include <cassert>
#include "vector_math.h"
/*****************************************************************************/
/////////////////////////////////////////////////
/// @file vector_math.h
/// @brief Holds all of the functions for math with vectors
/////////////////////////////////////////////////
/*****************************************************************************/
double math::getVectorLength(dblXY v1) {
  /////////////////////////////////////////////////
  /// @brief Gets length of a vector
  ///
  /// @param v1 = vector
  /// @return length of vector
  ///
  /////////////////////////////////////////////////

  return sqrt(pow(v1.x,2) + pow(v1.y,2));
}
/*****************************************************************************/
double math::dotProduct(dblXY v1,dblXY v2) {
  /////////////////////////////////////////////////
  /// @brief Gets dot product of two vectors
  ///
  /// @param v1 = vector 1
  /// @param v2 = vector 2
  /// @return value of dot product
  ///
  /////////////////////////////////////////////////
  return (v1.x * v2.x) + (v1.y * v2.y);
}
/*****************************************************************************/
double math::crossProduct(dblXY v1,dblXY v2){
  /////////////////////////////////////////////////
  /// @brief Gets cross product of two vectors. Since vectors are only 2D,
  ///        their cross product will only have a Z component
  ///
  /// @param v1 = vector 1
  /// @param v2 = vector 2
  /// @return Z-value of cross product
  ///
  /////////////////////////////////////////////////
  return (v1.x * v2.y) - (v2.x * v1.y);
}
/*****************************************************************************/
dblXY math::vectorDiv(dblXY v1,double ele){
  assert(ele != 0.0);
  return {v1.x / ele,v1.y / ele};
}
/*****************************************************************************/
dblXY math::vectorDiv(dblXY v1,dblXY v2){
  assert(v2.x != 0.0 && v2.y != 0.0);
  v1.x /= v2.x;
  v1.y /= v2.y;
  return v1;
}
/*****************************************************************************/
dblXY math::vectorAdd(dblXY v1,double ele){
  v1.x += ele;
  v1.y += ele;
  return v1;
}
/*****************************************************************************/
dblXY math::vectorAdd(dblXY v1,dblXY v2){
  v1.x += v2.x;
  v1.y += v2.y;
  return v1;
}
/*****************************************************************************/
dblXY math::vectorSub(dblXY v1,double ele){
  v1.x -= ele;
  v1.y -= ele;
  return v1;
}
/*****************************************************************************/
dblXY math::vectorSub(dblXY v1,dblXY v2){
  v1.x -= v2.x;
  v1.y -= v2.y;
  return v1;
}
/*****************************************************************************/
dblXY math::vectorSub(LOC v1,LOC v2){
  dblXY temp;
  temp.x = (double) (v1.x - v2.x);
  temp.y = (double) (v1.y - v2.y);
  return temp;
}
/*****************************************************************************/
dblXY math::vectorMul(dblXY v1,double ele){
  v1.x *= ele;
  v1.y *= ele;
  return v1;
}
/*****************************************************************************/
dblXY math::vectorMul(dblXY v1,dblXY v2){
  v1.x *= v2.x;
  v1.y *= v2.y;
  return v1;
}
/*****************************************************************************/

