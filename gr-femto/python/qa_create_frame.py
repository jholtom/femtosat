#!/usr/bin/env python
# -*- coding: utf-8 -*-
# 
# Copyright 2017 <+YOU OR YOUR COMPANY+>.
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
import sys
sys.path.insert(0, '/home/jacob/cygnus/gnuradio/sdlp')

import os 
dir_path = os.getcwd()
print "dir",dir_path

import sdlp
from gnuradio import gr, gr_unittest
from gnuradio import blocks
from create_frame import create_frame
from destruct_frame import destruct_frame

class qa_create_frame (gr_unittest.TestCase):

    def setUp (self):
        self.tb = gr.top_block ()

    def tearDown (self):
        self.tb = None

    def my_own_test(self,src,crt_frame,destruct_pkt,snk):
        input_req = 0
        crt_frame.forecast(1,input_req)
        nonsense = ["5hello my nAme is john what is your name?5"]
        output_from_create = [0]
        crt_frame.general_work(nonsense,output_from_create)
        incoming_frame = output_from_create[0]

        number_of_inputs_needed = [0,0,0]
        destruct_pkt.forecast(3,number_of_inputs_needed)
        output_stuff = [0,0,0,0,0]
        destruct_pkt.general_work(incoming_frame,output_stuff)

        print output_stuff[0]," :frameType"
        print output_stuff[1]," :APID"
        print output_stuff[2]," :frameSequenceCount"

    def test_001_t (self):
        # set up fg
        src = blocks.file_source(40,"/home/jacob/Downloads/hex.dat",True) #first parameter is the num of inputs that create_frame demands 
                                                                        #  = current size of a frame in bytes set by the class in spp.py changing the payload size

        ##packet = 0xEF
        scid = 0
        TF_version = 0
        FirstHeaderPointer = 0xDD
        ocf = 0xF0
        fecf = 0xFC
        crt_frame = create_frame(packet, scid, vcid, TF_version, FirstHeaderPointer, ocf, fecf)
        destruct_frm = destruct_frame()

        ####  *** might never be able to read the file as is because it is writing native binary to the file so if there is data there it may be unable to display it?
        snk = blocks.file_sink(10,"/home/jacob/Downloads/my_end_result1.dat",True) #10 = out_sig * number of output items = float16(2 bytes) * 5 output items

        #self.my_own_test(src,crt_frame,destruct_pkt,snk)


        self.tb.connect(src,crt_frame)
        self.tb.connect(crt_frame,destruct_frm)
        self.tb.connect(destruct_frm,snk)
        self.tb.run ()

if __name__ == '__main__':
    gr_unittest.run(qa_create_frame, "qa_create_frame.xml")
