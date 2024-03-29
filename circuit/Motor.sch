EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 4 4
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
L power:VCC #PWR0136
U 1 1 5FA52088
P 4150 2050
F 0 "#PWR0136" H 4150 1900 50  0001 C CNN
F 1 "VCC" H 4167 2223 50  0000 C CNN
F 2 "" H 4150 2050 50  0001 C CNN
F 3 "" H 4150 2050 50  0001 C CNN
	1    4150 2050
	1    0    0    -1  
$EndComp
$Comp
L Device:CP C18
U 1 1 5FA54BC5
P 4150 2350
F 0 "C18" H 4268 2396 50  0000 L CNN
F 1 "47u" H 4268 2305 50  0000 L CNN
F 2 "Capacitor_SMD:CP_Elec_6.3x7.7" H 4600 1850 50  0001 C CNN
F 3 "https://industrial.panasonic.com/cdbs/www-data/pdf/RDE0000/RDE0000COL98.pdf" H 4150 2350 50  0001 C CNN
	1    4150 2350
	1    0    0    -1  
$EndComp
$Comp
L Device:C C19
U 1 1 5FA556A7
P 4600 2350
F 0 "C19" H 4715 2396 50  0000 L CNN
F 1 "10u" H 4715 2305 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 4638 2200 50  0001 C CNN
F 3 "" H 4600 2350 50  0001 C CNN
	1    4600 2350
	1    0    0    -1  
$EndComp
$Comp
L Device:C C20
U 1 1 5FA55999
P 5000 2350
F 0 "C20" H 5115 2396 50  0000 L CNN
F 1 "10u" H 5115 2305 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 5038 2200 50  0001 C CNN
F 3 "" H 5000 2350 50  0001 C CNN
	1    5000 2350
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0137
U 1 1 5FA55E60
P 4150 2600
F 0 "#PWR0137" H 4150 2350 50  0001 C CNN
F 1 "GND" H 4155 2427 50  0000 C CNN
F 2 "" H 4150 2600 50  0001 C CNN
F 3 "" H 4150 2600 50  0001 C CNN
	1    4150 2600
	1    0    0    -1  
$EndComp
Wire Wire Line
	4150 2050 4150 2100
Wire Wire Line
	4150 2100 4600 2100
Wire Wire Line
	4600 2100 4600 2200
Connection ~ 4150 2100
Wire Wire Line
	4150 2100 4150 2200
Wire Wire Line
	4600 2100 5000 2100
Wire Wire Line
	5000 2100 5000 2200
Connection ~ 4600 2100
Wire Wire Line
	4150 2500 4150 2550
Wire Wire Line
	4600 2500 4600 2550
Wire Wire Line
	4600 2550 4150 2550
Connection ~ 4150 2550
Wire Wire Line
	4150 2550 4150 2600
Wire Wire Line
	5000 2500 5000 2550
Wire Wire Line
	5000 2550 4600 2550
Connection ~ 4600 2550
$Comp
L power:VCC #PWR0138
U 1 1 5FA58843
P 4950 3400
F 0 "#PWR0138" H 4950 3250 50  0001 C CNN
F 1 "VCC" H 4967 3573 50  0000 C CNN
F 2 "" H 4950 3400 50  0001 C CNN
F 3 "" H 4950 3400 50  0001 C CNN
	1    4950 3400
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR0139
U 1 1 5FA5A00C
P 3150 2050
F 0 "#PWR0139" H 3150 1900 50  0001 C CNN
F 1 "+3.3V" H 3165 2223 50  0000 C CNN
F 2 "" H 3150 2050 50  0001 C CNN
F 3 "" H 3150 2050 50  0001 C CNN
	1    3150 2050
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0140
U 1 1 5FA5C2C5
P 3150 2600
F 0 "#PWR0140" H 3150 2350 50  0001 C CNN
F 1 "GND" H 3155 2427 50  0000 C CNN
F 2 "" H 3150 2600 50  0001 C CNN
F 3 "" H 3150 2600 50  0001 C CNN
	1    3150 2600
	1    0    0    -1  
$EndComp
$Comp
L Device:C C17
U 1 1 5FA5F3FD
P 3150 2350
F 0 "C17" H 3265 2396 50  0000 L CNN
F 1 "10u" H 3265 2305 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 3188 2200 50  0001 C CNN
F 3 "" H 3150 2350 50  0001 C CNN
	1    3150 2350
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR0141
U 1 1 5FA6429D
P 4450 3400
F 0 "#PWR0141" H 4450 3250 50  0001 C CNN
F 1 "+3.3V" H 4465 3573 50  0000 C CNN
F 2 "" H 4450 3400 50  0001 C CNN
F 3 "" H 4450 3400 50  0001 C CNN
	1    4450 3400
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0142
U 1 1 5FA64FCB
P 5050 5800
F 0 "#PWR0142" H 5050 5550 50  0001 C CNN
F 1 "GND" H 5055 5627 50  0000 C CNN
F 2 "" H 5050 5800 50  0001 C CNN
F 3 "" H 5050 5800 50  0001 C CNN
	1    5050 5800
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0143
U 1 1 5FA66338
P 4450 5800
F 0 "#PWR0143" H 4450 5550 50  0001 C CNN
F 1 "GND" H 4455 5627 50  0000 C CNN
F 2 "" H 4450 5800 50  0001 C CNN
F 3 "" H 4450 5800 50  0001 C CNN
	1    4450 5800
	1    0    0    -1  
$EndComp
Text HLabel 3950 4400 0    50   Input ~ 0
PWM_L
Text HLabel 3950 4800 0    50   Input ~ 0
IN2_L
Text HLabel 3950 4700 0    50   Input ~ 0
IN1_L
Text HLabel 3950 4500 0    50   Input ~ 0
PWM_R
Text HLabel 3950 4900 0    50   Input ~ 0
IN1_R
Text HLabel 3950 5000 0    50   Input ~ 0
IN2_R
Text HLabel 3950 4200 0    50   Input ~ 0
STBY
$Comp
L Connector_Generic:Conn_02x03_Odd_Even J7
U 1 1 5FA84090
P 8050 3400
F 0 "J7" H 8100 3717 50  0000 C CNN
F 1 "Conn_02x03_Odd_Even" H 8100 3626 50  0000 C CNN
F 2 "Connector_IDC:IDC-Header_2x03_P2.54mm_Vertical" H 8050 3400 50  0001 C CNN
F 3 "https://akizukidenshi.com/download/ds/useconn/BH-XXSG.pdf" H 8050 3400 50  0001 C CNN
	1    8050 3400
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0144
U 1 1 5FA87B9E
P 7650 3650
F 0 "#PWR0144" H 7650 3400 50  0001 C CNN
F 1 "GND" H 7655 3477 50  0000 C CNN
F 2 "" H 7650 3650 50  0001 C CNN
F 3 "" H 7650 3650 50  0001 C CNN
	1    7650 3650
	1    0    0    -1  
$EndComp
Wire Wire Line
	7500 3300 7850 3300
Wire Wire Line
	7650 3650 7650 3400
Wire Wire Line
	7650 3400 7850 3400
$Comp
L power:+5V #PWR0145
U 1 1 5FA8D459
P 8550 3000
F 0 "#PWR0145" H 8550 2850 50  0001 C CNN
F 1 "+5V" H 8565 3173 50  0000 C CNN
F 2 "" H 8550 3000 50  0001 C CNN
F 3 "" H 8550 3000 50  0001 C CNN
	1    8550 3000
	1    0    0    -1  
$EndComp
Wire Wire Line
	8550 3000 8550 3400
Wire Wire Line
	8550 3400 8350 3400
Wire Wire Line
	8350 3300 8700 3300
Text HLabel 8700 3500 2    50   Output ~ 0
ENCA_L
Wire Wire Line
	8350 3500 8700 3500
Text HLabel 7500 3500 0    50   Output ~ 0
ENCB_L
Wire Wire Line
	7500 3500 7850 3500
$Comp
L power:GND #PWR0146
U 1 1 5FAA48C2
P 7700 4950
F 0 "#PWR0146" H 7700 4700 50  0001 C CNN
F 1 "GND" H 7705 4777 50  0000 C CNN
F 2 "" H 7700 4950 50  0001 C CNN
F 3 "" H 7700 4950 50  0001 C CNN
	1    7700 4950
	1    0    0    -1  
$EndComp
Wire Wire Line
	7700 4950 7700 4700
Wire Wire Line
	7700 4700 7850 4700
Wire Wire Line
	7500 4600 7850 4600
Wire Wire Line
	8350 4600 8700 4600
$Comp
L power:+5V #PWR0147
U 1 1 5FAA8E6E
P 8550 4400
F 0 "#PWR0147" H 8550 4250 50  0001 C CNN
F 1 "+5V" H 8565 4573 50  0000 C CNN
F 2 "" H 8550 4400 50  0001 C CNN
F 3 "" H 8550 4400 50  0001 C CNN
	1    8550 4400
	1    0    0    -1  
$EndComp
Wire Wire Line
	8550 4400 8550 4700
Wire Wire Line
	8550 4700 8350 4700
Text HLabel 8700 4800 2    50   Output ~ 0
ENCA_R
Text HLabel 7500 4800 0    50   Output ~ 0
ENCB_R
Wire Wire Line
	7500 4800 7850 4800
Wire Wire Line
	8350 4800 8700 4800
$Comp
L Connector_Generic:Conn_02x03_Odd_Even J8
U 1 1 5F9570AC
P 8050 4700
F 0 "J8" H 8100 5017 50  0000 C CNN
F 1 "Conn_02x03_Odd_Even" H 8100 4926 50  0000 C CNN
F 2 "Connector_IDC:IDC-Header_2x03_P2.54mm_Vertical" H 8050 4700 50  0001 C CNN
F 3 "https://akizukidenshi.com/download/ds/useconn/BH-XXSG.pdf" H 8050 4700 50  0001 C CNN
	1    8050 4700
	1    0    0    -1  
$EndComp
Text Label 5450 4400 0    50   ~ 0
MOTOR_L-
Text Label 5450 4700 0    50   ~ 0
MOTOR_R-
Text Label 5450 4900 0    50   ~ 0
MOTOR_R+
Text Label 7500 3300 2    50   ~ 0
MOTOR_L+
Text Label 8700 3300 0    50   ~ 0
MOTOR_L-
Text Label 7500 4600 2    50   ~ 0
MOTOR_R+
Text Label 8700 4600 0    50   ~ 0
MOTOR_R-
Text Label 5450 4200 0    50   ~ 0
MOTOR_L+
Wire Wire Line
	4450 5600 4450 5800
$Comp
L Driver_Motor:TB6612FNG U2
U 1 1 5FAE9218
P 4750 4600
F 0 "U2" H 4750 3511 50  0000 C CNN
F 1 "TB6612FNG" H 4750 3420 50  0000 C CNN
F 2 "Package_SO:SSOP-24_5.3x8.2mm_P0.65mm" H 6050 3700 50  0001 C CNN
F 3 "https://toshiba.semicon-storage.com/us/product/linear/motordriver/detail.TB6612FNG.html" H 5200 5200 50  0001 C CNN
	1    4750 4600
	1    0    0    -1  
$EndComp
Wire Wire Line
	5350 4200 5450 4200
Wire Wire Line
	5350 4700 5450 4700
Wire Wire Line
	5350 4900 5450 4900
Wire Wire Line
	5350 4400 5450 4400
Wire Wire Line
	4850 5600 4850 5700
Wire Wire Line
	4850 5700 5050 5700
Wire Wire Line
	5050 5700 5050 5800
Wire Wire Line
	5050 5600 5050 5700
Connection ~ 5050 5700
Wire Wire Line
	4950 3400 4950 3550
Wire Wire Line
	4850 3600 4850 3550
Wire Wire Line
	4850 3550 4950 3550
Connection ~ 4950 3550
Wire Wire Line
	4950 3550 4950 3600
Wire Wire Line
	4950 3550 5050 3550
Wire Wire Line
	5050 3550 5050 3600
Wire Wire Line
	4450 3400 4450 3600
Wire Wire Line
	3950 4700 4150 4700
Wire Wire Line
	3950 4800 4150 4800
Wire Wire Line
	3950 4900 4150 4900
Wire Wire Line
	3950 5000 4150 5000
Wire Wire Line
	3950 4500 4150 4500
Wire Wire Line
	3950 4400 4150 4400
Wire Wire Line
	3950 4200 4150 4200
Wire Wire Line
	3150 2050 3150 2200
Wire Wire Line
	3150 2500 3150 2600
$EndSCHEMATC
