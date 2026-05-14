# Secure Messaging System for CV2-X based on MQTT

A high-performance communication architecture developed for **Vehicular Ad-hoc Networks (VANET)**, optimized for remote environments with low cellular signal strength.
## 🛠 Technology Stack

### Hardware (On-Board Unit - OBU)
* **Microcontroller:** ESP32 (Dual-core). The system utilizes one core for GPS/GSM tasks and the second dedicated to cryptographic processes to maintain low latency.
* **GSM/GPRS:** SIM800L v2. Selected for its ability to operate on 2G bands, ensuring connectivity in rural areas where 4G/LTE is unavailable.
* **Positioning:** Neo6mv2 GPS Module for real-time coordinate tracking.

  <img width="582.5" height="249" alt="image" src="https://github.com/user-attachments/assets/739ecaba-00bc-4157-b686-0277bf1ea30e" />

### Software & Infrastructure
* **Messaging:** **MQTT** (Message Queuing Telemetry Transport). Chosen for its efficiency in low-bandwidth scenarios compared to traditional HTTP.
* **Encryption:** **AES-128 (Advanced Encryption Standard)**. Implemented in ECB and CBC modes to secure sensitive telemetry data.
* **Cloud Infrastructure:** **AWS (Amazon Web Services)**. An EC2 instance running Ubuntu Server 18.04 LTS hosts the MQTT broker.
* **Broker:** **EMQX (Open Source Edition)**. Handles high-concurrency pub/sub operations with minimal overhead.
  
## 🏗 System Architecture
The project follows a 4-layer architecture to bridge the gap between vehicles and emergency services:

1.  **Perception & Processing (OBU):** The ESP32 gathers GPS data, encrypts the payload via AES, and prepares the MQTT packet.
2.  **Access/Link (RSU Infrastructure):** Utilizes GPRS/GSM cellular base stations as the physical link to the cloud.
3.  **Network (Transport):** Uses the TCP/IP suite to ensure reliable delivery of encrypted MQTT messages to the AWS endpoint.
4.  **Application (Service):** The EMQX broker distributes data to subscribers (e.g., emergency response dashboards or central monitoring).

<img width="1405" height="452" alt="image" src="https://github.com/user-attachments/assets/ffe899c2-b3fa-49f1-9e2e-3108dfe04a05" />



## 📊 Performance Summary
Based on field tests conducted on the **Mochis-Obregón highway**, the architecture demonstrated:

* **High Reliability:** A packet delivery success rate of **99%** in various signal conditions.
* **Low Cryptographic Overhead:** Encryption times ranging from **5.9ms to 12.9ms**, ensuring real-time performance for vehicular safety.
* **Resiliency:** Successful transmission in "Marginal" signal areas (-109 to -93 dBm), where standard data services often fail.

## 📊 CAPÍTULO V. RESULTADOS

### 5.2. Comportamiento de la Calidad de Señal
The performance of the system was evaluated based on the Received Signal Strength Indicator (RSSI) to determine its reliability in adverse communication environments.

#### RSSI Values and Signal Stability
The following figures illustrate the signal behavior across different testing scenarios:

* **Figura 21. Valores de RSSI.** <img width="803" height="459" alt="image" src="https://github.com/user-attachments/assets/94e1481b-a73b-4341-ab59-a0ab0864f457" />

    This chart details the signal fluctuations during the highway transition, showing the system's ability to maintain MQTT connectivity even at low decibel levels.

* **Figura 25.** <img width="1107" height="379" alt="image" src="https://github.com/user-attachments/assets/f346ee91-9a04-4875-ba36-551740f09a03" />


* **Figura 31.** <img width="1013" height="600" alt="image" src="https://github.com/user-attachments/assets/8bdc7321-70c4-4e2f-b7fb-e0b8acd6300c" />


---

## 🔍 Comparative Analysis & Conclusion

A critical part of the study was comparing the two implementation modes of the AES-128 algorithm: **Electronic Codebook (ECB)** and **Cipher Block Chaining (CBC)**.

### Algorithm Comparison

| Feature | AES-128 ECB | AES-128 CBC |
| :--- | :--- | :--- |
| **Average Latency** | **5.9 ms** (Fastest) | **12.9 ms** |
| **Security Level** | Standard | High (Initialization Vector included) |
| **Packet Success Rate** | ~99% | ~97% |
| **Resource Usage** | Minimal | Moderate |

### Final Conclusion
The research concludes that for **CV2-X (Cellular Vehicle-to-Everything)** applications in remote areas, the **AES-128 ECB** mode provides the most efficient balance. While CBC offers higher security by preventing pattern recognition in encrypted data, the **lower latency (5.9ms)** of ECB is more critical for real-time vehicular safety alerts where every millisecond counts. 

Despite operating in "Marginal" signal zones (-109 to -93 dBm), the MQTT-based architecture proved resilient, maintaining a near-perfect delivery rate, making it a viable solution for emergency telemetry in areas lacking modern 4G/5G infrastructure.

---
*Reference: "SISTEMA SEGURO ORIENTADO A MENSAJERÍA CIFRADA CV2-X BASADA EN MQTT" - Francisco Antonio Palos Angulo (2022).*
