#!/usr/bin/env python
from http.server import HTTPServer
from http.server import BaseHTTPRequestHandler
from threading import Thread
from socketserver import ThreadingMixIn
import psutil
import sys
import time

class PrimeCalculatorHandler(BaseHTTPRequestHandler):
    def prime(self, n):
        i = 2
        while i * i <= n:
            if n % i == 0:
                return False
            i += 1
        return True

    def calc(self, n):
        p = 1
        while n > 0:
            p += 1
            if self.prime(p):
                n -= 1
        return p

    def do_GET(self):
        try:
            args = self.path.split('/')
            if len(args) != 2:
                raise Exception()
            n = int(args[1])
            self.send_response(200)
            self.end_headers()
            self.wfile.write(str(self.calc(n)).encode('utf-8'))
        except Exception as ex:
            self.send_response(500)
            self.end_headers()
            print(ex)


class CPULoadHandler(BaseHTTPRequestHandler):

    def do_GET(self):
        try:
            self.send_response(200)
            self.end_headers()
            self.wfile.write(str(psutil.cpu_percent()).encode('utf-8'))
        except Exception as ex:
            self.send_response(500)
            self.end_headers()
            print(ex)

class ThreadingHTTPServer(ThreadingMixIn, HTTPServer):
    pass

def serve_on_port(port,handler):
    #if port != -1:
        server = ThreadingHTTPServer(("localhost",port), handler)
        server.serve_forever()
    #else:
    #    print("Another thread incoming")
    #    try:
    #        while True:
    #            time.sleep(.1)
      #  except KeyboardInterrupt:
      #      print("Attempting to interrupt this...")
     #       os._exit()
    #        print("Interrupt succeeded")

if len(sys.argv) < 3:
    print("python server.py PortPrime PortCPULoad")
else:
    PORTPrime = int(sys.argv[1])
    PORTCPULoad = int(sys.argv[2])
    
    try:
        t1 = Thread(target=serve_on_port, args=[PORTCPULoad,CPULoadHandler])
        t2 = Thread(target=serve_on_port, args=[PORTPrime,PrimeCalculatorHandler])
        t3 = Thread(target=serve_on_port, args=[-1,CPULoadHandler])
 #       t1.setDaemon(True)
  #      t2.setDaemon(True)
   #     t3.setDaemon(True)
        t1.start()
        t2.start()
     #   t3.start()
    except KeyboardInterrupt:
        print("Attempting to interrupt...")
        os._exit()
        print("Interrupt succeeded")



