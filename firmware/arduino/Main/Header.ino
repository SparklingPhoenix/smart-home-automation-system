// Smart Home Controller
// Hardware: Arduino Mega Pro Mini (ATmega2560)
//
// This file contains hardware definitions,
// pin assignments and global declarations.

#include <Bounce2.h>                       // Debouncing library for mechanical switches
#include <EEPROM.h>                        // EEPROM library
#include <Wire.h>                          // I2C communication library
#include <ACS712.h>                        // ACS712 current sensor library
#include <ZMPT101B.h>                      // ZMPT101B AC voltage sensor library
#include <DHT.h>                           // DHT22 temperature and humidity sensor library
#include <DFRobot_AHT20.h>                 // AHT20 temperature and humidity sensor library
DFRobot_AHT20 aht20;
#include <IRremote.h>                      // Infrared transmitter library
IRsend IrSender;

int khz = 38;                              // NEC protocol carrier frequency (38 kHz)

#define TEL_NUMBER "380951234567"          // Primary phone number

// -----------------------------------------------------------------------------
// Global variables
// -----------------------------------------------------------------------------

// Input pin assignments
#define in_1 51                            // Hallway wall switch
#define in_2 49                            // Bedroom wall switch
#define in_3 23                            // Toilet wall switch
#define in_4 25                            // Bathroom wall switch
#define in_5 27                            // Living room wall switch
#define in_6 29                            // Kitchen wall switch
#define in_7 31                            // Home office wall switch
#define Valet 69                           // Service (Valet) button
#define DoorBell_btn 63                    // Doorbell push button
#define DoorLock 59                        // Entrance door deadbolt sensor
#define DoorState 61                       // Entrance door magnetic contact
#define WindowState 58                     // Living room window magnetic contact
#define Cover 54                           // Enclosure tamper switch
#define Current_Sens 57                    // 12 V DC current sensor (power supply output)
#define SENSITIVITY 206.0f                 // AC voltage sensor calibration factor
#define DHTPIN1 56                         // Indoor temperature and humidity sensor
#define DHTPIN2 54                         // Outdoor temperature and humidity sensor
#define DHTTYPE DHT22                      // DHT sensor type

// Output pin assignments
#define out_11 44                          // Hallway: Local lighting
#define out_12 35                          // Hallway: Main lighting
#define out_13 37                          // Hallway: Night lighting

#define out_21 42                          // Bedroom: Local lighting
#define out_22 33                          // Bedroom: Main lighting
#define out_23 53                          // Bedroom & Toilet: Ventilation

#define out_31 40                          // Toilet: Local lighting
#define out_32 39                          // Toilet: Main lighting

#define out_41 30                          // Bathroom: Local lighting
#define out_42 32                          // Bathroom: Main lighting
#define out_43 22                          // Bathroom: Mirror lighting
#define out_44 52                          // Bathroom: Ventilation

#define out_51 28                          // Living room: Local lighting
#define out_52 41                          // Living room: Track lighting #1
#define out_53 47                          // Living room: Track lighting #2
#define out_54 43                          // Living room: Track lighting #3
#define out_55 45                          // Living room: Track lighting #4

#define out_61 13                          // Kitchen: Local lighting
#define out_62 34                          // Kitchen: Bar counter lighting
#define out_63 36                          // Kitchen: Main lighting

#define out_71 38                          // Home office: Main lighting

#define GSM_Wake 12                        // GSM modem power control
#define PowerUnit_12V 50                   // 12 V power supply enable
#define DoorBell 11                        // Doorbell chime
#define howler 24                          // Alarm siren
#define Boiler 48                          // Gas boiler
#define VCC_dht22 65                       // DHT22 sensor power supply (VCC)

// Arrays used to configure and poll digital inputs (physical wall switches)
byte input_pins[14] = {0, in_1, in_2, in_3, in_4, in_5, in_6, in_7, Valet, DoorBell_btn, DoorLock, DoorState, Cover, WindowState};

// Array used to configure digital outputs
byte output_pins[40] = {0, 47, 45, 43, 41, 39, 37, 35, 33, 32, 34, 36, 38, 40, 42, 44, 46, 10, 8, 6, 4, 2, 3, 5, 7, 8, 10, 11, 48, 22, 24, 26, 28, 30, 12, 13, 50, 52, 53, 65};

// UART communication settings
int Delay_S1 = 30;                         // Delay after sending a command to the Orange Pi via UART

// -----------------------------------------------------------------------------
// Variables used for processing physical switch events
// -----------------------------------------------------------------------------

Bounce debouncer[14] = {};                 // Debouncer objects for physical switches
int flag[8] = {};                          // Event flags
int kit[8] = {};                           // Lighting combination state flags

byte Init_flag = 0;                        // Startup initialization flag

// Timer arrays for switch event processing
int start_point[8] = {};
int end_point[8] = {};
int control_point[8] = {};

// Timer configuration
uint32_t Init_pause = 2000;             // Startup delay to suppress false button events after power-up
uint32_t time_exp = 1000;               // Long-press detection time
uint32_t time_set = 3000;               // Delay before generating the OFF command after button release

// Ventilation timers
uint32_t SetPoint_BathRoom_Vent = 900000;    // Bathroom ventilation runtime (15 minutes)
uint32_t SetPoint_BedRoom_Vent = 36000000;   // Bedroom / dressing room ventilation runtime (10 hours)

// Security timers
uint32_t SetPoint_GirthRail = 60000;         // Entrance door lock reminder (60 seconds)
uint32_t SetPoint_DoorState = 120000;        // Entrance door open reminder (2 minutes)
uint32_t SetPoint_DoorBell = 1000;           // Doorbell lighting indication interval

// GSM modem
uint32_t GSM_modem_Check_1 = 5000;           // GSM modem response timeout (5 seconds)
uint32_t GSM_modem_Check_2 = 15000;          // GSM modem startup timeout (15 seconds)
uint32_t GSM_modem_Check_3 = 150000;         // Periodic GSM modem status check (5 minutes)

// Sensor polling
uint32_t SetPoint_Power_check = 1000;        // AC power monitoring interval
uint32_t SetPoint_Temp_check_1 = 10000;      // Indoor temperature and humidity update interval
uint32_t SetPoint_Temp_check_2 = 20000;      // Outdoor temperature and humidity update interval

// Boiler control
uint32_t boiler_cycle_ON = 240000;           // Minimum boiler operating time after startup

// Doorbell
uint32_t SetPoint_DoorButton = 3000;         // Doorbell light and sound indication duration

// Environmental sensors
uint32_t SetPoint_AHT20 = 15000;             // AHT20 sensor polling interval

// Air conditioner protection
uint32_t AC_cycle_CYCLE = 300000;            // Minimum air conditioner operating time
uint32_t AC_cycle_MIN = 900000;              // Minimum pause before restarting the air conditioner
uint32_t delayed_start_AC = 5000;            // Delayed AC start after window closure

// -----------------------------------------------------------------------------
// Runtime timer variables
// Start_*   - timestamp captured when the timer starts
// Current_* - elapsed time since Start_*
// -----------------------------------------------------------------------------
// Runtime timers
uint32_t Start_Init_pause = 0;            // Startup delay timer

// Bathroom ventilation
uint32_t Start_BathRoom_Vent = 0;         // Timer start
uint32_t Current_BathRoom_Vent = 0;       // Elapsed time

// Bedroom ventilation
uint32_t Start_BedRoom_Vent = 0;          // Timer start
uint32_t Current_BedRoom_Vent = 0;        // Elapsed time

// Entrance door lock
uint32_t Start_label_GirthRail = 0;       // Timer start
uint32_t Current_label_GirthRail = 0;     // Elapsed time

// Entrance door state
uint32_t Start_label_DoorState = 0;       // Timer start
uint32_t Current_label_DoorState = 0;     // Elapsed time

// Doorbell lighting indication
uint32_t Start_label_DoorBell = 0;        // Timer start
uint32_t Current_label_DoorBell = 0;      // Elapsed time

// GSM modem
uint32_t Start_GSM_modem = 0;             // GSM modem status polling
uint32_t Current_GSM_modem = 0;           // Elapsed time

// Power monitoring
uint32_t Start_Power_Measure = 0;         // AC power measurement

// Environmental sensors
uint32_t Start_Temp_Measure_1 = 0;        // Indoor temperature and humidity measurement
uint32_t Start_Temp_Measure_2 = 0;        // Outdoor temperature and humidity measurement

// Boiler
uint32_t Start_boiler_cycle = 0;          // Boiler minimum operating time

// DHT sensors
uint32_t DHT_Restart_timer = 0;           // DHT sensor power reset timer

// Alarm indication
uint32_t Start_Alarm_flicker = 0;         // Alarm flashing timer
uint32_t Current_Alarm_flicker = 0;       // Elapsed time

// Doorbell indication
uint32_t Start_label_DoorButton = 0;      // Light and sound indication timer
uint32_t Current_label_DoorButton = 0;    // Elapsed time

// AHT20 sensor
uint32_t Start_label_AHT20 = 0;           // AHT20 polling timer
uint32_t Current_label_AHT20 = 0;         // Elapsed time

// Air conditioner protection
uint32_t Start_AC_cycle = 0;              // Minimum AC operating time
uint32_t Start_AC_MIN = 0;                // Mandatory blower runtime
uint32_t Stop_AC_cycle = 0;               // Minimum restart delay

// Window monitoring
uint32_t start_mark_Window = 0;           // Delayed AC restart after window closure

// -----------------------------------------------------------------------------
// Living room lighting control
// -----------------------------------------------------------------------------
byte LR_Serv = 0;                         // Living room lighting service mode
byte Track_1, Track_2, Track_3, Track_4;  // Track lighting selection

// -----------------------------------------------------------------------------
// 12 V Power Supply Monitoring
// -----------------------------------------------------------------------------
float T_PowerUnit = 55.00;                // Maximum allowable PSU temperature (°C)
float P_acdc_max = 230.00;                // Maximum allowable PSU load (W)
byte PowerUnit_STOP = 0;                  // PSU overtemperature flag

// -----------------------------------------------------------------------------
// Audio System
// -----------------------------------------------------------------------------
byte Amp_state = 0;                       // Audio amplifier state
byte Ble_Amp_state = 0;                   // Bluetooth receiver state
byte Amp_TB = 0;                          // Telegram notification flag

// -----------------------------------------------------------------------------
// Security and Alarm System
// Security_State:
//   0 - Disarmed
//   1 - Armed
//   2 - Home mode
// -----------------------------------------------------------------------------
byte Flicker_list[8] = {0, out_12, out_22, out_32, out_42, out_55, out_63, out_71}; // Lighting outputs used for alarm flashing

byte Security_State = 0;                  // Security system state
uint8_t GSM_check = 0;                    // GSM communication status
byte Step = 0;                            // Alarm sequence step

byte Block_DBF_1 = 0;                     // DoorBell function lock flag
byte Call_flag = 1;                       // Master phone call request flag
byte Alarm_flag = 0;                      // Alarm indication active
byte Valet_flag = 0;                      // Valet function executed flag

byte Window = 0;                          // Living room window state
byte GirthRail = 0;                       // Entrance door deadbolt state
byte InnerDoor = 0;                       // Inner entrance door state
byte DoorButton = 0;                      // Doorbell button event flag

byte AF = 0;                              // Alarm flashing sequence index
int Offset = 0;                           // Delay between flashing outputs (ms)

int8_t Secur_index[15] = {};              // Security settings array

byte X = 0;                               // Auxiliary variable

byte SF_10 = 0;                           // Hallway light activation flag
byte DBF_1 = 0;                           // Hallway lighting state backup (DoorBell function)
byte DBF_2 = 0;                           // Bar lighting state backup (DoorBell function)

// ======================================================
// Air conditioner IR codes
//
// The AC uses a full-state IR protocol.
// Each transmission contains the complete operating state
// (mode, temperature, fan speed, swing position, etc.),
// therefore the command arrays differ only by a few bytes.
// ======================================================

// Cooling mode
// Fan speed 1
unsigned int AC_ON_1[] = {184,5480,9004,4432,672,1620,672,524,672,528,672,1620,672,1620,668,528,672,528,668,528,672,524,676,524,672,524,672,528,672,528,668,528,672,524,672,528,672,524,672,528,672,524,672,532,668,524,672,1620,672,1620,672,528,672,524,672,528,672,524,672,528,672,1620,668,528,672,1620,672,528,672,524,672,1620,672,528,672,19796,696,528,672,528,668,528,672,524,672,528,668,528,672,528,668,528,672,528,668,528,672,528,668,528,672,528,668,528,672,1620,672,528,672,524,672,524,672,528,672,524,672,528,672,524,672,528,668,528,672,528,668,528,672,524,672,528,672,1620,672,1620,672,1620,668,528,672,52632,184};

// Fan speed 2
unsigned int AC_ON_2[] = {8976,4436,668,1620,672,528,672,524,672,1620,672,524,676,1620,668,528,672,524,672,528,672,524,672,528,668,528,672,524,672,528,672,524,672,528,672,528,668,528,672,528,668,528,672,524,672,1620,672,1620,672,528,668,528,672,528,668,528,672,524,672,1620,700,500,672,1620,672,528,668,528,672,1616,672,528,672,19796,724,496,700,500,700,496,672,528,728,468,700,500,700,496,700,500,700,496,732,468,700,496,700,500,728,468,700,496,704,1588,700,500,700,496,700,500,700,496,732,468,700,496,700,496,700,500,700,496,732,468,732,464,704,496,728,468,700,1592,700,1592,700,1592,700,496,704};

// Fan speed 3
unsigned int AC_ON_3[] = {9008,4376,732,1588,732,464,732,464,736,1556,704,1588,732,1560,732,464,736,464,732,464,736,464,732,464,732,464,736,464,704,492,736,464,732,464,736,464,732,464,704,496,704,492,732,468,732,1556,704,1588,704,496,732,464,736,464,732,464,736,464,732,1556,736,464,732,1560,732,464,732,468,732,1560,732,464,732,19732,760,468,732,464,736,464,732,464,736,464,732,464,732,464,736,464,704,492,736,464,704,492,704,496,732,464,732,468,732,1560,732,464,732,464,736,464,736,460,736,464,732,464,736,464,732,464,736,464,704,492,704,496,704,492,704,492,704,1588,704,1588,704,1588,704,496,732,12044,120};

// Power OFF
unsigned int AC_OFF[] = {240,43668,132,5208,8996,4412,696,1624,636,560,672,528,668,528,672,528,668,1600,692,528,668,528,672,1620,640,560,668,1624,668,528,672,528,668,528,672,528,668,528,672,528,668,528,672,528,668,528,672,528,668,1620,672,528,668,528,672,528,668,528,672,528,668,528,672,1620,672,528,668,1624,636,560,672,528,668,1624,668,528,672,19820,672,528,668,528,672,528,668,528,672,528,668,528,672,528,668,528,672,528,668,528,672,528,668,528,672,524,672,528,668,1624,640,560,668,528,672,524,672,528,668,528,672,528,668,528,672,528,668,528,672,528,668,528,668,528,672,528,668,528,672,528,668,1624,668,528,672,1164,128};

// Heating mode
// Fan speed 1
unsigned int ON_AC_1[] = {8976,4432,672,528,668,528,672,1620,672,1620,668,528,672,1620,672,528,672,524,672,528,672,1624,668,1620,672,1620,668,528,672,528,668,528,672,524,672,528,672,524,672,528,672,524,672,528,672,1616,672,1620,672,528,672,524,672,528,668,528,672,528,668,1624,672,524,672,1620,672,528,668,528,672,1620,672,528,668,19800,696,528,672,524,672,528,672,524,672,528,672,524,672,528,672,524,672,528,672,524,672,528,672,524,672,528,672,524,672,1620,672,528,668,528,672,524,672,528,672,524,672,528,672,524,672,528,672,524,672,528,672,524,672,528,672,524,672,528,672,524,672,528,668,1620,672,65500,128};

// Fan speed 2
unsigned int ON_AC_2[] = {8976,4432,672,528,668,528,672,1620,672,1620,668,528,672,1620,672,528,672,524,672,528,672,1624,668,1620,672,1620,668,528,672,528,668,528,672,524,672,528,672,524,672,528,672,524,672,528,672,1616,672,1620,672,528,672,524,672,528,668,528,672,528,668,1624,672,524,672,1620,672,528,668,528,672,1620,672,528,668,19800,696,528,672,524,672,528,672,524,672,528,672,524,672,528,672,524,672,528,672,524,672,528,672,524,672,528,672,524,672,1620,672,528,668,528,672,524,672,528,672,524,672,528,672,524,672,528,672,524,672,528,672,524,672,528,672,524,672,528,672,524,672,528,668,1620,672,65500,128};

// Fan speed 3
unsigned int ON_AC_3[] = {8976,4432,672,528,668,528,672,1620,672,1620,668,528,672,1620,672,528,672,524,672,528,672,1624,668,1620,672,1620,668,528,672,528,668,528,672,524,672,528,672,524,672,528,672,524,672,528,672,1616,672,1620,672,528,672,524,672,528,668,528,672,528,668,1624,672,524,672,1620,672,528,668,528,672,1620,672,528,668,19800,696,528,672,524,672,528,672,524,672,528,672,524,672,528,672,524,672,528,672,524,672,528,672,524,672,528,672,524,672,1620,672,528,668,528,672,524,672,528,672,524,672,528,672,524,672,528,672,524,672,528,672,524,672,528,672,524,672,528,672,524,672,528,668,1620,672,65500,128};

// ======================================================
// Web interface settings
// ======================================================

uint8_t Manag_index[17] = {0};        // States of the management switches from the web interface
uint8_t Blackout_index[11] = {0};     // Blackout scenario configuration

int Therm[8] = {0, 223, 265, 0, -10, -17, 180, 285};
// Temperature setpoints (stored as temperature ×10)

int Temperature[3] = {0, 0, 0};
// Measured air temperatures

byte Compute = 0;
// Scheduler flag for periodic sensor calculations and database updates

int N = 0;
// Number of ADC samples used for averaging

// ======================================================
// AC voltage monitoring (ZMPT101B)
// ======================================================

ZMPT101B voltageSensor(67, 50.0);     // Voltage sensor (pin, mains frequency)

float volt = 0;
float sum = 0;

byte Power = 1;                       // Mains power status
byte Power_TB = 0;                    // Telegram/database notification flag

byte P1 = 0;
byte P2 = 0;

byte Vent_Bed = 0;
byte Vent_Bath = 0;

// ======================================================
// DC current monitoring
// ======================================================

long int Value_DC = 0;                // Raw ADC value
float I_dc = 0;                       // Measured DC current
float Uxx = 2.700;                    // Zero-current sensor output voltage

// ======================================================
// AC Current Sensor (ACS712)
// ======================================================

ACS712 sensor(ACS712_05B, A1);        // ACS712 current sensor

float I_ac = 0;                       // Measured AC current (RMS)
float i = 0;                          // Current measurement sample

// ======================================================
// Temperature & Humidity Sensors (DHT22)
// ======================================================

DHT dht1(DHTPIN1, DHTTYPE);           // Indoor sensor
DHT dht2(DHTPIN2, DHTTYPE);           // Outdoor sensor

byte Stage = 0;                       // Measurement sequence step
byte dht_Restart = 0;                 // DHT sensor power-cycle flag

int t1 = 0;                           // Indoor air temperature
float h1 = 0;                         // Indoor relative humidity

float t2 = 0;                         // Outdoor air temperature
float h2 = 0;                         // Outdoor relative humidity