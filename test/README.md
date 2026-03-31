# ViviFire Test Suite

This is the test suite for the ViviFire programming language. It runs a set of
tests that compare the output of `vf.exe` with the correct output for a given
input. The output can include errors and warnings.

The test suite is usually available only if you performed `git clone` on the
code repository. See the [home page](http://vivifire.me) for instructions.

## Variables you can change

In `test-all.bat`, you can change some variables if they are incorrect for your
system. When you open the file in a text editor, these variables are in a
block that starts on line four. But do not change the supplied file.

Make a copy of `test-all.bat`. Then change the variables in your copy.
The table that follows shows which variables you can change.

| Variable | Default value          | Comments
| -------- | ---------------------- | ---------
|`VF_EXE`  |`..\src\release\vf.exe` |relative path to the ViviFire EXE
|`FLAGS`   |`-v0`                   |command-line switches for the EXE
|`LOG_FILE`|`test.log`              |name of the log file
|`VIEWER`  |`start ""`              |program used to open the log file

## Running tests on Windows

 1. In Windows Explorer or a terminal window, change to the directory `test`.
 2. Run `test-all` (or your copy with changes).
    You want to see `All tests passed.`.
 3. If you see, for example, `1 failed. Open test log [Y,N]?`, there was
     a failure in one or more tests. Press `Y` to open the log file. The script
     waits a maximum of 10 seconds.

## The log file

The log file has the name `test.log`, by default. Each time you run the test
suite, it writes a new log file. If there are no failures, the log file is
empty.

## Making a new test

Select the applicable sub-directory under `test` for the given sub-system.
Make a new file with one of the extensions `.vfire` or `.vflib`. The name
must use small letter case with a hyphen between each word, for example,
`test-for-something.vfire`. If there is a file that performs a related test,
try to give the new file a name with one or more of the same words.

Keep the size of the test as small as possible. If the test makes a large
output, we recommend that you divide the test into two or more files. This is
because on Windows, `FINDSTR` has a limit of 511 characters it can compare.

If a test has no output, you can stop here. The test suite ignores the result
with `-- 0 errors`.

## Format of output files

The output file must have the same base name as the test file with the extension
`.err`. For example, if the test file is `"test-for-something.vfire"`, the
output file must be `"test-for-something.err"`.

The output file must be ASCII or ANSI-encoded. It must not contain control
characters, for example, linefeed and carriage return. But it can contain the
end-of-file (EOF) marker.

If your text editor has support for regular expressions, you can perform a
find/replace. Find the text that follows and replace it with nothing:

```regex
[\r\n]+
```
