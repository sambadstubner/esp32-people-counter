# IoT People Counter
Code for an ESP32 to count the number of cell phones in a given area and uploads the results to AWS IoT Core.

The device scans each WiFi channel and sniffs packets for unique MAC addresses and filters them by cell-phones. This gives an anonymous approximation to the number of people in the area.

The esp-idf toolchain will need to be initialized and configured with AWS and WiFi credentials using menu-config

This build upon an Amazon Web Services IoT MQTT (TLS Mutual Authentication) Example

This also relies on the esp32-wifi-manager component to manage WiFi credentials and reconnect automatically 
