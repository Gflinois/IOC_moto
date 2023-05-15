# Motorcycle Sensor Network

This Git project contains the code for a network of sensors connected to a Raspberry Pi that collects data from a motorcycle and displays it on a webpage. The sensors include:

- thermistance for monitoring the heat of the engine
- lumistance for measuring light (by connecting a relay you can automate headlights)
- IMU for measuring acceleration and speed in all directions (interraction with website not ready yet because we have no non broken IMU yet)
- Tachimeter spying for RPM monitoring

## Requirements

To use this code, you will need:

- Raspberry Pi 3 or later
- Python 3
- the python libraries shown in the dedicated file
- lumistance
- thermistance
- IMU sensor
- cables to divert the tachimeter's tension
- one esp32 by sensor with ble capability (exept for the IMU which's code is currently being developped for an arduino pro micro)

## Installation

1. Clone this Git repo's server file to your Raspi
2. Install the required Python packages on the Raspberry by running `pip3 install -r requirements.txt`
3. Install the BLE library on the ESPs (respectively arduino pro micro)
4. Install the driver for the correct sensor on the esp32s (respectively arduino pro micro)
5. Start the web server by running `python3 web.py`
6. Start the communication with the ESPs by using `python3 bleservice.py`

## Usage

After installing everything and instaling the sensors on the motorcycle , whatch the magic by connecting to the local server oppened by the raspberry.

## Contribution

Contributions to this Git project are welcome! Please submit pull requests to the `dev` branch and include a detailed description of your changes.


## License

This project is licensed under the MIT License. See the `LICENSE` file for details.

## Easter-egg for the ones that are working on a XJ400 (or that are having airbox issues)

Since airboxes are no longer available on the market for xj400 and as you may know this model is verry tightly packed I designed a 3D printable servicable low profile airbox in BAA. If anyone wants to build it please do, stl cad and gcode for an ender 3 pro are available. If anyone is interested in a simple airbox but for another model please try to modify my design and upload it here, I will be available to help you finish the work if need be.
