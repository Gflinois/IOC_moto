import asyncio
from bleak import BleakClient
from bleak.exc import BleakDeviceNotFoundError
from functools import partial
import json

open("file.json", "wt").close()

# FPGA2, HOTTOP, DSP, 
async def async_read_value_ble(address):
    async with BleakClient(address) as client:
        v = await client.read_gatt_char("177d03bb-a953-4f16-aabf-5246fbb3f72e")
        return v

def read_value_ble(address):
    return asyncio.run(async_read_value_ble(address))

def log_value(fname, value):
    with open(fname, "r+") as handler:
        try: 
            old = json.load(handler)
        except:
            old = []
        old.append(value)
        json.dump(old, handler)
    
log_id = 0
while 1:
    try:
        v = read_value_ble("b8:f0:09:cc:9d:6a").decode()
        print("[B8:F0:09:CC:9D:6A] " + v)
        log_value("file.json", {
            "id": log_id,
            "title": "Luminosite",
            "value": v,
            "unit": ""
        })
    except BleakDeviceNotFoundError: print("[B8:F0:09:CC:9D:6A] error")

    try:
        v = read_value_ble("b8:f0:09:cc:9e:3e").decode()
        print("[b8:f0:09:cc:9e:3e] ".upper() + v)
        log_value("file.json", {
            "id": log_id,
            "title": "Temperature",
            "value": v,
            "unit": "°C"
        })
    except BleakDeviceNotFoundError: print("[b8:f0:09:cc:9e:3e]".upper() + " error")

    log_id += 1