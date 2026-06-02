# Copyright (c) 2026 Fraunhofer IPMS, (www.ipms.fraunhofer.de)
# Licensed under the Solderpad Hardware License v2.1 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#     http://solderpad.org/licenses/SHL-2.1/
# Unless required by applicable law or agreed to in writing, distributed
# under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES
# OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
# \file     udplog.py
# \author   Martin Zimmerling
# \date     2024-11-11
# \brief    UDP trace data logger
#

import socket
import datetime
import struct
import ipaddress

# Configuration: Specify port number and log file name
UDP_PORT = 5055  # Set this to the port you want to listen on

# Function to log incoming data with a timestamp
def log_data(data, addr):
    tstamp = datetime.datetime.now()
    timestampbin = struct.pack("BBBBI", tstamp.day, tstamp.hour, tstamp.minute, tstamp.second, tstamp.microsecond)
    ipaddr, port = addr
    datalenbin = struct.pack("<I", len(data))

    with open(filename, 'ab') as f:
        f.write(timestampbin)
        f.write(ipaddress.IPv4Address(ipaddr).packed)
        f.write(datalenbin)
        f.write(data)

# Create file
curtime = datetime.datetime.now()
filename = curtime.strftime("tracelog_%Y%m%d_%H%M.bin")
print(f"Logging to file {filename}")

# Create UDP socket and bind to the specified port
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
sock.bind(('', UDP_PORT))  # '' listens on all available network interfaces

print(f"Listening for UDP packets on port {UDP_PORT}...")

try:
    i = 0
    while True:
        data, addr = sock.recvfrom(2048)
        i += 1
        print(f"UDP packet from {addr[0]} {i}")
        log_data(data, addr)
except KeyboardInterrupt:
    print("UDP logging stopped.")
except Exception as e:
    print(f"An error occurred: {e}")
finally:
    sock.close()
