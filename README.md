## OnagerLoop

### What is OnagerLoop
OnagerLoop.py is used to generate, with xperf, multiple .etl files that are automaticaly converted to .csv, with  Onager.exe (using Etw_Reader).
Those .csv files are also automaticaly converted to .Json files, also with Onager.exe.
Those .Json files can be imported directly to Chrome://tracing for analysing.
With the parameter -w (--html), the user can convert those .Json file in .html that are a reproduction of Chrome://tracing but contains all the stack
traces of the trace, which can be used to generate a flamegrah to vizualise those stack traces.

### IMPORTANT
All the traces with a .Json file bigger than 256MB will not be able to work due to the javascript architecture used by Chrome://tracing

### How to use OnagerLoop
```
OnagerLoop.py -w -n <number of iteration> -o <output Directory>
  Possible parameters:
    -h --help : You are watching it
    -w --html : It convert the .Json to .html
      default = Not activated
    -n --nb_iter <number of iteration> 
      default = 1
    -o --outputDir
      default = Launched directory
```
      
