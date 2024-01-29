#ifndef DOG_ENGINE_HELPER
#define DOG_ENGINE_HELPER

template<typename T>
inline bool instanceof(T* ptr) {
    return dynamic_cast<T*>(ptr) != nullptr;
}

#endif