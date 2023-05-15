import asyncio
from bleak import BleakClient
from bleak.exc import BleakDeviceNotFoundError
from functools import partial
import json

DATA_FILE = "file.json"

# FPGA2, HOTTOP, DSP, 
async def async_read_value_ble(address):
    async with BleakClient(address) as client:
        v = await client.read_gatt_char("177d03bb-a953-4f16-aabf-5246fbb3f72e")
        return v

def read_value_ble(address):
    return asyncio.run(async_read_value_ble(address))

def log_value(fname, name, value):
    with open(fname, "r") as handler:
        old = json.load(handler)
    
    for data in old:
        if data["title"] == name:
            data["value"] = value

    with open(fname, "w") as handler:
        json.dump(old, handler)
    
while 1:
    try:
        v = read_value_ble("b8:f0:09:cc:9d:6a").decode()
        print("[B8:F0:09:CC:9D:6A] " + v)
        log_value(DATA_FILE, "Luminosite", str(int(float(v)*100)))
    except BleakDeviceNotFoundError: print("[B8:F0:09:CC:9D:6A] error")

    try:
        v = read_value_ble("b8:f0:09:cc:9e:3e").decode()
        print("[b8:f0:09:cc:9e:3e] ".upper() + v)
        log_value(DATA_FILE, "Temperature", v)
    except BleakDeviceNotFoundError: print("[b8:f0:09:cc:9e:3e]".upper() + " error")
