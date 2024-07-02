## Description
This project focuses on real-time health monitoring for patients, where each patient's health data is monitored within their room. Data from each patient is collected and transmitted to a central card using a communication protocol(CAN BUS). Once collected at the central card, the data is sent to a cloud server for analysis and storage. Additionally, a user interface (GUI) has been developed to visualize patient data in case of cloud server downtime. As a future enhancement, remote control of room climate via the GUI is being considered
## Architecture Global

![architecture](https://github.com/WadiiGaied/Patient-Health-Monitoring-and-Room-Climate-Control-System/assets/171201731/05fd2b55-7483-4192-b1b0-fde3aa9b627a)

### Node 1: Patient Health Monitoring

Node 1 is designed for real-time monitoring of patient health parameters within their room. It includes sensors for measuring body temperature, heart rate, and an emergency button for immediate assistance.

- **Temperature Sensor**: Monitors the patient's body temperature to ensure optimal health conditions.
- **Heart Rate Sensor**: Tracks the patient's heart rate to monitor cardiovascular health.
- **Emergency Button**: Allows the patient to quickly request assistance in case of emergencies. If pressed, alerts are triggered to notify caregivers or medical staff.
- 
![cablage_noeud1](https://github.com/WadiiGaied/Patient-Health-Monitoring-and-Room-Climate-Control-System/assets/171201731/e325f542-e5ed-48d0-af3e-02d37a800a83)

## Node 2: Room Climate Control

Node 2 is responsible for managing the climate conditions within the patient's room, ensuring comfort and health.

- **Temperature and Humidity Sensors**: Monitor the room's temperature and humidity levels in real-time.
- **Climate Control Actuator**: Utilizes a motor to adjust the room's climate settings based on sensor data. It regulates devices like air conditioning units or heaters to maintain optimal conditions.
- **Remote Control Capability**: Enables remote adjustment of room climate settings through the user interface (UI), providing convenience and flexibility for caregivers or medical staff.
- 
![cablage_noeud2](https://github.com/WadiiGaied/Patient-Health-Monitoring-and-Room-Climate-Control-System/assets/171201731/ee03cabc-4341-47de-af49-904f0d78a382)


