#ifndef value_h
#define value_h

#include "common.h"

typedef enum {
    VAL_NIL,
    VAL_BOOL,
    VAL_NUMBER,
    VAL_STRING
} ValueType;


typedef struct {

    ValueType type;

    union {
        bool    boolean;
        double  number;
        const char* string;

    } as;


} Value;



/*====================CONSTRUCTERS=============================================*/


#define NIL_VAL         ((Value){VAL_NIL,{.number = 0}})
#define BOOL_VAL(b)     ((Value){VAL_BOOL,{.boolean = b}})
#define NUMBER_VAL(n)   ((Value){VAL_NUMBER,{.number = n}})
#define STRING_VAL(s)   ((Value){VAL_STRING, {.string = s}})


#define IS_NIL(v)         ((v).type == VAL_NIL)
#define IS_BOOL(v)        ((v).type == VAL_BOOL)
#define IS_NUMBER(v)      ((v).type == VAL_NUMBER)
#define IS_STRING(v)      ((v).type == VAL_STRING)


// unwrappers — get the raw C value back out
#define AS_BOOL(v)        ((v).as.boolean)
#define AS_NUMBER(v)      ((v).as.number)
#define AS_STRING(v)      ((v).as.string)


/*===========================================================================*/

#endif