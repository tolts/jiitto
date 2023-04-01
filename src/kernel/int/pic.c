#pragma once

void picAcknowledge(unsigned char nirq){
	if(nirq >= 8){
		portByteOut(picSlaveCommand, picEndInterrupt);
		portWait();
	}
	portByteOut(picMasterCommand, picEndInterrupt);
	portWait();
}

void picDisable(void){
	portByteOut(picMasterData, picDisable_);
	portWait();
	portByteOut(picSlaveData, picDisable_);
	portWait();
}

void picSetMask(unsigned char nirq){
    unsigned char port, value;
    if(nirq < 8){
        port = picMasterData;
    }else{
        port = picSlaveData;
        nirq -= 8;
    }
    value = portByteIn(port) | (1 << nirq);
    portByteOut(port, value);
}
 
void picClearMask(unsigned char nirq) {
    unsigned char port, value;
    if(nirq < 8){
        port = picMasterData;
    }else{
        port = picSlaveData;
        nirq -= 8;
    }
    value = portByteIn(port) & ~(1 << nirq);
    portByteOut(port, value); 
}

void picRemap(unsigned char ICW2Master, unsigned char ICW2Slave){

	unsigned char maskMaster = portByteIn(picMasterData), maskSlave = portByteIn(picSlaveData);

	portByteOut(picMasterCommand, ICW1Init|ICW1NoICW4);
	portWait();
	portByteOut(picSlaveCommand, ICW1Init|ICW1NoICW4);
	portWait();
	
	portByteOut(picMasterData, ICW2Master);
	portWait();
	portByteOut(picSlaveData, ICW2Slave);
	portWait();
	
	portByteOut(picMasterData, ICW3SlaveExist);
	portWait();
	portByteOut(picSlaveData, ICW3SlaveCascade);
	portWait();

	portByteOut(picMasterData, ICW4_8086);
	portWait();
	portByteOut(picSlaveData, ICW4_8086);
	portWait();

	portByteOut(picMasterData, maskMaster);
	portWait();
	portByteOut(picSlaveData, maskSlave);
	portWait();
}
