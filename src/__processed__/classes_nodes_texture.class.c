#include <stdarg.h>
#include "../types.h"
#include "../classes/classes.h"
#include "../math/math_util.h"
#include "../io/model.h"
#include "../render/framebuffer.h"
#include "../storage/node.h"
static unsigned __type__ __attribute__((unused)) = CLASS_TYPE_TEXTURE;
#line 10 "src/classes/nodes/texture.class.c"
void __class_method_texture_constructor(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);TextureMap * texture = va_arg(args, TextureMap *);(void)this;
        this->object = texture;
        this->type = __type__;
        SUPER(initialize_node);
}

    

#line 18 "src/classes/nodes/texture.class.c"
void __class_method_texture_load(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);(void)this;

}

#line 22 "src/classes/nodes/texture.class.c"
void __class_method_texture_save(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);(void)this;

}




    
