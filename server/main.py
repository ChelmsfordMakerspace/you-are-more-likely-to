#!/bin/env python3
import tornado.ioloop
import tornado.web
from json import dumps, loads
from random import choice
import urllib.request
facts = ["Fact A","Fact B","Fact C"]

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
            
        if not error:
            fact = choice(facts)
            chance = 9001
            policeData = loads(urllib.request.urlopen("http://data.police.uk/docs/method/crimes-at-location").read())
            #Work out chance
            response = {'lat':lat,'lon':lon,'error':error,'fact':fact,'chance':chance}
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
    application.listen(80)
    tornado.ioloop.IOLoop.instance().start()
