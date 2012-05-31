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
 * This file includes the SPI properties of the SPI kernel module.
 * This includes init, exit, read and write methods. 
 * These methods should only be called from the character driver part of the module,
 * and never directly.
 */
#ifndef PSOC5_SPI_H
#define PSOC5_SPI_H

#include <linux/err.h>
#include <asm/uaccess.h>
#include <linux/interrupt.h>
#include <linux/input.h>
#include <linux/spi/spi.h>
#include <plat/mcspi.h>
#include <linux/input.h>
#include <linux/module.h>
#include <linux/mutex.h>

/**
 * @brief The SPI init method.
 *
 * Initializes the SPI device and driver, and registers it in the kernel.
 *
 * @return 0 on success.
 */
int psoc5_spi_init(void);
/**
 * @brief The SPI exit method.
 *
 * unregisters the SPI device and driver from the kernel.
 *
 * @return 0 on success.
 */
int psoc5_spi_exit(void);
/**
 * @brief The SPI read method.
 *
 * Used to read from the SPI device, one character at a time.
 *
 * @return The character read from the PSoC5.
 */
char psoc5_spi_read(void);
/**
 * @brief The SPI write method. 
 *
 * Used to write data to the PSoC5 through SPI, in the form of a character array.
 *
 * @param dataToWrite A pointer to the character array of data to be written.
 * @param len The length of the character array "dataToWrite".
 */
void psoc5_spi_write(char* dataToWrite, unsigned int len);

/**
 * @brief A struct that represents the SPI device.
 *
 * Used to set the length of a word, and setup SPI.
 */
static struct spi_device *psoc5_spi_device;

/**
 * @brief The struct that contains the mcspi device information
 *
 * Disables turbo mode and sets the device to a single channel device.
 */
static struct omap2_mcspi_device_config psoc5_mcspi_device_config = {
	.turbo_mode 		= 0,
	.single_channel = 1, 
};

/**
 * @brief The struct that contains the SPI board info.
 *
 * Sets the modalias - the name of the SPI connection -, the bus number,
 * the chip select and the maximum speed to operate in. 
 * A value around 15MHz seems to be the sweet spot for our PSoC5 device.
 */


#endif
