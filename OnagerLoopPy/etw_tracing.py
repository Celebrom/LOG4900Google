#!/usr/bin/python

import subprocess
import tempfile

def StartTracing():
  tmp_directory = tempfile.gettempdir()
  subprocess.call(
      'xperf.exe -start "NT Kernel Logger" ' +
      '-on Latency+POWER+DISPATCHER+DISK_IO_INIT+FILE_IO+FILE_IO_INIT+' +
          'VIRT_ALLOC+MEMINFO ' +
      '-stackwalk PROFILE+CSWITCH+READYTHREAD+DiskReadInit ' +
      '-buffersize 1024 -minbuffers 1200 -maxbuffers 1200 ' +
      '-f "' + tmp_directory + '\\kernel.etl" ' +
      '-start ChromeStartupETWSession ' +
      '-on Microsoft-Windows-Win32k+Multi-MAIN+Multi-FrameRate+Multi-' +
          'Input+Multi-Worker+chrome:0x8000000000000200 ' +
      '-buffersize 1024 -minbuffers 200 -maxbuffers 200 ' +
      '-f "' + tmp_directory + '\\user.etl"')
  subprocess.call(
      'xperf.exe -capturestate ChromeStartupETWSession ' +
      'Microsoft-Windows-Win32k+Multi-MAIN+Multi-FrameRate+Multi-Input+' +
          'Multi-Worker+chrome:0x8000000000000200')

def StopTracing(trace_filename):
  tmp_directory = tempfile.gettempdir()
  subprocess.call(
      'xperf.exe -stop ChromeStartupETWSession -stop "NT Kernel Logger"')
  subprocess.call(
      'xperf.exe -merge ' +
      '"' + tmp_directory + '\\kernel.etl" ' +
      '"' + tmp_directory + '\\user.etl" ' +
      '"' + trace_filename + '" ' +
      '-compress')

  def LaunchETWTest():
      tmp_directory = tempfile.gettempdir()
      subprocess.call(
          'xperf.exe -start "NT Kernel Logger" ' +
          '-on Latency+POWER+DISPATCHER+DISK_IO_INIT+FILE_IO+FILE_IO_INIT+' +
          'VIRT_ALLOC+MEMINFO ' +
          '-stackwalk PROFILE+CSWITCH+READYTHREAD+DiskReadInit ' +
          '-buffersize 1024 -minbuffers 1200 -maxbuffers 1200 ' +
          '-f "' + tmp_directory + '\\kernel.etl" ' +
          '-start ChromeStartupETWSession ' +
          '-on Microsoft-Windows-Win32k+Multi-MAIN+Multi-FrameRate+Multi-' +
          'Input+Multi-Worker+chrome:0x8000000000000200 ' +
          '-buffersize 1024 -minbuffers 200 -maxbuffers 200 ' +
          '-f "' + tmp_directory + '\\user.etl"')
      subprocess.call(
          'xperf.exe -capturestate ChromeStartupETWSession ' +
          'Microsoft-Windows-Win32k+Multi-MAIN+Multi-FrameRate+Multi-Input+' +
          'Multi-Worker+chrome:0x8000000000000200')