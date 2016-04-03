EESchema Schematic File Version 2
LIBS:power
LIBS:device
LIBS:transistors
LIBS:conn
LIBS:linear
LIBS:regul
LIBS:74xx
LIBS:cmos4000
LIBS:adc-dac
LIBS:memory
LIBS:xilinx
LIBS:microcontrollers
LIBS:dsp
LIBS:microchip
LIBS:analog_switches
LIBS:motorola
LIBS:texas
LIBS:intel
LIBS:audio
LIBS:interface
LIBS:digital-audio
LIBS:philips
LIBS:display
LIBS:cypress
LIBS:siliconi
LIBS:opto
LIBS:atmel
LIBS:contrib
LIBS:valves
LIBS:pro_micro
LIBS:tandy_joystick_port
LIBS:TandySticks-cache
EELAYER 25 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title "Tandy Joystick to USB Adapter"
Date "2016-04-03"
Rev "0.1"
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L R R1
U 1 1 5700C09B
P 6100 2900
F 0 "R1" V 6180 2900 50  0000 C CNN
F 1 "100R" V 6100 2900 50  0000 C CNN
F 2 "" V 6030 2900 50  0000 C CNN
F 3 "" H 6100 2900 50  0000 C CNN
	1    6100 2900
	0    1    1    0   
$EndComp
$Comp
L R R2
U 1 1 5700C146
P 6100 4600
F 0 "R2" V 6180 4600 50  0000 C CNN
F 1 "100R" V 6100 4600 50  0000 C CNN
F 2 "" V 6030 4600 50  0000 C CNN
F 3 "" H 6100 4600 50  0000 C CNN
	1    6100 4600
	0    1    1    0   
$EndComp
$Comp
L DIN_6 P1
U 1 1 5700E0D8
P 5100 2850
F 0 "P1" H 5200 2500 50  0000 C CNN
F 1 "DIN_6" H 4950 2500 50  0000 C CNN
F 2 "" H 5100 2850 50  0000 C CNN
F 3 "" H 5100 2850 50  0000 C CNN
	1    5100 2850
	1    0    0    -1  
$EndComp
$Comp
L DIN_6 P2
U 1 1 5700E14F
P 5100 4550
F 0 "P2" H 5200 4200 50  0000 C CNN
F 1 "DIN_6" H 4950 4200 50  0000 C CNN
F 2 "" H 5100 4550 50  0000 C CNN
F 3 "" H 5100 4550 50  0000 C CNN
	1    5100 4550
	1    0    0    -1  
$EndComp
$Comp
L PRO_MICRO MC1
U 1 1 5700E5ED
P 6550 3750
F 0 "MC1" H 6550 3050 60  0000 C CNN
F 1 "PRO_MICRO" H 6550 4450 60  0000 C CNN
F 2 "" H 6650 4000 60  0000 C CNN
F 3 "" H 6650 4000 60  0000 C CNN
	1    6550 3750
	1    0    0    -1  
$EndComp
Wire Wire Line
	5550 2900 5850 2900
Wire Wire Line
	5850 2900 5950 2900
Wire Wire Line
	5850 3800 5950 3800
Wire Wire Line
	5550 4600 5850 4600
Wire Wire Line
	5850 4600 5950 4600
Wire Wire Line
	5850 4000 5950 4000
Wire Wire Line
	5550 2700 5750 2700
Wire Wire Line
	5750 2700 5750 3600
Wire Wire Line
	5750 3600 5950 3600
Wire Wire Line
	5550 3100 5650 3100
Wire Wire Line
	5650 3100 5650 3700
Wire Wire Line
	5650 3700 5950 3700
Wire Wire Line
	4650 2900 4450 2900
Wire Wire Line
	4450 2900 4450 2200
Wire Wire Line
	4450 2200 7350 2200
Wire Wire Line
	7350 2200 7350 3600
Wire Wire Line
	7350 3600 7150 3600
Wire Wire Line
	4550 2700 4550 2100
Wire Wire Line
	4550 2100 7450 2100
Wire Wire Line
	7450 2100 7450 3700
Wire Wire Line
	7450 3700 7150 3700
Wire Wire Line
	5100 2300 4350 2300
Wire Wire Line
	4350 2300 4350 3100
Wire Wire Line
	4350 3100 4350 3400
Wire Wire Line
	4650 3100 4350 3100
Connection ~ 4350 3100
Wire Wire Line
	4350 3400 5950 3400
Wire Wire Line
	4550 3500 5100 3500
Wire Wire Line
	5100 3500 5950 3500
Wire Wire Line
	5100 3500 5100 4100
Wire Wire Line
	4650 4800 4550 4800
Wire Wire Line
	4550 4800 4550 3500
Connection ~ 5100 3500
Wire Wire Line
	4650 4600 4450 4600
Wire Wire Line
	4450 4600 4450 5000
Wire Wire Line
	4450 5000 7350 5000
Wire Wire Line
	7350 5000 7350 3800
Wire Wire Line
	7350 3800 7150 3800
Wire Wire Line
	4650 4400 4350 4400
Wire Wire Line
	4350 4400 4350 5100
Wire Wire Line
	4350 5100 7450 5100
Wire Wire Line
	7450 5100 7450 3900
Wire Wire Line
	7450 3900 7150 3900
Wire Wire Line
	5550 4400 5650 4400
Wire Wire Line
	5650 4400 5650 3900
Wire Wire Line
	5650 3900 5950 3900
Wire Wire Line
	5550 4800 5750 4800
Wire Wire Line
	5750 4800 5750 4100
Wire Wire Line
	5750 4100 5950 4100
Wire Wire Line
	7150 3500 7250 3500
Wire Wire Line
	7250 2900 7250 3500
Wire Wire Line
	7250 3500 7250 4600
Wire Wire Line
	7250 2900 6250 2900
Wire Wire Line
	7250 4600 6250 4600
Connection ~ 7250 3500
Wire Wire Line
	5850 4000 5850 4600
Connection ~ 5850 4600
Wire Wire Line
	5850 3800 5850 2900
Connection ~ 5850 2900
Wire Wire Line
	5100 2300 5100 2400
Wire Wire Line
	4550 2700 4650 2700
Text Notes 4550 5400 0    60   ~ 0
Joystick pins are shown as when viewed from the exterior \nside of the female DIN connector (not the wiring side).
Text Notes 4550 5850 0    60   ~ 0
Pin locations on the microcontroller board are shown for the\nSparkfun Pro Micro board. When using an Arduino Micro or\nLeonardo board, the names of the pins used is the same, \nhowever the pin locations will be different.
$EndSCHEMATC
