#include "math.h"
#include "../state.h"
#include "../world/world.h"

static vec3s find(vec3s s, vec3s ds) {
    vec3s v;

    for (size_t i = 0; i < 3; i++) {
        v.raw[i] = (ds.raw[i] > 0 ? (ceilf(s.raw[i]) - s.raw[i]) : (s.raw[i] - floorf(s.raw[i]))) / fabsf(ds.raw[i]);
    }

    return v;
}

bool ray_intersect_block(Ray ray, ivec3s* pos_out, Direction* dir_out) {
    ivec3s pos, step;
    vec3s tmax, tdelta;
    f32 radius;

    pos = VEC3S2I(ray.origin);
    step = (ivec3s) {{ sign(ray.direction.x), sign(ray.direction.y), sign(ray.direction.z) }};
    tmax = find(ray.origin, ray.direction);
    tdelta = glms_vec3_div(IVEC3S2V(step), ray.direction);
    radius = state.world.player.reach / glms_vec3_norm(ray.direction);

    while (true) {
        if (world_get_data(&state.world, pos) != 0) {
            *pos_out = pos;
            return true;
        }

        if (tmax.x < tmax.y) {
            if (tmax.x < tmax.z) {
                if (tmax.x > radius) {
                    break;
                }

                pos.x += step.x;
                tmax.x += tdelta.x;
                *dir_out = ivec3s2dir(((ivec3s) {{ -step.x, 0, 0 }}));
            } 
            else {
                if (tmax.z > radius) {
                    break;
                }

                pos.z += step.z;
                tmax.z += tdelta.z;
                *dir_out = ivec3s2dir(((ivec3s) {{ 0, 0, -step.z }}));
            }
        } 
        else {
            if (tmax.y < tmax.z) {
                if (tmax.y > radius) {
                    break;
                }

                pos.y += step.y;
                tmax.y += tdelta.y;
                *dir_out = ivec3s2dir(((ivec3s) {{ 0, -step.y, 0 }}));
            } 
            else {
                if (tmax.z > radius) {
                    break;
                }

                pos.z += step.z;
                tmax.z += tdelta.z;
                *dir_out = ivec3s2dir(((ivec3s) {{ 0, 0, -step.z }}));
            }
        }
    }

    return false;
}

s32 ivec3scmp(ivec3s a, ivec3s b) {
    return memcmp(&a, &b, sizeof(ivec3s));
}
