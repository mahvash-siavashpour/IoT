from multiprocessing.connection import wait
from datetime import datetime, timedelta
import paho.mqtt.client as paho
from paho import mqtt
import requests, json
from requests.structures import CaseInsensitiveDict

class User():
    def __init__(self, user_id, room_id, light):
        self.user_id = user_id
        self.room_id = room_id
        self.light = light
        self.time = datetime.now()


cache_ttl_seconds = 120  # cache TTL seconds
cache ={}

base_url = "http://127.0.0.1:5000"
headers = CaseInsensitiveDict()
headers["Accept"] = "application/json"
headers["Content-Type"] = "application/json"

# local server authentication
data = {"servername":"local_server", "password":"12345678" }
resp = requests.post(base_url+'/server/login', headers=headers, json=data)
print("local server login successfully...")
headers["Authorization"] = "Bearer {}".format(resp.json()['access_token'])



def on_connect(client, userdata, flags, rc, properties=None):
    print("CONNACK received with code %s." % rc)


def on_publish(client, userdata, mid, properties=None):
    # print("mid: " + str(mid))
    pass


def on_subscribe(client, userdata, mid, granted_qos, properties=None):
    print("Subscribed: " + str(mid) + " " + str(granted_qos))


def check_cache(rfid_tag):
    if rfid_tag in cache.keys():
        if datetime.now() -cache[rfid_tag].time < timedelta(seconds=cache_ttl_seconds):
            return cache[rfid_tag]
        else:
            del cache[rfid_tag]
    return None


def check_office_light(msg):
    print("*"*30)
    current_time = datetime.now().strftime("%H:%M")
    office_id = int(msg.payload)
    data = {"office_id": office_id, "current_time":current_time}
    url = base_url + "/checklight"
    resp = requests.post(url, headers=headers, json=data)
    payload = resp.json()['message']
    print("check_office_light: ", payload)
    return payload


def commit_Activity(msg):
    print("*"*30)
    rfid_tag, office_id, activity_type = str(msg.payload)[2:-1].split("_")
    data = {"activity_type": activity_type, "rfid_tag":rfid_tag, "office_id":int(office_id)}
    url = base_url + "/commitactivity"
    resp = requests.post(url, headers=headers, json=data)
    resp = resp.json()
    if int(resp["message"]) > 0:
        print("activity commited")
    return resp, activity_type, rfid_tag


def get_LDR_value(rfid_tag, resp):
    cache_data = check_cache(rfid_tag)
    if cache_data:
        print("data loaded from from cache.")
        light = cache[rfid_tag].light
    else: 
        print("data got from central server")
        light = resp['message']
        if int(light) > 0 :
            cache[rfid_tag] = User(resp['user_id'], resp["room_id"],light)
    return light



def on_message(client, userdata, msg):
    print("*"*30)
    print("recivied: ", msg.topic + " " + str(msg.qos) + " " + str(msg.payload))

    if msg.topic == "/ultrasonic":
        payload = check_office_light(msg)
        client.publish("/time", payload=payload, qos=1)

    if msg.topic =="/rfid":
        resp, activity_type, rfid_tag = commit_Activity(msg)
        if activity_type == "enter":
            light = get_LDR_value(rfid_tag, resp)
            client.publish("/LDR", payload=light, qos=1)




client = paho.Client(client_id="", userdata=None, protocol=paho.MQTTv5)
client.on_connect = on_connect
client.tls_set(tls_version=mqtt.client.ssl.PROTOCOL_TLS)


client.username_pw_set("mahvash", "Mahvash123")
client.connect("2124b8ecf14e4692b62288592e5c4a12.s2.eu.hivemq.cloud", 8883)




client.on_subscribe = on_subscribe
client.on_message = on_message
client.on_publish = on_publish

client.subscribe("/ultrasonic", qos=1)
client.subscribe("/rfid", qos=1)

client.loop_forever()


