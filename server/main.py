#!/bin/env python3
import tornado.ioloop
import tornado.web
import os
from json import dumps, loads
from random import choice
import urllib.request
facts = []

class APIHandler(tornado.web.RequestHandler):
    def get(self):
        lat = self.get_argument("lat", default=361)
        lon = self.get_argument("lon", default=361)
        error = False
        global facts
        if lat == 361 or lon == 361:
            error = True
        try:
            lat = int(lat)
            lon = int(lon)
        except ValueError:
            error = True
        region = None
        if error:
            region = self.get_argument("region", default=None)
            if region == None:
                error = True
            
        if not error:
            fact = choice(facts)
            #Work out chance
            if region == None:
                response = {'lat':lat,'lon':lon,'error':error,'fact':fact['fact'],'chance':fact['chance']}
            else:
                response = {'region':region,'error':error,'fact':fact['fact'],'chance':fact['chance']}
        else:
            response = {'error':error}
        
        self.set_header('Content-Type', 'application/json; charset="utf-8"')
        self.write(dumps(response))

class MainHandler(tornado.web.RequestHandler):
    def get(self):
        self.write("Homepage not set up yet!")


application = tornado.web.Application([
    (r"/", MainHandler),
    (r"/main.html", MainHandler),
    (r"/index.html", MainHandler),
    (r"/api", APIHandler)
])

if __name__ == "__main__":
    #Load Data File
    f = open(os.getcwd() + "/../Stupid Statistics")
    lines = []
    for line in f.readlines():
        if line != "\n":
            lines.append(line[:-1])
    #Parse to numbers
    for line in lines:
        chance = line[line.index('in ')+3:line.index(' chance')+1]
        chance = float(chance.replace(',', ''))
        chance = chance
        fact = line[line.index('chance'):]
        facts.append({'chance':chance,'fact':fact})
    application.listen(80)
    tornado.ioloop.IOLoop.instance().start()
