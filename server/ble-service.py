from bleak import BleakClient
import asyncio
import time
import os

# TODO: use the model charac. to identify the ble devices
BLE_ADDRESS 	= "b8:f0:09:cc:9d:6a"
MODEL_NBR_UUID 	= "00002a00-0000-1000-8000-00805f9b34fb"
PR_UUID		    = "177D03BB-A953-4F16-AABF-5246FBB3F72E"

TMP_PR_DATA_FILE   = "/tmp/msn_pr.dat"

# Create the file (there's 100% better way to do this)
open(TMP_PR_DATA_FILE, "wt").close()

async def main(address):
    async with BleakClient(address) as client:
        # model_number = await client.read_gatt_char(MODEL_NBR_UUID)
        # print("Model Number: {0}".format("".join(map(chr, model_number))))
        last_value = 0

        while 1:
            pr = await client.read_gatt_char(PR_UUID)
            value = float(pr)

	        # it's useless here to store every value it's just to
	        # copy paste this easily for other controllers
            with open(TMP_PR_DATA_FILE, "a") as handler:
                delta = value - last_value
                handler.write("%f,%f\n" % (value, delta))
                last_value = value

            await asyncio.sleep(1)

asyncio.run(main(BLE_ADDRESS))
