# The ViviFire Programming Language

ViviFire is a brand new open-source programming language with modern features
and a familiar syntax.  It offers object-oriented programming with genericity
and traits, modularity, structured exception handling, and more.

Currently, ViviFire is at an alpha stage of development.  The language is still
evolving, and may change dramatically before it's complete.  If you are
interested in how a language is created, join us and help make it great!

The source code for ViviFire is released under the MIT license.
The documentation is released under Creative Commons CC0, public domain.

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

New Window win
Begin
    .Height = 120
    .Width = 300

    New Static text,
        Caption := "There have been no clicks yet.",
        Left := 30, Top := 20

    New Button button1,
        Caption := "Click me",
        Left := 100, Top := 50, Height := 25, Width := 100
    Begin
        .OnClick = buttonClick
    End
End

Sub buttonClick Handles Button.ClickEvent
    @Shared Var count As Int
    count += 1
    text.Caption = "Clicked " & count & " times."
End
```

## Dependencies

Use [Coco/R for C++](https://ssw.jku.at/Research/Projects/Coco/) to compile
VFGrammar.txt.  This generates the source files for the parser and scanner.
