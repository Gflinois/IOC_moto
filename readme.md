# Motorcycle Sensor Network

This Git project contains the code for a network of sensors connected to a Raspberry Pi that collects data from a motorcycle and displays it on a webpage. The sensors include:

- IMU for measuring acceleration and speed in all directions
- Gas sensor for monitoring fuel levels
- RPM sensor for tracking engine speed

## Requirements

To use this code, you will need:

- Raspberry Pi 3 or later
- Python 3
- Flask web framework
- IMU sensor
- Gas sensor
- RPM sensor

## Installation

1. Clone this Git repository onto your Raspberry Pi
2. Install the required Python packages by running `pip3 install -r requirements.txt`
3. Connect the sensors to your Raspberry Pi according to the instructions provided
4. Start the web server by running `streamlit run test_st.py`

## Usage

The dashboard displays real-time data from the sensors, including:

- Acceleration and speed in all directions
- Fuel levels
- Engine speed

You can also view historical data by selecting a date range on the dashboard.

## Contribution

Contributions to this Git project are welcome! Please submit pull requests to the `dev` branch and include a detailed description of your changes.

## License

This project is licensed under the MIT License. See the `LICENSE` file for details.


