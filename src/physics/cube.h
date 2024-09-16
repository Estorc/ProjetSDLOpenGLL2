void project_box_onto_axis(const vec3 axis, const vec3 halfExtents, const mat3 rotation, vec3 boxCenter, float *minProj, float *maxProj);
bool projections_overlap(float minA, float maxA, float minB, float maxB);
float compute_overlap(float minA, float maxA, float minB, float maxB);