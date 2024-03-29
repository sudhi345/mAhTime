# mAhTime

### Objective :

To calculate the battery life for a device using small batteries and intern use the data to calculate for any bigger battery.

###  Why do we need this?

The capacity of a battery is measured in Ah (ampere-hours, for large batteries) or mAh (milliampere-hours, for small batteries). 
This is usually mentioned on the battery or in the datasheets provided by the manufacturer.
But the capacity changes depending on the age of the battery, operating conditions, slightly different chemical composition, etc. 
So it affects the final result if use the capacity value provided by the manufacturer. 
*[learn more.](https://batteryuniversity.com/learn/article/battery_definitions)*
 
To get the actual capacity (close-enough-value), measuring the same is the method. 
Different methods using which one can measure the capacity, for example, Coulomb counting, Voltage monitoring, etc.
*(Learn more [here](https://www.quora.com/How-can-we-measure-a-batterys-real-capacity-in-Mah-or-ah-accurately) or 
[here](https://batteryuniversity.com/learn/article/how_to_measure_capacity)).*

Here we use the second method; monitoring the terminal voltage when a known load (resistive) is connected. 

### Method:

The batteries are charged and connected to a custom-designed shield for the esp8266 microcontroller and a load is selected using a button.
The microcontroller logs the terminal voltage read from its pin to a remote server until the lower cut off voltage for the batteries is reached.
The loads are automatically disconnected using logic level p-MOS switches. 
A python script displays the discharge curve and the area under the 
curve (area under the V*I v/s t curve is power rating and area under the I v/s t curve is the mAh / Ah of the batteries).<br>
Once again the batteries are charged but this time Device Under Test (DUT) is selected instead of a load (using the same microcontroller). 
The time taken for the DUT to discharge the batteries is found. This can then be used to calculate the life of any battery using ratios 
( Eg.: If for X (mAh) it is Y (no. of days) then for Z (mAh)? = YZ/X ).

### Hardware & Firmware:

A microcontroller unit (ESP8266-12E on LOLin board) has an inbuilt ADC (Analog to Digital Converter) with a precision of (1/1024)th of a volt.
But the input voltage range is only 0 (0) - 1V (1024). To use this in 0 - 3V range a resistor voltage divider is implemented 
using precision resistors and an n- MOS switch to enable this voltage divider.
The voltage divider is not the one which is present on the LOLin board. Those are not precision resistors, 
hence the readings may not be accurate.
The n-MOS switch is required as the voltage divider acts as another resistive load parallel to the selected load.
An OLED display is used to display the selected load, time, etc.

The hardware limits the maximum battery voltage to 3.6V as the battery voltage appears at pin A0 when voltage divider is disabled and the absolute maximum voltage given is 3.6V for esp8266. The time taken for measurement is determined by the capacity of the battery and the chosen load.

The circuit connections and the designed PCB can be found in the directory PCB_KICAD.
The details about the charger and the battery can be found in the 'Charger_hacked' directory. <br>
Arduino IDE is used to program the microcontroller. The required header files and libraries are in the 'upload_voltage' directory.

![Snapshots of the display](https://github.com/sudhi345/mAhTime/blob/master/gallery/display_snapshot.png)

### Result:

Here are pictures of graph and the PCB.
![Graph and the soldered PCB](https://github.com/sudhi345/mAhTime/blob/master/gallery/graph-and-pcb.png)

### Conclusion:

This project is helpful even if the DUT draws a few micro-amperes of current during sleep and few milli-amperes during high activity.
Hence the calculation of battery life is simplified with this.

