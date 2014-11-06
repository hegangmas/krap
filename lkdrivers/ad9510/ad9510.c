/*
 * AD9510 1.2 GHz Clock Distribution IC
 * PLL Core, Dividers, Delay Adjust, Eight Outputs
 *
 * Copyright 2013 Instrumentation technologies d.d.
 *
 * Licensed under the GPL-2 or later.
 *
 */
#include <linux/device.h>
#include <asm/string.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/sysfs.h>
#include <linux/spi/spi.h>
#include <linux/module.h>
#include <linux/err.h>
/*
#include <linux/hwmon.h>
#include <linux/hwmon-sysfs.h>
*/
enum ad9510_variant {
	ad9510,
};

struct ad9510_data {
	struct spi_device *spi_dev;
/*	struct device *hwmon_dev;*/
};

static int ad9510_write(struct ad9510_data *chip, u16 reg, u8 data, size_t size) {
	int ret;

	u8 tx[2 + size];

	if (size == 1) {
		tx[0] = ((((reg & 0xff00) >> 8) & 0x1F));
		tx[1] = (reg & 0xff);

	} else if (size == 2) {
		tx[0] = (0x20 | (((reg & 0xff00) >> 8) & 0x1F));
		tx[1] = (reg & 0xff);
	} else if (size == 3) {
		tx[0] = (0x40 | (((reg & 0xff00) >> 8) & 0x1F));
		tx[1] = (reg & 0xff);
	} else {
		tx[0] = (0x60 | (((reg & 0xff00) >> 8) & 0x1F));
		tx[1] = (reg & 0xff);
	}

	memcpy(tx + 2, &data, size);

	ret = spi_write(chip->spi_dev, (void*) &tx, sizeof(tx));

	if (ret < 0)
		dev_err(&chip->spi_dev->dev, "SPI write error\n");

	return ret;
}

static int ad9510_read(struct ad9510_data *chip, u8 reg, u8 *data, size_t size) {
	u8 ret;
	u8 tx[2];
	u8 rx[size];
	int i = 0;
	memset(rx, 0, size);
	if (size == 1) {
		tx[0] = (0x80 | (((reg & 0xff00) >> 8) & 0x1F));
		tx[1] = (reg & 0xff);
	} else if (size == 2) {
		tx[0] = (0xA0 | (((reg & 0xff00) >> 8) & 0x1F));
		tx[1] = (reg & 0xff);
	} else if (size == 3) {
		tx[0] = (0xC0 | (((reg & 0xff00) >> 8) & 0x1F));
		tx[1] = (reg & 0xff);
	} else {
		tx[0] = (0xD0 | (((reg & 0xff00) >> 8) & 0x1F));
		tx[1] = (reg & 0xff);
	}

	ret = spi_write_then_read(chip->spi_dev, (void*) &tx, sizeof(tx), &rx,
			sizeof(rx));
	printk("w :%d %d \n", tx[1], tx[0]);
	for (; i < size; i++)
		printk("r :%d", rx[i]);

	if (ret < 0) {
		dev_err(&chip->spi_dev->dev, "SPI read error\n");
		return ret;
	}

	memcpy(data, rx, size);

	return ret;
}


#define INIT_SETUP( val ,reg) ad9510_write(chip,(reg),(val),1)

static int ad9510_store_configuration(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t size) {

	struct ad9510_data *chip;
	int ret = 0;
	chip = dev_get_drvdata(dev);

	INIT_SETUP( 0x4, 0x7);//REG_LOR
	INIT_SETUP(0x7b, 0x8);//REG_MUX
	INIT_SETUP(0x30, 0x09);//REG_CNTRESET
	INIT_SETUP(0x01, 0x0A);//REG_POWER_PRESCALER
	INIT_SETUP(0x00, 0x0D);//REG_DLD
	
	INIT_SETUP(0x01, 0x34);//REG_DELAY_CONTROL_5
	INIT_SETUP(0x00, 0x35);//REG_RAMP_5
	INIT_SETUP(0x00, 0x36);//REG_RAMP_5
	
	INIT_SETUP(0x01, 0x38);//REG_DELAY_CONTROL_6
	INIT_SETUP(0x00, 0x39);//REG_RAMP_6
	INIT_SETUP(0x00, 0x3A);//REG_RAMP_6
	
	INIT_SETUP(0x08, 0x3C);//REG_LVPECL_0
	INIT_SETUP(0x08, 0x3D);//REG_LVPECL_1
	INIT_SETUP(0x08, 0x3E);//REG_LVPECL_2
	INIT_SETUP(0x08, 0x3F);//REG_LVPECL_3
	
	INIT_SETUP(0x03, 0x40);//REG_LVDSCMOS_4
	INIT_SETUP(0x10, 0x41);//REG_LVDSCMOS_5
	INIT_SETUP(0x03, 0x42);//REG_LVDSCMOS_6
	INIT_SETUP(0x02, 0x43);//REG_LVDSCMOS_7
	
	INIT_SETUP(0x02, 0x45);//REG_CLK_CONTROL
	
	INIT_SETUP(0x00, 0x48);//REG_OUT0_DUTY
	INIT_SETUP(0x80, 0x49);//REG_OUT0_BYPASS
	INIT_SETUP(0x00, 0x4A);//REG_OUT1_DUTY
	INIT_SETUP(0x80, 0x4B);//REG_OUT1_BYPASS
	INIT_SETUP(0x11, 0x4C);//REG_OUT2_DUTY
	INIT_SETUP(0x80, 0x4D);//REG_OUT2_BYPASS
	INIT_SETUP(0x33, 0x4E);//REG_OUT3_DUTY
	INIT_SETUP(0x80, 0x4F);//REG_OUT3_BYPASS
	INIT_SETUP(0x00, 0x50);//REG_OUT4_DUTY
	INIT_SETUP(0x00, 0x51);//REG_OUT4_BYPASS
	INIT_SETUP(0x44, 0x52);//REG_OUT5_DUTY
	INIT_SETUP(0x00, 0x53);//REG_OUT5_BYPASS
	INIT_SETUP(0x00, 0x54);//REG_OUT6_DUTY
	INIT_SETUP(0x00, 0x55);//REG_OUT6_BYPASS
	INIT_SETUP(0x00, 0x56);//REG_OUT7_DUTY
	INIT_SETUP(0x80, 0x57);//REG_OUT7_BYPASS
	
	INIT_SETUP(0x00, 0x58);//REG_FUNCTION //flush()

	if (ret < 0)
		return ret;
	return size;
}

static ssize_t ad9510_store_divider6(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t size) {

	struct ad9510_data *chip;
	int ret = 0;

	long unsigned int userinput;
	u8 data = 0;

	chip = dev_get_drvdata(dev);

	userinput = 0;
	ret=strict_strtoul(buf, 0, &userinput);
	if(ret<0)
		return ret;

	data = (u8) userinput;

	ret = ad9510_write(chip, 0x4, data, sizeof(data));
	if (ret < 0)
		return ret;
	ret = ad9510_write(chip, 0x5a, 1, 1); //flush
	if (ret < 0)
		return ret;

	return size;
}

static ssize_t ad9510_show_divider6(struct device *dev,
		struct device_attribute *attr, char *buf) {
	struct ad9510_data *chip;
	int ret = 0;
	u8 rx = 0;

	chip = dev_get_drvdata(dev);

	ret = ad9510_read(chip, 0x4, &rx, sizeof(rx));
	if (ret < 0)
		return ret;

	return sprintf(buf, "%d\n", rx);
}

static ssize_t ad9510_store_divider13(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t size) {

	struct ad9510_data *chip;
	int ret = 0;

	long unsigned int userinput;
	u8 txl = 0, txm = 0;

	chip = dev_get_drvdata(dev);

	userinput = 0;
	ret=strict_strtoul(buf, 0, &userinput);
	if(ret<0)
		return ret;

	txm = (userinput & 0xff00) >> 8;
	txl = userinput & 0xff;

	ret = ad9510_write(chip, 0x5, txm, sizeof(txm));
	if (ret < 0)
		return ret;
	ret = ad9510_write(chip, 0x6, txl, sizeof(txl));
	if (ret < 0)
		return ret;

	ret = ad9510_write(chip, 0x5a, 1, 1); //flush
	if (ret < 0)
		return ret;

	return size;
}

static ssize_t ad9510_show_divider13(struct device *dev,
		struct device_attribute *attr, char *buf) {
	struct ad9510_data *chip;
	int ret = 0;
	u8 rxl = 0;
	u8 rxm = 0;
	u16 data = 0;

	chip = dev_get_drvdata(dev);

	ret = ad9510_read(chip, 0x6, &rxl, sizeof(rxl));
	if (ret < 0)
		return ret;
	ret = ad9510_read(chip, 0x5, &rxm, sizeof(rxm));
	if (ret < 0)
		return ret;

	data = rxm << 8;
	data |= rxl;

	return sprintf(buf, "%d\n", data);
}

static ssize_t ad9510_store_divider14(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t size) {

	struct ad9510_data *chip;
	int ret = 0;

	long unsigned int userinput;
	u8 datal = 0;
	u8 datam = 0;
	chip = dev_get_drvdata(dev);

	userinput = 0;
	ret=strict_strtoul(buf, 0, &userinput);
	if(ret<0)
		return ret;

	datal = userinput & 0xff;
	datam = (userinput & 0xff00) >> 8;

	ret = ad9510_write(chip, 0xc, datal, sizeof(datal));
	if (ret < 0)
		return ret;
	ret = ad9510_write(chip, 0xb, datam, sizeof(datam));
	if (ret < 0)
		return ret;
	ret = ad9510_write(chip, 0x5a, 1, 1); //flush
	if (ret < 0)
		return ret;

	return size;
}

static ssize_t ad9510_show_divider14(struct device *dev,
		struct device_attribute *attr, char *buf) {
	struct ad9510_data *chip;
	int ret = 0;
	u8 rxm = 0, rxl = 0;
	u16 data = 0;

	chip = dev_get_drvdata(dev);

	ret = ad9510_read(chip, 0xb, &rxm, sizeof(rxm));
	if (ret < 0)
		return ret;
	ret = ad9510_read(chip, 0xc, &rxl, sizeof(rxl));
	if (ret < 0)
		return ret;

	data = rxm << 8;
	data |= rxl;
	return sprintf(buf, "%d\n", data);
}

static ssize_t ad9510_store_pll4(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t size) {

	struct ad9510_data *chip;
	int ret = 0;

	long unsigned int userinput;
	u8 data = 0;

	chip = dev_get_drvdata(dev);

	userinput = 0;

	ret=strict_strtoul(buf, 0, &userinput);
	if(ret<0)
		return ret;

	data = (userinput & 0xef);

	ret = ad9510_write(chip, 0xa, data, sizeof(data));
	if (ret < 0)
		return ret;

	ret = ad9510_write(chip, 0x5a, 1, 1); //flush
	if (ret < 0)
		return ret;

	return size;
}

static ssize_t ad9510_show_pll4(struct device *dev,
		struct device_attribute *attr, char *buf) {
	struct ad9510_data *chip;
	int ret = 0;
	u8 rx = 0;

	chip = dev_get_drvdata(dev);

	ret = ad9510_read(chip, 0xa, &rx, sizeof(rx));
	if (ret < 0)
		return ret;

	return sprintf(buf, "%d\n", rx);
}

static ssize_t ad9510_store_spicon(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t size) {

	struct ad9510_data *chip;
	int ret = 0;

	long unsigned int userinput;
	u8 data = 0;

	chip = dev_get_drvdata(dev);

	userinput = 0;

	ret=strict_strtoul(buf, 0, &userinput);
	if(ret<0)
		return ret;

	data = userinput & 0xff00;

	ret = ad9510_write(chip, 0, data, sizeof(data));

	if (ret < 0)
		return ret;
	//changes to SPI CON takes effect immediately no need to set 0x5a:0 register

	return size;
}

static ssize_t ad9510_show_spicon(struct device *dev,
		struct device_attribute *attr, char *buf) {
	struct ad9510_data *chip;
	int ret;
	u8 rx = 0;

	chip = dev_get_drvdata(dev);

	ret = ad9510_read(chip, 0x0, &rx, sizeof(rx));

	if (ret < 0)
		return ret;

	return sprintf(buf, "%d\n", rx);
}

static DEVICE_ATTR(pll4, S_IRUGO | S_IWUSR, ad9510_show_pll4, ad9510_store_pll4);
static DEVICE_ATTR(spicon, S_IRUGO | S_IWUSR, ad9510_show_spicon, ad9510_store_spicon);
static DEVICE_ATTR(dividerR, S_IRUGO | S_IWUSR, ad9510_show_divider14, ad9510_store_divider14);
static DEVICE_ATTR(dividerA, S_IRUGO | S_IWUSR, ad9510_show_divider6, ad9510_store_divider6);
static DEVICE_ATTR(dividerB, S_IRUGO | S_IWUSR, ad9510_show_divider13, ad9510_store_divider13);
static DEVICE_ATTR(configuration, S_IWUSR, NULL, ad9510_store_configuration);

static struct attribute *ad9510_attributes[] = {
		&dev_attr_pll4.attr,
		&dev_attr_spicon.attr,
		&dev_attr_dividerA.attr,
		&dev_attr_dividerB.attr,
		&dev_attr_dividerR.attr,
		&dev_attr_configuration.attr,
		NULL,
};

static const struct attribute_group ad9510_group =
		{
				.attrs = ad9510_attributes,
		};

static int __devinit ad9510_probe(struct spi_device *spi_dev)
{
	int ret;
	struct ad9510_data *chip;

	chip = kzalloc(sizeof(*chip), GFP_KERNEL);
	if (chip == NULL) {
		ret = -ENOMEM;
		goto error_ret;
	}
	chip->spi_dev=spi_dev;

	dev_set_drvdata(&spi_dev->dev, chip);

	ret = sysfs_create_group(&spi_dev->dev.kobj, &ad9510_group);
	if (ret < 0)
	goto error_free_chip;

/*	chip->hwmon_dev = hwmon_device_register(&spi_dev->dev);

	if (IS_ERR(chip->hwmon_dev)) {
		ret = PTR_ERR(chip->hwmon_dev);
		goto error_remove_group;
	}
*/
	return 0;
/*error_remove_group:
	sysfs_remove_group(&spi_dev->dev.kobj, &ad9510_group);*/
error_free_chip:
	kfree(chip);
error_ret:
	return ret;
}

static int __devexit ad9510_remove(struct spi_device *spi_dev)
{
	struct ad9510_data *chip;

	chip = dev_get_drvdata(&spi_dev->dev);
	printk("Spi protocol driver for ad9510 remove \n");
/*	hwmon_device_unregister(chip->hwmon_dev);*/
	sysfs_remove_group(&spi_dev->dev.kobj, &ad9510_group);
	kfree(chip);

	return 0;
}

static const struct spi_device_id ad9510_id[] = {
		{ "ad9510", ad9510 },
		{ }
};

MODULE_DEVICE_TABLE( spi, ad9510_id);

static struct spi_driver ad9510_driver = {
		.driver ={
				.name = "ad9510",
				.owner = THIS_MODULE,
		},
		.probe = ad9510_probe,
		.remove = __devexit_p(ad9510_remove),
		.id_table = ad9510_id,
};

static __init int ad9510_init(void)
{
	printk("Spi protocol driver for  ad9510 \n");
	return spi_register_driver(&ad9510_driver);
}
module_init( ad9510_init);

static __exit void ad9510_exit(void)
{
	printk("Spi protocol driver for ad9510 bye :) \n");
	spi_unregister_driver(&ad9510_driver);
}
module_exit( ad9510_exit);

MODULE_AUTHOR("Uros Golob <uros.golob@i-tech.si>");
MODULE_DESCRIPTION("ad9510" "1.2 GHz Clock Distribution IC");
MODULE_LICENSE("GPL v2");
