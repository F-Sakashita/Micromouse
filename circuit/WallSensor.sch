EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 3 4
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L device:Q_Photo_NPN Q3
U 1 1 5F95E869
P 4300 2150
F 0 "Q3" H 4490 2196 50  0000 L CNN
F 1 "ST-1KL3A" H 4490 2105 50  0000 L CNN
F 2 "LEDs:LED_D5.0mm" H 4500 2250 50  0001 C CNN
F 3 "http://www.kashinoki.co.jp/pdf/ST-1KL3A.pdf" H 4300 2150 50  0001 C CNN
	1    4300 2150
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR0132
U 1 1 5F9613E0
P 4400 1400
F 0 "#PWR0132" H 4400 1250 50  0001 C CNN
F 1 "+3.3V" H 4415 1573 50  0000 C CNN
F 2 "" H 4400 1400 50  0001 C CNN
F 3 "" H 4400 1400 50  0001 C CNN
	1    4400 1400
	1    0    0    -1  
$EndComp
$Comp
L device:R R12
U 1 1 5F969F9C
P 4400 2750
F 0 "R12" H 4470 2796 50  0000 L CNN
F 1 "1k" H 4470 2705 50  0000 L CNN
F 2 "" V 4330 2750 50  0001 C CNN
F 3 "" H 4400 2750 50  0001 C CNN
	1    4400 2750
	1    0    0    -1  
$EndComp
Wire Wire Line
	4400 2350 4400 2500
$Comp
L power:GND #PWR0133
U 1 1 5F96B006
P 4400 3150
F 0 "#PWR0133" H 4400 2900 50  0001 C CNN
F 1 "GND" H 4405 2977 50  0000 C CNN
F 2 "" H 4400 3150 50  0001 C CNN
F 3 "" H 4400 3150 50  0001 C CNN
	1    4400 3150
	1    0    0    -1  
$EndComp
Wire Wire Line
	4400 2900 4400 3050
Text HLabel 3900 2500 0    50   Input ~ 0
WALL_SEN0
Wire Wire Line
	3850 2500 4400 2500
Connection ~ 4400 2500
Wire Wire Line
	4400 2500 4400 2600
Wire Wire Line
	5650 1650 5650 1950
Wire Wire Line
	5650 1650 6950 1650
Wire Wire Line
	6950 1650 6950 1950
Wire Wire Line
	6950 1650 8150 1650
Wire Wire Line
	8150 1650 8150 1950
Connection ~ 6950 1650
$Comp
L device:R R15
U 1 1 5F984CD4
P 5650 2750
F 0 "R15" H 5720 2796 50  0000 L CNN
F 1 "1k" H 5720 2705 50  0000 L CNN
F 2 "" V 5580 2750 50  0001 C CNN
F 3 "" H 5650 2750 50  0001 C CNN
	1    5650 2750
	1    0    0    -1  
$EndComp
$Comp
L device:R R17
U 1 1 5F98543A
P 6950 2750
F 0 "R17" H 7020 2796 50  0000 L CNN
F 1 "1k" H 7020 2705 50  0000 L CNN
F 2 "" V 6880 2750 50  0001 C CNN
F 3 "" H 6950 2750 50  0001 C CNN
	1    6950 2750
	1    0    0    -1  
$EndComp
$Comp
L device:R R20
U 1 1 5F985747
P 8150 2750
F 0 "R20" H 8220 2796 50  0000 L CNN
F 1 "1k" H 8220 2705 50  0000 L CNN
F 2 "" V 8080 2750 50  0001 C CNN
F 3 "" H 8150 2750 50  0001 C CNN
	1    8150 2750
	1    0    0    -1  
$EndComp
Text HLabel 5300 2500 0    50   Input ~ 0
WALL_SEN1
Text HLabel 6600 2500 0    50   Input ~ 0
WALL_SEN2
Text HLabel 7950 2500 0    50   Input ~ 0
WALL_SEN3
Wire Wire Line
	5650 2350 5650 2500
Wire Wire Line
	4400 3050 5650 3050
Wire Wire Line
	5650 3050 5650 2900
Connection ~ 4400 3050
Wire Wire Line
	4400 3050 4400 3150
Wire Wire Line
	5650 3050 6950 3050
Wire Wire Line
	6950 3050 6950 2900
Connection ~ 5650 3050
Wire Wire Line
	6950 3050 8150 3050
Wire Wire Line
	8150 3050 8150 2900
Connection ~ 6950 3050
Wire Wire Line
	8150 2350 8150 2500
Wire Wire Line
	5300 2500 5650 2500
Connection ~ 5650 2500
Wire Wire Line
	5650 2500 5650 2600
Wire Wire Line
	6950 2350 6950 2500
Wire Wire Line
	6600 2500 6950 2500
Connection ~ 6950 2500
Wire Wire Line
	6950 2500 6950 2600
Wire Wire Line
	7950 2500 8150 2500
Connection ~ 8150 2500
Wire Wire Line
	8150 2500 8150 2600
Wire Wire Line
	4400 1650 4400 1950
Connection ~ 5650 1650
Wire Wire Line
	4400 1650 5650 1650
Connection ~ 4400 1650
Wire Wire Line
	4400 1400 4400 1650
$Comp
L device:LED D6
U 1 1 5F9D2F33
P 2250 4300
F 0 "D6" V 2289 4182 50  0000 R CNN
F 1 "SFH4550" V 2198 4182 50  0000 R CNN
F 2 "LEDs:LED_D5.0mm_Horizontal_O3.81mm_Z9.0mm" H 2250 4300 50  0001 C CNN
F 3 "https://docs.rs-online.com/103c/0900766b808b12d5.pdf" H 2250 4300 50  0001 C CNN
	1    2250 4300
	0    -1   -1   0   
$EndComp
$Comp
L device:R R13
U 1 1 5F9D7320
P 2250 4750
F 0 "R13" H 2320 4796 50  0000 L CNN
F 1 "10 1/10W" H 2320 4705 50  0000 L CNN
F 2 "" V 2180 4750 50  0001 C CNN
F 3 "" H 2250 4750 50  0001 C CNN
	1    2250 4750
	1    0    0    -1  
$EndComp
$Comp
L device:R R11
U 1 1 5F9DF0D4
P 1900 5700
F 0 "R11" H 1970 5746 50  0000 L CNN
F 1 "10k" H 1970 5655 50  0000 L CNN
F 2 "" V 1830 5700 50  0001 C CNN
F 3 "" H 1900 5700 50  0001 C CNN
	1    1900 5700
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0134
U 1 1 5F9DFEB4
P 1900 6000
F 0 "#PWR0134" H 1900 5750 50  0001 C CNN
F 1 "GND" H 1905 5827 50  0000 C CNN
F 2 "" H 1900 6000 50  0001 C CNN
F 3 "" H 1900 6000 50  0001 C CNN
	1    1900 6000
	1    0    0    -1  
$EndComp
Text Notes 950  4000 0    50   ~ 0
+IN1 = 3.3 * 10k/(10k+22k) = 0.97 = -IN\nI = 0.97/10 = 0.097A
Text HLabel 4750 4850 1    50   Input ~ 0
WALL_LED1
$Comp
L device:Q_Photo_NPN Q5
U 1 1 5F9D8BA2
P 5550 2150
F 0 "Q5" H 5740 2196 50  0000 L CNN
F 1 "ST-1KL3A" H 5740 2105 50  0000 L CNN
F 2 "LEDs:LED_D5.0mm" H 5750 2250 50  0001 C CNN
F 3 "http://www.kashinoki.co.jp/pdf/ST-1KL3A.pdf" H 5550 2150 50  0001 C CNN
	1    5550 2150
	1    0    0    -1  
$EndComp
$Comp
L device:Q_Photo_NPN Q7
U 1 1 5F9D942E
P 6850 2150
F 0 "Q7" H 7040 2196 50  0000 L CNN
F 1 "ST-1KL3A" H 7040 2105 50  0000 L CNN
F 2 "LEDs:LED_D5.0mm" H 7050 2250 50  0001 C CNN
F 3 "http://www.kashinoki.co.jp/pdf/ST-1KL3A.pdf" H 6850 2150 50  0001 C CNN
	1    6850 2150
	1    0    0    -1  
$EndComp
$Comp
L device:Q_Photo_NPN Q9
U 1 1 5F9D9876
P 8050 2150
F 0 "Q9" H 8240 2196 50  0000 L CNN
F 1 "ST-1KL3A" H 8240 2105 50  0000 L CNN
F 2 "LEDs:LED_D5.0mm" H 8250 2250 50  0001 C CNN
F 3 "http://www.kashinoki.co.jp/pdf/ST-1KL3A.pdf" H 8050 2150 50  0001 C CNN
	1    8050 2150
	1    0    0    -1  
$EndComp
Text HLabel 1900 4900 1    50   Input ~ 0
WALL_LED0
$Comp
L device:R R14
U 1 1 5FA15251
P 1900 5200
F 0 "R14" H 1970 5246 50  0000 L CNN
F 1 "24k" H 1970 5155 50  0000 L CNN
F 2 "" V 1830 5200 50  0001 C CNN
F 3 "" H 1900 5200 50  0001 C CNN
	1    1900 5200
	1    0    0    -1  
$EndComp
Wire Wire Line
	1900 4900 1900 5050
Wire Wire Line
	1900 5350 1900 5450
Wire Wire Line
	1900 5850 1900 6000
$Comp
L linear:AD8397 U5
U 1 1 5FA1D160
P 3250 5400
F 0 "U5" H 3250 5775 50  0000 C CNN
F 1 "AD8397" H 3250 5684 50  0000 C CNN
F 2 "Housings_SOIC:SOIC-8_3.9x4.9mm_Pitch1.27mm" H 3250 5250 50  0001 C CNN
F 3 "https://www.mouser.jp/datasheet/2/609/AD8397-1502324.pdf" H 3250 5250 50  0001 C CNN
	1    3250 5400
	1    0    0    -1  
$EndComp
Connection ~ 1900 5450
Wire Wire Line
	1900 5450 1900 5550
$Comp
L power:GND #PWR04
U 1 1 5FA1EBF8
P 2750 6000
F 0 "#PWR04" H 2750 5750 50  0001 C CNN
F 1 "GND" H 2755 5827 50  0000 C CNN
F 2 "" H 2750 6000 50  0001 C CNN
F 3 "" H 2750 6000 50  0001 C CNN
	1    2750 6000
	1    0    0    -1  
$EndComp
Wire Wire Line
	2750 5550 2750 6000
Wire Wire Line
	1900 5450 2750 5450
Wire Wire Line
	2250 4450 2250 4550
Connection ~ 2250 4550
Wire Wire Line
	2250 4550 2250 4600
Wire Wire Line
	2750 5350 2550 5350
Wire Wire Line
	2550 5350 2550 4550
Wire Wire Line
	2550 4550 2250 4550
Wire Wire Line
	2250 4050 2250 4150
$Comp
L power:GND #PWR03
U 1 1 5FA332BE
P 2250 5000
F 0 "#PWR03" H 2250 4750 50  0001 C CNN
F 1 "GND" H 2255 4827 50  0000 C CNN
F 2 "" H 2250 5000 50  0001 C CNN
F 3 "" H 2250 5000 50  0001 C CNN
	1    2250 5000
	1    0    0    -1  
$EndComp
Wire Wire Line
	2250 4900 2250 5000
Wire Wire Line
	2750 5250 2750 4050
Wire Wire Line
	2750 4050 2250 4050
$Comp
L device:LED D7
U 1 1 5FA38F00
P 4200 4300
F 0 "D7" V 4239 4182 50  0000 R CNN
F 1 "SFH4550" V 4148 4182 50  0000 R CNN
F 2 "LEDs:LED_D5.0mm_Horizontal_O3.81mm_Z9.0mm" H 4200 4300 50  0001 C CNN
F 3 "https://docs.rs-online.com/103c/0900766b808b12d5.pdf" H 4200 4300 50  0001 C CNN
	1    4200 4300
	0    -1   -1   0   
$EndComp
Wire Wire Line
	3950 4050 4200 4050
Wire Wire Line
	4200 4050 4200 4150
Wire Wire Line
	3750 5350 3950 5350
Wire Wire Line
	3950 5350 3950 4050
$Comp
L power:+5V #PWR05
U 1 1 5FA3CC0C
P 3750 5050
F 0 "#PWR05" H 3750 4900 50  0001 C CNN
F 1 "+5V" H 3765 5223 50  0000 C CNN
F 2 "" H 3750 5050 50  0001 C CNN
F 3 "" H 3750 5050 50  0001 C CNN
	1    3750 5050
	1    0    0    -1  
$EndComp
Wire Wire Line
	3750 5050 3750 5250
$Comp
L device:R R16
U 1 1 5FA41E2C
P 4200 4700
F 0 "R16" H 4270 4746 50  0000 L CNN
F 1 "10 1/10W" H 4270 4655 50  0000 L CNN
F 2 "" V 4130 4700 50  0001 C CNN
F 3 "" H 4200 4700 50  0001 C CNN
	1    4200 4700
	1    0    0    -1  
$EndComp
Wire Wire Line
	4200 4450 4200 4500
Wire Wire Line
	4200 4500 4050 4500
Wire Wire Line
	4050 4500 4050 5450
Wire Wire Line
	4050 5450 3750 5450
Connection ~ 4200 4500
Wire Wire Line
	4200 4500 4200 4550
$Comp
L device:R R18
U 1 1 5FA4597C
P 4750 5150
F 0 "R18" H 4820 5196 50  0000 L CNN
F 1 "24k" H 4820 5105 50  0000 L CNN
F 2 "" V 4680 5150 50  0001 C CNN
F 3 "" H 4750 5150 50  0001 C CNN
	1    4750 5150
	1    0    0    -1  
$EndComp
$Comp
L device:R R19
U 1 1 5FA4675D
P 4750 5650
F 0 "R19" H 4820 5696 50  0000 L CNN
F 1 "10k" H 4820 5605 50  0000 L CNN
F 2 "" V 4680 5650 50  0001 C CNN
F 3 "" H 4750 5650 50  0001 C CNN
	1    4750 5650
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR06
U 1 1 5FA4CD05
P 4750 6000
F 0 "#PWR06" H 4750 5750 50  0001 C CNN
F 1 "GND" H 4755 5827 50  0000 C CNN
F 2 "" H 4750 6000 50  0001 C CNN
F 3 "" H 4750 6000 50  0001 C CNN
	1    4750 6000
	1    0    0    -1  
$EndComp
Wire Wire Line
	4750 5800 4750 6000
Wire Wire Line
	4750 5300 4750 5400
Wire Wire Line
	4750 4850 4750 5000
Wire Wire Line
	3750 5550 4400 5550
Wire Wire Line
	4400 5550 4400 5400
Wire Wire Line
	4400 5400 4750 5400
Connection ~ 4750 5400
Wire Wire Line
	4750 5400 4750 5500
$Comp
L power:+5V #PWR01
U 1 1 5FA539D8
P 1450 5400
F 0 "#PWR01" H 1450 5250 50  0001 C CNN
F 1 "+5V" H 1465 5573 50  0000 C CNN
F 2 "" H 1450 5400 50  0001 C CNN
F 3 "" H 1450 5400 50  0001 C CNN
	1    1450 5400
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR02
U 1 1 5FA53D4E
P 1450 6000
F 0 "#PWR02" H 1450 5750 50  0001 C CNN
F 1 "GND" H 1455 5827 50  0000 C CNN
F 2 "" H 1450 6000 50  0001 C CNN
F 3 "" H 1450 6000 50  0001 C CNN
	1    1450 6000
	1    0    0    -1  
$EndComp
$Comp
L device:C C21
U 1 1 5FA54426
P 1450 5700
F 0 "C21" H 1565 5746 50  0000 L CNN
F 1 "10u" H 1565 5655 50  0000 L CNN
F 2 "" H 1488 5550 50  0001 C CNN
F 3 "" H 1450 5700 50  0001 C CNN
	1    1450 5700
	1    0    0    -1  
$EndComp
Wire Wire Line
	1450 5400 1450 5550
Wire Wire Line
	1450 5850 1450 6000
$Comp
L device:LED D8
U 1 1 5FA80E67
P 7050 4250
F 0 "D8" V 7089 4132 50  0000 R CNN
F 1 "SFH4550" V 6998 4132 50  0000 R CNN
F 2 "LEDs:LED_D5.0mm_Horizontal_O3.81mm_Z9.0mm" H 7050 4250 50  0001 C CNN
F 3 "https://docs.rs-online.com/103c/0900766b808b12d5.pdf" H 7050 4250 50  0001 C CNN
	1    7050 4250
	0    -1   -1   0   
$EndComp
$Comp
L device:R R23
U 1 1 5FA80E6D
P 7050 4700
F 0 "R23" H 7120 4746 50  0000 L CNN
F 1 "10 1/10W" H 7120 4655 50  0000 L CNN
F 2 "" V 6980 4700 50  0001 C CNN
F 3 "" H 7050 4700 50  0001 C CNN
	1    7050 4700
	1    0    0    -1  
$EndComp
$Comp
L device:R R22
U 1 1 5FA80E73
P 6700 5650
F 0 "R22" H 6770 5696 50  0000 L CNN
F 1 "10k" H 6770 5605 50  0000 L CNN
F 2 "" V 6630 5650 50  0001 C CNN
F 3 "" H 6700 5650 50  0001 C CNN
	1    6700 5650
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR09
U 1 1 5FA80E79
P 6700 5950
F 0 "#PWR09" H 6700 5700 50  0001 C CNN
F 1 "GND" H 6705 5777 50  0000 C CNN
F 2 "" H 6700 5950 50  0001 C CNN
F 3 "" H 6700 5950 50  0001 C CNN
	1    6700 5950
	1    0    0    -1  
$EndComp
Text HLabel 9550 4800 1    50   Input ~ 0
WALL_LED3
Text HLabel 6700 4850 1    50   Input ~ 0
WALL_LED2
$Comp
L device:R R21
U 1 1 5FA80E81
P 6700 5150
F 0 "R21" H 6770 5196 50  0000 L CNN
F 1 "24k" H 6770 5105 50  0000 L CNN
F 2 "" V 6630 5150 50  0001 C CNN
F 3 "" H 6700 5150 50  0001 C CNN
	1    6700 5150
	1    0    0    -1  
$EndComp
Wire Wire Line
	6700 4850 6700 5000
Wire Wire Line
	6700 5300 6700 5400
Wire Wire Line
	6700 5800 6700 5950
$Comp
L linear:AD8397 U6
U 1 1 5FA80E8A
P 8050 5350
F 0 "U6" H 8050 5725 50  0000 C CNN
F 1 "AD8397" H 8050 5634 50  0000 C CNN
F 2 "Housings_SOIC:SOIC-8_3.9x4.9mm_Pitch1.27mm" H 8050 5200 50  0001 C CNN
F 3 "https://www.mouser.jp/datasheet/2/609/AD8397-1502324.pdf" H 8050 5200 50  0001 C CNN
	1    8050 5350
	1    0    0    -1  
$EndComp
Connection ~ 6700 5400
Wire Wire Line
	6700 5400 6700 5500
$Comp
L power:GND #PWR011
U 1 1 5FA80E92
P 7550 5950
F 0 "#PWR011" H 7550 5700 50  0001 C CNN
F 1 "GND" H 7555 5777 50  0000 C CNN
F 2 "" H 7550 5950 50  0001 C CNN
F 3 "" H 7550 5950 50  0001 C CNN
	1    7550 5950
	1    0    0    -1  
$EndComp
Wire Wire Line
	7550 5500 7550 5950
Wire Wire Line
	6700 5400 7550 5400
Wire Wire Line
	7050 4400 7050 4500
Connection ~ 7050 4500
Wire Wire Line
	7050 4500 7050 4550
Wire Wire Line
	7550 5300 7350 5300
Wire Wire Line
	7350 5300 7350 4500
Wire Wire Line
	7350 4500 7050 4500
Wire Wire Line
	7050 4000 7050 4100
$Comp
L power:GND #PWR010
U 1 1 5FA80EA1
P 7050 4950
F 0 "#PWR010" H 7050 4700 50  0001 C CNN
F 1 "GND" H 7055 4777 50  0000 C CNN
F 2 "" H 7050 4950 50  0001 C CNN
F 3 "" H 7050 4950 50  0001 C CNN
	1    7050 4950
	1    0    0    -1  
$EndComp
Wire Wire Line
	7050 4850 7050 4950
Wire Wire Line
	7550 5200 7550 4000
Wire Wire Line
	7550 4000 7050 4000
$Comp
L device:LED D9
U 1 1 5FA80EAA
P 9000 4250
F 0 "D9" V 9039 4132 50  0000 R CNN
F 1 "SFH4550" V 8948 4132 50  0000 R CNN
F 2 "LEDs:LED_D5.0mm_Horizontal_O3.81mm_Z9.0mm" H 9000 4250 50  0001 C CNN
F 3 "https://docs.rs-online.com/103c/0900766b808b12d5.pdf" H 9000 4250 50  0001 C CNN
	1    9000 4250
	0    -1   -1   0   
$EndComp
Wire Wire Line
	8750 4000 9000 4000
Wire Wire Line
	9000 4000 9000 4100
Wire Wire Line
	8550 5300 8750 5300
Wire Wire Line
	8750 5300 8750 4000
$Comp
L power:+5V #PWR012
U 1 1 5FA80EB4
P 8550 5000
F 0 "#PWR012" H 8550 4850 50  0001 C CNN
F 1 "+5V" H 8565 5173 50  0000 C CNN
F 2 "" H 8550 5000 50  0001 C CNN
F 3 "" H 8550 5000 50  0001 C CNN
	1    8550 5000
	1    0    0    -1  
$EndComp
Wire Wire Line
	8550 5000 8550 5200
$Comp
L device:R R24
U 1 1 5FA80EBB
P 9000 4650
F 0 "R24" H 9070 4696 50  0000 L CNN
F 1 "10 1/10W" H 9070 4605 50  0000 L CNN
F 2 "" V 8930 4650 50  0001 C CNN
F 3 "" H 9000 4650 50  0001 C CNN
	1    9000 4650
	1    0    0    -1  
$EndComp
Wire Wire Line
	9000 4400 9000 4450
Wire Wire Line
	9000 4450 8850 4450
Wire Wire Line
	8850 4450 8850 5400
Wire Wire Line
	8850 5400 8550 5400
Connection ~ 9000 4450
Wire Wire Line
	9000 4450 9000 4500
$Comp
L device:R R25
U 1 1 5FA80EC7
P 9550 5100
F 0 "R25" H 9620 5146 50  0000 L CNN
F 1 "24k" H 9620 5055 50  0000 L CNN
F 2 "" V 9480 5100 50  0001 C CNN
F 3 "" H 9550 5100 50  0001 C CNN
	1    9550 5100
	1    0    0    -1  
$EndComp
$Comp
L device:R R26
U 1 1 5FA80ECD
P 9550 5600
F 0 "R26" H 9620 5646 50  0000 L CNN
F 1 "10k" H 9620 5555 50  0000 L CNN
F 2 "" V 9480 5600 50  0001 C CNN
F 3 "" H 9550 5600 50  0001 C CNN
	1    9550 5600
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR013
U 1 1 5FA80ED3
P 9550 5950
F 0 "#PWR013" H 9550 5700 50  0001 C CNN
F 1 "GND" H 9555 5777 50  0000 C CNN
F 2 "" H 9550 5950 50  0001 C CNN
F 3 "" H 9550 5950 50  0001 C CNN
	1    9550 5950
	1    0    0    -1  
$EndComp
Wire Wire Line
	9550 5750 9550 5950
Wire Wire Line
	9550 5250 9550 5350
Wire Wire Line
	9550 4800 9550 4950
Wire Wire Line
	8550 5500 9200 5500
Wire Wire Line
	9200 5500 9200 5350
Wire Wire Line
	9200 5350 9550 5350
Connection ~ 9550 5350
Wire Wire Line
	9550 5350 9550 5450
$Comp
L power:+5V #PWR07
U 1 1 5FA80EE1
P 6250 5350
F 0 "#PWR07" H 6250 5200 50  0001 C CNN
F 1 "+5V" H 6265 5523 50  0000 C CNN
F 2 "" H 6250 5350 50  0001 C CNN
F 3 "" H 6250 5350 50  0001 C CNN
	1    6250 5350
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR08
U 1 1 5FA80EE7
P 6250 5950
F 0 "#PWR08" H 6250 5700 50  0001 C CNN
F 1 "GND" H 6255 5777 50  0000 C CNN
F 2 "" H 6250 5950 50  0001 C CNN
F 3 "" H 6250 5950 50  0001 C CNN
	1    6250 5950
	1    0    0    -1  
$EndComp
$Comp
L device:C C22
U 1 1 5FA80EED
P 6250 5650
F 0 "C22" H 6365 5696 50  0000 L CNN
F 1 "10u" H 6365 5605 50  0000 L CNN
F 2 "" H 6288 5500 50  0001 C CNN
F 3 "" H 6250 5650 50  0001 C CNN
	1    6250 5650
	1    0    0    -1  
$EndComp
Wire Wire Line
	6250 5350 6250 5500
Wire Wire Line
	6250 5800 6250 5950
$Comp
L power:GND #PWR?
U 1 1 5FAA5075
P 4200 5050
F 0 "#PWR?" H 4200 4800 50  0001 C CNN
F 1 "GND" H 4205 4877 50  0000 C CNN
F 2 "" H 4200 5050 50  0001 C CNN
F 3 "" H 4200 5050 50  0001 C CNN
	1    4200 5050
	1    0    0    -1  
$EndComp
Wire Wire Line
	4200 4850 4200 5050
$Comp
L power:GND #PWR?
U 1 1 5FAA7F9E
P 9000 4950
F 0 "#PWR?" H 9000 4700 50  0001 C CNN
F 1 "GND" H 9005 4777 50  0000 C CNN
F 2 "" H 9000 4950 50  0001 C CNN
F 3 "" H 9000 4950 50  0001 C CNN
	1    9000 4950
	1    0    0    -1  
$EndComp
Wire Wire Line
	9000 4800 9000 4950
$EndSCHEMATC
