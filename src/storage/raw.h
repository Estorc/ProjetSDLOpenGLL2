#pragma once
#define NEW_RAW(type, value) \
    ((type *) memcpy(malloc(sizeof(type)), &(type){value}, sizeof(type)))

#define GET_RAW(type, value) \
    value ? (*((type *) value)) : 0
