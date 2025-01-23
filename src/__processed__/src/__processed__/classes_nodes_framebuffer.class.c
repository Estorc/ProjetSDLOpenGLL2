#line 1 "src/__processed__/classes_nodes_framebuffer.class.c"
#include <stdarg.h>
#include "../raptiquax.h"
#include "../classes/classes.h"
#include "../math/math_util.h"
#include "../io/model.h"
#include "../render/framebuffer.h"
#include "../storage/node.h"
static unsigned __type__ __attribute__((unused)) = CLASS_TYPE_FRAMEBUFFER;
#line 10 "src/classes/nodes/framebuffer.class.c"
void __class_method_framebuffer_constructor(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);FrameBuffer * frameBuffer = va_arg(args, FrameBuffer *);(void)this;
        this->object = frameBuffer;
        this->type = __type__;
        SUPER(initialize_node);
}

    

#line 18 "src/classes/nodes/framebuffer.class.c"
void __class_method_framebuffer_load(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);(void)this;
        FrameBuffer *msaa_framebuffer;
        msaa_framebuffer = malloc(sizeof(FrameBuffer));

        call_method_0(METHOD(constructor,this,msaa_framebuffer));
}

#line 25 "src/classes/nodes/framebuffer.class.c"
void __class_method_framebuffer_save(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);FILE * file = va_arg(args, FILE *);(void)this;
        fprintf(file, "%s", classManager.class_names[this->type]);
}




    
