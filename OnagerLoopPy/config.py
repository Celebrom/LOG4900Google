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
    'all-enabled-by-default-event': True,
    'all-disabled-by-default-event': False
}
