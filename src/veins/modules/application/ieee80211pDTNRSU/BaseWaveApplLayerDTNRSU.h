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

#ifndef BASEWAVEAPPLLAYERDTNRSU_H_
#define BASEWAVEAPPLLAYERDTNRSU_H_

#include <map>
#include "veins/base/modules/BaseApplLayer.h"
#include "veins/modules/utility/Consts80211p.h"
#include "veins/modules/messages/WaveShortMessage_m.h"
#include "veins/base/connectionManager/ChannelAccess.h"
#include "veins/modules/mac/ieee80211p/WaveAppToMac1609_4Interface.h"
#include <string>

#ifndef DBG
#define DBG EV
#endif
//#define DBG std::cerr << "[" << simTime().raw() << "] " << getParentModule()->getFullPath() << " "

/**
 * @brief
 * WAVE application layer base class.
 *
 * @author David Eckhoff
 *
 * @ingroup applLayer
 *
 * @see BaseWaveApplLayer
 * @see Mac1609_4
 * @see PhyLayer80211p
 * @see Decider80211p
 */


class BaseWaveApplLayerDTNRSU : public BaseApplLayer {

	public:
		~BaseWaveApplLayerDTNRSU();
		virtual void initialize(int stage);
		virtual void finish();

		virtual  void receiveSignal(cComponent* source, simsignal_t signalID, cObject* obj);
		//virtual void sendMessage(std::string myId);


		enum WaveApplMessageKinds {
			SERVICE_PROVIDER = LAST_BASE_APPL_MESSAGE_KIND,
			SEND_BEACON_EVT,
			SEND_DATA_TIMER
		};




		  static const int taille = 30;
		  std::string T [taille] = {};


static const int tailletab = 100;
int TAB [tailletab] ={};
std::string ssss ="";
std::string numbre2 ="";
std::string number3 ="";
std::string res ="";
std::string data ="";

std::string ss ="";
char abc[255];
int var ;

int neighboorsDelay = 2;
int newid, senderid;
 int nbcopy;
 int nextpos;
 unsigned int i, j, p;
 int lastpos = -1;
 int dernierepos = -1;
 int voisin = -1;
 int indice = -1;
 std::string msg = "";

 std::string donnee = "";
 std::string donneee = "";


 std::string message = "";
 //std::vector<string> message;

 std::string yos = "yosra";
 bool verifNeighExist;
bool verif;
bool reponse;
float moitie, nbcopyfinalvoisin;
float truncnbcopyfinalvoisin, nb;
int positionvoisin;
std::string msgrecu = "";
int nbcopyrecu;
bool existe;
bool stop;
bool stopdata;

int testid;
std::string chaine ="";

 char aa ;
 simtime_t temps;
 float importnbcopyfinal;
 int firstfree = -1;
 bool found = false;
 int idPaquet = 1;



	protected:

		static const simsignalwrap_t mobilityStateChangedSignal;

		/** @brief handle messages from below */
		virtual void handleLowerMsg(cMessage* msg);
		/** @brief handle self messages */
		virtual void handleSelfMsg(cMessage* msg);

		// virtual WaveShortMessage* prepareWSM(std::string name, int dataLengthBits, t_channel channel, int priority, int rcvId, int serial=0);
        virtual WaveShortMessage* prepareWSM(std::string name, int dataLengthBits, t_channel channel, int priority, int rcvId, int serial=0, float nbcopysaved= 0);

		virtual void sendWSM(WaveShortMessage* wsm);
		virtual void onBeacon(WaveShortMessage* wsm) = 0;
		virtual void onData(WaveShortMessage* wsm) = 0;
		virtual void onACK(WaveShortMessage* wsm) = 0;

		virtual void handlePositionUpdate(cObject* obj);
		virtual void sendMessage(std::string myId);
	protected:
		int beaconLengthBits;
		int beaconPriority;
		int ackLengthBits;
		int ackPriority;
		bool sendData;
		bool sendBeacons;
		simtime_t individualOffset;
		int dataLengthBits;
		bool dataOnSch;
		int dataPriority;
		Coord curPosition;
		int mySCH;
		int myId;
        double a;
        double b ;
        double c ;

        long nbPacketsSent;
      //  std::string donnee = "";
		bool sentMessage ;
        simtime_t tempsenvoie;

		cMessage* sendBeaconEvt;
        cMessage * delayTimer;

		WaveAppToMac1609_4Interface* myMac;

	//	 std::string buffer[20];
	//	 int i;



};

#endif /* BASEWAVEAPPLLAYER_H_ */
