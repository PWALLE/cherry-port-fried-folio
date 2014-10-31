#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*
 * Any Class
 */

//obj for Any
struct obj_Any {
    struct class_Any* class_ptr; 
    char* name;
};

//class for Any
struct class_Any {
    struct class_Any* parent;
    struct obj_Any* (*newAny) (void);
    struct obj_String* (*toString) (struct obj_Any*);
    int (*equals) (struct obj_Any*, struct obj_Any*);
    //boolean?
};

//Any constructor
struct obj_Any* newAny() {
    struct obj_Any* anyObj = malloc(sizeof(struct obj_Any));
    anyObj->class_ptr = 0; //this ok for null any parent?
    return anyObj;
}

//Any.toString
struct obj_String* method_toString(struct obj_Any* this) {
    //fill in
};

//Any.equals
int method_equals(struct obj_Any* this, struct obj_Any* x) {
    int ret;
    if (this == x) //is this ok for equals?
        ret = 1;
    else
        ret = 0;
    return ret;
};

//class object for Any
static struct class_Any Any = { 0, &newAny, &method_toString, &method_equals };

/*
 * ArrayAny Class
 */

//obj for ArrayAny
struct obj_ArrayAny {
    struct class_ArrayAny* class_ptr; 
    int length;
    struct obj_Any** array;
};

//class for ArrayAny
struct class_ArrayAny {
    struct class_Any* parent;
    struct obj_ArrayAny* (*newArrayAny) (int);
    struct obj_String* (*toString) (struct obj_ArrayAny*);
    int (*equals) (struct obj_ArrayAny*, struct obj_Any*);
    int (*length) (struct obj_ArrayAny*);
    struct obj_Any* (*get) (struct obj_ArrayAny*, int);
    struct obj_Any* (*set) (struct obj_ArrayAny*, int, struct obj_Any*);
};

//ArrayAny constructor
struct obj_ArrayAny* newArrayAny(int length) {
    struct obj_ArrayAny* aAnyObj = malloc(sizeof(struct obj_ArrayAny));
    aAnyObj->length = length;
    //do i have to do anything with array here?
    return aAnyObj;
}

//ArrayAny.length
int method_length(struct obj_ArrayAny* this) {
    return this->length;
}

//ArrayAny.get
struct obj_Any* method_get(struct obj_ArrayAny* this, int index) {
    return this->array[index];
}

//ArrayAny.set
struct obj_Any* method_set(struct obj_ArrayAny* this, int index, struct obj_Any* obj) {
    struct obj_Any* ret = this->class_ptr->get(this, index);
    this->array[index] = obj;
    return ret; //how does this look?
}

//class object for ArrayAny
static struct class_ArrayAny ArrayAny = { &Any, &newArrayAny, &method_toString, &method_equals, &method_length, &method_get, &method_set };

/*
 * String Class
 */

//obj for String
struct obj_String {
    struct class_String* class_ptr; 
    int length;
    char* str_field;
};

//class for String
struct class_String {
    struct class_Any* parent;
    struct obj_String* (*newString) (void);
    struct obj_String* (*toString) (struct obj_String*);
    int (*equals) (struct obj_String*, struct obj_String*);
    int (*length) (struct obj_String*);
    struct obj_String* (*concat) (struct obj_String*, struct obj_String* arg);
    struct obj_String* (*substr) (struct obj_String*, int start, int end);
    char (*charAt) (struct obj_String*, int index);
    int (*indexOf) (struct obj_String*, struct obj_String* sub);
};

//String constructor
struct obj_String* newString() {
    struct obj_String* stringObj = malloc(sizeof(struct obj_String));
    stringObj->length = 0;
    stringObj->str_field = '\0';
    return stringObj;
}

//String.toString override
struct obj_String* method_String_toString(struct obj_String* this) {
    return this;
}

//String.equals override
int method_String_equals(struct obj_String* this, struct obj_String* arg) {
    int match;
    if (strcmp(this->str_field,arg->str_field) != 0) {
        match = 0;
    }
    else {
        match = 1;
    }
    return match;
}

//String.length
int method_String_length(struct obj_String* this) {
    return this->length;
}

//String.concat
struct obj_String* method_concat(struct obj_String* this, struct obj_String* arg) {
    strcat(this->str_field, arg->str_field);
    return this;
}

//String.substring
struct obj_String* method_substring(struct obj_String* this, int start, int end) {
    struct obj_String* sub = malloc(sizeof(struct obj_String));
    sub->length = start - end;
    strncpy(sub->str_field, this->str_field+start, end);
    return sub;
}

//String.charAt
char method_charAt(struct obj_String* this, int index) {
    char ret = this->str_field[index];
    return ret;
}

//String.indexOf
int method_indexOf(struct obj_String* this, struct obj_String* sub) {
    int ret = strcspn(this->str_field, sub->str_field);
    return ret;
}

//class object for String
static struct class_String String = { &Any, &newString, &method_String_toString, &method_String_equals, &method_String_length, &method_concat, &method_substring, &method_charAt, &method_indexOf};

/*
 * Symbol Class
 */

//obj for Symbol
struct obj_Symbol {
    struct class_Symbol* class_ptr; 
    struct obj_String* symbol;
};

//class for Symbol
struct class_Symbol {
    struct class_Any* parent;
    struct obj_Symbol* (*newSymbol) (void);
    struct obj_String* (*toString) (struct obj_Symbol*);
    int (*equals) (struct obj_Symbol*, struct obj_Any*);
};

//Symbol constructor
struct obj_Symbol* newSymbol() {
    struct obj_Symbol* symObj = malloc(sizeof(struct obj_Symbol));
    struct obj_String* str = malloc(sizeof(struct obj_String));
    str = str->class_ptr->newString();
    symObj->symbol = str;
    return symObj;
}

//class object for Symbol
static struct class_Symbol Symbol = { &Any, &newSymbol, &method_toString, &method_equals };



/*
 * IO Class
 */

//obj for IO
struct obj_IO {
    struct class_IO* class_ptr; 
};

//class for IO
struct class_IO {
    struct class_Any* parent;
    struct obj_IO* (*toString) (struct obj_IO*);
    int (*equals) (struct obj_IO*, struct obj_Any*);
    void (*abort) (struct obj_IO*, struct obj_String*);
    struct obj_IO* (*out) (struct obj_IO*, struct obj_String*);
    int (*is_null) (struct obj_IO*, struct obj_Any*);
    struct obj_IO* (*out_any) (struct obj_IO*, struct obj_Any*);
    struct obj_String* (*in) (struct obj_IO*);
    struct obj_Symbol* (*symbol) (struct obj_IO*, struct obj_String*);
    struct obj_String* (*symbol_name) (struct obj_IO*, struct obj_Symbol*);

};

//IO constructor
struct obj_IO* newIO() {
    struct obj_IO* ioObj = malloc(sizeof(struct obj_IO));
    return ioObj;
}

//IO.abort
void method_abort(struct obj_IO* this, struct obj_String* message) {
    fprintf(stderr, "%s", message->str_field);
    exit(1);
}

//IO.out
struct obj_IO* method_out(struct obj_IO* this, struct obj_String* message) {
    printf("%s", message->str_field);
    return this;
}

//IO.is_null
int method_is_null(struct obj_IO* this, struct obj_Any* arg) {
    int ret;
    if (arg == 0)
        ret = 1;
    else
        ret = 0;
    return ret;
}

//IO.out_any
struct obj_IO* method_out_any(struct obj_IO* this, struct obj_Any* arg) {
    if (this->class_ptr->is_null(this, arg) == 1) {
        printf("null");
    }
    else {
        struct obj_String* msg = arg->class_ptr->toString(arg);
        printf("%s", msg->str_field);
    }
    return this;
}

//IO.in
struct obj_String* method_in(struct obj_IO* this) {
    struct obj_String* in = malloc(sizeof(struct obj_String));
    int numchar = 0;
    char c;
    char tmp[1000];
    while ((c = getchar()) != '\n') {
        tmp[numchar] = c;
        numchar++;
        if (numchar >= 1000) {
            fprintf(stderr, "Error: input cannot exceed 1000 chars");
            exit(1);
        }
    }
    tmp[numchar] = '\0';
    //fprintf(stdout, "%s", tmp);
    in->str_field = tmp;
    in->length = numchar;
    return in;
}

//IO.symbol
struct obj_Symbol* method_symbol(struct obj_IO* this, struct obj_String* name) {
    struct obj_Symbol* ret = malloc(sizeof(struct obj_Symbol));
    ret->symbol = name;
    return ret;
}

//IO.symbol_name
struct obj_String* method_symbol_name(struct obj_IO* this, struct obj_Symbol* sym) {
    return sym->symbol;
}

//class object for IO
static struct class_IO IO = { &Any, &newIO, &method_toString, &method_equals, &method_abort, &method_out, &method_is_null, &method_out_any, &method_in, &method_symbol, &method_symbol_name };


int ll_main(int junk) {
    return 0;
}
