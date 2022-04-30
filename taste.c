#import <stdint.h>
#import <memory.h>
#import <printf.h>
#import <objc/objc.h>
#import <objc/NSObjCRuntime.h>
#import <objc/message.h>

#define BRYCE_OBJC_IMPL
#import "bryce-objc.h"

CLASS(NSString);

// NSPasteboard.h
typedef NSString * NSPasteboardType;

SELECTOR(alloc);

SELECTOR_RET(char *, UTF8String);
SELECTOR_ARGS(initWithUTF8String, initWithUTF8String:, const char *);

CLASS(NSPasteboard);
SELECTOR(generalPasteboard);
SELECTOR(pasteboardItems);

CLASS(NSPasteboardItem);
SELECTOR(types);
SELECTOR_ARGS(stringForType, stringForType:, NSPasteboardType);

int main(int argc, char** argv) {
  if (argc > 2) {
    printf("Invalid invocation. Call as %s alone or with --all, -a, or the name of a clipboard type\n", argv[0]);
  } else {
    NSPasteboard* pasteboard = CALL(NSPasteboard_o, generalPasteboard);
    if (argc == 1 || !memcmp(argv[1], "-a", sizeof("-a")) || !memcmp(argv[1], "--all", sizeof("--all"))) {
      NSArray* copiedItems = CALL(pasteboard, pasteboardItems);
      CONST_FOR(NSPasteboardItem*, item, copiedItems) {
        NSArray* types = CALL(item, types);
        CONST_FOR(NSPasteboardType, type, types) {
          if (argc == 1) {
            printf("%s\n", CALL(type, UTF8String));
          } else {
            printf("%s:\n%s\n\n", CALL(type, UTF8String), CALL(CALL(item, stringForType, type), UTF8String));
          }
        }
      }
    } else {
      NSString* dataType = CALL(CALL(NSString_o, alloc), initWithUTF8String, argv[1]);
      NSString* result = CALL(pasteboard, stringForType, dataType);
      printf("%s\n", CALL(result, UTF8String));
    }
  }

  return 0;
}
