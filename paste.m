#import <memory.h>
#import <printf.h>
#import <CoreFoundation/CFArray.h>
#import <AppKit/NSPasteboard.h>

int main(int argc, char** argv) {
  if (argc > 2) {
    printf("Invalid invocation. Call as %s alone or with --all, -a, or the name of a clipboard type\n", argv[0]);
  } else {
    NSPasteboard* pasteboard = [NSPasteboard generalPasteboard];
    if (argc == 1 || !memcmp(argv[1], "-a", sizeof("-a")) || !memcmp(argv[1], "--all", sizeof("--all"))) {
      NSArray* copiedItems = [pasteboard pasteboardItems];
      CFIndex itemCount = CFArrayGetCount((CFArrayRef)copiedItems);
      for (CFIndex i = 0; i < itemCount; i++) {
        NSPasteboardItem* item = CFArrayGetValueAtIndex((CFArrayRef)copiedItems, i);
        NSArray* types = [item types];
        CFIndex typeCount = CFArrayGetCount((CFArrayRef)types);
        for (CFIndex j = 0; j < typeCount; j++) {
          NSPasteboardType type = CFArrayGetValueAtIndex((CFArrayRef)types, j);
          if (argc == 1) {
            printf("%s\n", [type UTF8String]);
          } else {
            printf("%s:\n%s\n\n", [type UTF8String], [[item stringForType:type] UTF8String]);
          }
        }
      }
    } else {
      NSString* dataType = [[NSString alloc] initWithUTF8String:argv[1]];
      NSString* result = [pasteboard stringForType:dataType];
      printf("%s\n", [result UTF8String]);
    }
  }

  return 0;
}
