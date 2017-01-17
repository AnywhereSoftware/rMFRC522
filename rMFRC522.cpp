
#include "B4RDefines.h"
namespace B4R {
	void B4MFRC522::Initialize(Byte CSPin, Byte ResetPin, SubVoidArrayByte CardPresentSub) {
		SPI.begin();
		rfid.PCD_Init(CSPin, ResetPin);
		this->CardPresentSub = CardPresentSub;
		FunctionUnion fu;
		fu.PollerFunction = looper;
		pollers.add(fu, this);
	}
	void B4MFRC522::LogVersion() {
		rfid.PCD_DumpVersionToSerial();
	}
	bool B4MFRC522::getIsMifare() {
		Byte piccType = rfid.PICC_GetType(rfid.uid.sak);
		return !(piccType != MFRC522::PICC_TYPE_MIFARE_MINI
        &&  piccType != MFRC522::PICC_TYPE_MIFARE_1K
        &&  piccType != MFRC522::PICC_TYPE_MIFARE_4K);
	}
	bool B4MFRC522::MifareAuthenticate(Byte BlockAddress) {
		ArrayByte ab;
		Byte b[] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
		ab.data = b;
		ab.length = 6;
		return MifareAuthenticate2(BlockAddress, &ab, true);
	}
			
	bool B4MFRC522::MifareAuthenticate2(Byte BlockAddress, ArrayByte* Key, bool KeyA) {
		MFRC522::StatusCode status = rfid.PCD_Authenticate (
		KeyA ? MFRC522::PICC_CMD_MF_AUTH_KEY_A : MFRC522::PICC_CMD_MF_AUTH_KEY_B, 
		BlockAddress,
			(MFRC522::MIFARE_Key*)Key->data, &(rfid.uid));
		return status == MFRC522::STATUS_OK;
	}
	byte B4MFRC522::MifareRead(Byte BlockAddress, ArrayByte* Buffer) {
		Byte length = Buffer->length;
		MFRC522::StatusCode status = rfid.MIFARE_Read (BlockAddress, (Byte*)Buffer->data, &length);
		if (status != MFRC522::STATUS_OK)
			return 0;
		return length;
	}
	bool B4MFRC522::MifareWrite (Byte BlockAddress, ArrayByte* Buffer) {
		return (rfid.MIFARE_Write(BlockAddress, (Byte*)Buffer->data, Common_Min(16, Buffer->length))) == MFRC522::STATUS_OK;
	}
	static UInt lastEvent = 0;
	void B4MFRC522::looper(void* b) {
		B4MFRC522* me = (B4MFRC522*)b;
		if ( ! me->rfid.PICC_IsNewCardPresent())
			return;
		if ( ! me->rfid.PICC_ReadCardSerial())
			return;
		if (lastEvent + 500 > millis())
			return;
		lastEvent = millis();
		const UInt cp = B4R::StackMemory::cp;
		ArrayByte* arr = CreateStackMemoryObject(ArrayByte);
		arr->data = me->rfid.uid.uidByte;
		arr->length = me->rfid.uid.size;
		me->CardPresentSub (arr, me->rfid.PICC_GetType(me->rfid.uid.sak));
		me->rfid.PICC_HaltA();
		me->rfid.PCD_StopCrypto1();
		B4R::StackMemory::cp = cp;
	}
}