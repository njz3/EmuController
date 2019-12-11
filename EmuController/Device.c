/*++

Module Name:

    device.c - Device handling events for example driver.

Abstract:

   This file contains the device entry points and callbacks.
    
Environment:

    User-mode Driver Framework 2

--*/

#include "driver.h"
#include "device.tmh"


HID_REPORT_DESCRIPTOR       G_DefaultReportDescriptor[] = {
	0x05,0x01,                //Usage_Page(Generic_Desktop_ID),
	0x09, 0x04,                     // Usage (Joystick)
	0xA1, 0x01,                     // Collection (Application)
	    0x85, 0x01,                     // Report_ID(1),
		0x15, 0x00,                     // Logical Minimum (0)
		0x25, 0x01,                     // Logical Maximum (1)
		0x75, 0x01,                     // Report Size (1)
		0x95, 0x80,                     // Report Count (128)
		0x05, 0x09,                     // Usage Page (Button)
		0x19, 0x01,                     // Usage_Minimum(Button1_ID),
		0x29, 0x80,	                    // Usage_Maximum(Button128_ID),
		0x81, 0x02,                     // Input (variable,absolute)


		0x05, 0x01,                     // Usage Page (Generic Desktop)
		0x15, 0x00,                     // Logical Minimum (0)
		0x25, 0x07,                     // Logical Maximum (7)
		0x35, 0x00,                     // Physical Minimum (0)
		0x46, 0x3B, 0x01,               // Physical Maximum (315)
		0x75, 0x08,                     // Report Size (8)
		0x95, 0x01,                     // Report Count (1)
		0x65, 0x14,                     // Unit (20)
		0x09, 0x39,                     // Usage (Hat switch)
	    0x81, 0x42,                     // Input (variable,absolute,null_state)
		0x09, 0x39,                     // Usage (Hat switch)
	    0x81, 0x42,                     // Input (variable,absolute,null_state)
		0x09, 0x39,                     // Usage (Hat switch)
		0x81, 0x42,                     // Input (variable,absolute,null_state)
		0x09, 0x39,                     // Usage (Hat switch)
		0x81, 0x42,                     // Input (variable,absolute,null_state)

	    0x15,0x00,                      // Logical_Minimum(0),
		0x35,0x00,                      // Physical_Minimum(0),
		0x25, 0x7F,						// Physical_Maximum(127),
		0x65, 0x00,                     // UNIT (None)
		0x05, 0x01,                     // Usage Page (Generic Desktop)
		0x09, 0x01,                     // Usage (Pointer)
		0xA1, 0x00,                     // Collection ()
			0x15, 0x00,                     //   Logical Minimum (0)
			0x27, 0xFF, 0xFF, 0x00, 0x00,   //   Logical Maximum (65535)
			0x75, 0x10,                     //   Report Size (16)
			0x95, 0x08,                     //   Report Count (8)
			0x09, 0x30, 					//   USAGE (X)
			0x09, 0x31, 					//   USAGE (Y)
			0x09, 0x32, 					//   USAGE (Z)
			0x09, 0x33, 					//   USAGE (Rx)
			0x09, 0x34, 					//   USAGE (Ry)
			0x09, 0x35, 					//   USAGE (Rz)
			0x09, 0x36, 					//   USAGE (Slider)
			0x09, 0x37, 					//   USAGE (Dial)
			0x81, 0x02,                     //   Input (variable,absolute)
		0xC0,                           // End Collection

	0x05,0x0F, // Usage Page Physical Interface
	0x09,0x92, // Usage PID State report 
	0xA1,0x02, // Collection Datalink (logical)

	   0x85,0x02, // Report ID 2
	   0x09,0x9F, // Usage Device is Pause 
	   0x09,0xA0, // Usage Actuators Enabled
	   0x09,0xA4, // Usage Safety Switch
	   0x09,0xA5, // Usage Actuator Override Switch
	   0x09,0xA6, // Usage Actuator Power
	   0x15,0x00, // Logical Minimum 0
	   0x25,0x01, // Logical Maximum 1
	   0x35,0x00, // Physical Minimum 0
	   0x45,0x01, // Physical Maximum 1
	   0x75,0x01, // Report Size 1
	   0x95,0x05, // Report Count 5
	   0x81,0x02, // Input (Variable)
	   0x95,0x03, // Report Count 3
	   0x81,0x03, // Input (Constant, Variable)


	   0x09,0x94, // Usage Effect Playing
	   0x15,0x00, // Logical Minimum 0
	   0x25,0x01, // Logical Maximum 1
	   0x35,0x00, // Physical Minimum 0
	   0x45,0x01, // Physical Maximum 1
	   0x75,0x01, // Report Size 1
	   0x95,0x01, // Report Count 1
	   0x81,0x02, // Input (Variable)
	   0x09,0x22, // Usage Effect Block Index
	   0x15,0x01, // Logical Minimum 1
	   0x25,MAX_EFFECT_BLOCKS, // Logical Maximum 28h (40d)
	   0x35,0x01, // Physical Minimum 1
	   0x45,MAX_EFFECT_BLOCKS, // Physical Maximum 28h (40d)
	   0x75,0x07, // Report Size 7
	   0x95,0x01, // Report Count 1
	   0x81,0x02, // Input (Variable)
	0xC0 , // End Collection

 /*
  Output
  Collection Datalink:
  Usage Set Effect Report

  ID:1
  Effect Block Index: 8bit

  subcollection Effect Type
  12 effect types, 8bit each

 */
	0x09,0x21, // Usage Set Effect Report
	0xA1,0x02, // Collection Datalink (Logical)
	   0x85,PID_SET_EFFECT_REPORT_ID, // Report ID 1
	   0x09,0x22, // Usage Effect Block Index
	   0x15,0x01, // Logical Minimum 1
	   0x25,MAX_EFFECT_BLOCKS, // Logical Maximum 28h (40d)
	   0x35,0x01, // Physical Minimum 1
	   0x45,MAX_EFFECT_BLOCKS, // Physical Maximum 28h (40d)
	   0x75,0x08, // Report Size 8
	   0x95,0x01, // Report Count 1
	   0x91,0x02, // Output (Variable)

	   0x09,0x25, // Usage Effect Type
	   0xA1,0x02, // Collection Datalink
		  0x09,0x26, // Usage ET Constant Force
		  0x09,0x27, // Usage ET Ramp
		  0x09,0x30, // Usage ET Square
		  0x09,0x31, // Usage ET Sine
		  0x09,0x32, // Usage ET Triangle
		  0x09,0x33, // Usage ET Sawtooth Up
		  0x09,0x34, // Usage ET Sawtooth Down
		  0x09,0x40, // Usage ET Spring
		  0x09,0x41, // Usage ET Damper
		  0x09,0x42, // Usage ET Inertia
		  0x09,0x43, // Usage ET Friction
		  0x09,0x28, // Usage ET Custom Force Data
		  0x25,0x0C, // Logical Maximum Ch (12d)
		  0x15,0x01, // Logical Minimum 1
		  0x35,0x01, // Physical Minimum 1
		  0x45,0x0C, // Physical Maximum Ch (12d)
		  0x75,0x08, // Report Size 8
		  0x95,0x01, // Report Count 1
		  0x91,0x00, // Output

	   0xC0 , // End Collection

	   0x09,0x50, // Usage Duration
	   0x09,0x54, // Usage Trigger Repeat Interval
	   0x09,0x51, // Usage Sample Period
	   0x15,0x00, // Logical Minimum 0
	   0x26,0xFF,0x7F, // Logical Maximum 7FFFh (32767d)
	   0x35,0x00, // Physical Minimum 0
	   0x46,0xFF,0x7F, // Physical Maximum 7FFFh (32767d)
	   0x66,0x03,0x10, // Unit 1003h (4099d)
	   0x55,0xFD, // Unit Exponent FDh (253d)
	   0x75,0x10, // Report Size 10h (16d)
	   0x95,0x03, // Report Count 3
	   0x91,0x02, // Output (Variable)
	   0x55,0x00, // Unit Exponent 0
	   0x66,0x00,0x00, // Unit 0

	   0x09,0x52, // Usage Gain
	   0x15,0x00, // Logical Minimum 0
	   0x26,0x10,0x27, // Logical Maximum FFh (10000d)
	   0x35,0x00, // Physical Minimum 0
	   0x46,0x10,0x27, // Physical Maximum 2710h (10000d)
	   0x75,0x10, // Report Size 8
	   0x95,0x01, // Report Count 1
	   0x91,0x02, // Output (Variable)

	   0x09,0x53, // Usage Trigger Button
	   0x15,0x01, // Logical Minimum 1
	   0x25,0x7F, // Logical Maximum 8
	   0x35,0x01, // Physical Minimum 1
	   0x45,0x7F, // Physical Maximum 8
	   0x75,0x08, // Report Size 8
	   0x95,0x01, // Report Count 1
	   0x91,0x02, // Output (Variable)

	   0x09,0x55, // Usage Axes Enable
	   0xA1,0x02, // Collection Datalink
		  0x05,0x01, // Usage Page Generic Desktop
		  0x09,0x30, // Usage X
		  0x09,0x31, // Usage Y
		  0x15,0x00, // Logical Minimum 0
		  0x25,0x01, // Logical Maximum 1
		  0x35,0x00, // Physical Minimum 1
		  0x45,0x01, // Physical Maximum 8
		  0x75,0x01, // Report Size 1
		  0x95,0x02, // Report Count 2
		  0x91,0x02, // Output (Variable)

	   0xC0 , // End Collection

	   0x05,0x0F, // Usage Page Physical Interface
	   0x09,0x56, // Usage Direction Enable
	   0x95,0x01, // Report Count 1
	   0x91,0x02, // Output (Variable)

	   0x95,0x05, // Report Count 5
	   0x91,0x03, // Output (Constant, Variable)

	   0x09,0x57, // Usage Direction
	   0xA1,0x02, // Collection Datalink
		  0x0B,0x01,0x00,0x0A,0x00, // Usage Ordinals: Instance 1
		  0x0B,0x02,0x00,0x0A,0x00, // Usage Ordinals: Instance 2
		  0x66,0x14,0x00, // Unit 14h (20d)
		  0x55,0xFE, // Unit Exponent FEh (254d)
		  0x15,0x00, // Logical Minimum 0
		  0x27, 0xA0, 0x8C, 0x00, 0x00, // Logical Maximum FFh (255d)
		  0x35,0x00, // Physical Minimum 0
		  0x47,0xA0,0x8C,0x00,0x00, // Physical Maximum 8CA0h (36000d)
		  0x66,0x00,0x00, // Unit 0
		  0x75,0x10, // Report Size 8
		  0x95,0x02, // Report Count 2
		  0x91,0x02, // Output (Variable)
		  0x55,0x00, // Unit Exponent 0
		  0x66,0x00,0x00, // Unit 0
	   0xC0 , // End Collection

	   0x05,0x0F, // Usage Page Physical Interface
	   0x09,0xA7, // Usage Undefined
	   0x66,0x03,0x10, // Unit 1003h (4099d)
	   0x55,0xFD, // Unit Exponent FDh (253d)
	   0x15,0x00, // Logical Minimum 0
	   0x26, 0xFF, 0x7F, // Logical Maximum 7FFFh (32767d)
	   0x35,0x00, // Physical Minimum 0
	   0x46, 0xFF, 0x7F, // Physical Maximum 7FFFh (32767d)
	   0x75,0x10, // Report Size 10h (16d)
	   0x95,0x01, // Report Count 1
	   0x91,0x02, // Output (Variable)
	   0x66,0x00,0x00, // Unit 0
	   0x55,0x00, // Unit Exponent 0
	0xC0 , // End Collection

	0x05,0x0F, // Usage Page Physical Interface
	0x09,0x5A, // Usage Set Envelope Report
	0xA1,0x02, // Collection Datalink
	   0x85,PID_SET_ENVELOPE_REPORT_ID, // Report ID 2
	   0x09,0x22, // Usage Effect Block Index
	   0x15,0x01, // Logical Minimum 1
       0x25, MAX_EFFECT_BLOCKS, // Logical Maximum 28h (40d)
	   0x35,0x01, // Physical Minimum 1
	   0x45, MAX_EFFECT_BLOCKS, // Logical Maximum 28h (40d)
	   0x75,0x08, // Report Size 8
	   0x95,0x01, // Report Count 1
	   0x91,0x02, // Output (Variable)
	   0x09,0x5B, // Usage Attack Level
	   0x09,0x5D, // Usage Fade Level
	   0x15,0x00, // Logical Minimum 0
	   0x26,0x10,0x27, // Logical Maximum FFh (10000d)
	   0x35,0x00, // Physical Minimum 0
	   0x46,0x10,0x27, // Physical Maximum 2710h (10000d)
	   0x75, 0x10, // Report Size 16
	   0x95,0x02, // Report Count 2
	   0x91,0x02, // Output (Variable)
	   0x09,0x5C, // Usage Attack Time
	   0x09,0x5E, // Usage Fade Time
	   0x66,0x03,0x10, // Unit 1003h (4099d)
	   0x55,0xFD, // Unit Exponent FDh (253d)
	   0x26,0xFF,0x7F, // Logical Maximum 7FFFh (32767d)
	   0x46,0xFF,0x7F, // Physical Maximum 7FFFh (32767d)
	   0x75,0x10, // Report Size 10h (16d)
	   0x91,0x02, // Output (Variable)
	   0x45,0x00, // Physical Maximum 0
	   0x66,0x00,0x00, // Unit 0
	   0x55,0x00, // Unit Exponent 0
	0xC0 , // End Collection

	0x09,0x5F, // Usage Set Condition Report
	0xA1,0x02, // Collection Datalink
	   0x85, PID_SET_CONDITION_REPORT_ID, // Report ID 3
	   0x09,0x22, // Usage Effect Block Index
	   0x15,0x01, // Logical Minimum 1
	   0x25,MAX_EFFECT_BLOCKS, // Logical Maximum 28h (40d)
	   0x35,0x01, // Physical Minimum 1
	   0x45,MAX_EFFECT_BLOCKS, // Physical Maximum 28h (40d)
	   0x75,0x08, // Report Size 8
	   0x95,0x01, // Report Count 1
	   0x91,0x02, // Output (Variable)
	   0x09,0x23, // Usage Parameter Block Offset
	   0x15,0x00, // Logical Minimum 0
	   0x25,0x01, // Logical Maximum 1
	   0x35,0x00, // Physical Minimum 0
	   0x45,0x01, // Physical Maximum 1
	   0x75,0x04, // Report Size 4
	   0x95,0x01, // Report Count 1
	   0x91,0x02, // Output (Variable)
	   0x09,0x58, // Usage Type Specific Block Off...
	   0xA1,0x02, // Collection Datalink

		  0x0B,0x01,0x00,0x0A,0x00, // Usage Ordinals: Instance 1
		  0x0B,0x02,0x00,0x0A,0x00, // Usage Ordinals: Instance 2
		  0x75,0x02, // Report Size 2
		  0x95,0x02, // Report Count 2
		  0x91,0x02, // Output (Variable)
	   0xC0 , // End Collection

	   0x16,0xF0,0xD8, // Logical Minimum 80h (-128d)
	   0x26,0x10,0x27, // Logical Maximum 7Fh (127d)
	   0x36,0xF0,0xD8, // Physical Minimum D8F0h (-10000d)
	   0x46,0x10,0x27, // Physical Maximum 2710h (10000d)
	   0x09,0x60, // Usage CP Offset
	   0x75,0x10, // Report Size 8
	   0x95,0x01, // Report Count 1
	   0x91,0x02, // Output (Variable)
	   0x09,0x61, // Usage Positive Coefficient
	   0x09,0x62, // Usage Negative Coefficient
	   0x95,0x02, // Report Count 2
	   0x91,0x02, // Output (Variable)
	   0x15,0x00, // Logical Minimum 0
	   0x35,0x00, // Physical Minimum D8F0h (0)
	   0x09,0x63, // Usage Positive Saturation
	   0x09,0x64, // Usage Negative Saturation
	   0x75,0x10, // Report Size 8
	   0x95,0x02, // Report Count 2
	   0x91,0x02, // Output (Variable)
	   0x09,0x65, // Usage Dead Band
	   0x95,0x01, // Report Count 1
	   0x91,0x02, // Output (Variable)
	0xC0 , // End Collection
	0x09,0x6E, // Usage Set Periodic Report
	0xA1,0x02, // Collection Datalink
	   0x85,PID_SET_PERIODIC_REPORT_ID, // Report ID 4
	   0x09,0x22, // Usage Effect Block Index
	   0x15,0x01, // Logical Minimum 1
	   0x25, MAX_EFFECT_BLOCKS, // Logical Maximum 28h (40d)
	   0x35,0x01, // Physical Minimum 1
	   0x45, MAX_EFFECT_BLOCKS, // Physical Maximum 28h (40d)
	   0x75,0x08, // Report Size 8
	   0x95,0x01, // Report Count 1
	   0x91,0x02, // Output (Variable)
	   0x09,0x70, // Usage Magnitude
	   0x15,0x00, // Logical Minimum 0
	   0x26,0x10,0x27, // Logical Maximum FFh (255d)
	   0x35,0x00, // Physical Minimum 0
	   0x46,0x10,0x27, // Physical Maximum 2710h (10000d)
	   0x75,0x10, // Report Size 16
	   0x95,0x01, // Report Count 1
	   0x91,0x02, // Output (Variable)
	   0x09,0x6F, // Usage Offset
	   0x16,0xF0,0xD8, // Logical Minimum 80h (-10000d)
	   0x26,0x10,0x27, // Logical Maximum 7Fh (10000d)
	   0x36,0xF0,0xD8, // Physical Minimum D8F0h (-10000d)
	   0x46,0x10,0x27, // Physical Maximum 2710h (10000d)
	   //0x95,0x01, // Report Count 1
	   0x91,0x02, // Output (Variable)
	   0x09,0x71, // Usage Phase
	   0x66,0x14,0x00, // Unit 14h (20d)
	   0x55,0xFE, // Unit Exponent FEh (254d)
	   0x15,0x00, // Logical Minimum 0
	   0x27,0xA0,0x8C,0x00,0x00, // Logical Maximum FFh (36000)
	   0x35,0x00, // Physical Minimum 0
	   0x47,0xA0,0x8C,0x00,0x00, // Physical Maximum 8CA0h (36000)
	   0x91,0x02, // Output (Variable)
	   0x09,0x72, // Usage Period
	   0x26,0xFF,0x7F, // Logical Maximum 7FFFh (32767d)
	   0x46,0xFF,0x7F, // Physical Maximum 7FFFh (32767d)
	   0x66,0x03,0x10, // Unit 1003h (4099d)
	   0x55,0xFD, // Unit Exponent FDh (253d)
	   //0x75,0x10, // Report Size 10h (16d)
	   //0x95,0x01, // Report Count 1
	   0x91,0x02, // Output (Variable)
	   0x66,0x00,0x00, // Unit 0
	   0x55,0x00, // Unit Exponent 0
	0xC0 , // End Collection
	0x09,0x73, // Usage Set Constant Force Rep...
	0xA1,0x02, // Collection Datalink
	   0x85,PID_SET_CONSTANT_FORCE_REPORT_ID, // Report ID 5
	   0x09,0x22, // Usage Effect Block Index
	   0x15,0x01, // Logical Minimum 1
	   0x25,MAX_EFFECT_BLOCKS, // Logical Maximum 28h (40d)
	   0x35,0x01, // Physical Minimum 1
	   0x45,MAX_EFFECT_BLOCKS, // Physical Maximum 28h (40d)
	   0x75,0x08, // Report Size 8
	   0x95,0x01, // Report Count 1
	   0x91,0x02, // Output (Variable)
	   0x09,0x70, // Usage Magnitude
	   0x16,0xF0,0xD8, // Logical Minimum FF01h (-255d)
	   0x26,0x10,0x27, // Logical Maximum FFh (255d)
	   0x36,0xF0,0xD8, // Physical Minimum D8F0h (-10000d)
	   0x46,0x10,0x27, // Physical Maximum 2710h (10000d)
	   0x75,0x10, // Report Size 10h (16d)
	   0x95,0x01, // Report Count 1
	   0x91,0x02, // Output (Variable)
	0xC0 , // End Collection
	0x09,0x74, // Usage Set Ramp Force Report
	0xA1,0x02, // Collection Datalink
	   0x85,PID_SET_RAMP_FORCE_REPORT_ID, // Report ID 6
	   0x09,0x22, // Usage Effect Block Index
	   0x15,0x01, // Logical Minimum 1
	   0x25,MAX_EFFECT_BLOCKS, // Logical Maximum 28h (40d)
	   0x35,0x01, // Physical Minimum 1
	   0x45,MAX_EFFECT_BLOCKS, // Physical Maximum 28h (40d)
	   0x75,0x08, // Report Size 8
	   0x95,0x01, // Report Count 1
	   0x91,0x02, // Output (Variable)
	   0x09,0x75, // Usage Ramp Start
	   0x09,0x76, // Usage Ramp End
	   0x16,0xF0,0xD8, // Logical Minimum 80h (-10000d)
	   0x26,0x10,0x27, // Logical Maximum 7Fh (10000d)
	   0x36,0xF0,0xD8, // Physical Minimum D8F0h (-10000d)
	   0x46,0x10,0x27, // Physical Maximum 2710h (10000d)
	   0x75,0x10, // Report Size 16
	   0x95,0x02, // Report Count 2
	   0x91,0x02, // Output (Variable)
	0xC0 , // End Collection
	0x09,0x68, // Usage Custom Force Data Rep...
	0xA1,0x02, // Collection Datalink
	   0x85,PID_SET_CUSTOM_FORCE_DATA_REPORT_ID, // Report ID 7
	   0x09,0x22, // Usage Effect Block Index
	   0x15,0x01, // Logical Minimum 1
	   0x25,MAX_EFFECT_BLOCKS, // Logical Maximum 28h (40d)
	   0x35,0x01, // Physical Minimum 1
	   0x45,MAX_EFFECT_BLOCKS, // Physical Maximum 28h (40d)
	   0x75,0x08, // Report Size 8
	   0x95,0x01, // Report Count 1
	   0x91,0x02, // Output (Variable)
	   0x09,0x6C, // Usage Custom Force Data Offset
	   0x15,0x00, // Logical Minimum 0
	   0x26,0x10,0x27, // Logical Maximum 2710h (10000d)
	   0x35,0x00, // Physical Minimum 0
	   0x46,0x10,0x27, // Physical Maximum 2710h (10000d)
	   0x75,0x10, // Report Size 10h (16d)
	   0x95,0x01, // Report Count 1
	   0x91,0x02, // Output (Variable)
	   0x09,0x69, // Usage Custom Force Data
	   0x15,0x81, // Logical Minimum 81h (-127d)
	   0x25,0x7F, // Logical Maximum 7Fh (127d)
	   0x35,0x00, // Physical Minimum 0
	   0x46,0xFF,0x00, // Physical Maximum FFh (255d)
	   0x75,0x08, // Report Size 8
	   0x95,0x0C, // Report Count Ch (12d)
	   0x92,0x02,0x01, // Output (Variable, Buffered)
	0xC0 , // End Collection
	0x09,0x66, // Usage Download Force Sample
	0xA1,0x02, // Collection Datalink
	   0x85,PID_DOWNLOAD_SAMPLE_REPORT_ID, // Report ID 8
	   0x05,0x01, // Usage Page Generic Desktop
	   0x09,0x30, // Usage X
	   0x09,0x31, // Usage Y
	   0x15,0x81, // Logical Minimum 81h (-127d)
	   0x25,0x7F, // Logical Maximum 7Fh (127d)
	   0x35,0x00, // Physical Minimum 0
	   0x46,0xFF,0x00, // Physical Maximum FFh (255d)
	   0x75,0x08, // Report Size 8
	   0x95,0x02, // Report Count 2
	   0x91,0x02, // Output (Variable)
	0xC0 , // End Collection
	0x05,0x0F, // Usage Page Physical Interface
	0x09,0x77, // Usage Effect Operation Report
	0xA1,0x02, // Collection Datalink
	   0x85,PID_EFFECT_OPERATION_REPORT, // Report ID Ah (10d)
	   0x09,0x22, // Usage Effect Block Index
	   0x15,0x01, // Logical Minimum 1
	   0x25,MAX_EFFECT_BLOCKS, // Logical Maximum 28h (40d)
	   0x35,0x01, // Physical Minimum 1
	   0x45,MAX_EFFECT_BLOCKS, // Physical Maximum 28h (40d)
	   0x75,0x08, // Report Size 8
	   0x95,0x01, // Report Count 1
	   0x91,0x02, // Output (Variable)
	   0x09,0x78, // Usage Effect Operation
	   0xA1,0x02, // Collection Datalink
		  0x09,0x79, // Usage Op Effect Start
		  0x09,0x7A, // Usage Op Effect Start Solo
		  0x09,0x7B, // Usage Op Effect Stop
		  0x15,0x01, // Logical Minimum 1
		  0x25,0x03, // Logical Maximum 3
		  0x75,0x08, // Report Size 8
		  0x95,0x01, // Report Count 1
		  0x91,0x00, // Output
	   0xC0 , // End Collection
	   0x09,0x7C, // Usage Loop Count
	   0x15,0x00, // Logical Minimum 0
	   0x26,0xFF,0x00, // Logical Maximum FFh (255d)
	   0x35,0x00, // Physical Minimum 0
	   0x46,0xFF,0x00, // Physical Maximum FFh (255d)
	   0x91,0x02, // Output (Variable)
	0xC0 , // End Collection
	0x09,0x90, // Usage PID Block Free Report
	0xA1,0x02, // Collection Datalink
	   0x85,PID_BLOCK_FREE_REPORT_ID, // Report ID Bh (11d)
	   0x09,0x22, // Usage Effect Block Index
	   0x25,MAX_EFFECT_BLOCKS, // Logical Maximum 28h (40d)
	   0x15,0x01, // Logical Minimum 1
	   0x35,0x01, // Physical Minimum 1
	   0x45,MAX_EFFECT_BLOCKS, // Physical Maximum 28h (40d)
	   0x75,0x08, // Report Size 8
	   0x95,0x01, // Report Count 1
	   0x91,0x02, // Output (Variable)
	0xC0 , // End Collection
	0x09,0x96, // Usage PID Device Control
	0xA1,0x02, // Collection Datalink
	   0x85,PID_DEVICE_CONTROL_REPORT_ID, // Report ID Ch (12d)
	   0x09,0x97, // Usage DC Enable Actuators
	   0x09,0x98, // Usage DC Disable Actuators
	   0x09,0x99, // Usage DC Stop All Effects
	   0x09,0x9A, // Usage DC Device Reset
	   0x09,0x9B, // Usage DC Device Pause
	   0x09,0x9C, // Usage DC Device Continue
	   0x15,0x01, // Logical Minimum 1
	   0x25,0x06, // Logical Maximum 6
	   0x75,0x08, // Report Size 8
	   0x95,0x01, // Report Count 1
	   0x91,0x00, // Output
	0xC0 , // End Collection
	0x09,0x7D, // Usage Device Gain Report
	0xA1,0x02, // Collection Datalink
	   0x85,PID_DEVICE_GAIN_REPORT_ID, // Report ID Dh (13d)
	   0x09,0x7E, // Usage Device Gain
	   0x15,0x00, // Logical Minimum 0
	   0x26,0x10,0x27, // Logical Maximum FFh (255d)
	   0x35,0x00, // Physical Minimum 0
	   0x46,0x10,0x27, // Physical Maximum 2710h (10000d)
	   0x75,0x10, // Report Size 8
	   0x95,0x01, // Report Count 1
	   0x91,0x02, // Output (Variable)
	0xC0 , // End Collection
	0x09,0x6B, // Usage Set Custom Force Report
	0xA1,0x02, // Collection Datalink
	   0x85, PID_SET_CUSTOM_FORCE_REPORT_ID, // Report ID Eh (14d)
	   0x09,0x22, // Usage Effect Block Index
	   0x15,0x01, // Logical Minimum 1
	   0x25,MAX_EFFECT_BLOCKS, // Logical Maximum 28h (40d)
	   0x35,0x01, // Physical Minimum 1
	   0x45,MAX_EFFECT_BLOCKS, // Physical Maximum 28h (40d)
	   0x75,0x08, // Report Size 8
	   0x95,0x01, // Report Count 1
	   0x91,0x02, // Output (Variable)
	   0x09,0x6D, // Usage Sample Count
	   0x15,0x00, // Logical Minimum 0
	   0x26,0xFF,0x00, // Logical Maximum FFh (255d)
	   0x35,0x00, // Physical Minimum 0
	   0x46,0xFF,0x00, // Physical Maximum FFh (255d)
	   0x75,0x08, // Report Size 8
	   0x95,0x01, // Report Count 1
	   0x91,0x02, // Output (Variable)
	   0x09,0x51, // Usage Sample Period
	   0x66,0x03,0x10, // Unit 1003h (4099d)
	   0x55,0xFD, // Unit Exponent FDh (253d)
	   0x15,0x00, // Logical Minimum 0
	   0x26,0xFF,0x7F, // Logical Maximum 7FFFh (32767d)
	   0x35,0x00, // Physical Minimum 0
	   0x46,0xFF,0x7F, // Physical Maximum 7FFFh (32767d)
	   0x75,0x10, // Report Size 10h (16d)
	   0x95,0x01, // Report Count 1
	   0x91,0x02, // Output (Variable)
	   0x55,0x00, // Unit Exponent 0
	   0x66,0x00,0x00, // Unit 0
	0xC0 , // End Collection
	0x09,0xAB, // Usage Create New Effect Report
	0xA1,0x02, // Collection Datalink
	   0x85,PID_NEW_EFFECT_REPORT_ID, // Report ID 1
	   0x09,0x25, // Usage Effect Type
	   0xA1,0x02, // Collection Datalink
	   0x09,0x26, // Usage ET Constant Force
	   0x09,0x27, // Usage ET Ramp
	   0x09,0x30, // Usage ET Square
	   0x09,0x31, // Usage ET Sine
	   0x09,0x32, // Usage ET Triangle
	   0x09,0x33, // Usage ET Sawtooth Up
	   0x09,0x34, // Usage ET Sawtooth Down
	   0x09,0x40, // Usage ET Spring
	   0x09,0x41, // Usage ET Damper
	   0x09,0x42, // Usage ET Inertia
	   0x09,0x43, // Usage ET Friction
	   0x09,0x28, // Usage ET Custom Force Data
	   0x25,0x0C, // Logical Maximum Ch (12d)
	   0x15,0x01, // Logical Minimum 1
	   0x35,0x01, // Physical Minimum 1
	   0x45,0x0C, // Physical Maximum Ch (12d)
	   0x75,0x08, // Report Size 8
	   0x95,0x01, // Report Count 1
	   0xB1,0x00, // Feature
	0xC0 , // End Collection
	0x05,0x01, // Usage Page Generic Desktop
	0x09,0x3B, // Usage Reserved
	0x15,0x00, // Logical Minimum 0
	0x26,0xFF,0x01, // Logical Maximum 1FFh (511d)
	0x35,0x00, // Physical Minimum 0
	0x46,0xFF,0x01, // Physical Maximum 1FFh (511d)
	0x75,0x0A, // Report Size Ah (10d)
	0x95,0x01, // Report Count 1
	0xB1,0x02, // Feature (Variable)
	0x75,0x06, // Report Size 6
	0xB1,0x01, // Feature (Constant)
 0xC0 , // End Collection
 0x05,0x0F, // Usage Page Physical Interface
 0x09,0x89, // Usage Block Load Status
 0xA1,0x02, // Collection Datalink
	0x85,PID_BLOCK_LOAD_REPORT_ID, // Report ID 2
	0x09,0x22, // Usage Effect Block Index
	0x25,MAX_EFFECT_BLOCKS, // Logical Maximum 28h (40d)
	0x15,0x01, // Logical Minimum 1
	0x35,0x01, // Physical Minimum 1
	0x45,MAX_EFFECT_BLOCKS, // Physical Maximum 28h (40d)
	0x75,0x08, // Report Size 8
	0x95,0x01, // Report Count 1
	0xB1,0x02, // Feature (Variable)
	0x09,0x8B, // Usage Block Load Status
	0xA1,0x02, // Collection Datalink
	   0x09,0x8C, // Usage Block Load Success
	   0x09,0x8D, // Usage Block Load Full
	   0x09,0x8E, // Usage Block Load Error
	   0x25,0x03, // Logical Maximum 3
	   0x15,0x01, // Logical Minimum 1
	   0x35,0x01, // Physical Minimum 1
	   0x45,0x03, // Physical Maximum 3
	   0x75,0x08, // Report Size 8
	   0x95,0x01, // Report Count 1
	   0xB1,0x00, // Feature
	0xC0 , // End Collection
	0x09,0xAC, // Usage Undefined
	0x15,0x00, // Logical Minimum 0
	0x27,0xFF,0xFF,0x00,0x00, // Logical Maximum FFFFh (65535d)
	0x35,0x00, // Physical Minimum 0
	0x47,0xFF,0xFF,0x00,0x00, // Physical Maximum FFFFh (65535d)
	0x75,0x10, // Report Size 10h (16d)
	0x95,0x01, // Report Count 1
	0xB1,0x00, // Feature
 0xC0 , // End Collection
 0x09,0x7F, // Usage PID Pool Report
 0xA1,0x02, // Collection Datalink
	0x85,PID_POOL_REPORT_ID, // Report ID 3
	0x09,0x80, // Usage RAM Pool size
	0x75,0x10, // Report Size 10h (16d)
	0x95,0x01, // Report Count 1
	0x15,0x00, // Logical Minimum 0
	0x35,0x00, // Physical Minimum 0
	0x27,0xFF,0xFF,0x00,0x00, // Logical Maximum FFFFh (65535d)
	0x47,0xFF,0xFF,0x00,0x00, // Physical Maximum FFFFh (65535d)
	0xB1,0x02, // Feature (Variable)
	0x09,0x83, // Usage Simultaneous Effects Max
	0x26,0xFF,0x00, // Logical Maximum FFh (255d)
	0x46,0xFF,0x00, // Physical Maximum FFh (255d)
	0x75,0x08, // Report Size 8
	0x95,0x01, // Report Count 1
	0xB1,0x02, // Feature (Variable)
	0x09,0xA9, // Usage Device Managed Pool
	0x09,0xAA, // Usage Shared Parameter Blocks
	0x75,0x01, // Report Size 1
	0x95,0x02, // Report Count 2
	0x15,0x00, // Logical Minimum 0
	0x25,0x01, // Logical Maximum 1
	0x35,0x00, // Physical Minimum 0
	0x45,0x01, // Physical Maximum 1
	0xB1,0x02, // Feature (Variable)
	0x75,0x06, // Report Size 6
	0x95,0x01, // Report Count 1
	0xB1,0x03, // Feature (Constant, Variable)
	0xC0, // End Collection

	0xc0                           // END_COLLECTION
};

HID_DESCRIPTOR              G_DefaultHidDescriptor = {
	0x09,   // length of HID descriptor
	0x21,   // descriptor type == HID  0x21
	0x0100, // hid spec release
	0x00,   // country code == Not Specified
	0x01,   // number of HID class descriptors
	{                                       //DescriptorList[0]
		0x22,                               //report descriptor type 0x22
		sizeof(G_DefaultReportDescriptor)   //total length of report descriptor
	}
};

NTSTATUS
EmuControllerCreateDevice(
    _Inout_ PWDFDEVICE_INIT DeviceInit
    )
/*++

Routine Description:

    Worker routine called to create a device and its software resources.

Arguments:

    DeviceInit - Pointer to an opaque init structure. Memory for this
                    structure will be freed by the framework when the WdfDeviceCreate
                    succeeds. So don't access the structure after that point.

Return Value:

    NTSTATUS

--*/
{
    WDF_OBJECT_ATTRIBUTES deviceAttributes;
    PDEVICE_CONTEXT deviceContext;
    WDFDEVICE device;
    NTSTATUS status;

	//
    // Mark ourselves as a filter, which also relinquishes power policy ownership
    //
	WdfFdoInitSetFilter(DeviceInit);


    WDF_OBJECT_ATTRIBUTES_INIT_CONTEXT_TYPE(&deviceAttributes, DEVICE_CONTEXT);

    status = WdfDeviceCreate(&DeviceInit, &deviceAttributes, &device);

    if (NT_SUCCESS(status)) {
        //
        // Get a pointer to the device context structure that we just associated
        // with the device object. We define this structure in the device.h
        // header file. DeviceGetContext is an inline function generated by
        // using the WDF_DECLARE_CONTEXT_TYPE_WITH_NAME macro in device.h.
        // This function will do the type checking and return the device context.
        // If you pass a wrong object handle it will return NULL and assert if
        // run under framework verifier mode.
        //
        deviceContext = DeviceGetContext(device);

        //
        // Initialize the context.
        //
		deviceContext->Device = device;

		//
		//	Define device hardware Ids
		//

		ULONG productId;

		status = GetProductIdFromRegistry(device, &productId);

		if (!NT_SUCCESS(status))
		{
			TraceEvents(TRACE_LEVEL_ERROR, TRACE_DEVICE, 
				"GetProductIdFromRegistry failed with  %!STATUS!", 
				status);
			return status;
		}

		deviceContext->HidDeviceAttributes.VendorID = HID_DEVICE_VID;		
		deviceContext->HidDeviceAttributes.ProductID = (USHORT)productId;
		deviceContext->HidDeviceAttributes.VersionNumber = HID_DEVICE_VERSION;


		// Use default HID descriptor and report descriptor (hardcoded).
		//
		deviceContext->HidDescriptor = G_DefaultHidDescriptor;
		deviceContext->ReportDescriptor = G_DefaultReportDescriptor;

        //
        // Create a device interface so that applications can find and talk
        // to us.
        //
        status = WdfDeviceCreateDeviceInterface(
            device,
            &GUID_DEVINTERFACE_EmuController,
            NULL // ReferenceString
            );

		if (NT_SUCCESS(status)) {
			//
			// Initialize the I/O Package and any Queues
			//
			status = EmuControllerQueueInitialize(device, &deviceContext->DefaultQueue);
			status = EmuControllerManualQueueInitialize(device, &deviceContext->ManualQueue);
		}

		if (NT_SUCCESS(status)) {

		   if (CreateNamedPipeServer(device) > 0)
		   {
			   status = STATUS_UNSUCCESSFUL;
		   }

		   TraceEvents(TRACE_LEVEL_INFORMATION, TRACE_DEVICE,
			   "Device VID_%X&PID_%X added.", 
			   deviceContext->HidDeviceAttributes.VendorID, 
			   deviceContext->HidDeviceAttributes.ProductID);
		}
    }

    return status;
}

NTSTATUS
GetProductIdFromRegistry(
	_In_ WDFDEVICE Device,
	_Out_ PULONG ProductId
)
/*++
Routine Description:
	Read "ProductId" key value from device parameters in the registry.
Arguments:
	device - pointer to a device object.
Return Value:
	NT status code.
--*/

{
	WDFKEY          hKey = NULL;
	NTSTATUS        status;
	UNICODE_STRING  valueName;


	status = WdfDeviceOpenRegistryKey(Device,
		PLUGPLAY_REGKEY_DEVICE,
		KEY_READ,
		WDF_NO_OBJECT_ATTRIBUTES,
		&hKey);
	if (NT_SUCCESS(status)) {

		RtlInitUnicodeString(&valueName, 
			L"ProductId");

		status = WdfRegistryQueryULong(hKey,
			&valueName,
			ProductId
		);

		WdfRegistryClose(hKey);
	}

	return status;
}