# The ViviFire Programming Language

ViviFire is a new open-source programming language with modern features and
a familiar syntax.  Its features include object-oriented, functional,
declarative and modular programming, generics, traits, units of measure, and
more.

ViviFire is alpha software.  The language is not stable.  It can change very
frequently and quickly before it becomes stable.

The source code for ViviFire is released under the MIT license.  And the
documentation is released under Creative Commons CC0 (public domain).

To learn more about the language, [visit the home page](http://vivifire.com).

## Example code

```text
Program Hello
Require ViviFire.IO

PrintLine "Hello world!"
```
---
```text
Program SimpleWindow
Require ViviFire.UI

Object win Is Window
    Height = 120
    Width = 300

    Static text,
        Caption := "There have been no clicks yet.",
        Left := 30, Top := 20

    Button button1,
        Caption := "Click me",
        Left := 100, Top := 50, Height := 25, Width := 100

    button1.OnClick = Sub
        @Shared Var count As Int32
        count += 1
        text.Caption = $"Clicked {count} times."
    End Sub
End Object
```

## Prerequisites & dependencies

C++ 17 is the recommended language version.

Use [Coco/R for C++](https://ssw.jku.at/Research/Projects/Coco/) to compile
VFGrammar.txt.  This generates the source files for the parser and scanner.
