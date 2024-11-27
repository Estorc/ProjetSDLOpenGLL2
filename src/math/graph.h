#include <GL/gl.h>
#include <math.h>
#include <cglm/cglm.h>

#ifndef GRAPH_H
#define GRAPH_H

/*typedef struct {
    f32 x;
    f32 y;
    f32 z;
    f32 w;
} Vec4f;

typedef struct {
    f32 x;
    f32 y;
    f32 z;
} Vec3f;

typedef struct {
    f32 x;
    f32 y;
} Vec2f;

typedef struct {
    f32 x;
} Vec1f;*/


typedef vec4 Vec4f;
typedef vec3 Vec3f;
typedef vec2 Vec2f;
typedef float Vec1f;

typedef mat4 Mat4;

f32 get_vec3f_magnitude(Vec3f *vec);
f32 get_vec3f_sqr_magnitude(Vec3f *vec);
void set_vec3f_magnitude(Vec3f *vec, f32 magnitude);
void glPolygonVec3(Vec3f *vecs, u8 length);


#define Mat4SetIdentity(a) (a[0][0] = 1.0f, a[0][1] = 0.0f, a[0][2] = 0.0f, a[0][3] = 0.0f, \
                            a[1][0] = 0.0f, a[1][1] = 1.0f, a[1][2] = 0.0f, a[1][3] = 0.0f, \
                            a[2][0] = 0.0f, a[2][1] = 0.0f, a[2][2] = 1.0f, a[2][3] = 0.0f, \
                            a[3][0] = 0.0f, a[3][1] = 0.0f, a[3][2] = 0.0f, a[3][3] = 1.0f)

#define Vec3fScale(a,s) (a[0]*=s, a[1]*=s, a[2]*=s)
#define Vec3fZero(a) (a[0]=0,a[1]=0,a[2]=0)
#define Vec3fOne(a) (a[0]=1,a[1]=1,a[2]=1)
#define Vec3fSet(a,b) (a[0]=b[0],a[1]=b[1],a[2]=b[2])
#define Vec3fApproach(a,b,t) ((a)[0]+=((b)[0]-(a)[0])/t,(a)[1]+=((b)[1]-(a)[1])/t,(a)[2]+=((b)[2]-(a)[2])/t)
#define Vec3fSum(a,b) (a[0] += b[0],a[1] += b[1],a[2] += b[2])
#define Vec3fDiff(a,b) (a[0] -= b[0],a[1] -= b[1],a[2] -= b[2])
#define Vec3fDot(a,b) (a[0]*b[0] + a[1]*b[1] + a[2]*b[2])
#define Vec3fNormalize(a) (set_vec3f_magnitude(&a, 1))
#define glVertexVec3(a) glVertex3f( a[0], a[1], a[2] )

#endif