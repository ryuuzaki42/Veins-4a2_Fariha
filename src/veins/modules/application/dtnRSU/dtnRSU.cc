//
// Copyright (C) 2006-2011 Christoph Sommer <christoph.sommer@uibk.ac.at>
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

#include "veins/modules/application/dtnRSU/dtnRSU.h"
#include "veins/modules/messages/WaveShortMessage_m.h"
#include <cmath>
#include <string>
#include <stdlib.h>
#include<stdio.h>

//#include <iostream>
//using namespace std;

#define DBG_Y EV

using Veins::TraCIMobilityAccess;
using Veins::AnnotationManagerAccess;

const simsignalwrap_t dtnRSU::parkingStateChangedSignal = simsignalwrap_t(TRACI_SIGNAL_PARKING_CHANGE_NAME);

Define_Module(dtnRSU);

void dtnRSU::initialize(int stage) {
	BaseWaveApplLayerDTNRSU::initialize(stage);
	if (stage == 0) {
		
debugEV<< "in initialize() stage 0...";
		

		mobility = TraCIMobilityAccess().get(getParentModule());
		traci = mobility->getCommandInterface();
		traciVehicle = mobility->getVehicleCommandInterface();
		annotations = AnnotationManagerAccess().getIfExists();
		ASSERT(annotations);
		
 //nbPackets = par("nbPackets").doubleValue();
 initializationTime = 0;// par("initializationTime");
 delayTimer = new cMessage("appDelay", SEND_DATA_TIMER);
 
 //nbPacketsSent = 0;
 nbPacketsReceived = 0;
 firstPacketGeneration = -1;
 lastPacketReception = -2;
reponse = false;
 
		sentMessage = false;
		lastDroveAt = simTime();
		findHost()->subscribe(parkingStateChangedSignal, this);
		isParking = false;
		sendWhileParking = par("sendWhileParking").boolValue();
		
			sentPackets = 0;
pos = -1;
existe = false;
stop = false;
stopdata = false;

			//scheduleAt(simTime() +uniform(initializationTime, initializationTime + 2), delayTimer);
			scheduleAt(simTime() + 1, delayTimer);
	}
}

void dtnRSU::onBeacon(WaveShortMessage* wsm) {
    EV << "222222222222222222222222222222222222 \n";

    std::cout << findHost()->getFullName() << " onBeacon at: " << simTime();

    EV << "I have received a beacon! ";
    
}

void dtnRSU::onData(WaveShortMessage* wsm) {

    EV << "44444444444444444444444444444444  \n";
    std::cout << findHost()->getFullName() << " onData at: " << simTime();

	findHost()->getDisplayString().updateWith("r=16,green");
	annotations->scheduleErase(1, annotations->drawLine(wsm->getSenderPos(), mobility->getPositionAt(simTime()), "blue"));


	EV <<"I am "<< myId << " and I received data  " << wsm->getWsmData() << "  \n";
	data = wsm->getWsmData();

	for (i = 0 ; i < taille ; i++) {

	           if (T[i] == "") {

	               T[i] = data;
	               break;

	           }

	        }


	for (i = 0 ; i < taille ; i++) {

	    EV << " The table  T  contains:  " << T[i] << "   \n";
	}

}


void dtnRSU::onACK(WaveShortMessage* wsm) {


}


void dtnRSU::receiveSignal(cComponent* source, simsignal_t signalID, cObject* obj) {
	Enter_Method_Silent();
	if (signalID == mobilityStateChangedSignal) {
	       handlePositionUpdate(obj);
	    }
	    else
	 if (signalID == parkingStateChangedSignal) {
		handleParkingUpdate(obj);
	}
}
void dtnRSU::handleParkingUpdate(cObject* obj) {
	isParking = mobility->getParkingState();
	if (sendWhileParking == false) {
		if (isParking == true) {
			(FindModule<BaseConnectionManager*>::findGlobalModule())->unregisterNic(this->getParentModule()->getSubmodule("nic"));
		}
		else {
			Coord pos = mobility->getCurrentPosition();
			(FindModule<BaseConnectionManager*>::findGlobalModule())->registerNic(this->getParentModule()->getSubmodule("nic"), (ChannelAccess*) this->getParentModule()->getSubmodule("nic")->getSubmodule("phy80211p"), &pos);
		}
	}
}

void dtnRSU::handlePositionUpdate(cObject* obj) {
    // BaseWaveApplLayer::handlePositionUpdate(obj);


}

void dtnRSU::sendWSM(WaveShortMessage* wsm) {
	if (isParking && !sendWhileParking) return;
	sendDelayedDown(wsm,individualOffset);
}

void dtnRSU::finish() {
    recordScalar("# Sent Pkts", nbPacketsSent);
}
