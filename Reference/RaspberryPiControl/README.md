#RaspberryShield
<table class="table table-bordered table-striped table-condensed table-hover">
<tr>
    <th>版本</th>
    <th>时间</th>
    <th>更新内容</th>
</tr>
<tr>
    <td>0.1</td>
    <td>2014-07-15</td>
    <td>初稿</td>
</tr>
</table>

#通讯协议简介（rpi_serial2）

此通讯协议为Makeblock串口通用协议，与scratch为通用固件。

#通讯波特率:

*115200

#发送:

*基本格式：

[0xff,0x55,功能,]

起始(1字节) 0x55

功能(1字节) 0x1:只读模块，0x2:只写模块, 0x4:reset


只读模块(各种传感器)：

[0xff,0x55,0x1,长度,模块1,模块2]

长度(1字节):当前burst剩余字节长度

模块(2字节):

	[模块类型,模块引脚]

	模块类型(1字节):定义如下

```c
		#define VERSION 0
		#define ULTRASONIC_SENSOR 1
		#define TEMPERATURE_SENSOR 2
		#define LIGHT_SENSOR 3
		#define POTENTIONMETER 4
		#define JOYSTICK 5
		#define GYRO 6
		#define RGBLED 8
		#define SEVSEG 9
		#define MOTOR 10
		#define SERVO 11
		#define ENCODER 12
		#define INFRARED 16
		#define LINEFOLLOWER 17
		#define DIGITAL_INPUT 30
		#define ANALOG_INPUT 31
		#define DIGITAL_OUTPUT 32
		#define ANALOG_OUTPUT 33
		#define PWM_OUTPUT 34
```

	模块引脚(1字节):

		高4位:port号

		低4位:slot号

		对应arduino引脚如下:

```c
		#if defined(__AVR_ATmega32U4__) //MeBaseBoard use ATmega32U4 as MCU
		MePort_Sig mePort[11] = {{NC, NC}, {11, A8}, {13, A11}, {A10, A9}, {1, 0},
		    {MISO, SCK}, {A0, A1}, {A2, A3}, {A4, A5}, {6, 7}, {5, 4}
		};
		#else // else ATmega328
		MePort_Sig mePort[11] = {{NC, NC}, {11, 10}, {3, 9}, {12, 13}, {8, 2},
		    {NC, NC}, {A2, A3}, {NC, A1}, {NC, A0}, {6, 7}, {5, 4}
		};
		#endif
```

只写模块(电机,舵机,RGB灯等等):

[0xff,0x55,0x2,长度,模块1,模块1数据]

长度(1字节):当前burst剩余字节长度,一般为0x6

模块类型(1字节):

模块数据(4字节浮点):


#接收:

只写模块固件返回:

[0xff,0x55,'\n']
只读固件返回:
[0xff,0x55,模块1数据,模块2数据,'\n']
模块返回数据为4字节浮点
