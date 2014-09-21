meshDNS
=======

mesh DNS is a research about creating a Domain name system for a mesh network. 

The meshDNS software link a (non-unique) domain name system to an IP address, which could change due to IP colision in the mesh network. This link is based on a public key, for a unique device, between those two values to ensure a correct translation. 

example:
foo.bar ---> theuniquekeyforauniquedevice1234example123456 ---> 42.158.1.89

this project is, for the moment, just a proof of concept.
