#!/bin/bash
python3 server/ble-service.py & >/dev/null
python3 -m streamlit run server/web.py