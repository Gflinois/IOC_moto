from bleak import BleakClient
import asyncio
import fcntl
import json
import time
import os

# TODO: use the model charac. to identify the ble devices
TR_BLE_ADDRESS 	= "b8:f0:09:cc:9d:6a"
PR_BLE_ADDRESS  = "b8:f0:09:cc:9e:3e"

MODEL_NBR_UUID 	= "00002a00-0000-1000-8000-00805f9b34fb"
ATTR_UUID		= "177D03BB-A953-4F16-AABF-5246FBB3F72E"

DATA_FILE   = "file.json"

# Create the file (there's 100% better way to do this)
open(DATA_FILE, "wt").close()

async def photoresistor_loop(address):
    print("Starting photoresistor loop ...")

    async with BleakClient(address) as client:
        # model_number = await client.read_gatt_char(MODEL_NBR_UUID)
        # print("Model Number: {0}".format("".join(map(chr, model_number))))
        last_value = 0

        while 1:
            pr = await client.read_gatt_char(ATTR_UUID)
            print(b"pr: " + pr)
            value = float(pr)

            with open(DATA_FILE, "a") as handler:
                # Lock the file
                #fcntl.flock(handler.fileno(), fcntl.LOCK_EX)

                delta = value - last_value
                last_value = value
                """
                try:
                    prev_data = json.load(handler)
                    last_id = prev_data[-1]
                except json.decoder.JSONDecodeError as e:
                    print(e)
                    prev_data = []
                    last_id = 0
                """
                """prev_data.append({
                    "id": last_id + 1,
                    "title": "Luminosite",
                    "value": str(value),
                    "delta": str(delta),
                    "unit": ""
                })
                json.dump(prev_data, handler)"""
                """
                print(prev_data)

                # Unlock
                fcntl.flock(handler.fileno(), fcntl.LOCK_UN)
                """
                handler.write(str({
                    "id": 1,
                    "title": "Luminosite",
                    "value": str(value),
                    "delta": str(delta),
                    "unit": ""
                })+"\n")

            await asyncio.sleep(1)


async def thermistor_loop(address):
    print("Starting thermistor loop ...")

    async with BleakClient(address) as client:
        while 1:
            pr = await client.read_gatt_char(ATTR_UUID)
            print(b"tr: " + pr)
            value = float(pr)

            with open(DATA_FILE, "w+") as handler:
                # Lock the file
                """
                fcntl.flock(handler.fileno(), fcntl.LOCK_EX)

                try:
                    prev_data = json.load(handler)
                    last_id = prev_data[-1]
                except json.decoder.JSONDecodeError:
                    prev_data = []
                    last_id = 0

                prev_data.append({
                    "id": last_id + 1,
                    "title": "Temperature",
                    "value": str(value),
                    "unit": ""
                })
                json.dump(prev_data, handler)

                # Unlock
                fcntl.flock(handler.fileno(), fcntl.LOCK_UN)
                """
                handler.write(str({
                    "id": last_id + 1,
                    "title": "Temperature",
                    "value": str(value),
                    "unit": ""
                })+"\n")
            await asyncio.sleep(1)

async def main():
    pr_task = asyncio.create_task(photoresistor_loop(PR_BLE_ADDRESS))
    tr_task = asyncio.create_task(thermistor_loop(TR_BLE_ADDRESS))

    await pr_task
    await tr_task

asyncio.run(main())