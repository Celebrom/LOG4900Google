import etw_tracing
import optparse
import os
import re
import shutil
import subprocess
import sys
import tempfile
import time
import win32process

from launch_chrome import WaitForWindow
from quit_chrome import QuitChrome

def AddQuotes(str):
  return '"%s"' % str

def RunAsyncCommand(cmd):
  # Use CreateProcess rather than subprocess.Popen to avoid side effects such as
  # handle interitance.
  _, _, process_id, _ = win32process.CreateProcess(None, cmd, None,
                                                   None, 0, 0, None, None,
                                                   win32process.STARTUPINFO())
  return process_id

def LaunchChrome(chrome_path, chrome_flags, profile_path):
  cmd = (AddQuotes(chrome_path) + " " + chrome_flags +
      " --user-data-dir=" + AddQuotes(profile_path))
  process_id = RunAsyncCommand(cmd)
  if not WaitForWindow(process_id, 'Chrome_WidgetWin_'):
    raise Exception('Could not launch Chrome.')
  return 0

def EmptyStandbyList(empty_standby_list_tool_path):
  if subprocess.call(empty_standby_list_tool_path) != 0:
    raise Exception(
        "Could not empty standby list. Do you have elevated privileges?")

def WaitForFirstPaint(chrome_path, profile_path, histogram_dump_path):
  time.sleep(10)
  cmd = (AddQuotes(chrome_path) + " --user-data-dir=" +
         AddQuotes(profile_path) + " --dump-browser-histograms=" +
         AddQuotes(histogram_dump_path))

  while(True):
    RunAsyncCommand(cmd)
    time.sleep(6)
    with open(histogram_dump_path, 'r') as f:
      for line in f:
        index = line.find('NonEmptyPaint')

        if index != -1:
          return

def GetHistogramValue(histogram_dump_path, metric_name):
  with open(histogram_dump_path, 'r') as f:
    for line in f:
      index = line.find(metric_name)

      if index == -1:
        raise Exception("Error finding the value of {0}.".format(metric_name))

      first_sum = line.find(r'"sum"', index)
      second_sum = line.find(r'"sum"', first_sum + 1)

      for item in re.findall(r'"sum":\d+.\d+', line[first_sum:second_sum]):
        for value in re.findall(r'\d+.\d+', item):
          return float(value)

      raise Exception("Error finding the value of {0}.".format(metric_name))

def WriteMetricsHeader(metrics_file, metrics):
  for metric in metrics:
    metrics_file.write(metric + ',')
  metrics_file.write('\n')

def WriteMetrics(metrics_file, metrics, histogram_dump_path):
  for metric in metrics:
    value = GetHistogramValue(histogram_dump_path, metric)
    metrics_file.write(str(value) + ',')
  metrics_file.write('\n')
  metrics_file.flush()

def TestChromeStartup(empty_standby_list_tool_path, chrome_path,
                      chrome_path_warmup, user_data_dir, user_data_dir_warmup,
                      chrome_flags, metrics_path, trace_path, nb_iter, metrics):
  # Make sure chrome is closed.
  QuitChrome(chrome_path)

  # Generate an histogram dump path.
  histogram_dump_dir = tempfile.mkdtemp()
  histogram_dump_path = os.path.join(histogram_dump_dir, 'dump.json')

  # Add flag to export Chrome events to ETW if tracing is enabled.
  if (trace_path):
    chrome_flags += " --trace-export-events-to-etw"

  # Open the output file.
  with open(metrics_path, 'w') as metrics_file:

    # Write the header of the metrics file.
    WriteMetricsHeader(metrics_file, metrics)

    for i in range(nb_iter):
      print 'Iteration #' + str(i)

      print 'Emptying standby list'
      EmptyStandbyList(empty_standby_list_tool_path)

      print 'Launching warmup Chrome'
      LaunchChrome(chrome_path_warmup, "", user_data_dir_warmup)
      WaitForFirstPaint(chrome_path_warmup, user_data_dir_warmup,
                        histogram_dump_path)
      QuitChrome(chrome_path_warmup)

      if (trace_path):
        print 'Starting ETW'
        etw_tracing.StartTracing()

      print 'Launching Chrome'
      LaunchChrome(chrome_path, chrome_flags, user_data_dir)
      WaitForFirstPaint(
          chrome_path, user_data_dir, histogram_dump_path)

      if (trace_path):
        print 'Stopping ETW'
        etw_tracing.StopTracing(trace_path)

      print 'Exiting Chrome'
      QuitChrome(chrome_path)

      # Write metrics for this iteration.
      WriteMetrics(metrics_file, metrics, histogram_dump_path)

  # Clean up.
  shutil.rmtree(histogram_dump_dir, ignore_errors=True)

def main():
  usage = 'usage: %prog [options]'

  # Define options.
  parser = optparse.OptionParser(usage,
                                 description="Repeat experiment for stats")
  parser.add_option('--empty_standby_list_tool_path',
                    help='The path of empty_standby_list_tool.exe.')
  parser.add_option('--chrome_path',
                    help='The path of chrome exe used for tests.')
  parser.add_option('--chrome_path_warmup',
                    help='The path of chrome exe used for warm-up.')
  parser.add_option('--user_data_dir',
                    help='The path for the tested user data directory.')
  parser.add_option('--user_data_dir_warmup',
                    help='The path for the warm-up user data directory.')
  parser.add_option('--chrome_flags',
                    help='Flags used to launch Chrome.')
  parser.add_option('--metrics_path',
                    help='The path of the metrics file.')
  parser.add_option('--trace_path',
                    help='The path of the ETW trace. Empty to disable tracing.')
  parser.add_option('--nb_iter',
                    type='int',
                    default='1',
                    help='The number of iterations to do.')
  parser.add_option('--metrics',
                    help='The metrics to collect for each iteration.')
  options, _ = parser.parse_args()

  empty_standby_list_tool_path = options.empty_standby_list_tool_path
  chrome_path = options.chrome_path
  chrome_path_warmup = options.chrome_path_warmup
  user_data_dir = options.user_data_dir
  user_data_dir_warmup = options.user_data_dir_warmup
  chrome_flags = options.chrome_flags
  metrics_path = options.metrics_path
  trace_path = options.trace_path
  nb_iter = options.nb_iter
  metrics = [metric.strip() for metric in options.metrics.split(',')]

  if nb_iter < 1:
    raise ValueError('The iteration number must be a non-zero positive '
                     'integer.')

  if not empty_standby_list_tool_path:
    parser.error('Please specify the --empty_standby_list_tool_path option.')
    return 1

  if not chrome_path:
    parser.error('Please specify the --chrome_path option.')
    return 1

  if not user_data_dir:
    parser.error('Please specify the --user_data_dir option.')
    return 1

  if not user_data_dir_warmup:
    parser.error('Please specify the --user_data_dir_warmup option.')
    return 1

  if not metrics_path:
    parser.error('Please specify the --metrics_path option.')

  if not chrome_path_warmup:
    chrome_path_warmup = chrome_path

  if not chrome_flags:
    chrome_flags = ""

  TestChromeStartup(empty_standby_list_tool_path, chrome_path,
                    chrome_path_warmup, user_data_dir, user_data_dir_warmup,
                    chrome_flags, metrics_path, trace_path, nb_iter, metrics)

if __name__ == '__main__':
  sys.exit(main())