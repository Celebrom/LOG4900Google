#Onager
Onager is a console application used to convert an ETW .etl trace to a format readable by Chrome Tracing (.json).

## How to use Onager

To convert a track, you need to enter the command line:

```
	Onager.exe --trace <Path of the .etl trace>
```
The .json file of the converted trace will be created in the same folder as the original trace. If a .csv of the trace already exist at that location, Onager will use that file instead of generating it again for the trace. Onager is unable to process a trace with a .csv whose size if larger than 3.6 GB.


## OnagerLoop

### What is OnagerLoop
OnagerLoop.py is used to generate, with xperf, multiple .etl files that are automaticaly converted to .csv, with  Onager.exe (using Etw_Reader).
Those .csv files are then automaticaly converted to .Json files, also using with Onager.exe.
Those .Json files can be imported directly to Chrome://tracing for analysing.
With the parameter -w (--html), the user converts, with the help of trace2html, those .Json file in .html that is a reproduction of Chrome://tracing. This .html file contains all the stack of the trace, which can be used to generate a flamegrah to vizualise those stack traces.

### IMPORTANT
All the traces with a .Json file bigger than 256MB will not be able to work due to the javascript architecture used by Chrome://tracing.

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
### Config.py
The Config.py file can be used to change all the parameters found in UIforETW by setting the parameters to true or false.
```
#!/usr/bin/python

#Xperf options
XperfOptions = {
    'Context switch call stacks': True,
    'CPU sampling call stacks': True,
    'GPU tracing': False
}

#Chrome Tracing Categories
ChromeCategories = {
    'benchmark': False,
    'blink': False,
    'browser': False,
    'CC': False,
    'evdev': False,
    'gpu': False,
    'input': False,
    'netlog': False,
    'renderer.scheduler': False,
    'toplevel': True,
    'v8': False,
    'disabled-by-default-cc.debug': False,
    'disabled-by-default-cc.debug.picture': False,
    'disabled-by-default-cc.toplevel.flow': False,
    'startup': False,
    'all-enabled-by-default-event': False,
    'all-disabled-by-default-event': False
}
```

## trace2html
### How to use trace2html
```
usage: python trace2html <options> trace_file1 [trace_file2 ...]

positional arguments:
  trace_files

optional arguments:
  -h, --help            show this help message and exit
  --config {chrome,full,lean,systrace}
                        Picks a browser config. Valid choices: chrome, full,
                        lean, systrace
  --output OUTPUT       Where to put the generated result. If not given, the
                        trace filename is used, with an html suffix.
  --quiet               Dont print the output file name

Takes the provided trace file and produces a standalone HTML file that
contains both the trace and the trace viewer.
```

### Limitations 
```
1. Large JSON (> ~140 MB) will often cause a MemoryError.
2. Chrome tracing can't open JSON files bigger than ~256 MB.
```
