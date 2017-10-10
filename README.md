# SIMPLE Static Program Analyzer

Static Program Analyzer for source code written in language SIMPLE which analyzes a given program and answers queries on it.

## Coding Standard 
(Short Version) [C++ Coding Standards by lefticus](https://gist.github.com/lefticus/10191322)  
(Long Version) [Google C++ Style Guide](https://google.github.io/styleguide/cppguide.html)

## Documentation

Documentation pages hosted on Github Pages [here](https://turtle96.github.io/cs3201-doxygen/html/annotated.html).

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

1. Double-click the autotester_automate.bat file in the AutoTester folder to run all the tests for acceptance, regression, system and validation
> * Note: The test cases are in the ./AutoTester/Tests folder
> * Note: The XML outputs will  be in the ./AutoTester/Output folder
2. Alternatively, there are other scripts to run just the acceptance/regression/system/validation test cases as listed below:
> * Acceptance Tests: autotester_automate_acceptance.bat
> * Regression Tests: autotester_automate_regression.bat
> * System Tests: autotester_automate_system.bat
> * Validation Tests: autotester_automate_validate.bat
3. Navigate to ./AutoTester/Output folder and open the XML files with Firefox to view the results of the test cases
