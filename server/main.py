#!/bin/env python3
import tornado.ioloop
import tornado.web
import os
import csv
from json import dumps, loads
from random import choice
import urllib.request
facts = []
upperchance = 0

uppercrime = 0

latlngcrimes = {}

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
                lat = round(lat,1)
                lon = round(lon,1)
                latlon = str(lat) + str(lon)
                if latlon in latlngcrimes:
                    crimerate = latlngcrimes[latlon]
            if crimerate == -1:
                error = True
            relativecrimerate = 100 * (crimerate / uppercrime)
            print("Relative Crime Rate in the area:",round(relativecrimerate,2),"%")
            fact = choice(facts)
            timesmorelikely = (relativecrimerate / fact['chance'])
            if relativecrimerate < 0:
                error = True
                
        if error:
            response = {'error':error}
        else:
            response = {'region':region,'error':error,'fact':fact['fact'],'chance':fact['chance'],'timesmorelikely':timesmorelikely,'crimerate':relativecrimerate}
            
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
        facts.append({'chance':chance,'fact':fact.strip()})
    for fact in facts:
        fact['chance'] = 100 * (fact['chance'] / upperchance)
    with open('201501.csv', newline='\n', encoding='ISO-8859-1') as csvfile:   
        reader = csv.reader(csvfile, delimiter=',', quotechar='|')
        for row in reader:
            try:
                Lat = round(float(row[5]),1)
                Lan = round(float(row[4]),1)
                LatLan = str(Lat)+str(Lan)
                if LatLan not in latlngcrimes:
                    latlngcrimes[LatLan] = 0
                latlngcrimes[LatLan] += 1
            except:
                pass #Crap Data
    for location in latlngcrimes:
        if uppercrime < latlngcrimes[location]:
            uppercrime = latlngcrimes[location]
        
    application.listen(80)
    tornado.ioloop.IOLoop.instance().start()
