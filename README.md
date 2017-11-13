# SIMPLE Static Program Analyzer

Static Program Analyzer for source code written in language SIMPLE which analyzes a given program and answers queries on it.

## Coding Standard 
(Short Version) [C++ Coding Standards by lefticus](https://gist.github.com/lefticus/10191322)  
(Long Version) [Google C++ Style Guide](https://google.github.io/styleguide/cppguide.html)

## Documentation

Documentation pages hosted on Github Pages [here](https://turtle96.github.io/cs3201-doxygen/html/annotated.html).

Repository for documentation code [here](https://github.com/turtle96/cs3201-doxygen).

## Generate Documentation

1. Download [Doxygen](http://www.stack.nl/~dimitri/doxygen/) and [Graphviz](http://www.graphviz.org/)
2. Add graphviz /bin filepath to path in system environment variables
3. Open Command prompt in project directory and run:
```
doxygen doxygenConfig
```
> * Note: running this in Git bash may not work sometimes, so try using command prompt if Git bash does not work
4. Navigate to ./Doxygen/html/index.html and open the file in a browser to view documentation

## Tests

1. Build solution in Visual Studio as Release (or Debug depending on which .bat file is used to run)
2. Double-click the release_autotester_automate.bat file in the AutoTester folder to run all the tests for acceptance, regression, stress, system and validation
> * Note: The test cases are in the ./AutoTester/Tests folder
> * Note: The XML outputs will  be in the ./AutoTester/Output folder
4. Navigate to ./AutoTester/Output folder and open the XML files with Firefox to view the results of the test cases
