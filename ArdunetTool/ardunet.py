#!/usr/bin/env python
#
# Ardunet upload tool
# https://github.com/karang/ardunet
#
# Copyright (C) 2014 Arthur Hennequin
#
# This program is free software; you can redistribute it and/or modify it under
# the terms of the GNU General Public License as published by the Free Software
# Foundation; either version 2 of the License, or (at your option) any later version.
# 
# This program is distributed in the hope that it will be useful, but WITHOUT 
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
# FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License along with
# this program; if not, write to the Free Software Foundation, Inc., 51 Franklin
# Street, Fifth Floor, Boston, MA 02110-1301 USA.

from SimpleWebSocketServer import WebSocket, SimpleWebSocketServer
from esptool import ESPROM
import sys
import glob
import serial
import math

def serial_ports():
    """Lists serial ports

    :raises EnvironmentError:
        On unsupported or unknown platforms
    :returns:
        A list of available serial ports
    """
    if sys.platform.startswith('win'):
        ports = ['COM' + str(i + 1) for i in range(256)]

    elif sys.platform.startswith('linux') or sys.platform.startswith('cygwin'):
        # this is to exclude your current terminal "/dev/tty"
        ports = glob.glob('/dev/tty[A-Za-z]*')

    elif sys.platform.startswith('darwin'):
        ports = glob.glob('/dev/tty.*')

    else:
        raise EnvironmentError('Unsupported platform')

    result = []
    for port in ports:
        try:
            s = serial.Serial(port)
            s.close()
            result.append(port)
        except (OSError, serial.SerialException):
            pass
    return result

class ArdunetWeb(WebSocket):

    def write_flash(self, esp, image, address):
        print "Erasing flash..."
        esp.flash_begin(len(image), address)
        seq = 0
        blocks = math.ceil(len(image)/float(esp.ESP_FLASH_BLOCK))
        while len(image) > 0:
            pcent = 100*(seq+1)/blocks
            print '\rWriting at 0x%08x... (%d %%)' % (address + seq*esp.ESP_FLASH_BLOCK, pcent),
            sys.stdout.flush()
            self.sendMessage('{ "address":'+str(address)+', "pcent":'+str(pcent)+' }');
            esp.flash_block(image[0:esp.ESP_FLASH_BLOCK], seq)
            image = image[esp.ESP_FLASH_BLOCK:]
            seq += 1
        print
    
    def handleMessage(self):
        if self.data is None:
            self.data = ''

        if self.data == 'listPort':
            self.sendMessage('{ "list":'+str(serial_ports()).replace("'", '"')+" }");    
        elif self.opcode==2: #binary
            app_len = self.data[0]<<24 | self.data[1]<<16 | self.data[2]<<8 | self.data[3];
            app_image = "".join(map(chr,self.data[4:(4+app_len)]))
            sdk_image = "".join(map(chr,self.data[(4+app_len):]))

            print "\nReceived transfert request:"
            print "App: "+str(len(app_image))+" SDK: "+str(len(sdk_image))+"\n "

            try:
                esp = ESPROM(self.ser_port)
                esp.connect()
                print "\nWriting app at 0x00000"
                self.write_flash(esp, app_image, 0x00000)
                print "\nWriting sdk at 0x40000"
                self.write_flash(esp, sdk_image, 0x40000)
                print '\nLeaving...'
                esp.flash_finish(False)
            except Exception,e:
                print e
        elif self.opcode==1: # text
            self.ser_port = str(self.data)
            print "Changed port: "+self.ser_port;            

    def handleConnected(self):
        print self.address, 'connected'

    def handleClose(self):
        print self.address, 'closed'

if __name__ == '__main__':
    print "######################"
    print "#### Ardunet Tool ####"
    print "######################\n"
    server = SimpleWebSocketServer('', 8000, ArdunetWeb)
    server.serveforever()
    
