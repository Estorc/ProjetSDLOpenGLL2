void project_box_onto_axis(const vec3 axis, const vec3 halfExtents, const mat3 rotation, vec3 boxCenter, float *minProj, float *maxProj);
void project_capsule_onto_axis(vec3 axis, vec3 capsuleStart, vec3 capsuleEnd, float capsuleRadius, float *minProj, float *maxProj);
void closest_point_on_triangle(vec3 p, vec3 a, vec3 b, vec3 c, vec3 closestPoint);
bool projections_overlap(float minA, float maxA, float minB, float maxB);
float compute_overlap(float minA, float maxA, float minB, float maxB);