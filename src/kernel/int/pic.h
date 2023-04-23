#pragma once

void picAcknowledge(unsigned char nirq){
	if(nirq >= 8){
		portByteOut(picSlaveCommand, picEndInterrupt);
	}
	portByteOut(picMasterCommand, picEndInterrupt);
}

void picRemap(unsigned char ICW2Master, unsigned char ICW2Slave){

	unsigned char maskMaster = portByteIn(picMasterData), maskSlave = portByteIn(picSlaveData);

	portByteOut(picMasterCommand, ICW1Init|ICW1NoICW4);
	portByteOut(picSlaveCommand, ICW1Init|ICW1NoICW4);
	
	portByteOut(picMasterData, ICW2Master);
	portByteOut(picSlaveData, ICW2Slave);
	
	portByteOut(picMasterData, ICW3SlaveExist);
	portByteOut(picSlaveData, ICW3SlaveCascade);

	portByteOut(picMasterData, ICW4_8086);
	portByteOut(picSlaveData, ICW4_8086);

	portByteOut(picMasterData, maskMaster);
	portByteOut(picSlaveData, maskSlave);
}
