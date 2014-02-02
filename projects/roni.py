from time import time, sleep
from urllib2 import Request, urlopen, URLError, HTTPError

url = "http://195.188.87.186:8888/cam_4.jpg"
out_path = "D:\\filip\\roni"
timeout = 1 #second
num_hours = 10

image_index = 1
def get_image():
    global image_index
    file_name = out_path + "\\roni_%06d.jpg" % image_index
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
