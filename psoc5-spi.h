#ifndef PSOC5_SPI_H
#define PSOC5_SPI_H

#include <linux/err.h>
#include <asm/uaccess.h>
#include <linux/interrupt.h>
#include <linux/input.h>
#include <linux/spi/spi.h>
#include <plat/mcspi.h>
#include <linux/spi/spi.h>
#include <linux/input.h>
#include <linux/module.h>
#include <linux/mutex.h>

//int __devinit psoc5_probe(struct spi_device* spi); 
//int __devexit psoc5_exit(struct spi_device* spi);

int psoc5_spi_init(void);
int psoc5_spi_exit(void);

char psoc5_spi_read(void);
void psoc5_spi_write(char dataToWrite);

#endif
