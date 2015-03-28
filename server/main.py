#!/bin/env python3
import tornado.ioloop
import tornado.web
import os
import csv
from json import dumps, loads
from random import choice
import urllib.request
facts = []
locations = {}

upperchance = 0

uppercrime = 0
class APIHandler(tornado.web.RequestHandler):
    def get(self):
        lat = self.get_argument("lat", default=361)
        lon = self.get_argument("lon", default=361)
        error = False
        
        if lat == 361 or lon == 361:
            error = True
        try:
            lat = float(lat)
            lon = float(lon)
        except ValueError:
            error = True
        region = None
        if error:
            region = self.get_argument("region", default=None)
            error = False
            if region == None:
                error = True
        if not error:
            # Get the current location
            crimerate = -1
            if region == None:
                URL = "http://api.openweathermap.org/data/2.5/weather?lat={0}&lon={1}".format(lat,lon)
                jsonstr = urllib.request.urlopen(URL).read()
                weatherdata = loads(jsonstr.decode('utf-8'))
                region = weatherdata['name']
            if region in locations:
                print(region,"has a crimerate of",locations[region])
                crimerate = locations[region]
            else:
                print("Location not found!")
            relativecrimerate = 100 * (crimerate / uppercrime)
            print("Relative Crime Rate in the area:",round(relativecrimerate,2),"%")
            fact = choice(facts)
            timesmorelikely = (relativecrimerate / fact['chance'])
            if relativecrimerate < 0:
                error = True
            response = {'region':region,'error':error,'fact':fact['fact'],'chance':fact['chance'],'timesmorelikely':timesmorelikely,'crimerate':relativecrimerate}
        else:
            response = {'error':error}
        
        self.set_header('Content-Type', 'application/json; charset="utf-8"')
        self.write(dumps(response))


application = tornado.web.Application([
    (r"/api", APIHandler),
    (r'/(.*)', tornado.web.StaticFileHandler, {'path': "../staticpage" })
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
        start = line.index('in ')
        end = line.index(' ',start+3)
        chance = line[line.index('in ')+3:end+1]
        chance = float(chance.replace(',', ''))
        chance = chance
        if upperchance < chance:
            upperchance = chance
        fact = line[end:]
        facts.append({'chance':chance,'fact':fact})
    for fact in facts:
        fact['chance'] = 100 * (fact['chance'] / upperchance)
        
    with open('anti-soc-behav.csv', newline='\n', encoding='ISO-8859-1') as csvfile:
        reader = csv.reader(csvfile, delimiter=',', quotechar='|')
        for row in reader:
            if row[0][0] == "E":
                location = row[1]
                if location not in locations:
                    locations[location] = 1
                locations[location] += 1
        for location in locations:
            if uppercrime < locations[location]:
                uppercrime = locations[location]
        
    application.listen(80)
    tornado.ioloop.IOLoop.instance().start()
