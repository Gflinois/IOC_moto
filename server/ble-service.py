import asyncio
from bleak import BleakClient
from bleak.exc import BleakDeviceNotFoundError
from functools import partial
import json

READ_CHARAC     = "177d03bb-a953-4f16-aabf-5246fbb3f72e"
WRITE_CHARAC    = "0dcdd589-bd02-42a5-b9c8-973ac9331c53"

DATA_FILE = "file.json"

async def async_read_value_ble(address, charac=READ_CHARAC):
    async with BleakClient(address) as client:
        v = await client.read_gatt_char(charac)
        return v

async def async_write_value_ble(address, v):
    async with BleakClient(address) as client:
        await client.write_gatt_char(WRITE_CHARAC, v)
    
def read_value_ble(address, charac=READ_CHARAC):
    return asyncio.run(async_read_value_ble(address, charac))

def write_value_ble(address, v):
    return asyncio.run(async_write_value_ble(address, v))

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
        print("<- [B8:F0:09:CC:9D:6A] " + v)
        lightning = str(int(float(v)*100))
        log_value(DATA_FILE, "Luminosite", lightning)
    except BleakDeviceNotFoundError: print("<- [B8:F0:09:CC:9D:6A] error")

    try:
        write_value_ble("b8:f0:09:cc:9e:3e", (lightning+"%").encode())
    except BleakDeviceNotFoundError: print("-> [b8:f0:09:cc:9e:3e] error")

    try:
        v = read_value_ble("b8:f0:09:cc:9e:3e").decode()
        print("<- [b8:f0:09:cc:9e:3e] ".upper() + v)
        log_value(DATA_FILE, "Temperature", v)
    except BleakDeviceNotFoundError: print("<- [b8:f0:09:cc:9e:3e]".upper() + " error")
