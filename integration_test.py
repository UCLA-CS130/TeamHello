import httplib
from subprocess import Popen
from subprocess import check_output
import os
import signal
import filecmp
import re
import subprocess
import time

HOSTNAME = "localhost"
PORT = 8080
CONFIGFILENAME = "config"

def testCasesGenerator(url, method, expected_body, case_number):
    connection = httplib.HTTPConnection(HOSTNAME, PORT)
    connection.request(method, url)
    response = connection.getresponse()
    body = response.read()
    if expected_body == body:
        print "Case{0:d} passed!".format(case_number)
    else:
        print "Case{0:d} failed!".format(case_number)
    connection.close()

def testCase1():
    testCasesGenerator("/echo", "GET", "GET /echo HTTP/1.1\r\nHost: localhost:8080\r\nAccept-Encoding: identity\r\n\r\n", 1)
    
def testCase2():
    testCasesGenerator("/static/b.txt", "GET", "File Not found or No Handlers to handle uri.", 2)
    
def testCase3():
    basePath = ""
    configFile = open(CONFIGFILENAME, 'r')
    for line in configFile:
        tokens = line.lstrip(' ')
        tokens = re.split(' ', tokens)
        if tokens[0] == "base-path" or tokens[0] == "root":
            
            basePath = tokens[1].replace(";\n", "")
            break;
    configFile.close()
    testFile = open(basePath + "/b.txt", 'w')
    testFile.write("abcde")
    testFile.close()
    testCasesGenerator("/static/b.txt", "GET", "abcde", 3)
    os.remove(basePath + "/b.txt")

def testCase4():
    curl_proc = subprocess.call(["curl", "-s", "localhost:8080", "-o", "output1"])
    curl_proc = subprocess.call(["curl", "-s", "my.ucla.edu", "-o", "output2"])
    diff_proc = subprocess.Popen(["diff", "-u", "output1", "output2"], stdout=subprocess.PIPE)
    diff = diff_proc.stdout.read().decode('utf-8')
    os.remove('output1')
    os.remove('output2')
    if diff != '':
        print "Case4 failed!"
        print diff
        exit(1)
    else:
        print "Case4 passed!"

def main():
    
    Popen(["./webserver", CONFIGFILENAME])
    time.sleep(0.01)
    pid = check_output(["pidof", "-s", "./webserver"])
    testCase1()
    testCase2()
    testCase3()
    testCase4()
    
    os.kill(int(pid), signal.SIGKILL)
    
if __name__ == "__main__":
    try:
        main()
    except:
        pid = check_output(["pidof", "-s", "./webserver"])
        if not pid == "":
            os.kill(int(pid), signal.SIGKILL)
        raise

