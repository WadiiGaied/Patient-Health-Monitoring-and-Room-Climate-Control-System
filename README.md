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

![cablage_noeud2](https://github.com/WadiiGaied/Patient-Health-Monitoring-and-Room-Climate-Control-System/assets/171201731/ee03cabc-4341-47de-af49-904f0d78a382)

## Central Node: Data Aggregation and Transmission
The central node in this system acts as a pivotal point for consolidating data from all connected nodes, which include patient health monitoring and room climate control devices. It collects data from each node, stores it locally, and then transmits the aggregated data to the ThingSpeak cloud platform using a WiFi module for storage and analysis. Additionally, the central node interfaces with the user interface (UI) through a TTL adapter, enabling real-time data visualization and remote control of room climate settings.

![cablage_noeud3](https://github.com/WadiiGaied/Patient-Health-Monitoring-and-Room-Climate-Control-System/assets/171201731/aade7457-7d60-4102-b773-7f30eee73b6d)
# Data Memorization

Data transmitted from the central node in this system is securely stored on the ThingSpeak cloud platform. This setup ensures that all collected data, aggregated from various nodes including patient health monitoring and room climate control devices, is securely archived and readily accessible for analysis and monitoring purposes. ThingSpeak provides a reliable cloud-based storage solution that facilitates data management and enables seamless integration with analytics tools for comprehensive insights into monitored parameters.
![cloud](https://github.com/WadiiGaied/Patient-Health-Monitoring-and-Room-Climate-Control-System/assets/171201731/8acb038e-df8f-4edf-b922-37a61244c685)

# Data Visualization on the HMI

The Human-Machine Interface (HMI) in this system provides comprehensive data visualization capabilities for monitoring patient health and room climate control parameters. It allows users to verify the connectivity status of each node, visualize aggregated data, and access detailed patient information.
- **Homepage** :The homepage of the HMI displays the connectivity status of  node central.

![QT-1](https://github.com/WadiiGaied/Patient-Health-Monitoring-and-Room-Climate-Control-System/assets/171201731/9b917347-4a73-4da8-8e69-fca6e53a1b88)
- **Room View** : Users can select a room (e.g., Room 1) to visualize detailed patient data, including temperature, heart rate, and emergency alerts if triggered.
  ![QT-2](https://github.com/WadiiGaied/Patient-Health-Monitoring-and-Room-Climate-Control-System/assets/171201731/1aa56158-5eac-4201-a364-4ee0f7feb971)
## Final Project Photo
![IMG_20240530_124608](https://github.com/WadiiGaied/Patient-Health-Monitoring-and-Room-Climate-Control-System/assets/171201731/92ed5bd3-9c87-464b-ac53-c0c9e6e6ad9d)
