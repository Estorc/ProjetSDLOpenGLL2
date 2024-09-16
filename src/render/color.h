#ifndef COLOR_H
#define COLOR_H

typedef struct {
    f32 r;
    f32 g;
    f32 b;
    f32 a;
} ColRGBA;

typedef struct {
    f32 r;
    f32 g;
    f32 b;
} ColRGB;

ColRGBA get_color(u8 r, u8 g, u8 b, u8 a);
void darken_color(ColRGBA *col, f32 rate);

#endif

#define get_hex_color(x) get_color((x>>24)&0xff, (x>>16)&0xff, (x>>8)&8, x&0xff)
#define glColorRGBA(x) glColor4f(x.r, x.g, x.b, x.a)