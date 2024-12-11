#include "SaveGameObject.h"
#include <atomic>

// 使用静态原子变量生成唯一ID
std::atomic<int> nextUID(1);

int SaveGameObjectFactory::GenerateUniqueID() {
    return nextUID++;
}
