/**
 * @file 
 * @author Bjarke Vad Andersen
 *
 * @section LICENSE
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details at
 * http://www.gnu.org/copyleft/gpl.html
 *
 * @section DESCRIPTION
 *
 * This kernel module is written to used on a DevKit8000, 
 * and is supposed  used with a PSoC5 device connected trough SPI.
 * This file contains the character driver properties of the SPI kernel module.
 * This includes init, exit, read, write, open and release methods.
 *
 */
#include <linux/err.h>
#include <linux/ctype.h>
#include <linux/cdev.h>
#include <asm/uaccess.h>
#include <linux/interrupt.h>
#include <linux/input.h>
#include <linux/module.h>

/**
 * @brief The Major number of the node.
 */
#define PSOC5_MAJOR 62
/**
 * @brief The Minor number of the node.
 */
#define PSOC5_MINOR 0
/**
 * @brief The maximum length for data to write to the device node in one function call.
 * */
#define MAXLEN			512
/**
 * @brief The GPIO pin which is used for receiving "word complete" interrupts from the PSoC5.
 */
#define INTGPIO 130 //7/137  


/**
 * @brief The init method.
 *
 * This method is called whenever the module is inserted into the kernel.
 * Here the basic setup and registering of the character driver takes place, including requesting
 * GPIO resources for the interrupt pin, and setting said GPIO to input. 
 * psoc5_spi_init() is called to initialize the SPI part of the module.
 *
 * @return 0 on success.
 */
static int __init psoc5_cdrv_init(void);
/*
 * @brief The exit method.
 *
 * This method is called whenever the module is removed from the kernel.
 * The character device is unregistered and deleted, and the requested GPIO resources are freed.
 */
static void __exit psoc5_cdrv_exit(void);
/**
 * @brief The open method.
 *
 * This method is called whenever a user is trying to read from or write to the device.
 * Here interrupt resources are requested. This is done here and not in the init method 
 * to prevent allocating resources that are not used.
 *
 * @return 0 on success.
 */
int psoc5_cdrv_open(struct inode *inodep, struct file *filep);
/**
 * @brief The release method.
 *
 * This method is called when a user is closing an open PSoC5 device, 
 * e.g. after reading from the device.
 * Here interrupt line resources are freed.
 *
 * @return 0 on success.
 */
int psoc5_cdrv_release(struct inode *inodep, struct file *filep);
/**
 * @brief The write method.
 *
 * This method is called when a user is writing to the device. 
 * Prevents the user from sending strings longer than MAXLEN,
 * and sends the data received from userspace through SPI by calling "psoc5_spi_write()".
 * 
 * @param ubuf The received data from userspace, represented as a character array.
 * @param count The length of the received character array.
 *
 * @return The length of the string written to the device.
 */
ssize_t psoc5_cdrv_write(struct file *filep, const char __user *ubuf,
												size_t count, loff_t *f_pos);
/**
 * @brief The read method.
 *
 * This method is called when a user is trying to read from the device.
 * Waits for an interrupt, and then continues to read out the data from the SPI, 
 * and copies them to userspace.
 * If no interrupt is received within 1000 jiffies 0 is sent to userspace.
 *
 * @param ubuf The address on which the user expects data to be copied to.
 * @return The length of the received character array.
 */
ssize_t psoc5_cdrv_read(struct file *filep, char __user *ubuf,
												size_t count, loff_t *f_pos);

/**
 * @brief The interrupt service routine.
 *
 * This ISR is called whenever a interrupt is received on the assigned GPIO pin,
 * meaning that a new word is received, it sets newData to 1.
 *
 * @return IRQ_HANDLED.
 */
irqreturn_t psoc5_isr(int irq, void *dev_id);

/**
 * @brief The character device struct.
 *
 * Used to register the device in the kernel. 
 * Contains all information of the character driver characteristics of the module,
 * including the file_operations struct.
 */
static struct cdev psoc5Dev;
/**
 * @brief The file operations struct
 *
 * Used to define which operations are called when the device 
 * is used as a file/node.
 */
struct file_operations psoc5_Fops;
/**
 * @brief A static in that contains the major/minor number characteristics returned from MKDEV.
 */
static int devno;
/**
 * @brief The wait queue which psoc5_cdrv_read() waits for.
 *
 * This wait queue is used to signal the character driver read method
 * from the ISR, that new data is ready from the PSoC5 device. 
 */
DECLARE_WAIT_QUEUE_HEAD(wait_queue);
/**
 * @brief The variable that is checked to see if new data is ready.
 */
int newData = 0;
