# Paste
When copying something to the clipboard on macOS, there will often be multiple types that the clipboard
contents can be pasted as. For some reason, there is no good way to find out what types are on the
clipboard and extract the one you want. The built in `pbpaste` command does plain text just fine and claims
to work with rtf and PostScript though I have not found either of those to work. So I searched up the API
so I could write a basic utility that will help. It only works properly with text types like plain text,
rtf, html, etc. Other types will show up but display as `(null)` if you try to past them.

## Compiling
Compilation is very simple since this is just one file. I use clang to compile. It's probably possible to
cross compile for macOS on some other platform if you know the right flags. Anyway, just run
`clang paste.m -framework AppKit -framework CoreFoundation -o taste`. This will output a `./taste` binary
that you can install or do whatever else you want with.

## Using
To run, use `taste` which shows all of the paste types found on your clipboard. Specify the type to get it
`taste public.utf8-plain-text` or `taste public.rtf`. If you want to get all types, use
`--all` or `-a`: `taste --all`. More than one argument is not supported. That's about all there is to it.

## License
Created by Bryce Wilson. See the license file for the license. If you need anything outside of that or
have any other questions or comments you can visit [my GitHub page](https://github.com/Bryce-MW/) where my
email is listed. You should also be able to find the repository for this project there if you somehow got
this file another way.
