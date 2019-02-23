//
// Created by matth on 22/02/2019.
//

#ifndef NOVELRT_NOVELINTERACTIONRECT_H
#define NOVELRT_NOVELINTERACTIONRECT_H

#include "NovelObject.h"

namespace NovelRT {
class NovelInteractionRect : public NovelObject {
public:
  NovelInteractionRect(NovelRenderingService* novelRenderer, const float screenScale, const GeoVector<float>& size,
                       const NovelCommonArgs& args);

};
}

#endif //NOVELRT_NOVELINTERACTIONRECT_H
