//
// Copyright (C) 2011 David Eckhoff <eckhoff@cs.fau.de>
//
// Documentation for these modules is at http://veins.car2x.org/
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
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//

package org.car2x.veins.modules.application.ieee80211pDTNRSU;
import org.car2x.veins.base.modules.IBaseApplLayer;

//
// WAVE application layer base class.
//
// @author David Eckhoff
//
// @see BaseWaveApplLayer
// @see Nic80211p
// @see Mac1609_4
// @see PhyLayer80211p
//
simple BaseWaveApplLayerDTNRSU like IBaseApplLayer
{
    parameters:
        bool debug = default(false); //whether to print debug output
        int headerLength @unit(bit) = default(88bit); //header length of the application

        bool sendBeacons = default(false); //tell the applayer to periodically send beacons
        int beaconLengthBits @unit(bit) = default(256bit); //the length of a beacon packet
        int beaconPriority = default(3); //the access category of the beacon messages
        volatile double beaconInterval @unit(s) = default(1s); //the intervall between 2 beacon messages

        double maxOffset @unit(s) = default(0s); //the maximum delay of messages sent down to avoid synchronous channel access
        bool sendData = default(false); //whether this applayer should send data
        int dataLengthBits @unit(bit) = default(1024bit); //the length of a data packet
        bool dataOnSch = default(true); //tells the applayer whether to use a service channel for datapackets or the control channel
        int dataPriority = default(3); //the access category for data packets
        int nbcopy = default(1);
//int taillebuffer = default (10);

        int ackLengthBits @unit(bit) = default(256bit); //the length of a beacon packet
        int ackPriority = default(2); //the access category of the beacon messages

    gates:
        input lowerLayerIn; // from mac layer
        output lowerLayerOut; // to mac layer
        input lowerControlIn;
        output lowerControlOut;
}
