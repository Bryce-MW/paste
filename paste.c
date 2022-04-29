#import <stdint.h>
#import <memory.h>
#import <printf.h>
#import <objc/objc.h>
#import <objc/NSObjCRuntime.h>
#import <objc/message.h>

struct {
    uint32_t version;
    uint32_t flags;
} objc_imageinfo __attribute__((section("__DATA,__objc_imageinfo,regular,no_dead_strip"))) = {0, 64};


#define CLASS(name) \
  extern struct objc_class OBJC_CLASS_$_ ## name ; \
  id name ## _o __attribute__((section("__DATA,__objc_classrefs,regular,no_dead_strip"))) = (id)&OBJC_CLASS_$_ ## name ; \
  typedef struct objc_object name

#define SELECTOR_ARGS(name, val) \
  const char OBJC_METH_VAR_NAME_ ## name [] __attribute__((section("__TEXT,__objc_methname,cstring_literals"))) = #val ; \
  SEL name ## _s __attribute__((section("__DATA,__objc_selrefs,literal_pointers,no_dead_strip"))) = (SEL)(const char*) OBJC_METH_VAR_NAME_ ## name

#define SELECTOR(name) SELECTOR_ARGS(name, name)

#define CALL0(obj, sel)             ((id (*)(id, SEL))                                    objc_msgSend)(obj, sel ## _s)
#define CALL1(obj, sel, a1)         ((id (*)(id, SEL, typeof(a1)))                        objc_msgSend)(obj, sel ## _s, a1)
#define CALL2(obj, sel, a1, a2)     ((id (*)(id, SEL, typeof(a1), typeof(a2)))            objc_msgSend)(obj, sel ## _s, a1, a2)
#define CALL3(obj, sel, a1, a2, a3) ((id (*)(id, SEL, typeof(a1), typeof(a2), typeof(a3)))objc_msgSend)(obj, sel ## _s, a1, a2, a3)

#define CALLR0(ret, obj, sel)             ((ret (*)(id, SEL))                                    objc_msgSend)(obj, sel ## _s)
#define CALLR1(ret, obj, sel, a1)         ((ret (*)(id, SEL, typeof(a1)))                        objc_msgSend)(obj, sel ## _s, a1)
#define CALLR2(ret, obj, sel, a1, a2)     ((ret (*)(id, SEL, typeof(a1), typeof(a2)))            objc_msgSend)(obj, sel ## _s, a1, a2)
#define CALLR3(ret, obj, sel, a1, a2, a3) ((ret (*)(id, SEL, typeof(a1), typeof(a2), typeof(a3)))objc_msgSend)(obj, sel ## _s, a1, a2, a3)

#define CONST_FOR(ty, elm, cont) \
  ty elm; \
  NSFastEnumerationState cont ## State = {0}; \
  ty cont ## _items[16]; \
  for ( \
    NSUInteger cont ## Limit = CALLR3(NSUInteger, cont, countByEnumeratingWithState, & cont ## State, (id*) cont ## _items, (NSUInteger)16), cont ## Counter = (elm = * cont ## State.itemsPtr, 0); \
    cont ## Counter < cont ## Limit && cont ## Limit; \
    ((cont ## Counter < cont ## Limit) && (elm = cont ## State.itemsPtr[++cont ## Counter])) || (cont ## Limit = CALLR3(NSUInteger, cont, countByEnumeratingWithState, & cont ## State, cont ## _items, (NSUInteger)16), cont ## Counter = 0) \
  )


SELECTOR(alloc);

CLASS(NSString);
SELECTOR(UTF8String);
SELECTOR_ARGS(initWithUTF8String, initWithUTF8String:);

CLASS(NSArray);
SELECTOR_ARGS(countByEnumeratingWithState, countByEnumeratingWithState:objects:count:);

CLASS(NSPasteboard);
SELECTOR(generalPasteboard);
SELECTOR(pasteboardItems);

CLASS(NSPasteboardItem);
SELECTOR(types);
SELECTOR_ARGS(stringForType, stringForType:);

// Foundation.h
typedef struct {
    unsigned long state;
    id __unsafe_unretained _Nullable * _Nullable itemsPtr;
    unsigned long * _Nullable mutationsPtr;
    unsigned long extra[5];
} NSFastEnumerationState;

// NSPasteboard.h
typedef NSString * NSPasteboardType;


int main(int argc, char** argv) {
  if (argc > 2) {
    printf("Invalid invocation. Call as %s alone or with --all, -a, or the name of a clipboard type\n", argv[0]);
  } else {
    NSPasteboard* pasteboard = CALL0(NSPasteboard_o, generalPasteboard);
    if (argc == 1 || !memcmp(argv[1], "-a", sizeof("-a")) || !memcmp(argv[1], "--all", sizeof("--all"))) {
      NSArray* copiedItems = CALL0(pasteboard, pasteboardItems);
      CONST_FOR(NSPasteboardItem*, item, copiedItems) {
        NSArray* types = CALL0(item, types);
        CONST_FOR(NSPasteboardType, type, types) {
          if (argc == 1) {
            printf("%s\n", CALLR0(char *, type, UTF8String));
          } else {
            printf("%s:\n%s\n\n", CALLR0(char *, type, UTF8String), CALLR0(char *, CALL1(item, stringForType, type), UTF8String));
          }
        }
      }
    } else {
      NSString* dataType = CALL1(CALL0(NSString_o, alloc), initWithUTF8String, argv[1]);
      NSString* result = CALL1(pasteboard, stringForType, dataType);
      printf("%s\n", CALLR0(char *, result, UTF8String));
    }
  }

  return 0;
}
