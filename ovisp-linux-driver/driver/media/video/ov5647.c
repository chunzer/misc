
#include <linux/init.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/i2c.h>
#include <linux/delay.h>
#include <linux/videodev2.h>
#include <media/v4l2-device.h>
#include <media/v4l2-chip-ident.h>
#include <media/v4l2-mediabus.h>
#include <media/ov5647.h>
#include <mach/ovisp-v4l2.h>

#define OV5647_CHIP_ID_H	(0x56)
#define OV5647_CHIP_ID_L	(0x47)

#define SXGA_WIDTH		1280
#define SXGA_HEIGHT		960
#define MAX_WIDTH		2560
#define MAX_HEIGHT		1920
//#define MAX_WIDTH		2592
//#define MAX_HEIGHT		1944

#define OV5647_REG_END		0xffff
#define OV5647_REG_DELAY	0xfffe

struct ov5647_format_struct;
struct ov5647_info {
	struct v4l2_subdev sd;
	struct ov5647_format_struct *fmt;
	struct ov5647_win_size *win;
};

struct regval_list {
	unsigned short reg_num;
	unsigned char value;
};

struct otp_struct {
	unsigned int customer_id;
	unsigned int module_integrator_id;
	unsigned int lens_id;
	unsigned int rg_ratio;
	unsigned int bg_ratio;
	unsigned int user_data[5];
};

#if 1
static struct regval_list ov5647_init_regs[] = {
	{0x0100, 0x00}, // software standby
	{0x0103, 0x01}, // software reset
	//delay(5ms) 
	{OV5647_REG_DELAY, 5},
	{0x370c, 0x03}, // analog control
	{0x5000, 0x06}, // lens off, bpc on, wpc on
	{0x5003, 0x08}, // buf_en
	{0x5a00, 0x08}, //
	{0x3000, 0xff}, // D[9:8] output
	{0x3001, 0xff}, // D[7:0] output
	{0x3002, 0xff}, // Vsync, Href, PCLK, Frex, Strobe, SDA, GPIO1, GPIO0 output
	{0x301d, 0xf0}, //
	{0x3a18, 0x00}, // gain ceiling = 15.5x
	{0x3a19, 0xf8}, // gain ceiling
	{0x3c01, 0x80}, // band detection manual mode
	{0x3b07, 0x0c}, // strobe frex mode
	//analog control
	{0x3630, 0x2e},
	{0x3632, 0xe2},
	{0x3633, 0x23},
	{0x3634, 0x44},
	{0x3620, 0x64},
	{0x3621, 0xe0},
	{0x3600, 0x37},
	{0x3704, 0xa0},
	{0x3703, 0x5a},
	{0x3715, 0x78},
	{0x3717, 0x01},
	{0x3731, 0x02},
	{0x370b, 0x60},
	{0x3705, 0x1a},
	{0x3f05, 0x02},
	{0x3f06, 0x10},
	{0x3f01, 0x0a},
	// AG/AE target
	{0x3a0f, 0x58}, // stable in high
	{0x3a10, 0x50}, // stable in low
	{0x3a1b, 0x58}, // stable out high
	{0x3a1e, 0x50}, // stable out low
	{0x3a11, 0x60}, // fast zone high
	{0x3a1f, 0x28}, // fast zone low
	{0x4001, 0x02}, // BLC start line
	{0x4000, 0x09}, // BLC enable
	{0x4005, 0x18}, // BLC trigger
	{0x3000, 0x00}, // D[9:8] input
	{0x3001, 0x00}, // D[7:0] input
	{0x3002, 0x00}, // Vsync, Href, PCLK, Frex, Strobe, SDA, GPIO1, GPIO0 input
	{0x3017, 0xe0}, // MIPI PHY
	{0x301c, 0xfc}, //
	{0x3636, 0x06}, // analog control
	{0x3016, 0x08}, // MIPI pad enable
	{0x3827, 0xec}, //
	{0x3018, 0x44}, // MIPI 2 lane, MIPI enable
	{0x3035, 0x21}, // PLL
	{0x3106, 0xf5}, // PLL 
	{0x3034, 0x1a}, // PLL
	{0x301c, 0xf8}, //
	{0x3503, 0x23}, // Gain has no latch delay, AGC manual, AEC manual                
	{0x3501, 0x10}, // exposure[15:8]
	{0x3502, 0x80}, // exposure[7:0]
	{0x350a, 0x00}, // gain[9:8]
	{0x350b, 0x7f}, // gain[7:0]
	{0x5001, 0x00},// lcm_dbg, 0521, close AWB. 0x01, 8}, // AWB on
	{0x5180, 0x08}, // AWB manual gain enable
	{0x5186, 0x04}, // manual red gain high
	{0x5187, 0x00}, // manual red gain low
	{0x5188, 0x04}, // manual green gain high
	{0x5189, 0x00}, // manual green gain low
	{0x518a, 0x04}, // manual blue gain high
	{0x518b, 0x00}, // manual blue gain low
	{0x5000, 0x06}, // lenc off, bpc on, wpc on

	//1280* 960, 30fps, 19.5M mclk, MIPI clock 279.5M bps/lane
#if 1
	{0x0100, 0x00}, // software standby
	{0x3035, 0x21}, // PLL
	{0x3036, 0x56}, // PLL
	{0x303c, 0x11}, // PLL
	{0x3821, 0x07}, // ISP mirror on, Sensor mirror on, bin o
	{0x3820, 0x41}, // ISP flip off, Sensor flip off, bin on
	{0x3612, 0x59}, // analog control
	{0x3618, 0x00}, // analog control
	{0x380c, 0x07}, // HTS = 1896
	{0x380d, 0x68}, // HTS
	{0x380e, 0x03}, // VTS = 984
	{0x380f, 0xd8}, // VTS
	{0x3814, 0x31}, // X INC
	{0x3815, 0x31}, // Y INC
	{0x3708, 0x64}, // analog control
	{0x3709, 0x52}, // analog control
	{0x3808, 0x05}, // X OUTPUT SIZE = 1280
	{0x3809, 0x00}, // X OUTPUT SIZE
	{0x380a, 0x03}, // Y OUTPUT SIZE = 960
	{0x380b, 0xc0}, // Y OUTPUT SIZE
	{0x3800, 0x00}, // X Start
	{0x3801, 0x18}, // X Start
	{0x3802, 0x00}, // Y Start
	{0x3803, 0x0e}, // Y Start
	{0x3804, 0x0a}, // X End
	{0x3805, 0x27}, // X End
	{0x3806, 0x07}, // Y End
	{0x3807, 0x95}, // Y End
	//banding filte
	{0x3a08, 0x01}, // B50
	{0x3a09, 0x27}, // B50
	{0x3a0a, 0x00}, // B60
	{0x3a0b, 0xf6}, // B60
	{0x3a0d, 0x04}, // B50 max
	{0x3a0e, 0x03}, // B60 max
	{0x4004, 0x02}, // black line number
	{0x4005, 0x18}, // BLC trigger
	{0x4837, 0x24}, // MIPI pclk period
	{0x0100, 0x01}, // wake up from software standby
#endif

	/* Sensor enter LP11*/
	//{0x4202, 0x0f},
	//{0x4800, 0x01},

	{OV5647_REG_DELAY, 50},
	//2560* 1920, 10.29fps, 19.5M mclk, MIPI clock 279.5M bps/lane
	{0x3201, 0x00}, // group 1 start addr 0
	{0x3208, 0x01},
	{0x0100, 0x00}, // software standby
	{0x3035, 0x21}, // PLL
	{0x3036, 0x56}, // PLL
	{0x303c, 0x11}, // PLL
	{0x3821, 0x07}, // ISP mirror on, Sensor mirror on, bin o
	{0x3820, 0x41}, // ISP flip off, Sensor flip off, bin on
	{0x3612, 0x59}, // analog control
	{0x3618, 0x00}, // analog control
	{0x380c, 0x07}, // HTS = 1896
	{0x380d, 0x68}, // HTS
//	{0x380e, 0x03}, // VTS = 984
//	{0x380f, 0xd8}, // VTS
	{0x3814, 0x31}, // X INC
	{0x3815, 0x31}, // Y INC
	{0x3708, 0x64}, // analog control
	{0x3709, 0x52}, // analog control
	{0x3808, 0x05}, // X OUTPUT SIZE = 1280
	{0x3809, 0x00}, // X OUTPUT SIZE
	{0x380a, 0x03}, // Y OUTPUT SIZE = 960
	{0x380b, 0xc0}, // Y OUTPUT SIZE
	{0x3800, 0x00}, // X Start
	{0x3801, 0x18}, // X Start
	{0x3802, 0x00}, // Y Start
	{0x3803, 0x0e}, // Y Start
	{0x3804, 0x0a}, // X End
	{0x3805, 0x27}, // X End
	{0x3806, 0x07}, // Y End
	{0x3807, 0x95}, // Y End
	//banding filter
	{0x3a08, 0x01}, // B50
	{0x3a09, 0x27}, // B50
	{0x3a0a, 0x00}, // B60
	{0x3a0b, 0xf6}, // B60
	{0x3a0d, 0x04}, // B50 max
	{0x3a0e, 0x03}, // B60 max
	{0x4005, 0x18}, // BLC trigger
	{0x4004, 0x02}, // black line number
	{0x4837, 0x24}, // MIPI pclk period
	{0x0100, 0x01}, // wake up from software standby
	//
	{0x3208, 0x11},
	{OV5647_REG_DELAY, 5},
	{0x3202, 0x08}, // group 2 start addr 0x80
	{0x3208, 0x02},
	{0x0100, 0x00}, // software standby
	{0x3035, 0x21}, // PLL
	{0x3036, 0x56}, //lcm_dbg, 0618,0x57, 8}, //lcm_dbg, 0604, modify to 141MHz lane, 10.41fps. 0x7d, 8}, // PLL
	{0x303c, 0x11}, // PLL
	{0x3821, 0x06}, // ISP mirror on, Sensor mirror on
	{0x3820, 0x00}, // ISP flip off, Sensor flip off
	{0x3612, 0x5b}, // analog control
	{0x3618, 0x04}, // analog control
	{0x380c, 0x0a}, // HTS = 2752
	{0x380d, 0xc0}, // HTS
//	{0x380e, 0x07}, // VTS = 1974
//	{0x380f, 0xb6}, // VTS
	{0x3814, 0x11}, // X INC
	{0x3815, 0x11}, // X INC
	{0x3708, 0x64}, // analog control
	{0x3709, 0x12}, // analog control
	{0x3808, 0x0a}, // X OUTPUT SIZE = 2592
	{0x3809, 0x00}, // X OUTPUT SIZE
	{0x380a, 0x07}, // Y OUTPUT SIZE = 1944
	{0x380b, 0x80}, // Y OUTPUT SIZE
	{0x3800, 0x00}, // X Start
	{0x3801, 0x0c}, // X Start
	{0x3802, 0x00}, // Y Start
	{0x3803, 0x02}, // Y Start
	{0x3804, 0x0a}, // X End
	{0x3805, 0x33}, // X End
	{0x3806, 0x07}, // Y End
	{0x3807, 0xa1}, // Y End
	//Banding filter
	{0x3a08, 0x01}, // B50
	{0x3a09, 0x28}, // B50
	{0x3a0a, 0x00}, // B60
	{0x3a0b, 0xf6}, // B60
	{0x3a0d, 0x07}, // B60 max
	{0x3a0e, 0x06}, // B50 max
	{0x4004, 0x04}, // black line number
	{0x4005, 0x1a}, // BLC trigger
	{0x4837, 0x24}, //lcm_dbg, 0618, 0x23, 8}, //lcm_dbg, 0604, modify to 141MHz lane, 10.41fps.0x57, 8}, // MIPI pclk period
	{0x0100, 0x01}, // wake up from software standby
#if 0
	{0x3000, 0x00},
	{0x3001, 0x00},
	{0x3002, 0x00},
	{0x3017, 0xe0},
	{0x301c, 0xfc},
	{0x3636, 0x06},
	{0x3016, 0x08},
	{0x3827, 0xec},
	{0x3018, 0x44},
	{0x3035, 0x21},
	{0x3106, 0xf5},
	{0x3034, 0x1a},
	{0x301c, 0xf8},
#endif
	{0x3208, 0x12},
	/* Sensor enter LP11*/
	{0x4202, 0x0f},
	{0x4800, 0x01},

	{OV5647_REG_END, 0x00},	/* END MARKER */
};

#else
static struct regval_list ov5647_init_regs[] = {
	{0x0100, 0x00}, // software standby
		{0x0103, 0x01}, // software reset
		//delay(5ms) 
		{OV5647_REG_DELAY, 5},
		//write initialization table
		{0x370c, 0x03}, // analog control
		{0x5000, 0x06}, // lens off, bpc on, wpc on
		{0x5003, 0x08}, // buf_en
		{0x5a00, 0x08}, //
		{0x3000, 0xff}, // D[9:8] output
		{0x3001, 0xff}, // D[7:0] output
		{0x3002, 0xff}, // Vsync, Href, PCLK, Frex, Strobe, SDA, GPIO1, GPIO0 output
		{0x301d, 0xf0}, //
		{0x3a18, 0x00}, // gain ceiling = 15.5x
		{0x3a19, 0xf8}, // gain ceiling
		{0x3c01, 0x80}, // band detection manual mode
		{0x3b07, 0x0c}, // strobe frex mode
		//analog control
		{0x3630, 0x2e},
		{0x3632, 0xe2},
		{0x3633, 0x23},
		{0x3634, 0x44},
		{0x3620, 0x64},
		{0x3621, 0xe0},
		{0x3600, 0x37},
		{0x3704, 0xa0},
		{0x3703, 0x5a},
		{0x3715, 0x78},
		{0x3717, 0x01},
		{0x3731, 0x02},
		{0x370b, 0x60},
		{0x3705, 0x1a},
		{0x3f05, 0x02},
		{0x3f06, 0x10},
		{0x3f01, 0x0a},
		// AG/AE target
		{0x3a0f, 0x58}, // stable in high
		{0x3a10, 0x50}, // stable in low
		{0x3a1b, 0x58}, // stable out high
		{0x3a1e, 0x50}, // stable out low
		{0x3a11, 0x60}, // fast zone high
		{0x3a1f, 0x28}, // fast zone low
		{0x4001, 0x02}, // BLC start line
		{0x4000, 0x09}, // BLC enable
		// add by zdq 2012.8.10 begin
		{0x4005, 0x18}, // BLC triggered by gain changed
		//{0x4051, 0x8f}, // BLC triggered when changed too much
		// add by zdq 2012.8.10 end
		{0x3000, 0x00}, // D[9:8] input
		{0x3001, 0x00}, // D[7:0] input
		{0x3002, 0x00}, // Vsync, Href, PCLK, Frex, Strobe, SDA, GPIO1, GPIO0 input
		{0x3017, 0xe0}, // MIPI PHY
		{0x301c, 0xfc}, //
		{0x3636, 0x06}, // analog control
		{0x3016, 0x08}, // MIPI pad enable
		{0x3827, 0xec}, //
		{0x3018, 0x44}, // MIPI 2 lane, MIPI enable
		{0x3035, 0x21}, // PLL
		{0x3106, 0xf5}, // PLL 
		{0x3034, 0x1a}, // PLL
		// add by zdq 2012.8.10 begin
		//{0x3035, 0x21}, // PLL
		//{0x3036, 0x56/*66*/}, // PLL
		//{0x303c, 0x11}, // PLL
		// add by zdq 2012.8.10 end
		{0x301c, 0xf8}, //
		{0x3503, 0x23}, // Gain has no latch delay, AGC manual, AEC manual                
		{0x3501, 0x10}, // exposure[15:8]
		{0x3502, 0x80}, // exposure[7:0]
		{0x350a, 0x00}, // gain[9:8]
		{0x350b, 0x7f}, // gain[7:0]
		// edit by zdq 2012.8.10 01:awb_on 00:awb_off
		{0x5001, 0x00}, // lcm_dbg, 0521, close AWB. 0x01, 8}, // AWB on
		{0x5180, 0x08}, // AWB manual gain enable
		{0x5186, 0x04}, // manual red gain high
		{0x5187, 0x00}, // manual red gain low
		{0x5188, 0x04}, // manual green gain high
		{0x5189, 0x00}, // manual green gain low
		{0x518a, 0x04}, // manual blue gain high
		{0x518b, 0x00}, // manual blue gain low
		{0x5000, 0x06}, // lenc off, bpc on, wpc on
		// add by zdq 2012.8.10 begin
		/*
		// LENC setting should be tuned from typical module
		//OV5647 LENC settings for CM8312
		{0x5800, 0x11},
		{0x5801, 0x0a},
		{0x5802, 0x09},
		{0x5803, 0x09},
		{0x5804, 0x0a},
		{0x5805, 0x0f},
		{0x5806, 0x07},
		{0x5807, 0x05},
		{0x5808, 0x03},
		{0x5809, 0x03},
		{0x580a, 0x05},
		{0x580b, 0x07},
		{0x580c, 0x05},
		{0x580d, 0x02},
		{0x580e, 0x00},
		{0x580f, 0x00},
		{0x5810, 0x02},
		{0x5811, 0x05},
		{0x5812, 0x05},
		{0x5813, 0x02},
		{0x5814, 0x00},
		{0x5815, 0x00},
		{0x5816, 0x01},
		{0x5817, 0x05},
		{0x5818, 0x08},
		{0x5819, 0x05},
		{0x581a, 0x03},
		{0x581b, 0x03},
		{0x581c, 0x04},
		{0x581d, 0x07},
		{0x581e, 0x10},
		{0x581f, 0x0b},
		{0x5820, 0x09},
		{0x5821, 0x09},
		{0x5822, 0x09},
		{0x5823, 0x0e},
		// R/B LSC gain should be even number
		{0x5824, 0x28},
		{0x5825, 0x1a},
		{0x5826, 0x1a},
		{0x5827, 0x1a},
		{0x5828, 0x46},
		{0x5829, 0x2a},
		{0x582a, 0x26},
		{0x582b, 0x44},
		{0x582c, 0x26},
		{0x582d, 0x2a},
		{0x582e, 0x28},
		{0x582f, 0x42},
		{0x5830, 0x40},
		{0x5831, 0x42},
		{0x5832, 0x28},
		{0x5833, 0x0a},
		{0x5834, 0x16},
		{0x5835, 0x44},
		{0x5836, 0x26},
		{0x5837, 0x2a},
		{0x5838, 0x28},
		{0x5839, 0x0a},
		{0x583a, 0x0a},
		{0x583b, 0x0a},
		{0x583c, 0x26},
		{0x583d, 0xbe},
		{0x5000, 0x86}, // lenc on, bpc on, wpc on
		*/
		// add by zdq 2012.8.10 end
		// add by zdq 2012.8.10 begin
		{0x4837, 0x24}, // MIPI pclk period
		//{0x4202, 0x0f}, // stream off
		{0x0100, 0x01}, // wake up from software standby
		{OV5647_REG_DELAY, 5},
		// add by zdq 2012.8.10 end
		// add by zdq 2012.8.10 begin
		// update OTP after initialization
		// add by zdq 2012.8.10 end
		 
		// add by zdq 2012.8.10 begin
		/*
		// Detect Light Frequency
		{0x3622, 0x01},
		{0x3635, 0x1c},
		{0x3634, 0x40},
		{0x3c01, 0x34},
		{0x3c00, 0x00},
		{0x3c04, 0x28},
		{0x3c05, 0x98},
		{0x3c06, 0x00},
		{0x3c07, 0x08},
		{0x3c08, 0x00},
		{0x3c09, 0x1c},
		{0x300c, 0x22},
		{0x3c0a, 0x7e}, // xclk/600 high
		{0x3c0b, 0xf4}, // xclk/600 low
		*/
		// add by zdq 2012.8.10 end
		 
		// preview setting
		//1280* 960, 30fps, 19.5M mclk, MIPI clock 279.5M bps/lane
		{0x0100, 0x00}, // software standby
		//{0x3035, 0x21}, // PLL
		{0x3036, 0x56}, // PLL
		{0x303c, 0x11}, // PLL
		{0x3821, 0x07}, // ISP mirror on, Sensor mirror on, bin o
		{0x3820, 0x41}, // ISP flip off, Sensor flip off, bin on
		{0x3612, 0x59}, // analog control
		{0x3618, 0x00}, // analog control
		{0x380c, 0x07}, // HTS = 1896 (preview_HTS/256)
		{0x380d, 0x68}, // HTS (preview_HTS%256)
		{0x380e, 0x03}, // VTS = 984 (preview_VTS/256)
		{0x380f, 0xd8}, // VTS (preview_VTS%256)
		{0x3814, 0x31}, // X INC
		{0x3815, 0x31}, // Y INC
		{0x3708, 0x64}, // analog control
		{0x3709, 0x52}, // analog control
		{0x3808, 0x05}, // X OUTPUT SIZE = 1280
		{0x3809, 0x00}, // X OUTPUT SIZE
		{0x380a, 0x03}, // Y OUTPUT SIZE = 960
		{0x380b, 0xc0}, // Y OUTPUT SIZE
		{0x3800, 0x00}, // X Start 00
		{0x3801, 0x18}, // X Start 08
		{0x3802, 0x00}, // Y Start 00
		{0x3803, 0x0e}, // Y Start 00
		{0x3804, 0x0a}, // X End 0a
		{0x3805, 0x27}, // X End 37
		{0x3806, 0x07}, // Y End 07
		{0x3807, 0x95}, // Y End a7
		//banding filte
		{0x3a08, 0x01}, // B50
		{0x3a09, 0x27}, // B50
		{0x3a0a, 0x00}, // B60
		{0x3a0b, 0xf6}, // B60
		{0x3a0d, 0x04}, // B50 max
		{0x3a0e, 0x03}, // B60 max
		{0x4004, 0x02}, // black line number
		// add by zdq 2012.8.10 begin
		{0x4005, 0x18}, // BLC triggered by gain changed
		// add by zdq 2012.8.10 end
		//{0x4837, 0x24}, // MIPI pclk period
		{0x0100, 0x01}, // wake up from software standby
		 
		/* Sensor enter LP11*/
		//{0x4202, 0x0f},
		//{0x4800, 0x01},
		 
		{OV5647_REG_DELAY, 50},
		 
		// video setting
		{0x3201, 0x00}, // group 1 start addr 0
		{0x3208, 0x01},
		//1280* 960, 30fps, 19.5M mclk, MIPI clock 279.5M bps/lane
		{0x0100, 0x00}, // software standby
		//{0x3035, 0x21}, // PLL
		//{0x3036, 0x56}, // PLL
		//{0x303c, 0x11}, // PLL
		{0x3821, 0x07}, // ISP mirror on, Sensor mirror on, bin o
		{0x3820, 0x41}, // ISP flip off, Sensor flip off, bin on
		{0x3612, 0x59}, // analog control
		{0x3618, 0x00}, // analog control
		{0x380c, 0x07}, // HTS = 1896 (video_HTS/256)  
		{0x380d, 0x68}, // HTS        (video_HTS%256)
		{0x380e, 0x03}, // VTS = 984  (video_VTS/256)  
		{0x380f, 0xd8}, // VTS        (video_VTS%256)
		{0x3814, 0x31}, // X INC
		{0x3815, 0x31}, // Y INC
		//{//0x3708, 0x64}, // analog control
		{0x3709, 0x52}, // analog control
		{0x3808, 0x05}, // X OUTPUT SIZE = 1280
		{0x3809, 0x00}, // X OUTPUT SIZE
		{0x380a, 0x03}, // Y OUTPUT SIZE = 960
		{0x380b, 0xc0}, // Y OUTPUT SIZE
		//{0x3800, 0x00}, // X Start 00
		{0x3801, 0x18}, // X Start 08
		//{0x3802, 0x00}, // Y Start 00
		{0x3803, 0x0e}, // Y Start 00
		//{0x3804, 0x0a}, // X End 0a
		{0x3805, 0x27}, // X End 37
		//{0x3806, 0x07}, // Y End 07
		{0x3807, 0x95}, // Y End a7
		{0x4004, 0x02}, // black line number
		// add by zdq 2012.8.10 begin
		//{0x4005, 0x18}, // BLC triggered by gain changed
		// add by zdq 2012.8.10 end
		//{0x4837, 0x24}, // MIPI pclk period
		{0x0100, 0x01}, // wake up from software standby
		{0x3208, 0x11},
		{OV5647_REG_DELAY, 5},
		//capture setting
		{0x3202, 0x08}, // group 2 start addr 0x80
		{0x3208, 0x02},
		{0x0100, 0x00}, // software standby
		//{0x3035, 0x21}, // PLL
		//{0x3036, 0x56}, //lcm_dbg, 0618,0x57, 8}, //lcm_dbg, 0604, modify to 141MHz lane, 10.41fps. 0x7d, 8}, // PLL
		//{0x303c, 0x11}, // PLL
		{0x3821, 0x06}, // ISP mirror on, Sensor mirror on
		{0x3820, 0x00}, // ISP flip off, Sensor flip off
		{0x3612, 0x5b}, // analog control
		{0x3618, 0x04}, // analog control
		{0x380c, 0x0a}, // HTS = 2752 (capture_HTS/256)  
		{0x380d, 0xc0}, // HTS        (capture_HTS%256)
		{0x380e, 0x07}, // VTS = 1974 (capture_VTS/256)  
		{0x380f, 0xb6}, // VTS        (capture_VTS%256)
		{0x3814, 0x11}, // X INC
		{0x3815, 0x11}, // X INC
		//{0x3708, 0x64}, // analog control
		{0x3709, 0x12}, // analog control
		{0x3808, 0x0a}, // X OUTPUT SIZE = 2592
		{0x3809, 0x20}, // X OUTPUT SIZE
		{0x380a, 0x07}, // Y OUTPUT SIZE = 1944
		{0x380b, 0x98/*98*/}, // Y OUTPUT SIZE
		//{0x3800, 0x00}, // X Start
		{0x3801, 0x0c}, // X Start
		//{0x3802, 0x00}, // Y Start
		{0x3803, 0x02}, // Y Start
		//{0x3804, 0x0a}, // X End
		{0x3805, 0x33}, // X End
		//{0x3806, 0x07}, // Y End
		{0x3807, 0xa1}, // Y End
		{0x4005, 0x1a}, // BLC triggered every frame
		//{0x4837, 0x24}, //lcm_dbg, 0618, 0x23, 8}, //lcm_dbg, 0604, modify to 141MHz lane, 10.41fps.0x57, 8}, // MIPI pclk period
		{0x0100, 0x01}, // wake up from software standby
		{0x3208, 0x12},
		{OV5647_REG_DELAY, 5},
		/* Sensor enter LP11*/
		{0x4202, 0x0f}, // stream off
		{0x4800, 0x01},
		{OV5647_REG_END, 0x00}, /* END MARKER */
};

#endif

static struct regval_list ov5647_stream_on[] = {
	{0x4202, 0x00},
	{0x4800, 0x04},

	{OV5647_REG_END, 0x00},	/* END MARKER */
};

static struct regval_list ov5647_stream_off[] = {
	/* Sensor enter LP11*/
	{0x4202, 0x0f},
	{0x4800, 0x01},

	{OV5647_REG_END, 0x00},	/* END MARKER */
};

static struct regval_list ov5647_win_sxga[] = {
	{OV5647_REG_END, 0x00},	/* END MARKER */
};

static struct regval_list ov5647_win_5m[] = {
	{OV5647_REG_END, 0x00},	/* END MARKER */
};

static int ov5647_read(struct v4l2_subdev *sd, unsigned short reg,
		unsigned char *value)
{
	struct i2c_client *client = v4l2_get_subdevdata(sd);
	unsigned char buf[2] = {reg >> 8, reg & 0xff};
	struct i2c_msg msg[2] = {
		[0] = {
			.addr	= client->addr,
			.flags	= 0,
			.len	= 2,
			.buf	= buf,
		},
		[1] = {
			.addr	= client->addr,
			.flags	= I2C_M_RD,
			.len	= 1,
			.buf	= value,
		}
	};
	int ret;

	ret = i2c_transfer(client->adapter, msg, 2);
	if (ret > 0)
		ret = 0;

	return ret;
}

static int ov5647_write(struct v4l2_subdev *sd, unsigned short reg,
		unsigned char value)
{
	struct i2c_client *client = v4l2_get_subdevdata(sd);
	unsigned char buf[3] = {reg >> 8, reg & 0xff, value};
	struct i2c_msg msg = {
		.addr	= client->addr,
		.flags	= 0,
		.len	= 3,
		.buf	= buf,
	};
	int ret;

	ret = i2c_transfer(client->adapter, &msg, 1);
	if (ret > 0)
		ret = 0;

	return ret;
}

static int ov5647_write_array(struct v4l2_subdev *sd, struct regval_list *vals)
{
	int ret;

	while (vals->reg_num != OV5647_REG_END) {
		if (vals->reg_num == OV5647_REG_DELAY) {
			if (vals->value >= (1000 / HZ))
				msleep(vals->value);
			else
				mdelay(vals->value);
		} else {
			ret = ov5647_write(sd, vals->reg_num, vals->value);
			if (ret < 0)
				return ret;
		}
		vals++;
	}
	return 0;
}

/* R/G and B/G of typical camera module is defined here. */
static unsigned int rg_ratio_typical = 0x58;
static unsigned int bg_ratio_typical = 0x5a;

/*
	index: index of otp group. (0, 1, 2)
 	return: 0, group index is empty
 	1, group index has invalid data
 	2, group index has valid data
*/
static int ov5647_check_otp(struct v4l2_subdev *sd, unsigned short index)
{
	unsigned char temp;
	unsigned short i;
	unsigned short address;
	int ret;

	// read otp into buffer
	ov5647_write(sd, 0x3d21, 0x01);
	mdelay(5);
	address = 0x3d05 + index * 9;
	ret = ov5647_read(sd, address, &temp);
	if (ret < 0)
		return ret;
	
	// disable otp read
	ov5647_write(sd, 0x3d21, 0x00);
	
	// clear otp buffer
	for (i = 0; i < 32; i++)
		ov5647_write(sd, 0x3d00 + i, 0x00);
	
	if (!temp)
		return 0;
	else if (!(temp & 0x80) && (temp & 0x7f))
		return 2;
	else
		return 1;
}

/*
	index: index of otp group. (0, 1, 2)
	return: 0,
*/
static int ov5647_read_otp(struct v4l2_subdev *sd, 
				unsigned short index, struct otp_struct* otp)
{
	unsigned char temp;
	unsigned short address;
	unsigned short i;
	int ret;

	// read otp into buffer
	ov5647_write(sd, 0x3d21, 0x01);
	mdelay(5);

	address = 0x3d05 + index * 9;

	ret = ov5647_read(sd, address, &temp);
	if (ret < 0)
		return ret;
	otp->customer_id = (unsigned int)(temp & 0x7f);

	ret = ov5647_read(sd, address, &temp);
	if (ret < 0)
		return ret;
	otp->module_integrator_id = (unsigned int)temp;

	ret = ov5647_read(sd, address + 1, &temp);
	if (ret < 0)
		return ret;
	otp->lens_id = (unsigned int)temp;

	ret = ov5647_read(sd, address + 2, &temp);
	if (ret < 0)
		return ret;
	otp->rg_ratio = (unsigned int)temp;

	ret = ov5647_read(sd, address + 3, &temp);
	if (ret < 0)
		return ret;
	otp->bg_ratio = (unsigned int)temp;

	printk("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otp.rg_ratio=%d. \n", otp->rg_ratio);
	printk("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otp.bg_ratio=%d. \n", otp->bg_ratio);
	printk("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ rg_ratio_typical=%d. \n", rg_ratio_typical);
	printk("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ bg_ratio_typical=%d. \n", bg_ratio_typical);

	for (i = 0; i<= 4; i++) {
		ret = ov5647_read(sd, address + 4 + i, &temp);
		if (ret < 0)
			return ret;
		otp->user_data[i] = (unsigned int)temp;
	}

	// disable otp read
	ov5647_write(sd, 0x3d21, 0x00);

	// clear otp buffer
	for (i = 0; i < 32; i++)
		ov5647_write(sd, 0x3d00 + i, 0x00);

	return 0;
}

/*
	R_gain, sensor red gain of AWB, 0x400 =1
	G_gain, sensor green gain of AWB, 0x400 =1
	B_gain, sensor blue gain of AWB, 0x400 =1
	return 0;
*/
static int ov5647_update_awb_gain(struct v4l2_subdev *sd, 
				unsigned int R_gain, unsigned int G_gain, unsigned int B_gain)
{
	printk("R_gain:%04x, G_gain:%04x, B_gain:%04x \n ", R_gain, G_gain, B_gain);
	if (R_gain > 0x400) {
		ov5647_write(sd, 0x5186, (unsigned char)(R_gain >> 8));
		ov5647_write(sd, 0x5187, (unsigned char)(R_gain & 0x00ff));
	}

	if (G_gain > 0x400) {
		ov5647_write(sd, 0x5188, (unsigned char)(G_gain >> 8));
		ov5647_write(sd, 0x5189, (unsigned char)(G_gain & 0x00ff));
	}

	if (B_gain > 0x400) {
		ov5647_write(sd, 0x518a, (unsigned char)(B_gain >> 8));
		ov5647_write(sd, 0x518b, (unsigned char)(B_gain & 0x00ff));
	}

	return 0;
}

/*
	call this function after OV5647 initialization
	return value: 0 update success
	1, no OTP
*/
static int ov5647_update_otp(struct v4l2_subdev *sd)
{
	struct otp_struct current_otp;
	unsigned int i;
	unsigned int otp_index;
	unsigned int temp;
	unsigned int R_gain, G_gain, B_gain, G_gain_R, G_gain_B;
	int ret = 0;

	// R/G and B/G of current camera module is read out from sensor OTP
	// check first OTP with valid data
	for (i = 0; i < 3; i++) {
		temp = ov5647_check_otp(sd, i);
		if (temp == 2) {
			otp_index = i;
			break;
		}
	}

	if (i == 3)
		// no valid wb OTP data
		return 1;

	ret = ov5647_read_otp(sd, otp_index, &current_otp);
	if (ret < 0)
		return ret;

	//calculate G gain
	//0x400 = 1x gain
	if (current_otp.bg_ratio < bg_ratio_typical) {
		if (current_otp.rg_ratio < rg_ratio_typical) {
			// current_otp.bg_ratio < bg_ratio_typical &&
			// current_otp.rg_ratio < rg_ratio_typical
			G_gain = 0x400;
			B_gain = 0x400 * bg_ratio_typical / current_otp.bg_ratio;
			R_gain = 0x400 * rg_ratio_typical / current_otp.rg_ratio;
		} else {
			// current_otp.bg_ratio < bg_ratio_typical &&
			// current_otp.rg_ratio >= rg_ratio_typical
			R_gain = 0x400;
			G_gain = (0x400 * current_otp.rg_ratio) / rg_ratio_typical;
			B_gain = (G_gain * bg_ratio_typical) / current_otp.bg_ratio;
			printk("G_gain:%04x\n ", G_gain);
		}
	} else {
		if (current_otp.rg_ratio < rg_ratio_typical) {
			// current_otp.bg_ratio >= bg_ratio_typical &&
			// current_otp.rg_ratio < rg_ratio_typical
			B_gain = 0x400;
			G_gain = 0x400 * current_otp.bg_ratio / bg_ratio_typical;
			R_gain = G_gain * rg_ratio_typical / current_otp.rg_ratio;
		} else {
			// current_otp.bg_ratio >= bg_ratio_typical &&
			// current_otp.rg_ratio >= rg_ratio_typical
			G_gain_B = 0x400 * current_otp.bg_ratio / bg_ratio_typical;
			G_gain_R = 0x400 * current_otp.rg_ratio / rg_ratio_typical;
			if (G_gain_B > G_gain_R ) {
				B_gain = 0x400;
				G_gain = G_gain_B;
				R_gain = G_gain * rg_ratio_typical / current_otp.rg_ratio;
			} else {
				R_gain = 0x400;
				G_gain = G_gain_R;
				B_gain = G_gain * bg_ratio_typical / current_otp.bg_ratio;
			}
		}
	}

	ov5647_update_awb_gain(sd, R_gain, G_gain, B_gain);

	return 0;
}

static int ov5647_reset(struct v4l2_subdev *sd, u32 val)
{
	return 0;
}

static int ov5647_init(struct v4l2_subdev *sd, u32 val)
{
	struct ov5647_info *info = container_of(sd, struct ov5647_info, sd);
	int ret = 0;

	info->fmt = NULL;
	info->win = NULL;

	ret = ov5647_write_array(sd, ov5647_init_regs);
	if (ret < 0)
		return ret;

	ret = ov5647_update_otp(sd);
	if (ret < 0)
		return ret;

	return 0;
}

static int ov5647_detect(struct v4l2_subdev *sd)
{
	unsigned char v;
	int ret;

	ret = ov5647_read(sd, 0x300a, &v);
	if (ret < 0)
		return ret;
	if (v != OV5647_CHIP_ID_H)
		return -ENODEV;
	ret = ov5647_read(sd, 0x300b, &v);
	if (ret < 0)
		return ret;
	if (v != OV5647_CHIP_ID_L)
		return -ENODEV;
	return 0;
}

static struct ov5647_format_struct {
	enum v4l2_mbus_pixelcode mbus_code;
	enum v4l2_colorspace colorspace;
	struct regval_list *regs;
} ov5647_formats[] = {
	{
		.mbus_code	= V4L2_MBUS_FMT_SBGGR8_1X8,
		.colorspace	= V4L2_COLORSPACE_SRGB,
		.regs 		= NULL,
	},
};
#define N_OV5647_FMTS ARRAY_SIZE(ov5647_formats)

static struct ov5647_win_size {
	int	width;
	int	height;
	int	vts;
	struct regval_list *regs; /* Regs to tweak */
} ov5647_win_sizes[] = {
	/* 2560*1920 */
	{
		.width		= MAX_WIDTH,
		.height		= MAX_HEIGHT,
		.vts		= 0x7b6,
		.regs 		= ov5647_win_5m,
	},
	/* SXGA */
	{
		.width		= SXGA_WIDTH,
		.height		= SXGA_HEIGHT,
		.vts		= 0x3d8,
		.regs 		= ov5647_win_sxga,
	},
};
#define N_WIN_SIZES (ARRAY_SIZE(ov5647_win_sizes))

static int ov5647_enum_mbus_fmt(struct v4l2_subdev *sd, unsigned index,
					enum v4l2_mbus_pixelcode *code)
{
	if (index >= N_OV5647_FMTS)
		return -EINVAL;

	*code = ov5647_formats[index].mbus_code;
	return 0;
}

static int ov5647_try_fmt_internal(struct v4l2_subdev *sd,
		struct v4l2_mbus_framefmt *fmt,
		struct ov5647_format_struct **ret_fmt,
		struct ov5647_win_size **ret_wsize)
{
	int index;
	struct ov5647_win_size *wsize;

	for (index = 0; index < N_OV5647_FMTS; index++)
		if (ov5647_formats[index].mbus_code == fmt->code)
			break;
	if (index >= N_OV5647_FMTS) {
		/* default to first format */
		index = 0;
		fmt->code = ov5647_formats[0].mbus_code;
	}
	if (ret_fmt != NULL)
		*ret_fmt = ov5647_formats + index;

	fmt->field = V4L2_FIELD_NONE;

	for (wsize = ov5647_win_sizes; wsize < ov5647_win_sizes + N_WIN_SIZES;
	     wsize++)
		if (fmt->width >= wsize->width && fmt->height >= wsize->height)
			break;
	if (wsize >= ov5647_win_sizes + N_WIN_SIZES)
		wsize--;   /* Take the smallest one */
	if (ret_wsize != NULL)
		*ret_wsize = wsize;

	fmt->width = wsize->width;
	fmt->height = wsize->height;
	fmt->colorspace = ov5647_formats[index].colorspace;
	return 0;
}

static int ov5647_try_mbus_fmt(struct v4l2_subdev *sd,
			    struct v4l2_mbus_framefmt *fmt)
{
	return ov5647_try_fmt_internal(sd, fmt, NULL, NULL);
}

static int ov5647_s_mbus_fmt(struct v4l2_subdev *sd,
			  struct v4l2_mbus_framefmt *fmt)
{
	struct ov5647_info *info = container_of(sd, struct ov5647_info, sd);
	struct i2c_client *client = v4l2_get_subdevdata(sd);
	struct v4l2_fmt_data *data = v4l2_get_fmt_data(fmt);
	struct ov5647_format_struct *fmt_s;
	struct ov5647_win_size *wsize;
	int ret;
	printk("ov5647_s_mbus_fmt\n");
	ret = ov5647_try_fmt_internal(sd, fmt, &fmt_s, &wsize);
	if (ret)
		return ret;
	data->vts = wsize->vts;
	data->mipi_clk = 282;
	if ((info->fmt != fmt_s) && fmt_s->regs) {
		ret = ov5647_write_array(sd, fmt_s->regs);
		if (ret)
			return ret;
	}

	if ((info->win != wsize) && wsize->regs) {

		memset(data, 0, sizeof(*data));
		data->vts = wsize->vts;
		data->mipi_clk = 282; /* Mbps. */
		if ((wsize->width == MAX_WIDTH)
			&& (wsize->height == MAX_HEIGHT)) {
			data->flags = V4L2_I2C_ADDR_16BIT;
			data->slave_addr = client->addr;
			data->reg_num = 1;
			data->reg[0].addr = 0x3208;
			data->reg[0].data = 0xa2;
		} else if ((wsize->width == SXGA_WIDTH)
			&& (wsize->height == SXGA_HEIGHT)) {
			data->flags = V4L2_I2C_ADDR_16BIT;
			data->slave_addr = client->addr;
			data->reg_num = 1;
			data->reg[0].addr = 0x3208;
			data->reg[0].data = 0xa1;
		}
	}

	info->fmt = fmt_s;
	info->win = wsize;

	return 0;
}

static int ov5647_s_stream(struct v4l2_subdev *sd, int enable)
{
	int ret = 0;
	if (enable)
		ret = ov5647_write_array(sd, ov5647_stream_on);
	//else
	//	ret = ov5647_write_array(sd, ov5647_stream_off);

	return ret;
}

static int ov5647_g_crop(struct v4l2_subdev *sd, struct v4l2_crop *a)
{
	a->c.left	= 0;
	a->c.top	= 0;
	a->c.width	= MAX_WIDTH;
	a->c.height	= MAX_HEIGHT;
	a->type		= V4L2_BUF_TYPE_VIDEO_CAPTURE;

	return 0;
}

static int ov5647_cropcap(struct v4l2_subdev *sd, struct v4l2_cropcap *a)
{
	a->bounds.left			= 0;
	a->bounds.top			= 0;
	a->bounds.width			= MAX_WIDTH;
	a->bounds.height		= MAX_HEIGHT;
	a->defrect			= a->bounds;
	a->type				= V4L2_BUF_TYPE_VIDEO_CAPTURE;
	a->pixelaspect.numerator	= 1;
	a->pixelaspect.denominator	= 1;

	return 0;
}

static int ov5647_g_parm(struct v4l2_subdev *sd, struct v4l2_streamparm *parms)
{
	return 0;
}

static int ov5647_s_parm(struct v4l2_subdev *sd, struct v4l2_streamparm *parms)
{
	return 0;
}

static int ov5647_frame_rates[] = { 30, 15, 10, 5, 1 };

static int ov5647_enum_frameintervals(struct v4l2_subdev *sd,
		struct v4l2_frmivalenum *interval)
{
	if (interval->index >= ARRAY_SIZE(ov5647_frame_rates))
		return -EINVAL;
	interval->type = V4L2_FRMIVAL_TYPE_DISCRETE;
	interval->discrete.numerator = 1;
	interval->discrete.denominator = ov5647_frame_rates[interval->index];
	return 0;
}

static int ov5647_enum_framesizes(struct v4l2_subdev *sd,
		struct v4l2_frmsizeenum *fsize)
{
	int i;
	int num_valid = -1;
	__u32 index = fsize->index;

	/*
	 * If a minimum width/height was requested, filter out the capture
	 * windows that fall outside that.
	 */
	for (i = 0; i < N_WIN_SIZES; i++) {
		struct ov5647_win_size *win = &ov5647_win_sizes[index];
		if (index == ++num_valid) {
			fsize->type = V4L2_FRMSIZE_TYPE_DISCRETE;
			fsize->discrete.width = win->width;
			fsize->discrete.height = win->height;
			return 0;
		}
	}

	return -EINVAL;
}

static int ov5647_queryctrl(struct v4l2_subdev *sd,
		struct v4l2_queryctrl *qc)
{
	return 0;
}

static int ov5647_g_ctrl(struct v4l2_subdev *sd, struct v4l2_control *ctrl)
{
	return 0;
}

static int ov5647_s_ctrl(struct v4l2_subdev *sd, struct v4l2_control *ctrl)
{
	return 0;
}

static int ov5647_g_chip_ident(struct v4l2_subdev *sd,
		struct v4l2_dbg_chip_ident *chip)
{
	struct i2c_client *client = v4l2_get_subdevdata(sd);

	return v4l2_chip_ident_i2c_client(client, chip, V4L2_IDENT_OV5647, 0);
}

#ifdef CONFIG_VIDEO_ADV_DEBUG
static int ov5647_g_register(struct v4l2_subdev *sd, struct v4l2_dbg_register *reg)
{
	struct i2c_client *client = v4l2_get_subdevdata(sd);
	unsigned char val = 0;
	int ret;

	if (!v4l2_chip_match_i2c_client(client, &reg->match))
		return -EINVAL;
	if (!capable(CAP_SYS_ADMIN))
		return -EPERM;
	ret = ov5647_read(sd, reg->reg & 0xffff, &val);
	reg->val = val;
	reg->size = 2;
	return ret;
}

static int ov5647_s_register(struct v4l2_subdev *sd, struct v4l2_dbg_register *reg)
{
	struct i2c_client *client = v4l2_get_subdevdata(sd);

	if (!v4l2_chip_match_i2c_client(client, &reg->match))
		return -EINVAL;
	if (!capable(CAP_SYS_ADMIN))
		return -EPERM;
	ov5647_write(sd, reg->reg & 0xffff, reg->val & 0xff);
	return 0;
}
#endif

static const struct v4l2_subdev_core_ops ov5647_core_ops = {
	.g_chip_ident = ov5647_g_chip_ident,
	.g_ctrl = ov5647_g_ctrl,
	.s_ctrl = ov5647_s_ctrl,
	.queryctrl = ov5647_queryctrl,
	.reset = ov5647_reset,
	.init = ov5647_init,
#ifdef CONFIG_VIDEO_ADV_DEBUG
	.g_register = ov5647_g_register,
	.s_register = ov5647_s_register,
#endif
};

static const struct v4l2_subdev_video_ops ov5647_video_ops = {
	.enum_mbus_fmt = ov5647_enum_mbus_fmt,
	.try_mbus_fmt = ov5647_try_mbus_fmt,
	.s_mbus_fmt = ov5647_s_mbus_fmt,
	.s_stream = ov5647_s_stream,
	.cropcap = ov5647_cropcap,
	.g_crop	= ov5647_g_crop,
	.s_parm = ov5647_s_parm,
	.g_parm = ov5647_g_parm,
	.enum_frameintervals = ov5647_enum_frameintervals,
	.enum_framesizes = ov5647_enum_framesizes,
};

static const struct v4l2_subdev_ops ov5647_ops = {
	.core = &ov5647_core_ops,
	.video = &ov5647_video_ops,
};

static ssize_t ov5647_rg_ratio_typical_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	return sprintf(buf, "%d", rg_ratio_typical);
}

static ssize_t ov5647_rg_ratio_typical_store(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t size)
{
	char *endp;
	int value;

	value = simple_strtoul(buf, &endp, 0);
	if (buf == endp)
		return -EINVAL;

	rg_ratio_typical = (unsigned int)value;

	return size;
}

static ssize_t ov5647_bg_ratio_typical_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	return sprintf(buf, "%d", bg_ratio_typical);
}

static ssize_t ov5647_bg_ratio_typical_store(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t size)
{
	char *endp;
	int value;

	value = simple_strtoul(buf, &endp, 0);
	if (buf == endp)
		return -EINVAL;

	bg_ratio_typical = (unsigned int)value;

	return size;
}

static DEVICE_ATTR(ov5647_rg_ratio_typical, 0664, ov5647_rg_ratio_typical_show, ov5647_rg_ratio_typical_store);
static DEVICE_ATTR(ov5647_bg_ratio_typical, 0664, ov5647_bg_ratio_typical_show, ov5647_bg_ratio_typical_store);

static int ov5647_probe(struct i2c_client *client,
			const struct i2c_device_id *id)
{
	struct v4l2_subdev *sd;
	struct ov5647_info *info;
	int ret;

	info = kzalloc(sizeof(struct ov5647_info), GFP_KERNEL);
	if (info == NULL)
		return -ENOMEM;
	sd = &info->sd;
	v4l2_i2c_subdev_init(sd, client, &ov5647_ops);

	/* Make sure it's an ov5647 */
	ret = ov5647_detect(sd);
	if (ret) {
		v4l_err(client,
			"chip found @ 0x%x (%s) is not an ov5647 chip.\n",
			client->addr, client->adapter->name);
		kfree(info);
		return ret;
	}
	v4l_info(client, "ov5647 chip found @ 0x%02x (%s)\n",
			client->addr, client->adapter->name);

	ret = device_create_file(&client->dev, &dev_attr_ov5647_rg_ratio_typical);
	if(ret){
		v4l_err(client, "create dev_attr_ov5647_rg_ratio_typical failed!\n");
		goto err_create_dev_attr_ov5647_rg_ratio_typical;
	}

	ret = device_create_file(&client->dev, &dev_attr_ov5647_bg_ratio_typical);
	if(ret){
		v4l_err(client, "create dev_attr_ov5647_bg_ratio_typical failed!\n");
		goto err_create_dev_attr_ov5647_bg_ratio_typical;
	}

	return 0;

err_create_dev_attr_ov5647_bg_ratio_typical:
	device_remove_file(&client->dev, &dev_attr_ov5647_rg_ratio_typical);
err_create_dev_attr_ov5647_rg_ratio_typical:
	kfree(info);
	return ret;
}

static int ov5647_remove(struct i2c_client *client)
{
	struct v4l2_subdev *sd = i2c_get_clientdata(client);
	struct ov5647_info *info = container_of(sd, struct ov5647_info, sd);

	device_remove_file(&client->dev, &dev_attr_ov5647_rg_ratio_typical);
	device_remove_file(&client->dev, &dev_attr_ov5647_bg_ratio_typical);

	v4l2_device_unregister_subdev(sd);
	kfree(info);
	return 0;
}

static const struct i2c_device_id ov5647_id[] = {
	{ "ov5647", 0 },
	{ }
};
MODULE_DEVICE_TABLE(i2c, ov5647_id);

static struct i2c_driver ov5647_driver = {
	.driver = {
		.owner	= THIS_MODULE,
		.name	= "ov5647",
	},
	.probe		= ov5647_probe,
	.remove		= ov5647_remove,
	.id_table	= ov5647_id,
};

static __init int init_ov5647(void)
{
	return i2c_add_driver(&ov5647_driver);
}

static __exit void exit_ov5647(void)
{
	i2c_del_driver(&ov5647_driver);
}

module_init(init_ov5647);
module_exit(exit_ov5647);

MODULE_DESCRIPTION("A low-level driver for OmniVision ov5647 sensors");
MODULE_LICENSE("GPL");
