# SyncLights

SyncLights is an Arduino Application that allows your arduino board to receive patterns to play on the attached Adafruit WS2801 LED string over the internet.

## Future

I'm currently working on a way to have a central server queue up patterns (possibly submitted via a public website) to play across any Arduinos that decide to connect to it. People will be able to set up their arduino with this and point it at a server which people can submit patterns to. They will enter a queue and when that pattner comes up in the queue the server will send it to all of the arduinos registered with the server so they will all play the same pattern simultaneously. All of the related code will also be release here on my [GitHub profile](http://github.com/theanti9).


### Dependencies:

* [Adafruit WS2801 Library](https://github.com/adafruit/Adafruit-WS2801-Library)
* [Arduino Time](http://www.arduino.cc/playground/uploads/Code/Time.zip)


