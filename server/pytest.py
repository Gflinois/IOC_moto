import json
import time
for i in range (200) :
	with open('file.json', 'r+') as f:
		data = json.load(f)
		data[1]["value"] =i 
		f.seek(0)        # <--- should reset file position to the beginning.
		json.dump(data, f, indent=4)
		f.truncate()     # remove remaining part
	time.sleep(1)
