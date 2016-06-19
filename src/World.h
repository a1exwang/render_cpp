//
// Created by alexwang on 6/19/16.
//

#ifndef RENDER_WORLD_H
#define RENDER_WORLD_H

#include "objects/ObjectBase.h"
#include "ObjectManager.h"
#include <vector>
#include <memory>

namespace alex {
class World {
public:
  void addObject(std::shared_ptr<ObjectBase> object) { objectManager.addObject(object); }
private:
  ObjectManager objectManager;
};
}




#endif //RENDER_WORLD_H
