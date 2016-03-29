

def WaitForWindow(process_id, class_pattern):
    start_time = time.time()
    while time.time() - start_time < 60:
      if chrome_helper.WindowExists([process_id], class_pattern):
        return True
      time.sleep(1.0)
    return False