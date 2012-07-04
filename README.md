# SyncLights

SyncLights is an Arduino Application that allows your arduino board to receive patterns to play on the attached Adafruit WS2801 LED string over the internet.

## Future

I'm currently working on a way to have a central server queue up patterns (possibly submitted via a public website) to play across any Arduinos that decide to connect to it. People will be able to set up their arduino with this and point it at a server which people can submit patterns to. They will enter a queue and when that pattner comes up in the queue the server will send it to all of the arduinos registered with the server so they will all play the same pattern simultaneously. All of the related code will also be release here on my [GitHub profile](http://github.com/theanti9).


## Arduino Setup

From a hardware perspective there are a few things you need for the Arduino:

* [Ethernet Shield](http://arduino.cc/en/Main/ArduinoEthernetShield) or some variant of arduino with Ethernet capability.
* [Adafruit WS2801 LED Pixels](http://www.adafruit.com/products/738). Note that you can string multiples of these together easily but more than one will probably require a power source more powerful than just the arduinos +5v pin.
* [5V 2A Power Supply](http://www.adafruit.com/products/276). If you read the documentation on the pixels, unless you're only going to be powering a few at a time, it suggests using an external power supply in order to handle the amperage.
* (tehcnically optional. Good if you don't already have a way to connect the external power supply) [Female DC Power adapter](http://www.adafruit.com/products/368). 
* (optional if you don't want to have to cut the end of the cable) [4-pin JST SM Plug Cable](http://www.adafruit.com/products/578) Pop this onto the end of the WS2801 string and then put the appropriate wires into their slots on the arduino.

### Wiring

The wiring for this project is extremely simple. You can find the documentation for the WS2801's [here](http://www.ladyada.net/products/pixel12mm/#wiring). As I stated in the setup list, you should have an external power supply for the LED Pixels, though I noticed that plugging them into the arduino directly at least for testing worked ok (DISCLAIMER: It's not my fault if you do this and something bad happens to your arduino board!).

Assuming you did get the external power supply, this leaves two wires to go into the arduino board. The "Data" (yellow) and the "Clock" (green). If you look at the arduino code you'll see at the top the following:

```c
int dataPin = 2;  // Yellow wire
int clockPin = 3;  // Green wire
```

So if you don't want to bother with changing these numbers, just put the data in digital pin 2, and the clock in digital pin 3. There is also another important setting, the number of pixels you have!

```c
int stripLength = 25;  // How many lights
```

The WS2801's have 25 pixels each. If you use one strand you have 25, if you hook up a second you have 50 and so on. Set this to that number.

> NOTE: These are meant to be arranged in a grid. You'll see a multi-dimensional array aptly named *grid* not too far below the previously mentioned settings. This grid allows the code to use an (x,y) coordinate system to access the pixels.
> By default, the grid is set for a 5 x 5 grid (which is just one strand of Pixels). If you want something different which __includes MORE than one strand__ you need to adjust *grid* accordingly.

* * *

Alright! From an electronics perspective, you're done! Now for some network settings. Here's the network settings block:

```c
// Server settings
int serverPort = 1190;
boolean useDHCP = true;
IPAddress arduinoServerIP(192,168,1,125);  // Only used if DHCP is off. This will depend on your network setup.
// Syncing stuff
boolean doSync = true;
IPAddress centralServer(192,168,1,122);
int centralServerPort = 80;
```

### Server stuff

- int *serverPort*: This is the port that you want the arduino server to listen on for incoming connections; pretty self-explanatory. (Default is 1190)
- boolean *useDHCP*: Whether the arduino should try and acquire it's IP address from a DHCP server or not. If this is true, the next setting can be disregarded.
- IPAddress *arduinoServerIP*: This obviously depends on your network setup and also doesn't matter if *useDHCP* is *true*.

### Syncing stuff

If you want to connect to a central server and have the arduino receive it's patterns from there, set *doSync* to true and fill these in. Otherwise, set *doSync* to false, and ignore them.

- boolean *doSync*: Whether or not to sync the patterns with a central server.
- IPAddress *centralServer*: the IP Address of the central server to connect to and register with.
- int *centralServerPort*: The port the central server is listening on. (Default is 80 since it takes HTTP requests)

### You're done!

Hopefully at this point, it should be setup and working! Feel free to send me a message on GitHub if you're having trouble. Also, please submit any bugs you come across on the issues page!

If you're looking for the syncing server, you can find it on my profile [here](https://github.com/theanti9/SyncLightsServer). More documentation on the sync server to come..

## Protocol Spec

Currently the protocol for the SyncLights arduino server is very simple and really only does one thing: Accept pattern definitions. It is a standard TCP server.

> NOTE: Make sure all multi-byte integer values are Network Byte Order!

Here's the order of things that should be sent:

1. Initialization byte
2. Pattern Length - 32-bit integer
3. Pattern Time - 8-bit integer
4. *Pattern Length* Instructions (8 bytes each)

Each instruction goes as follows:

1. Blank byte (0x00)
2. Pixel Red Value - byte (0x00 - 0xFF)
3. Pixel Green Value - byte (0x00 - 0xFF)
4. Pixel Blue Value - byte (0x00 - 0xFF)
5. Pixel X Coordinate - 8-bit integer
6. Pixel Y Coordinate - 8-bit integer
7. Time Delay - 16-bit integer

### Initialization byte

The very first byte that should be sent is called the Initialization byte and is equal to 1 (0x01).

### Pattern Length

The pattern length is a 32-bit integer that represents how many "Instructions" or "Steps" there will be.

### Pattern Time

The pattern time designates how long to play the submitted pattern for in seconds (0-255)

### Instructions

This is where you start reapeating the 8-byte instructions. The number of bytes following the Pattern Time should be equal to (8 * Pattern Length)

* * *

### Blank Byte

This should always be 0x00, although it really doesn't matter, it's just the extra byte from storing the 24-bit color value in a 32-bit integer

### Pixel Red Value

This byte should be the Red "RGB" value and should be between 0 and 255 (0x00 - 0xFF)

### Pixel Green Value

This byte should be the Green "RGB" value and should be between 0 and 255 (0x00 - 0xFF)

### Pixel Blue Value

This byte should be the Blue "RGB" value and should be between 0 and 255 (0x00 - 0xFF)

### Pixel X Coordinate

This is an 8-bit integer that represents x coordinate on the grid of lights for which light the color should be applied to.

### Pixel Y Coordinate

This is an 8-bit integer that represents y coordinate on the grid of lights for which light the color should be applied to.

### Time Delay

This is a 16-bit integer that represents the number of *miliseconds* to wait after applying this instruction before moving on to the next one.

* * *

### SyncLights Arduino Server Response

After a pattern is sent and the arduino parses it, it will reply with the ASCII string "load complete" and close the connection.



## Dependencies:

* [Adafruit WS2801 Library](https://github.com/adafruit/Adafruit-WS2801-Library)
* [Arduino Time](http://www.arduino.cc/playground/uploads/Code/Time.zip)


