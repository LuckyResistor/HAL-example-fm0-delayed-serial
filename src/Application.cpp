//
// HAL delayed serial example
// ---------------------------------------------------------------------------
// (c)2019 by Lucky Resistor. See LICENSE for details.
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along
// with this program; if not, write to the Free Software Foundation, Inc.,
// 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
//
#include "Application.hpp"


#include "hal-common/event/Loop.hpp"
#include "hal-common/Timer.hpp"
#include "hal-common/SerialLineBuffer.hpp"
#include "hal-common/SerialLineStringWriter.hpp"
#include "hal-feather-m0/GPIO_Pin_FeatherM0.hpp"
#include "hal-feather-m0/SerialLine_ArduinoUSB.hpp"

#pragma clang diagnostic ignored "-Wmissing-noreturn"


namespace lr {
namespace Application {


/// Create an event loop with a queue for 32 events.
///
event::BasicLoop<event::StaticStorage<32>> gEventLoop;

/// Define the serial USB interface.
///
SerialLine_ArduinoUSB gSerialUSB;

/// Create a buffer for the USB serial line.
///
SerialLineBuffer gSerialBuffer(&gSerialUSB, 0x100, 0);

/// Create a write to write strings to the serial line.
///
SerialLineStringWriter gSerial(&gSerialBuffer);

/// The pin with the LED.
///
const auto gLedPin = GPIO::Pin13();


// Forward declarations
void synchronizeBufferEvent();
void turnOnEvent();
void turnOffEvent();
void writeTextAEvent();
void writeTextBEvent();


void setup()
{
    // Initialize the native USB serial line.
    gSerialUSB.initialize();

    // Write a number of messages.
    // In a real application, this could be error messages while startup.
    gSerial.writeLine("Delayed serial, setup");

    // Make sure the buffer is synchronized in the event loop.
    event::mainLoop().addPollEvent(&synchronizeBufferEvent);

    // Start the event based blinking on 100_ms
    event::mainLoop().addDelayedEvent(&turnOnEvent, 100_ms);

    // Start text write events at different times.
    event::mainLoop().addRepeatedEvent(&writeTextAEvent, 2500_ms);
    event::mainLoop().addRepeatedEvent(&writeTextBEvent, 4200_ms);

    // Setup the LED pin.
    gLedPin.configureAsOutput();
}


/// Event to synchronize the serial line buffer.
///
void synchronizeBufferEvent()
{
    gSerialBuffer.synchronize();
}


/// An event to turn the status LED on.
///
void turnOnEvent()
{
    gLedPin.setOutputHigh();
    event::mainLoop().addDelayedEvent(&turnOffEvent, 500_ms);
}


/// An event to turn the status LED off.
///
void turnOffEvent()
{
    gLedPin.setOutputLow();
    event::mainLoop().addDelayedEvent(&turnOnEvent, 500_ms);
}


/// An event A, writing text to the serial line.
///
void writeTextAEvent()
{
    static uint32_t counter = 1;
    String line("Event A: 0x");
    line.appendHex(counter);
    counter += 1;
    gSerial.writeLine(line);
}


/// An event B, writing text to the serial line.
///
void writeTextBEvent()
{
    static uint32_t counter = 1;
    String line("Event B: 0x");
    line.appendHex(counter);
    counter += 1;
    gSerial.writeLine(line);
}


void loop()
{
    gEventLoop.loopOnce();
}


}
}

