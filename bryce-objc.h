#import <stdint.h>
#import <objc/objc.h>
#import <objc/NSObjCRuntime.h>
#import <objc/message.h>

#ifdef BRYCE_OBJC_IMPL

struct {
    uint32_t version;
    uint32_t flags;
} objc_imageinfo __attribute__((section("__DATA,__objc_imageinfo,regular,no_dead_strip"))) = {0, 64};

// Foundation.h
typedef struct {
    unsigned long state;
    id __unsafe_unretained _Nullable * _Nullable itemsPtr;
    unsigned long * _Nullable mutationsPtr;
    unsigned long extra[5];
} NSFastEnumerationState;

#endif // BRYCE_OBJC_IMPL

#define CLASS(name) \
  extern struct objc_class OBJC_CLASS_$_ ## name ; \
  _Nonnull id name ## _o __attribute__((section("__DATA,__objc_classrefs,regular,no_dead_strip"))) = (id)&OBJC_CLASS_$_ ## name ; \
  typedef struct objc_object name

#define SELECTOR_RET_ARGS(ret, name, val, ...) \
  const char OBJC_METH_VAR_NAME_ ## name [] __attribute__((section("__TEXT,__objc_methname,cstring_literals"))) = #val ; \
  _Nonnull SEL name ## _s __attribute__((section("__DATA,__objc_selrefs,literal_pointers,no_dead_strip"))) = (SEL)(const char*) OBJC_METH_VAR_NAME_ ## name ; \
  typedef ret (* name ## _msgSend)(_Nonnull id, _Nonnull SEL, ## __VA_ARGS__)

#define SELECTOR_ARGS(name, val, ...) SELECTOR_RET_ARGS(id,  name, val, ## __VA_ARGS__)
#define SELECTOR_RET(ret, name)       SELECTOR_RET_ARGS(ret, name, name)
#define SELECTOR(name)                SELECTOR_RET_ARGS(id,  name, name)

#define CALL(obj, sel, ...) ((sel ## _msgSend)objc_msgSend)(obj, sel ## _s, ## __VA_ARGS__)

#define CONST_FOR(ty, elm, cont) \
  ty elm; \
  NSFastEnumerationState cont ## State = {0}; \
  ty cont ## _items[16]; \
  for ( \
    NSUInteger cont ## Limit = CALL(cont, countByEnumeratingWithState, & cont ## State, (id*) cont ## _items, (NSUInteger)16), cont ## Counter = (elm = * cont ## State.itemsPtr, 0); \
    cont ## Counter < cont ## Limit && cont ## Limit; \
    ((cont ## Counter < cont ## Limit) && (elm = cont ## State.itemsPtr[++cont ## Counter])) || (cont ## Limit = CALL(cont, countByEnumeratingWithState, & cont ## State, cont ## _items, (NSUInteger)16), cont ## Counter = 0) \
  )

#ifdef BRYCE_OBJC_IMPL

CLASS(NSArray);
SELECTOR_RET_ARGS(NSUInteger, countByEnumeratingWithState, countByEnumeratingWithState:objects:count:, NSFastEnumerationState* _Nonnull, _Nullable id* _Nonnull, NSUInteger);

#endif
