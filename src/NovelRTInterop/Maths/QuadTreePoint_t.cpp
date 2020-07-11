// Copyright © Matt Jones and Contributors. Licensed under the MIT Licence (MIT). See LICENCE.md in the repository root for more information.
#include "NovelRTInterop/Maths/QuadTreePoint_t.h"
#include "NovelRT.h"

#ifdef __cplusplus
using namespace NovelRT;
extern "C" {
#endif

  QuadTreePoint_t QuadTreePoint_create(GeoVector2F_t& position)
  {
    auto pos = reinterpret_cast<Maths::GeoVector2<float>&>(position);
    auto handle = reinterpret_cast<QuadTreePoint_t>(new Maths::QuadTreePoint(pos));
    return handle;
  }

  QuadTreePoint_t QuadTreePoint_createFromFloat(float x, float y)
  {
    auto handle = reinterpret_cast<QuadTreePoint_t>(new Maths::QuadTreePoint(x, y));
    return handle;
  }

  const GeoVector2F_t QuadTreePoint_getPosition(QuadTreePoint_t& point)
  {
    Maths::GeoVector2<float>* pos = new Maths::GeoVector2<float>();
    *pos = reinterpret_cast<Maths::QuadTreePoint*>(point)->getPosition();
    return reinterpret_cast<const GeoVector2F_t&>(*pos);
  }

#ifdef __cplusplus

}
#endif
