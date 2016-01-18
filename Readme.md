Source code for ReflowOven project
==================================

This code is for board versions **1.***

## Setup

1. Install the Arduino IDE (See www.arduino.cc)
2. Append the following text to
 
   Windows:  ```<Arduino Installation>/hardware/arduino/avr/boards.txt```

   Max OS X: ```/Applications/Arduino/Contents/java/hardware/arduino/avr/boards.txt```
   
   ```
   reflowctrl.name=ReflowCtrl
   reflowctrl.build.mcu=atmega8
   reflowctrl.build.f_cpu=8000000L
   reflowctrl.build.board=AVR_NG
   reflowctrl.build.core=arduino
   reflowctrl.build.variant=standard

   reflowctrl.upload.maximum_size=7168
   reflowctrl.upload.maximum_data_size=
   reflowctrl.upload.tool=avrdude
   reflowctrl.upload.protocol=arduino
   reflowctrl.upload.speed=19200
   
   reflowctrl.bootloader.tool=avrdude
   reflowctrl.bootloader.unlock_bits=0x3F
   reflowctrl.bootloader.lock_bits=0x0F
   reflowctrl.bootloader.low_fuses=0xe4
   reflowctrl.bootloader.high_fuses=0xca
   reflowctrl.bootloader.file=atmega8/ATmegaBOOT-prod-firmware-2009-11-07.hex"
```

## License
The MIT License (MIT)
  
  Copyright (c) 2016 Sandro Lutz
  
  Permission is hereby granted, free of charge, to any person obtaining a copy of
  this software and associated documentation files (the "Software"), to deal in
  the Software without restriction, including without limitation the rights to
  use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
  the Software, and to permit persons to whom the Software is furnished to do so,
  subject to the following conditions:
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
  
  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
  FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
  COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
  IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
  CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
