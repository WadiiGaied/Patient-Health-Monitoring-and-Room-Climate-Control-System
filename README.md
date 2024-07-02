## Description
This project focuses on real-time health monitoring for patients, where each patient's health data is monitored within their room. Data from each patient is collected and transmitted to a central card using a communication protocol(CAN BUS). Once collected at the central card, the data is sent to a cloud server for analysis and storage. Additionally, a user interface (GUI) has been developed to visualize patient data in case of cloud server downtime. As a future enhancement, remote control of room climate via the GUI is being considered
## Architecture Global
![architecture](https://github.com/WadiiGaied/Patient-Health-Monitoring-and-Room-Climate-Control-System/assets/171201731/05fd2b55-7483-4192-b1b0-fde3aa9b627a)
## Node 1: Patient Health Monitoring
Node 1 is designed for real-time monitoring of patient health parameters within their room. It includes sensors for measuring body temperature, heart rate, and an emergency button for immediate assistance.

-Temperature Sensor: Monitors the patient's body temperature to ensure optimal health conditions.
-Heart Rate Sensor: Tracks the patient's heart rate to monitor cardiovascular health.
-Emergency Button: Allows the patient to quickly request assistance in case of emergencies. If pressed, alerts are triggered to notify caregivers or medical staff.
![IMG_20240530_095652](https://github.com/WadiiGaied/Patient-Health-Monitoring-and-Room-Climate-Control-System/assets/171201731/93e7dc28-d744-4366-a4cc-85af90434c0b)


