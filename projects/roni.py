from time import time, sleep
from urllib2 import Request, urlopen, URLError, HTTPError
import os, errno
import datetime

url = "http://195.188.87.186:8888/cam_4.jpg"
out_path = os.getcwd() + "\\roni"

timeout = 1 #second
num_hours = 10

if not os.path.exists(out_path):
    os.makedirs(out_path)

image_index = 1
def get_image():
    global image_index
    global out_path

    new_path = out_path + "\\" + datetime.datetime.now().strftime("%Y-%m-%d")
    if not os.path.exists(new_path):
        os.makedirs(new_path)

    filename1 = datetime.datetime.now().strftime("%H-%M-%S")
    file_name = new_path + "\\roni_" + filename1 + ".jpg"
    print file_name
    image_index += 1
    try:
        req = Request(url)
        f = urlopen(req)
        local_file = open(file_name, "wb")
        local_file.write(f.read())
        local_file.close()
    except HTTPError, e:
        print "HTTP Error:",e.code , url
    except URLError, e:
        print "URL Error:",e.reason , url

end_time = time() + num_hours * 3600 # time() is calculated in seconds

while time() < end_time:
    get_image()
    sleep(timeout)
