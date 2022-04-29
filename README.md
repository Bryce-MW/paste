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
`clang paste.m -framework AppKit -framework CoreFoundation -o taste -O3`. This will output a `./taste` binary
that you can install or do whatever else you want with.

Additionally, I've written a version that uses C rather than Objective-C. I've done some hacks to allow it
to register selectors and the like the way that Objective-C does rather than using the runtime library.
I've tested the speed and both programs take almost exactly the same time to run. I mostly did it to see
if I could and to figure out how to do it in an easy environment before I implement it in a more difficult
one like another language where you couldn't just compile in Objective-C mode if you want. Anyway, to
compile this version, run
`clang paste.c -fuse-ld=lld -framework AppKit -framework CoreFoundation -o tastec -O3`. It may not work
with the default compiler than apple ships, I had some issues with that. I used the version from Homebrew
which does take some setting up to get working, another reason why I don't recommed doing this. I'm sure
there is some way to trick Apple's linker into working but I have not figured it out yet! Let me know if
you can figure it out.

## Using
To run, use `taste` which shows all of the paste types found on your clipboard. Specify the type to get it
`taste public.utf8-plain-text` or `taste public.rtf`. If you want to get all types, use
`--all` or `-a`: `taste --all`. More than one argument is not supported. That's about all there is to it.

## Benchmarks
I tested the C and Objective-C versions on my MacBook Pro (16-inch, 2021) with the Apple M1 Max and 64 GB
of ram. I tested with copied info that creates the following output from the program:
```shell
[~/randomProj/paste]$ ./taste --all                                                                *[main]
public.rtf:
{\rtf1\ansi\ansicpg1252\cocoartf2636
\cocoatextscaling0\cocoaplatform0{\fonttbl\f0\fnil\fcharset0 FiraCode-Light;}
{\colortbl;\red255\green255\blue255;\red189\green189\blue199;\red23\green22\blue29;}
{\*\expandedcolortbl;;\cspthree\c78824\c78824\c81961;\cspthree\c11765\c11373\c14902;}
\pard\tx560\tx1120\tx1680\tx2240\tx2800\tx3360\tx3920\tx4480\tx5040\tx5600\tx6160\tx6720\pardirnatural\partightenfactor0

\f0\fs26 \cf2 \cb3 \CocoaLigature0 \{\\rtf1\\ansi\\ansicpg1252\\cocoartf2636\
\\cocoatextscaling0\\cocoaplatform0\{\\fonttbl\\f0\\fnil\\fcharset0 FiraCode-Light;\}\
\{\\colortbl;\\red255\\green255\\blue255;\\red189\\green189\\blue199;\\red23\\green22\\blue29;\}\
\{\\*\\expandedcolortbl;;\\cspthree\\c78824\\c78824\\c81961;\\cspthree\\c11765\\c11373\\c14902;\}\
\\pard\\tx560\\tx1120\\tx1680\\tx2240\\tx2800\\tx3360\\tx3920\\tx4480\\tx5040\\tx5600\\tx6160\\tx6720\\pardirnatural\\partightenfactor0\
\
\\f0\\fs26 \\cf2 \\cb3 \\CocoaLigature0 hyperfine --warmup 100 --min-runs 1000 --parameter-list\}}

public.utf8-plain-text:
{\rtf1\ansi\ansicpg1252\cocoartf2636
\cocoatextscaling0\cocoaplatform0{\fonttbl\f0\fnil\fcharset0 FiraCode-Light;}
{\colortbl;\red255\green255\blue255;\red189\green189\blue199;\red23\green22\blue29;}
{\*\expandedcolortbl;;\cspthree\c78824\c78824\c81961;\cspthree\c11765\c11373\c14902;}
\pard\tx560\tx1120\tx1680\tx2240\tx2800\tx3360\tx3920\tx4480\tx5040\tx5600\tx6160\tx6720\pardirnatural\partightenfactor0

\f0\fs26 \cf2 \cb3 \CocoaLigature0 hyperfine --warmup 100 --min-runs 1000 --parameter-list}

public.utf16-external-plain-text:
(null)
```
Here are the results:
```
[~/randomProj/paste]$ hyperfine --warmup 100 --min-runs 1000 './taste' './tastec'                  *[main]
Benchmark 1: ./taste
  Time (mean ± σ):       6.0 ms ±   0.3 ms    [User: 2.5 ms, System: 1.7 ms]
  Range (min … max):     5.5 ms …   9.2 ms    1000 runs
 
  Warning: Statistical outliers were detected. Consider re-running this benchmark on a quiet PC without any interferences from other programs. It might help to use the '--warmup' or '--prepare' options.
 
Benchmark 2: ./tastec
  Time (mean ± σ):       6.0 ms ±   0.4 ms    [User: 2.5 ms, System: 1.7 ms]
  Range (min … max):     5.4 ms …  10.4 ms    1000 runs
 
  Warning: Statistical outliers were detected. Consider re-running this benchmark on a quiet PC without any interferences from other programs. It might help to use the '--warmup' or '--prepare' options.
 
Summary
  './tastec' ran
    1.00 ± 0.09 times faster than './taste'
[~/randomProj/paste]$ hyperfine --warmup 100 --min-runs 1000 './taste --all' './tastec --all'      *[main]
Benchmark 1: ./taste --all
  Time (mean ± σ):       6.4 ms ±   0.4 ms    [User: 2.7 ms, System: 1.8 ms]
  Range (min … max):     5.9 ms …  11.1 ms    1000 runs
 
  Warning: Statistical outliers were detected. Consider re-running this benchmark on a quiet PC without any interferences from other programs. It might help to use the '--warmup' or '--prepare' options.
 
Benchmark 2: ./tastec --all
  Time (mean ± σ):       6.5 ms ±   0.4 ms    [User: 2.7 ms, System: 1.9 ms]
  Range (min … max):     5.9 ms …  11.1 ms    1000 runs
 
  Warning: Statistical outliers were detected. Consider re-running this benchmark on a quiet PC without any interferences from other programs. It might help to use the '--warmup' or '--prepare' options.
 
Summary
  './taste --all' ran
    1.01 ± 0.09 times faster than './tastec --all'
[~/randomProj/paste]$ hyperfine --warmup 100 --min-runs 1000 './taste public.rtf' './tastec public.rtf'
Benchmark 1: ./taste public.rtf
  Time (mean ± σ):       6.2 ms ±   0.4 ms    [User: 2.6 ms, System: 1.8 ms]
  Range (min … max):     5.7 ms …   9.1 ms    1000 runs
 
  Warning: Statistical outliers were detected. Consider re-running this benchmark on a quiet PC without any interferences from other programs. It might help to use the '--warmup' or '--prepare' options.
 
Benchmark 2: ./tastec public.rtf
  Time (mean ± σ):       6.3 ms ±   0.4 ms    [User: 2.6 ms, System: 1.8 ms]
  Range (min … max):     5.8 ms …   9.6 ms    1000 runs
 
  Warning: Statistical outliers were detected. Consider re-running this benchmark on a quiet PC without any interferences from other programs. It might help to use the '--warmup' or '--prepare' options.
 
Summary
  './taste public.rtf' ran
    1.00 ± 0.09 times faster than './tastec public.rtf'
```
Other interesting notes. `tastec` is 52k but `taste` is only 50k however the dissassembly for taste is 288
lines but for tastec it is 277.

## License
Created by Bryce Wilson. See the license file for the license. If you need anything outside of that or
have any other questions or comments you can visit [my GitHub page](https://github.com/Bryce-MW/) where my
email is listed. You should also be able to find the repository for this project there if you somehow got
this file another way.

