#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
using namespace std;

void Write1Led(char *pBase, int ledNum, int state);
int Read1Switch(char *pBase, int switchNum);
int ReadAllSwitches(char *pBase);
void WriteAllLeds(char *pBase, int value);

// Physical base address of GPIO
const unsigned gpio_address = 0x400d0000;

// Length of memory-mapped IO window
const unsigned gpio_size = 0xfff;

const int gpio_led1_offset = 0x12C;  // Offset for LED1
const int gpio_led2_offset = 0x130;  // Offset for LED2
const int gpio_led3_offset = 0x134;  // Offset for LED3
const int gpio_led4_offset = 0x138;  // Offset for LED4
const int gpio_led5_offset = 0x13C;  // Offset for LED5
const int gpio_led6_offset = 0x140;  // Offset for LED6
const int gpio_led7_offset = 0x144;  // Offset for LED7
const int gpio_led8_offset = 0x148;  // Offset for LED8

const int gpio_sw1_offset = 0x14C;  // Offset for Switch 1
const int gpio_sw2_offset = 0x150;  // Offset for Switch 2
const int gpio_sw3_offset = 0x154;  // Offset for Switch 3
const int gpio_sw4_offset = 0x158;  // Offset for Switch 4
const int gpio_sw5_offset = 0x15C;  // Offset for Switch 5
const int gpio_sw6_offset = 0x160;  // Offset for Switch 6
const int gpio_sw7_offset = 0x164;  // Offset for Switch 7
const int gpio_sw8_offset = 0x168;  // Offset for Switch 8

const int gpio_pbtnl_offset = 0x16C;  // Offset for left push button
const int gpio_pbtnr_offset = 0x170;  // Offset for right push button
const int gpio_pbtnu_offset = 0x174;  // Offset for up push button
const int gpio_pbtnd_offset = 0x178;  // Offset for down push button
const int gpio_pbtnc_offset = 0x17C;  // Offset for center push button

/**
 * Write a 4-byte value at the specified general-purpose I/O location.
 *
 * @param pBase		Base address returned by 'mmap'.
 * @parem offset	Offset where device is mapped.
 * @param value		Value to be written.
 */
void RegisterWrite(char *pBase, int offset, int value)
{
	* (int *) (pBase + offset) = value;
}

/**
 * Read a 4-byte value from the specified general-purpose I/O location.
 *
 * @param pBase		Base address returned by 'mmap'.
 * @param offset	Offset where device is mapped.
 * @return		Value read.
 */
int RegisterRead(char *pBase, int offset)
{
	return * (int *) (pBase + offset);
}

/**
 * Initialize general-purpose I/O
 *  - Opens access to physical memory /dev/mem
 *  - Maps memory at offset 'gpio_address' into virtual address space
 *
 * @param  fd	File descriptor passed by reference, where the result
 *		of function 'open' will be stored.
 * @return	Address to virtual memory which is mapped to physical,
 *          	or MAP_FAILED on error.
 */
char *Initialize(int *fd)
{
	*fd = open( "/dev/mem", O_RDWR);
	return (char *) mmap(NULL, gpio_size, PROT_READ | PROT_WRITE, MAP_SHARED,
		*fd, gpio_address);
}

/**
 * Close general-purpose I/O.
 *
 * @param pBase	Virtual address where I/O was mapped.
 * @param fd	File descriptor previously returned by 'open'.
 */
void Finalize(char *pBase, int fd)
{
	munmap(pBase, gpio_size);
	close(fd);
}

int main()
{
	// Initialize
	int fd, switchRead;
	char *pBase = Initialize(&fd);

	// Check error
	if (pBase == MAP_FAILED)
	{
		perror("Mapping I/O memory failed - Did you run with 'sudo'?\n");
		return -1;
	}	
	
// ************** Put your code here **********************

   int num = ReadAllSwitches(pBase);
   WriteAllLeds(pBase,num);
   int read;
   bool active = true;
   
   while (active) {
   
   	cout << num;
   
   	read = PushButtonGet(pBase);
	
	if (read == 1) {
	
		num = num*2;
		WriteAllLeds(pBase,num);
	
	}
	
	if (read == 2) {
	
		num = num/2;;
		WriteAllLeds(pBase,num);
	
	}
	
	if (read == 3) {
	
		num++;
		WriteAllLeds(pBase,num);
	
	}
	
	if (read == 4) {
	
		num--;
		WriteAllLeds(pBase,num);
	
	}
	
	if (read == 1) {

		num = ReadAllSwitches(pBase);
		WriteAllLeds(pBase,num);
	
	}
   
   }
   
  
  // Done
	Finalize(pBase, fd);
	return 0;
}

void Write1Led(char *pBase, int ledNum, int state) {

	int offset = 0x12C;
	for (int x = 0; x < ledNum; x++)
		offset = offset + 0x004;
	RegisterWrite(pBase, offset, state);

}

int Read1Switch(char *pBase, int switchNum) {

	int offset = 0x14C;
	for (int x = 0; x < switchNum; x++)
		offset = offset + 0x004;
	return RegisterRead(pBase, offset);

}

int ReadAllSwitches(char *pBase) {
    int switch_val = 0;
    switch_val = switch_val + Read1Switch(pBase, 0);
    switch_val = switch_val + 2*Read1Switch(pBase, 1);
    switch_val = switch_val + 4*Read1Switch(pBase, 2);
    switch_val = switch_val + 8*Read1Switch(pBase, 3);
    switch_val = switch_val + 16*Read1Switch(pBase, 4);
    switch_val = switch_val + 32*Read1Switch(pBase, 5);
    switch_val = switch_val + 64*Read1Switch(pBase, 6);
    switch_val = switch_val + 128 *Read1Switch(pBase, 7);
    return switch_val;
}

void WriteAllLeds(char *pBase, int value) {
    int bit_val = 0;
    Write1Led(pBase, 0,value%2);
    Write1Led(pBase, 1,(value/2)%2);
    Write1Led(pBase, 2, (value/4)%2);
    Write1Led(pBase, 3, (value/8)%2);
    Write1Led(pBase, 4, (value/16)%2);
    Write1Led(pBase, 5, (value/32)%2);
    Write1Led(pBase, 6, (value/64)%2);
    Write1Led(pBase, 7, (value/128)%2);


}

int PushButtonGet(char *pBase) {
    if (RegisterRead(pBase, gpio_pbtnl_offset) == 1)
        return 1;

    else
        if (RegisterRead(pBase, gpio_pbtnr_offset) == 1)
            return 2;
        else
            if (RegisterRead(pBase, gpio_pbtnu_offset) == 1)
                return 3;
            else
                if (RegisterRead(pBase, gpio_pbtnd_offset) == 1)
                    return 4;
                else
                    if (RegisterRead(pBase, gpio_pbtnc_offset) == 1)
                        return 5;
			else
				return 0;

}
