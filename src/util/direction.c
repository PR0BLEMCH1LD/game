#include "direction.h"
#include "types.h"
#include <string.h>

const ivec3s DIRECTION_IVEC[6] = {
    {{  0,  0, -1 }},
    {{  0,  0,  1 }},
    {{  1,  0,  0 }},
    {{ -1,  0,  0 }},
    {{  0,  1,  0 }},
    {{  0, -1,  0 }},
};

const vec3s DIRECTION_VEC[6] = {
    {{  0.0f,  0.0f, -1.0f }},
    {{  0.0f,  0.0f,  1.0f }},
    {{  1.0f,  0.0f,  0.0f }},
    {{ -1.0f,  0.0f,  0.0f }},
    {{  0.0f,  1.0f,  0.0f }},
    {{  0.0f, -1.0f,  0.0f }},
};

Direction ivec3s2dir(ivec3s vec) {
    for (s32 idx = 0; idx < 6; idx++) {
        if (!memcmp(&DIR2IVEC3S(idx), &vec, sizeof(ivec3s))) {
            return idx;
        }
    }

    return -1;
}
