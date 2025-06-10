#pragma once

namespace overlay_framework {
    namespace utility {
        template <typename T>
        concept releasable = requires(T t) {
            { t.Release() };
        };

        template <typename T>
            requires releasable<T>
        void safe_release(T** pointer) {
            if (pointer && *pointer) {
                (*pointer)->Release();
                *pointer = nullptr;
            }
        }
    } // namespace utility
} // namespace overlay_framework
