#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
# Copyright 2017 Jacob Holtom and Wes Rogers.
#
# This is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3, or (at your option)
# any later version.
#
# This software is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this software; see the file COPYING.  If not, write to
# the Free Software Foundation, Inc., 51 Franklin Street,
# Boston, MA 02110-1301, USA.
#

import numpy
from numpy import *
from numpy import ndarray
from gnuradio import gr
import sys
import os
import pmt
from pmt import *
from pmt import pmt_to_python
import Queue
from struct import *
import binascii

#  unloads the frames based on specific format
def unframe(frame):
    print "DF:unframe:len of rec frame:",len(frame),"\n"

    header0_0, header0_1, header0_2, header0_3 = unpack('!HH', frame[:4])
    print "DF:UF:header0_0 second",hex(header0_0),"\n"
    print "DF:UF:header0_1 second",hex(header0_1),"\n"
    print "DF:UF:header0_2 second",hex(header0_2),"\n"
    print "DF:UF:header0_3 second",hex(header0_3),"\n"

    return header0_0, header0_1, header0_2, header0_3


class destruct_frame(gr.basic_block):
        # block destruct_frame takes in a SDLP frame and removes the frame structure and passes on the contents

    #constants
    _out_port = pmt.intern("out")
    _in_port = pmt.intern("in")
    MAX_SIZE_OF_QUEUE = 10000
    data_queue = Queue.Queue(MAX_SIZE_OF_QUEUE)
    message_queue = Queue.Queue(MAX_SIZE_OF_QUEUE)

    def __init__(self):
        gr.basic_block.__init__(self,
            name="destruct_frame",
            in_sig=[],
            out_sig=[])

        self.message_port_register_in(self._in_port) #this creates the input port for the messages
        self.set_msg_handler(self._in_port, self.msg_handler_method) #this determines what is done with any recieved messages as soon as they are recieved
        self.message_port_register_out(self._out_port) #this creates the output port for messages

        self.frame = "" #this will be recieved in a stream
        self.contents = "" #this will be output in a message

    def msg_handler_method(self, msg):
        OID_send = False #default value

        if pmt.is_symbol(msg):
            msg_str = str(pmt.symbol_to_string(msg)) #changes the data back into usable data
            self.data = msg_str
        elif pmt.is_pair(msg):
            print "DF: came through as a pair\n"
            incoming_dict = pmt.car(msg)
            incoming_data = pmt.cdr(msg)
            if pmt.is_dict(incoming_dict):
                print "DF:found dict\n"
            if pmt.is_symbol(incoming_data):
                print "DF:found symbol\n"
            if pmt.is_vector(incoming_data):
                print "DF:found vector\n"
            if pmt.is_pair(incoming_data):
                print "DF:found pair\n"
            if pmt.is_complex(incoming_data):
                print "DF:found complex\n"
            if pmt.is_dict(incoming_data):
                print "DF:data is a dict\n"
            self.data = pmt.to_python(incoming_data)
            print "DF:self.data.dtype",self.data.dtype,"\n"
            #self.data = self.data.flatten() #makes 1D array
            numpy.set_printoptions(threshold='nan')
            output = []
            print "DF:len of array:",len(self.data),"\n"
            output = ':'.join(hex(x)[2:] for x in self.data) ## what is the 2 for?
            print "DF:output:",output,"\n"
            print "DF:len of self.data",len(self.data),"\n"
        else:
            print "ERROR:DF: Wrong type came through message\n"
        frame = self.data
        ctl0, mynodeid, tonodeid, ctl1 = unframe(frame)

        while self.data_queue.qsize() > 1: #there is a previous packet that needs to be sent now that we have the current one
            packet_to_send = self.data_queue.get(False)
            if type(packet_to_send) == numpy.ndarray:
                print "found the array to string\n"
                packet_to_send = numpy.array_str(packet_to_send)
            packet_to_send_final = pack('!%(length_packet)ds' % {"length_packet" : len(packet_to_send)},packet_to_send)
            print "DF:len(packet_to_send)",len(packet_to_send_final),type(packet_to_send_final),"\n"
            length_of_this_packet_now = packet_to_send[4:6]
            print "DF:Recorded length of packet being sent:",length_of_this_packet_now,"\n",packet_to_send_final,"\n"
            #deliver_current_packet = pmt.string_to_symbol(numpy.array_str(packet_to_send))
            deliver_current_packet = pmt.string_to_symbol(packet_to_send_final)
            self.message_port_pub(self._out_port, deliver_current_packet) #send the packet in a message


    def general_work(self, input_items, output_items):
        return len(output_items)
