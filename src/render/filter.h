#ifndef FILTER_H
#define FILTER_H

struct Node;

typedef struct {
    void (*beginScript)(struct Node *node, TextureMap *texture);
    void (*endScript)(struct Node *node, TextureMap *texture);
    u8 flags;
    TextureMap texture;
} Filter;

enum FilterFlags {
    FILTER_ACTIVE       = 1 << 0, // 0000 0001
    FILTER_NEED_RENDER  = 1 << 1, // 0000 0010
    FILTER_UNUSED1      = 1 << 2, // 0000 0100
    FILTER_UNUSED2      = 1 << 3, // 0000 1000
    FILTER_UNUSED3      = 1 << 4, // 0001 0000
    FILTER_UNUSED4      = 1 << 5, // 0010 0000
    FILTER_UNUSED5      = 1 << 6, // 0100 0000
    FILTER_UNUSED6      = 1 << 7, // 1000 0000
};

#define FILTER_ACTIVE_AND_NEED_RENDER FILTER_ACTIVE | FILTER_NEED_RENDER // 0000 0011
#define FILTER_DEFAULT_FLAGS FILTER_ACTIVE_AND_NEED_RENDER // 0000 0011

#endif

void anti_aliasing_begin_script(struct Node *node, TextureMap *texture);
void anti_aliasing_end_script(struct Node *node, TextureMap *texture);