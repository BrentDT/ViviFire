================================================================================
ViviFire Test Suite
================================================================================

--------------------------------------------------------------------------------
Installation
--------------------------------------------------------------------------------

Extract all files and folders into the folder containing "VF.exe".

If you had a previous version of the test suite, you should delete the old
"test" folder before installing the new version.
--------------------------------------------------------------------------------
Running a Test
--------------------------------------------------------------------------------

To run all test, execute "test-all.bat". Results are written to "test.log".

If a test fails, "test.log" will contain one or more lines telling which
test (or tests) failed.

The last line in the log tells how many test failed. You want to see the
following:

0 failed.

--------------------------------------------------------------------------------
Test Files & Results Files
--------------------------------------------------------------------------------

Inside each subfolder are several files. The files with the ".in.txt" extension
contain ViviFire source code that is being tested.

With each test file there is a file with the same name but having a ".v0.txt"
extension. These are the results files and represent the output captured by a
previous version of "VF.exe" run against the test file.

When you run "test-all.bat", the test files are passed to "VF.exe" and the
output is sent to a temporary file. Then this output can be compared to the
results file. If these files are exactly alike, the test passes.

--------------------------------------------------------------------------------
Additional Executable Files
--------------------------------------------------------------------------------

* rebuild-all.bat

Runs through all of the test files, either creating or regenerating the
results files.

* remove-all.bat

Deletes all of the results files.
