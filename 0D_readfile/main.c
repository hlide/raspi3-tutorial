/*
 * Copyright (C) 2018 bzt (bztsrc@github)
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 */

#include "uart.h"
#include "sd.h"
#include "fat.h"

void main()
{
    unsigned int cluster;
    // set up serial console
    uart_init();

    // initialize EMMC and detect SD card type
    if(sd_init()==SD_OK) {
        // read the master boot record and find our partition
        if(fat_getpartition()) {
            // find out file in root directory entries
            cluster=fat_getcluster("LICENC~1BRO");
            if(cluster==0)
                cluster=fat_getcluster("KERNEL8 IMG");
            if(cluster) {
                // read into memory
                uart_dump(fat_readfile(cluster));
            }
        } else {
            uart_puts("FAT partition not found???\n");
        }
    }

    // echo everything back
    while(1) {
        uart_send(uart_getc());
    }
}
