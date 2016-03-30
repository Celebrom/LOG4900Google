#!/usr/bin/python
import time
import sys
import getopt
import os
import subprocess
import tempfile
import datetime

nbIter = 1
actualIter = 0
actualdire = os.path.dirname(os.path.realpath(__file__))
outputdire = actualdire
chromePath = os.path.join('C:\\', 'Program Files (x86)', 'Google', 'Chrome', 'Application', 'chrome.exe')

def CloseChrome():
    os.system("taskkill /IM chrome.exe /f >nul 2>&1")

def LaunchChrome():
    os.system("start chrome")

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
      '-buffering '
   )
   subprocess.call(
      'xperf.exe -capturestate ChromeStartupETWSession ' +
      'Microsoft-Windows-Win32k:0xfdffffffefffffff+Multi-MAIN+Multi-FrameRate+' +
      'Multi-Input+Multi-Worker+chrome:0xa000000000000200'
   )

def SaveTrace():
   now = datetime.datetime.now()
   tmp_directory = tempfile.gettempdir()
   #Saving trace to disk...
   subprocess.call(
      'xperf.exe -flush "NT Kernel Logger" ' +
      '-f ' + tmp_directory + '\\kernel.etl ' +
      '-flush ChromeStartupETWSession ' +
      '-f ' + tmp_directory + '\\user.etl '
   )
   time.sleep(5)
   #Merging trace...
   subprocess.call(
      'xperf.exe ' +
      '-merge ' + tmp_directory + '\\kernel.etl ' +
      tmp_directory + '"\\user.etl" ' +
      outputdire + '"\\Chrome_Trace_"' + str(actualIter) + '_' + str(
         datetime.date.today()) + '-' + str(now.hour) + 'h-' + str(now.minute) + 'm' + '.etl ' +
      '-compress'
   )
   time.sleep(5)
   #Stripping Chrome symbols
   subprocess.call(
      'python.exe -u ' +
      actualdire + '\StripChromeSymbols.py ' +
      outputdire + '"\\Chrome_Trace_"' + str(actualIter) + '_' + str(
         datetime.date.today()) + '-' + str(now.hour) + 'h-' + str(now.minute) + 'm' + '.etl '
   )
   time.sleep(5)
   #Pre-translating chrome symbols from stripped PDBs to avoid 10-15 minute translation times.
   subprocess.call(
      'xperf -i ' +
      outputdire + '"\\Chrome_Trace_"' + str(actualIter) + '_' + str(
         datetime.date.today()) + '-' + str(now.hour) + 'h-' + str(now.minute) + 'm' + '.etl ' +
      '-tle -tti -a symcache -dbgid'
   )
   time.sleep(5)
   #Preprocessing trace to identify Chrome processes...
   subprocess.call(
      'python.exe -u ' +
      actualdire + '\IdentifyChromeProcesses.py ' +
      outputdire + '"\\Chrome_Trace_"' + str(actualIter) + '_' + str(
         datetime.date.today()) + '-' + str(now.hour) + 'h-' + str(now.minute) + 'm' + '.etl '
   )
   #Finished recording trace


def StopTrace():
   tmp_directory = tempfile.gettempdir()
   subprocess.call(
      'xperf.exe -stop ChromeStartupETWSession -stop "NT Kernel Logger"'
   )


def main(argv):

   try:
      opts, args = getopt.getopt(argv,"hn:c:o:",["nb_iter=","chromePath=","outputDir="])
   except getopt.GetoptError:
      print '\n Example:'
      print 'OnagerLoop.py -n <number of iteration> -c <chrome path> -o <output Directory>\n'
      print '-n --nb_iter <number of iteration> '
      print '     default = 1'
      print '-c --chromePath <chrome path> '
      print '     default = C:\Program Files (x86)\Google\Chrome\Application\chrome.exe'
      print '-o --outputDir'
      print '     default = Launched directory'
      sys.exit(2)
   for opt, arg in opts:
      if opt == '-h':
         print '\n Example:'
         print 'OnagerLoop.py -n <number of iteration> -c <chrome path> -o <output Directory>\n'
         print '-n --nb_iter <number of iteration> '
         print '     default = 1'
         print '-c --chromePath <chrome path> '
         print '     default = C:\Program Files (x86)\Google\Chrome\Application\chrome.exe'
         print '-o --outputDir'
         print '     default = Launched directory'
         sys.exit()
      elif opt in ("-n", "--nb_iter"):
         global nbIter
         nbIter = arg
      elif opt in ("-c", "--chromePath"):
         global chromePath
         chromePath = arg
      elif opt in ("-o", "--outputDir"):
         global outputdire
         outputdire = arg


if __name__ == "__main__":
   main(sys.argv[1:])

   #Warmup Chrome
   CloseChrome()
   LaunchChrome()
   CloseChrome()
   time.sleep(1)

   for num in range(0, int(nbIter)):

      LaunchTrace()
      LaunchChrome()
      time.sleep(3)
      SaveTrace()
      time.sleep(3)
      StopTrace()
      CloseChrome()
      time.sleep(5)
      actualIter += 1













