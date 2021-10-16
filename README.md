# ESP8266-NodeMCU-MQTT-JVC-Projector-Control
Control JVC Projector via MQTT on RS232 connector
# What you need
- NodeMCU with ESP8266 on board
- MAX3232 RS232 to TTL Serial Port Converter Module DB9 Connector
- Basic wires

Connect the pins like this:
| NodeMCU | RS232-TTL |
| ------- | --------- |
| G | GND |
| 3V | VCC |
| D6 | RXD |
| D7 | TXD |
# What you need
- Connects to wifi
- Listens for command from MQTT server, then executes it
# How to use
Configure wifi and mqtt server credentials.

The module listens to commands published to the topic 'projector/commands'. A full list of commands is available in the code. Codes are sourced from this document: [DILA_remote_control_guide](https://support.jvc.com/consumer/support/documents/DILAremoteControlGuide.pdf). A list of compatible models is in this guide as well.

E.g. Publish 'PowerOn' in plain text to the topic 'projector/commands' and any compatible JVC projector will switch on. I use this code to integrate my projector in Home Assistant.
