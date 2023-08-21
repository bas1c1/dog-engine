#ifndef DOG_ENGINE_HELPER
#define DOG_ENGINE_HELPER

template<typename Base, typename T>
inline bool instanceof(const T* ptr) {
    return dynamic_cast<const Base*>(ptr) != nullptr;
}

#endif