Configuration: "Release" and "win32"
Compiler: Microsoft Visual Studio Community 2015

To run AutoTester:

1) Navigate to "./Team13/Code13/cs3201-simple-spa" folder (where ./ is the directory you saved it in)
2) Open the EmptyGeneralTesting.sln with Visual Studio 2015 from the cs3201-simple-spa folder
3) Select the "Release" and "Win32" configuration, then under "Build" select "Build Solution".
4) Navigate to "./Team13/Code13/cs3201-simple-spa/Release" folder.
5) Ensure that analysis.xsl is in the "./Team13/Code13/cs3201-simple-spa/Release" folder as well.
6) Open up cmd or Command Prompt on your computer and navigate to the "./Team13/Code13/cs3201-simple-spa/Release" folder.
7) In the cmd window, enter "AutoTester.exe <prog-file> <query-file> <output-file>", 
   replacing the text in <> with the corresponding file paths if the files are not in the Release folder.
   The <output-file> has to be a xml file and it will be in the Release folder if no path is specified.
   For example, "AutoTester.exe ../bonus/pattern_matching_for_expressions_with_plus_source.txt ../bonus/pattern_matching_for_expressions_with_plus_testcases.txt out.xml".
8) Upon completion, use Mozilla Firefox to open the <output-file> to view the results of AutoTester.
