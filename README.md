# SIMPLE Static Program Analyzer

Static Program Analyzer for source code written in language SIMPLE which analyzes a given program and answers queries on it.

Documentation hosted on Github Pages [here](https://turtle96.github.io/cs3201-doxygen/html/annotated.html).

## Coding Standard 
(Short Version) [C++ Coding Standards by lefticus](https://gist.github.com/lefticus/10191322)  
(Long Version) [Google C++ Style Guide](https://google.github.io/styleguide/cppguide.html)

## Generate Documentation

1. Download [Doxygen](http://www.stack.nl/~dimitri/doxygen/) and [Graphviz](http://www.graphviz.org/)
2. Add `<graphviz directory>/bin` filepath to path in system environment variables
3. Open Command prompt in project directory and run:
```
doxygen doxygenConfig
```
> * Note: running this in Git bash may not work sometimes, so try using command prompt if Git bash does not work
4. Navigate to ./Doxygen/html/index.html and open the file in a browser to view documentation
