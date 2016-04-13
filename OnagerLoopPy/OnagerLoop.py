#!/usr/bin/python
import config
from time import sleep
from sys import argv
from sys import exit
from getopt import getopt
from getopt import GetoptError
from os import path
from os import devnull
from os import system
from os import makedirs
from subprocess import call
from subprocess import STDOUT
from tempfile import gettempdir
from datetime import datetime
from datetime import date

nbIter = 1
actualIter = 1
actualdire = path.dirname(path.realpath(__file__))
outputdire = actualdire + '\\Traces'
FNULL = open(devnull, 'w')
ChromeConfigs = ''
XperfConfigs = ''
CpuConfigs = ''
buffSize = ''
html = False

def LaunchChrome():
    system('start chrome.exe')

def CloseChrome():
    system("taskkill /IM chrome.exe /f >nul 2>&1")

def ChromeStartupProfiler():
   global outputdire

   try:
      if not path.exists(outputdire):
         makedirs(outputdire)

      if not path.exists(outputdire + '\dump.json'):
         file = open(outputdire + '\dump.json', 'w+')
         file.close()

      while(True):
         system('start chrome.exe --dump-browser-histograms=' + outputdire + '\dump.json')
         sleep(0.3)
         with open(outputdire + '\dump.json', 'r') as f:
            for line in f:
               index = line.find('NonEmptyPaint')
               if index != -1:
                  return
   except:
      ChromeStartupProfiler()


def LaunchTrace():
   global ChromeConfigs
   global CpuConfigs
   global buffSize
   call(
      'xperf.exe -start "NT Kernel Logger" ' +
      '-on Latency+POWER+DISPATCHER+DISK_IO_INIT+FILE_IO+FILE_IO_INIT+' +
         'VIRT_ALLOC+MEMINFO ' +
      '-stackwalk ' + XperfConfigs + ' ' +
      '-buffersize 1024 -minbuffers 600 -maxbuffers 600 ' +
      '-buffering ' +
      '-start ChromeStartupETWSession ' +
      '-on Microsoft-Windows-Win32k:0xfdffffffefffffff+Multi-MAIN+Multi-FrameRate+Multi' +
         '-Input+Multi-Worker+chrome:' + ChromeConfigs + CpuConfigs + ' ' +
      '-buffersize 1024 -minbuffers ' + buffSize + ' -maxbuffers ' + buffSize + ' ' +
      '-buffering ',
      stdout=FNULL, stderr=STDOUT
   )
   call(
      'xperf.exe -capturestate ChromeStartupETWSession ' +
      'Microsoft-Windows-Win32k:0xfdffffffefffffff+Multi-MAIN+Multi-FrameRate+' +
      'Multi-Input+Multi-Worker+chrome:' + ChromeConfigs + CpuConfigs,
      stdout=FNULL, stderr=STDOUT
   )

def SaveTrace():
   global outputFile
   tmp_directory = gettempdir()
   now = datetime.now()

   outputFile = outputdire + '"\\Chrome_Trace_"' + str(actualIter) + '_' + str(
      date.today()) + '-' + str(now.hour) + 'h-' + str(now.minute) + 'm' + '.etl'

   #Saving trace to disk...
   call(
      'xperf.exe -flush "NT Kernel Logger" ' +
      '-f ' + tmp_directory + '\kernel.etl ' +
      '-flush ChromeStartupETWSession ' +
      '-f ' + tmp_directory + '\user.etl ',
      stdout=FNULL, stderr=STDOUT
   )
   #Merging trace...
   call(
      'xperf.exe ' +
      '-merge ' + tmp_directory + '\kernel.etl ' +
      tmp_directory + '\user.etl ' +
      outputFile +
      ' -compress',
      stdout=FNULL, stderr=STDOUT
   )
   #Stripping Chrome symbols
   call(
      'python.exe -u ' +
      actualdire + '\Dependencies\StripChromeSymbols.py ' +
      outputFile,
      stdout=FNULL, stderr=STDOUT
   )
   #Pre-translating chrome symbols from stripped PDBs to avoid 10-15 minute translation times.

   call(
      'xperf -i ' +
      outputFile +
      ' -tle -tti -a symcache -dbgid',
      stdout=FNULL, stderr=STDOUT
   )
   #Preprocessing trace to identify Chrome processes...
   call(
      'python.exe -u ' +
      actualdire + '\Dependencies\IdentifyChromeProcesses.py ' +
      outputFile,
      stdout=FNULL, stderr=STDOUT
   )
   #Finished recording trace

def StopTrace():
   call('xperf.exe -stop ChromeStartupETWSession -stop "NT Kernel Logger"')

def ConvertEtlToJson():
   try:
      call('Dependencies\Onager.exe --trace ' + outputFile)
   except:
      print "Onager.exe crashed because the trace was to big..."

def ConvertJsonToHtml():
   call('python Dependencies\\tracing\\bin\\trace2html "' + outputFile + '.csv.json"')

def ParseConfig():
   catValue = 0
   if (config.ChromeCategories['benchmark']):
      catValue += 1
   if (config.ChromeCategories['blink']):
      catValue += 2
   if (config.ChromeCategories['browser']):
      catValue += 4
   if (config.ChromeCategories['CC']):
      catValue += 8
   if (config.ChromeCategories['evdev']):
      catValue += 10
   if (config.ChromeCategories['gpu']):
      catValue += 20
   if (config.ChromeCategories['input']):
      catValue += 40
   if (config.ChromeCategories['netlog']):
      catValue += 80
   if (config.ChromeCategories['renderer.scheduler']):
      catValue += 100
   if (config.ChromeCategories['toplevel']):
      catValue += 200
   if (config.ChromeCategories['v8']):
      catValue += 400
   if (config.ChromeCategories['disabled-by-default-cc.debug']):
      catValue += 800
   if (config.ChromeCategories['disabled-by-default-cc.debug.picture']):
      catValue += 1000
   if (config.ChromeCategories['disabled-by-default-cc.toplevel.flow']):
      catValue += 2000
   if (config.ChromeCategories['startup']):
      catValue += 4000

   global ChromeConfigs
   ChromeConfigs = '0x800000000000' + parseCatValue(catValue)

   if (config.ChromeCategories['all-enabled-by-default-event']):
      ChromeConfigs = '0xa00000000000' + parseCatValue(catValue)
   if (config.ChromeCategories['all-disabled-by-default-event']):
      ChromeConfigs = '0xc00000000000' + parseCatValue(catValue)
   if (config.ChromeCategories['all-disabled-by-default-event'] and
          config.ChromeCategories['all-disabled-by-default-event']):
      ChromeConfigs = '0xe00000000000' + parseCatValue(catValue)

   global CpuConfigs
   global buffSize
   if (config.XperfOptions['GPU tracing']):
      CpuConfigs = '+Microsoft-Windows-DxgKrnl:0xFFFF:5+Microsoft-Windows-MediaEngine'
      buffSize = '300'
   else:
      CpuConfigs = ''
      buffSize = '200'

   global XperfConfigs
   if (config.XperfOptions['CPU sampling call stacks']):
      XperfConfigs += 'PROFILE'
   if (config.XperfOptions['CPU sampling call stacks'] and
          config.XperfOptions['Context switch call stacks']):
      XperfConfigs += '+CSWITCH+READYTHREAD+DiskReadInit'
   elif (config.XperfOptions['Context switch call stacks']):
      XperfConfigs += 'CSWITCH+READYTHREAD+DiskReadInit'

def parseCatValue(catValue):
   if (catValue == 0):
      return '0000'
   elif (catValue < 10):
      return '000' + str(catValue)
   elif (catValue < 100):
      return '00' + str(catValue)
   elif (catValue < 1000):
      return '0' + str(catValue)
   else:
      return str(catValue)

def main(argv):

   try:
      opts, args = getopt(argv,'hw:n:o:',['html','nb_iter=','outputDir='])
   except GetoptError:
      print '\n Example:'
      print 'OnagerLoop.py -w -n <number of iteration> -o <output Directory>\n'
      print '-w --html : It convert the .Json to .html'
      print '     default = Not activated'
      print '-n --nb_iter <number of iteration> '
      print '     default = 1'
      print '-o --outputDir'
      print '     default = Launched directory'
      exit(2)
   for opt, arg in opts:
      if opt in ('-h', '--help'):
         print '\n Example:'
         print 'OnagerLoop.py -w -n <number of iteration> -o <output Directory>\n'
         print '-w --html : It convert the .Json to .html'
         print '     default = Not activated'
         print '-n --nb_iter <number of iteration> '
         print '     default = 1'
         print '-o --outputDir'
         print '     default = Launched directory'
         exit()
      elif opt in ('-w', '--html'):
         global html
         html = True
      elif opt in ('-n', '--nb_iter'):
         global nbIter
         nbIter = arg
      elif opt in ('-o', '--outputDir'):
         global outputdire
         outputdire = arg
         if outputdire.find('./') != -1:
            outputdire = outputdire.replace('./', actualdire + '\\')
         elif outputdire.find('.\\') != -1:
            outputdire = outputdire.replace('.\\', actualdire + '\\')
         elif outputdire:
            outputdire = actualdire + '\\' + arg

if __name__ == "__main__":
   main(argv[1:])

   ParseConfig()

   #Warmup Chrome
   CloseChrome()
   LaunchChrome()
   CloseChrome()

   for num in range(0, int(nbIter)):
      LaunchTrace()
      LaunchChrome()
      ChromeStartupProfiler()
      print 'Creation of trace #' + str(actualIter)
      SaveTrace()
      StopTrace()
      CloseChrome()
      ConvertEtlToJson()
      if html:
         ConvertJsonToHtml()
      actualIter += 1













