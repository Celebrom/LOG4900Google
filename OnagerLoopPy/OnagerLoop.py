#!/usr/bin/python
import time
import sys
import getopt
import os
import subprocess
import tempfile
import datetime

nbIter = 1
actualIter = 1
actualdire = os.path.dirname(os.path.realpath(__file__))
outputdire = actualdire + '\\Traces'
outputFile = ""

def LaunchChrome():
    os.system('start chrome.exe')

def CloseChrome():
    os.system("taskkill /IM chrome.exe /f >nul 2>&1")

def ChromeStartupProfiler():
   while(True):
      os.system('start chrome.exe --dump-browser-histograms=' + outputdire + '\\dump.json')
      time.sleep(1)
      with open(outputdire + '\\dump.json', 'r') as f:
         for line in f:
            index = line.find('NonEmptyPaint')
      if index != -1:
         return

def LaunchTrace():
   subprocess.call(
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
      stdout=subprocess.PIPE
   )
   subprocess.call(
      'xperf.exe -capturestate ChromeStartupETWSession ' +
      'Microsoft-Windows-Win32k:0xfdffffffefffffff+Multi-MAIN+Multi-FrameRate+' +
      'Multi-Input+Multi-Worker+chrome:0xa000000000000200',
      stdout=subprocess.PIPE
   )

def SaveTrace():
   global outputFile
   FNULL = open(os.devnull, 'w')
   tmp_directory = tempfile.gettempdir()
   now = datetime.datetime.now()
   outputFile = outputdire + '"\\Chrome_Trace_"' + str(actualIter) + '_' + str(
      datetime.date.today()) + '-' + str(now.hour) + 'h-' + str(now.minute) + 'm' + '.etl '

   #Saving trace to disk...
   subprocess.call(
      'xperf.exe -flush "NT Kernel Logger" ' +
      '-f ' + tmp_directory + '\\kernel.etl ' +
      '-flush ChromeStartupETWSession ' +
      '-f ' + tmp_directory + '\\user.etl ',
      stdout=FNULL, stderr=subprocess.STDOUT
   )
   #Merging trace...
   subprocess.call(
      'xperf.exe ' +
      '-merge ' + tmp_directory + '\\kernel.etl ' +
      tmp_directory + '"\\user.etl" ' +
      outputFile +
      '-compress',
      stdout=FNULL, stderr=subprocess.STDOUT
   )
   #Stripping Chrome symbols
   subprocess.call(
      'python.exe -u ' +
      actualdire + '\Dependencies\StripChromeSymbols.py ' +
      outputFile,
      #stdout=FNULL, stderr=subprocess.STDOUT
   )
   #Pre-translating chrome symbols from stripped PDBs to avoid 10-15 minute translation times.

   subprocess.call(
      'xperf -i ' +
      outputFile +
      '-tle -tti -a symcache -dbgid',
      stdout=FNULL, stderr=subprocess.STDOUT
   )
   #Preprocessing trace to identify Chrome processes...
   subprocess.call(
      'python.exe -u ' +
      actualdire + '\Dependencies\IdentifyChromeProcesses.py ' +
      outputFile,
      #stdout=FNULL, stderr=subprocess.STDOUT
   )
   #Finished recording trace

def StopTrace():
   tmp_directory = tempfile.gettempdir()
   subprocess.call('xperf.exe -stop ChromeStartupETWSession -stop "NT Kernel Logger"')

def ConvertEtlToJson():
   global outputFile
   subprocess.call(
      'Dependencies\Onager.exe --trace ' + outputFile
   )

def main(argv):

   try:
      opts, args = getopt.getopt(argv,'hn:c:o:',['nb_iter=','outputDir='])
   except getopt.GetoptError:
      print '\n Example:'
      print 'OnagerLoop.py -n <number of iteration> -o <output Directory>\n'
      print '-n --nb_iter <number of iteration> '
      print '     default = 1'
      print '-o --outputDir'
      print '     default = Launched directory'
      sys.exit(2)
   for opt, arg in opts:
      if opt in ('-h', '--help'):
         print '\n Example:'
         print 'OnagerLoop.py -n <number of iteration> -o <output Directory>\n'
         print '-n --nb_iter <number of iteration> '
         print '     default = 1'
         print '-o --outputDir'
         print '     default = Launched directory'
         sys.exit()
      elif opt in ('-n', '--nb_iter'):
         global nbIter
         nbIter = arg
      elif opt in ('-o', '--outputDir'):
         global outputdire
         outputdire = arg

if __name__ == "__main__":
   main(sys.argv[1:])

   if not os.path.exists(outputdire):
      os.makedirs(outputdire)

   #Warmup Chrome
   CloseChrome()
   LaunchChrome()
   CloseChrome()

   for num in range(0, int(nbIter)):
      LaunchTrace()
      LaunchChrome()
      ChromeStartupProfiler()
      SaveTrace()
      StopTrace()
      CloseChrome()
      ConvertEtlToJson()
      actualIter += 1













