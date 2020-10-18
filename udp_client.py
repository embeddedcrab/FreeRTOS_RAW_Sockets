# -*- coding: utf-8 -*-

from time import sleep
import socket

client = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

host = '192.168.1.9'
port = 6000
number = 0

print("Starting UDP Client")
while(1):
    data = "RAW UDP SERVER " + str(number) + "\r\n"
    client.sendto(str.encode(data), (host, port));
    data, ip = client.recvfrom(1024)
    print("{}: {}, {}".format(ip, data.decode(),number))
    number += 1

# Close socket
client.close()
