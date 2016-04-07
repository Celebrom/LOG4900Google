#!/usr/bin/python
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

def LaunchChrome():
    system('start chrome.exe')

def CloseChrome():
    system("taskkill /IM chrome.exe /f >nul 2>&1")

def ChromeStartupProfiler():
   global outputdire
   try:
      while(True):
         system('start chrome.exe --dump-browser-histograms=' + outputdire + '\dump.json')
         sleep(1)
         with open(outputdire + '\dump.json', 'r') as f:
            for line in f:
               index = line.find('NonEmptyPaint')
         if index != -1:
            return
   except:
      print '\n No such directory!'
      print 'Try a new path\n'
      quit()

def LaunchTrace():
   call(
      'xperf.exe -start "NT Kernel Logger" ' +
      '-on Latency+POWER+DISPATCHER+DISK_IO_INIT+FILE_IO+FILE_IO_INIT+' +
         'VIRT_ALLOC+MEMINFO ' +
      '-stackwalk PROFILE+CSWITCH+READYTHREAD+DiskReadInit ' +
      '-buffersize 1024 -minbuffers 600 -maxbuffers 600 ' +
      '-buffering ' +
      '-start ChromeStartupETWSession ' +
      '-on Microsoft-Windows-Win32k:0xfdffffffefffffff+Multi-MAIN+Multi-FrameRate+Multi' +
         '-Input+Multi-Worker+chrome:0xa000000000000200 ' +
      '-buffersize 1024 -minbuffers 200 -maxbuffers 200 ' +
      '-buffering ',
      stdout=FNULL, stderr=STDOUT
   )
   call(
      'xperf.exe -capturestate ChromeStartupETWSession ' +
      'Microsoft-Windows-Win32k:0xfdffffffefffffff+Multi-MAIN+Multi-FrameRate+' +
      'Multi-Input+Multi-Worker+chrome:0xa000000000000200',
      stdout=FNULL, stderr=STDOUT
   )

def SaveTrace():
   global outputFile
   tmp_directory = gettempdir()
   now = datetime.now()

   outputFile = outputdire + '"\\Chrome_Trace_"' + str(actualIter) + '_' + str(
      date.today()) + '-' + str(now.hour) + 'h-' + str(now.minute) + 'm' + '.etl '

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
      '-compress',
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
      '-tle -tti -a symcache -dbgid',
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
   call('Dependencies\Onager.exe --trace ' + outputFile)

def main(argv):

   try:
      opts, args = getopt(argv,'hn:c:o:',['nb_iter=','outputDir='])
   except GetoptError:
      print '\n Example:'
      print 'OnagerLoop.py -n <number of iteration> -o <output Directory>\n'
      print '-n --nb_iter <number of iteration> '
      print '     default = 1'
      print '-o --outputDir'
      print '     default = Launched directory'
      exit(2)
   for opt, arg in opts:
      if opt in ('-h', '--help'):
         print '\n Example:'
         print 'OnagerLoop.py -n <number of iteration> -o <output Directory>\n'
         print '-n --nb_iter <number of iteration> '
         print '     default = 1'
         print '-o --outputDir'
         print '     default = Launched directory'
         exit()
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
         print outputdire + ' 1'

if __name__ == "__main__":
   main(argv[1:])

   if not path.exists(outputdire):
      makedirs(outputdire)
      sleep(1)

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
      actualIter += 1













