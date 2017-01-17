#pragma once
#include "B4RDefines.h"
#include "MFRC522.h"
//~version: 1.00
//~dependson: <SPI.h>
//~event: CardPresent (UID() As Byte, CardType As Byte)
namespace B4R {
	typedef void (*SubVoidArrayByte)(Array* barray, Byte type) ;
	//~shortname: MFRC522
	class B4MFRC522 {
		private:
			MFRC522 rfid;
			SubVoidArrayByte CardPresentSub;
			static void looper(void* b);
		public:
			//Initializes the object.
			//CSPin - Chip Select pin (also called SS).
			//ResetPin - Reset pin.
			//CardPresentSub - Sub that will handle the CardPresent event.
			void Initialize(Byte CSPin, Byte ResetPin, SubVoidArrayByte CardPresentSub);
			#define /*Byte TYPE_UNKNOWN;*/ 		B4MFRC522_TYPE_UNKNOWN 0
			//PICC compliant with ISO/IEC 14443-4 
			#define /*Byte TYPE_ISO_14443_4;*/ 	B4MFRC522_TYPE_ISO_14443_4 1
			//PICC compliant with ISO/IEC 18092 (NFC)
			#define /*Byte TYPE_ISO_18092;*/ 		B4MFRC522_TYPE_ISO_18092 2
			//MIFARE Classic protocol, 320 bytes
			#define /*Byte TYPE_MIFARE_MINI;*/ 		B4MFRC522_TYPE_MIFARE_MINI 3
			//MIFARE Classic protocol, 1KB
			#define /*Byte TYPE_MIFARE_1K;*/ 		B4MFRC522_TYPE_MIFARE_1K 4
			//MIFARE Classic protocol, 4KB
			#define /*Byte TYPE_MIFARE_4K;*/ 		B4MFRC522_TYPE_MIFARE_4K 5
			//MIFARE Ultralight or Ultralight C
			#define /*Byte TYPE_MIFARE_UL;*/ 		B4MFRC522_TYPE_MIFARE_UL 6
			//MIFARE Plus
			#define /*Byte TYPE_MIFARE_PLUS;*/ 		B4MFRC522_TYPE_MIFARE_PLUS 7
			//Only mentioned in NXP AN 10833 MIFARE Type Identification Procedure
			#define /*Byte TYPE_TNP3XXX;*/ 		B4MFRC522_TYPE_TNP3XXX 8
			#define /*Byte TYPE_NOT_COMPLETE;*/ 		B4MFRC522_TYPE_NOT_COMPLETE 0xff
			//Prints the firmware version.
			void LogVersion();
			//Returns true if the current card is a Mifare card.
			bool getIsMifare();
			/**
			*Gains access to the specified block. Uses the default key value with key A.
			*Returns True if successful.
			*/
			bool MifareAuthenticate(Byte BlockAddress);
			/**
			*Gains access to the specified block.
			*BlockAddress - Block index.
			*Key - Array of 6 bytes.
			*KeyA - If true then key A will be used. Otherwise key B will be used.
			*/
			bool MifareAuthenticate2(Byte BlockAddress, ArrayByte* Key, bool KeyA);
			/**
			*Reads the data from the specified block. Returns the number of bytes read.
			*You should first call MifareAuthenticate.
			*BlockAddress - Block index.
			*Buffer - Data will be written to this buffer. Must be 18 bytes or more.
			*The last two bytes are the checksum.
			*/
			Byte MifareRead(Byte BlockAddress, ArrayByte* Buffer);
			/**
			*Writes data to the specified block. Returns true if writing was successful.
			*You should first call MifareAuthenticate.
			*BlockAddress - Block index.
			*Buffer - Data to write. 16 bytes will be written (array length must be 16 or more).
			*/
			bool MifareWrite (Byte BlockAddress, ArrayByte* Buffer);
	};
}