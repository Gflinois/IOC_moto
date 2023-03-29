import streamlit as st
import pandas as pd

# Load data
df = pd.read_csv('data.csv')


# Set page title
st.set_page_config(page_title='Motorcycle Stats Dashboard')

# Set page layout
col1, col2 = st.columns(2)

# Display speedometer and stats
with col1:
    st.header('Speedometer')
#    st.image('speedometer.png', width=200)
    st.subheader('Speed Stats')
    st.write(f"Average Speed: {df['speed'].mean():.2f} km/h")
    st.write(f"Max Speed: {df['speed'].max():.2f} km/h")
    st.write(f"Min Speed: {df['speed'].min():.2f} km/h")

# Display gas gauge and stats
with col2:
    st.header('Gas Gauge')
#    st.image('gas_gauge.png', width=200)
    st.subheader('Gas Stats')
    st.write(f"Average Gas Level: {df['gas'].mean():.2f} L")
    st.write(f"Max Gas Level: {df['gas'].max():.2f} L")
    st.write(f"Min Gas Level: {df['gas'].min():.2f} L")

# Display RPM graph
st.header('RPM Graph')
st.line_chart(df['rpm'])

# Display acceleration graph
st.header('Acceleration Graph')
st.line_chart(df[['accel_x', 'accel_y', 'accel_z']])

# Display orientation graph
st.header('Orientation Graph')
mapdat = df[['roll','accel_x']].rename(columns={'roll':'lat','accel_x':'lon'})

st.map(mapdat)
