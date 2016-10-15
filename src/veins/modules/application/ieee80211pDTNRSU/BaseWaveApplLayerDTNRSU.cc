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

#include "veins/modules/application/ieee80211pDTNRSU/BaseWaveApplLayerDTNRSU.h"
//#include "veins/modules/mobility/traci/TraCIMobility.h"
//#include "TraCIMobility.h"
#include "veins/base/modules/BaseMobility.h"

#include <sstream>
#include <string>
#include <iostream>
#include <cstddef>


using std::cout;



const simsignalwrap_t BaseWaveApplLayerDTNRSU::mobilityStateChangedSignal = simsignalwrap_t(MIXIM_SIGNAL_MOBILITY_CHANGE_NAME);

void BaseWaveApplLayerDTNRSU::initialize(int stage) {
	BaseApplLayer::initialize(stage);

	if (stage==0) {
		myMac = FindModule<WaveAppToMac1609_4Interface*>::findSubModule(
		            getParentModule());
		assert(myMac);

		myId = getParentModule()->getIndex();

		nbPacketsSent = 0;
		sentMessage = false;

		headerLength = par("headerLength").longValue();
		double maxOffset = par("maxOffset").doubleValue();
		sendBeacons = par("sendBeacons").boolValue();
		beaconLengthBits = par("beaconLengthBits").longValue();
		beaconPriority = par("beaconPriority").longValue();

		sendData = par("sendData").boolValue();
		dataLengthBits = par("dataLengthBits").longValue();
		dataOnSch = par("dataOnSch").boolValue();
		dataPriority = par("dataPriority").longValue();

		nbcopy = par ("nbcopy").longValue();

        nextpos = 0;
        verifNeighExist = false;
        verif = false;
        moitie = 0;
        positionvoisin = 0;
        nbcopyfinalvoisin = 0;

        ackLengthBits = par("ackLengthBits").longValue();
        ackPriority = par("ackPriority").longValue();

		sendBeaconEvt = new cMessage("beacon evt", SEND_BEACON_EVT);



		//simulate asynchronous channel access
		double offSet = dblrand() * (par("beaconInterval").doubleValue()/2);
		offSet = offSet + floor(offSet/0.050)*0.050;
		individualOffset = dblrand() * maxOffset;

		findHost()->subscribe(mobilityStateChangedSignal, this);

		if (sendBeacons) {
			scheduleAt(simTime() + offSet, sendBeaconEvt);
		}

	}
}


WaveShortMessage*  BaseWaveApplLayerDTNRSU::prepareWSM(std::string name, int lengthBits, t_channel channel, int priority, int rcvId, int serial, float nbcopysaved) {

	WaveShortMessage* wsm =		new WaveShortMessage(name.c_str());
	wsm->addBitLength(headerLength);
	wsm->addBitLength(lengthBits);

	switch (channel) {
		case type_SCH: wsm->setChannelNumber(Channels::SCH1); break; //will be rewritten at Mac1609_4 to actual Service Channel. This is just so no controlInfo is needed
		case type_CCH: wsm->setChannelNumber(Channels::CCH); break;
	}
	wsm->setPsid(0);
	wsm->setPriority(priority);
	wsm->setWsmVersion(1);
	wsm->setTimestamp(simTime());
	wsm->setSenderAddress(myId);
	wsm->setRecipientAddress(rcvId);
	wsm->setSenderPos(curPosition);
	wsm->setSerial(serial);
	wsm->setNbcopysaved(nbcopysaved);



	if (name == "beacon") {
		DBG << "Creating Beacon with Priority " << priority << " at Applayer at " << wsm->getTimestamp() << std::endl;

	    std::string s = std::to_string(myId);
        wsm->setWsmData(s.c_str());



	}
	if (name == "data") {
		DBG << "Creating Data with Priority " << priority << " at Applayer at " << wsm->getTimestamp() << std::endl;
	}
	if (name == "ack") {
          
	    }

	return wsm;
}

void BaseWaveApplLayerDTNRSU::receiveSignal(cComponent* source, simsignal_t signalID, cObject* obj) {
	Enter_Method_Silent();
	if (signalID == mobilityStateChangedSignal) {
		handlePositionUpdate(obj);
	}
}

void BaseWaveApplLayerDTNRSU::handlePositionUpdate(cObject* obj) {
	ChannelMobilityPtrType const mobility = check_and_cast<ChannelMobilityPtrType>(obj);
	curPosition = mobility->getCurrentPosition();
}

void BaseWaveApplLayerDTNRSU::handleLowerMsg(cMessage* msg) {

	WaveShortMessage* wsm = dynamic_cast<WaveShortMessage*>(msg);
	ASSERT(wsm);

	EV <<" The type of the received message is:  " << wsm->getName() << "'\n";

	if (std::string(wsm->getName()) == "beacon") {
		onBeacon(wsm);
	}
	else if (std::string(wsm->getName()) == "data") {
	    onData(wsm);
	}
	else if (std::string(wsm->getName()) == "ack") {

	    onACK(wsm);

	   	}
	else {
		DBG << "unknown message (" << wsm->getName() << ")  received\n";
	}
	delete(msg);
}

void BaseWaveApplLayerDTNRSU::handleSelfMsg(cMessage* msg) {
	switch (msg->getKind()) {
		case SEND_BEACON_EVT: {
		    EV << "11111111111111111111111111111111111  '\n";
			sendWSM(prepareWSM("beacon", beaconLengthBits, type_CCH, beaconPriority, 0, -1, 0));
			scheduleAt(simTime() + par("beaconInterval").doubleValue(), sendBeaconEvt);
			break;
		}
		
		case SEND_DATA_TIMER: {
		    EV << "333333333333333333333333333333333333333 \n";
		    std::cerr << "send data timer";
		    EV << "SEND DATA TIMER\n";
		    WaveShortMessage* wsm = new WaveShortMessage();

		    std::string donnee = std::to_string(myId);
		    wsm->setWsmData(donnee.c_str());
		    sendMessage(wsm->getWsmData());


			break;
		}
		
		default: {
			if (msg)
				DBG << "APP: Error: Got Self Message of unknown kind! Name: " << msg->getName() << endl;
			break;
		}
	}
}

void BaseWaveApplLayerDTNRSU::sendMessage(std::string donnee) {
	
	debugEV<< "Sending data packet!\n"<< donnee << "'\n";
	
	sentMessage = true;

	t_channel channel = dataOnSch ? type_SCH : type_CCH;


	WaveShortMessage* wsm = prepareWSM("data", dataLengthBits, channel, dataPriority, -1, 2, 0);


	     wsm->setWsmData(donnee.c_str());
	 
         sendWSM(wsm);

   

}


void BaseWaveApplLayerDTNRSU::sendWSM(WaveShortMessage* wsm) {
	sendDelayedDown(wsm,individualOffset);
}

void BaseWaveApplLayerDTNRSU::finish() {
	if (sendBeaconEvt->isScheduled()) {
		cancelAndDelete(sendBeaconEvt);
	}
	else {
		delete sendBeaconEvt;
	}

	findHost()->unsubscribe(mobilityStateChangedSignal, this);

}

BaseWaveApplLayerDTNRSU::~BaseWaveApplLayerDTNRSU() {

}
