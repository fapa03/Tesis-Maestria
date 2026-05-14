# Secure Messaging System for CV2-X based on MQTT

A secure, lightweight communication architecture designed for **Vehicular Ad-hoc Networks (VANET)**. [cite_start]This project focuses on ensuring message confidentiality and integrity in remote areas with unstable cellular signals[cite: 612].

## 🛠 Technology Stack

### Hardware (On-Board Unit - OBU)
* [cite_start]**Microcontroller:** ESP32 (Dual-core) - Handles cryptographic processes and wireless communication[cite: 847, 873].
* [cite_start]**GSM/GPRS Module:** SIM800L v2 - Provides 2G connectivity, chosen for its extensive coverage in rural/isolated areas[cite: 847, 874, 853].
* [cite_start]**GPS Module:** Neo6mv2 - Captures real-time global positioning data (Latitude/Longitude)[cite: 847, 875].
* [cite_start]**Power:** 4000mA Power Bank - Ensures stable current for GSM transmission peaks[cite: 847, 897].

### Software & Infrastructure
* [cite_start]**Messaging Protocol:** MQTT (Message Queuing Telemetry Transport) - A lightweight Pub/Sub protocol ideal for low-bandwidth, high-latency environments[cite: 710, 742, 855].
* [cite_start]**Cloud Infrastructure:** Amazon Web Services (AWS) EC2 Instance (t2.micro) running Ubuntu Server 18.04 LTS[cite: 847, 880, 924].
* [cite_start]**MQTT Broker:** EMQX (Open Source Edition) - Scalable broker managing secure connections and message routing[cite: 936, 937].
* [cite_start]**Security & Encryption:** AES-128 (Advanced Encryption Standard) implemented in **ECB** and **CBC** modes to protect sensitive GPS coordinates[cite: 627, 867, 919].
* [cite_start]**Analysis Tools:** Wireshark & tcpdump - Used for network traffic interception and cryptographic performance auditing[cite: 934, 955, 960].

## 🏗 System Architecture

[cite_start]The architecture is divided into four main layers to facilitate communication between the vehicle and emergency services[cite: 869]:

1.  [cite_start]**OBU Layer (ESP32):** * Reads GPS data and signal strength (RSSI)[cite: 901].
    * [cite_start]Encrypts data packets using AES-128[cite: 903].
    * [cite_start]Publishes encrypted payloads to the Cloud via MQTT[cite: 921].
2.  [cite_start]**Infrastructure Layer (RSU):** * Relies on existing 2G/GSM cellular infrastructure to bridge the vehicle to the internet[cite: 876].
3.  **Transport Layer (Internet):** * Utilizes TCP/IP for reliable delivery of MQTT messages[cite: 877].
4.  [cite_start]**Service Layer (Cloud/Server):** * An AWS-hosted broker receives messages and routes them to emergency dashboards or other vehicles (V2V/V2I)[cite: 879].

<img width="1405" height="452" alt="image" src="https://github.com/user-attachments/assets/ffe899c2-b3fa-49f1-9e2e-3108dfe04a05" />

<img width="1165" height="498" alt="image" src="https://github.com/user-attachments/assets/739ecaba-00bc-4157-b686-0277bf1ea30e" />

## 📊 Performance Summary

* [cite_start]**Efficiency:** Achieved up to 99% packet delivery efficiency in ECB mode over real-world highway testing (Mochis-Obregón)[cite: 629, 1002].
* [cite_start]**Latency:** Cryptographic execution takes ~5.9ms (ECB) to ~12.9ms (CBC), well within the requirements for emergency response[cite: 1006, 1020].
* [cite_start]**Resilience:** Maintained stable communication even in "Marginal" signal conditions (-109 to -93 dBm)[cite: 882, 1016].

---
[cite_start]*Developed as a Master's Thesis in Engineering Sciences at Instituto Tecnológico de Sonora (ITSON), 2022.* [cite: 531, 541]
