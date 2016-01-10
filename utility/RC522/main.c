/* Copyright 2015 Henorvell Ge
 *
 * This file is a part of CampusExpBot
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>
 */
#include "RFID.h"

#include <bcm2835.h>
#include <stdlib.h>
#include <stdio.h>

void RFID_init()
{
	bcm2835_spi_begin();
	bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST); 
	bcm2835_spi_setDataMode(BCM2835_SPI_MODE0);
	bcm2835_spi_setClockDivider(2); 
	bcm2835_spi_chipSelect(BCM2835_SPI_CS0); 
	bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS0, LOW);
	writeMFRC522(CommandReg, PCD_RESETPHASE); 
	writeMFRC522(TModeReg, 0x8D);//Tauto=1; f(Timer) = 6.78MHz/TPreScaler
	writeMFRC522(TPrescalerReg, 0x3E);//TModeReg[3..0] + TPrescalerReg
	writeMFRC522(TReloadRegL, 30);
	writeMFRC522(TReloadRegH, 0);
	writeMFRC522(TxAutoReg, 0x40);//100%ASK
	writeMFRC522(ModeReg, 0x3D);// CRC valor inicial de 0x6363
	antennaOn();
}
void writeMFRC522(unsigned char Address, unsigned char value)
{
	char buff[2];
	buff[0] = (char)((Address<<1)&0x7E);
	buff[1] = (char)value;
	bcm2835_spi_transfern(buff,2); 
}

unsigned char readMFRC522(unsigned char Address)
{
    char buff[2];
    buff[0] = ((Address<<1)&0x7E)|0x80;
    bcm2835_spi_transfern(buff,2);
    return (uint8_t)buff[1];
}

void setBitMask(unsigned char reg, unsigned char mask)
{
	unsigned char tmp;
	tmp = readMFRC522(reg);
	writeMFRC522(reg, tmp | mask);  // set bit mask
}

void clearBitMask(unsigned char reg, unsigned char mask)
{
	unsigned char tmp;
	tmp = readMFRC522(reg);
	writeMFRC522(reg, tmp & (~mask));  // clear bit mask
}

void antennaOn()
{
	unsigned char temp;
	temp = readMFRC522(TxControlReg);
	if (!(temp & 0x03))
	{
		setBitMask(TxControlReg, 0x03);
	}
}

void antennaOff()
{
	unsigned char temp;
	temp = readMFRC522(TxControlReg);
	if (!(temp & 0x03))
	{
		clearBitMask(TxControlReg, 0x03);
	}
}

void calculateCRC(unsigned char *pIndata, unsigned char len, unsigned char *pOutData)
{
	unsigned char i, n;
	clearBitMask(DivIrqReg, 0x04);//CRCIrq = 0
	setBitMask(FIFOLevelReg, 0x80);//clean FIFO pointer
	writeMFRC522(CommandReg, PCD_IDLE);
	for (i=0; i<len; i++)
		writeMFRC522(FIFODataReg, *(pIndata+i)); //write to FIFO
	writeMFRC522(CommandReg, PCD_CALCCRC);
	//wait
	i = 0xFF;
	do{
		n = readMFRC522(DivIrqReg);
		i--;
	}while ((i!=0) && !(n&0x04));//CRCIrq = 1
	//Read CRC calc result
	pOutData[0] = readMFRC522(CRCResultRegL);
	pOutData[1] = readMFRC522(CRCResultRegM);
}

unsigned char MFRC522ToCard(unsigned char command, unsigned char *sendData, unsigned char sendLen, unsigned char *backData, unsigned int *backLen)
{
	unsigned char status = MI_ERR;
	unsigned char irqEn = 0x00;
	unsigned char waitIRq = 0x00;
	unsigned char lastBits;
	unsigned char n;
	unsigned int i;
	switch (command){
	case PCD_AUTHENT:   //Auth Key
		irqEn = 0x12;
		waitIRq = 0x10;
		break;
	case PCD_TRANSCEIVE://Send FIFO Data
		irqEn = 0x77;
		waitIRq = 0x30;
		break;
	default:
		break;
	}
	writeMFRC522(CommIEnReg, irqEn|0x80); //allow int request
	clearBitMask(CommIrqReg, 0x80);       //clear all int request
	setBitMask(FIFOLevelReg, 0x80);       //FlushBuffer=1, FIFO init
	writeMFRC522(CommandReg, PCD_IDLE);   //no action, cancel action
	for (i=0; i<sendLen; i++)
		writeMFRC522(FIFODataReg, sendData[i]);//write Data to FIFO
	writeMFRC522(CommandReg, command);
	if (command == PCD_TRANSCEIVE)
		setBitMask(BitFramingReg, 0x80);//StartSend=1,transmission of data starts
	//wait to finish
	i = 2000; //max wait time is 25ms.
	do{
		//CommIrqReg[7..0]
		//Set1 TxIRq RxIRq IdleIRq HiAlerIRq LoAlertIRq ErrIRq TimerIRq
		n = readMFRC522(CommIrqReg);
		i--;
	}while ((i!=0) && !(n&0x01) && !(n&waitIRq));
	clearBitMask(BitFramingReg, 0x80);//StartSend=0
	if (i != 0){
		if(!(readMFRC522(ErrorReg) & 0x1B)){//BufferOvfl Collerr CRCErr ProtecolErr
			status = MI_OK;
			if (n & irqEn & 0x01)
				status = MI_NOTAGERR;
			if (command == PCD_TRANSCEIVE){
				n = readMFRC522(FIFOLevelReg);
				lastBits = readMFRC522(ControlReg) & 0x07;
				if (lastBits)
					*backLen = (n-1)*8 + lastBits;
				else
					*backLen = n*8;
				if (n == 0)
					n = 1;
				if (n > MAX_LEN)
					n = MAX_LEN;
				for (i=0; i<n; i++)
					backData[i] = readMFRC522(FIFODataReg);
			}
		}
		else
			status = MI_ERR;
	}
	//SetBitMask(ControlReg,0x80);           //timer stops
	//Write_MFRC522(CommandReg, PCD_IDLE);
	return status;
}

unsigned char findCard(unsigned char reqMode, unsigned char *TagType)
{
	unsigned char status;
	unsigned int backBits;      //captured length of Bits
	writeMFRC522(BitFramingReg, 0x07);    //TxLastBists = BitFramingReg[2..0]
	TagType[0] = reqMode;
	status = MFRC522ToCard(PCD_TRANSCEIVE, TagType, 1, TagType, &backBits);
	if ((status != MI_OK) || (backBits != 0x10))
		status = MI_ERR;
	return status;
}

unsigned char anticoll(unsigned char *serNum)
{
	unsigned char status;
	unsigned char i;
	unsigned char serNumCheck=0;
	unsigned int unLen;
	clearBitMask(Status2Reg, 0x08);   //TempSensclear
	clearBitMask(CollReg,0x80);     //ValuesAfterColl
	writeMFRC522(BitFramingReg, 0x00);    //TxLastBists = BitFramingReg[2..0]
	serNum[0] = PICC_ANTICOLL;
	serNum[1] = 0x20;
	status = MFRC522ToCard(PCD_TRANSCEIVE, serNum, 2, serNum, &unLen);
	if (status == MI_OK){
    	//check card ID
		for (i=0; i<4; i++){
			*(serNum+i)  = serNum[i];
			serNumCheck ^= serNum[i];
		}
	if (serNumCheck != serNum[i]){
		status = MI_ERR;
	}
	}
	setBitMask(CollReg, 0x80);    //ValuesAfterColl=1
	return status;
}
unsigned char RFID_auth(unsigned char authMode, unsigned char BlockAddr, unsigned char *Sectorkey, unsigned char *serNum)
{
	unsigned char status;
	unsigned int recvBits;
	unsigned char i;
	unsigned char buff[12];
	//check command+block address+sector password+ card ID
	buff[0] = authMode;
	buff[1] = BlockAddr;
	for (i=0; i<6; i++)
		buff[i+2] = *(Sectorkey+i);
	for (i=0; i<4; i++)
		buff[i+8] = *(serNum+i);
	status = MFRC522ToCard(PCD_AUTHENT, buff, 12, buff, &recvBits);
	if ((status != MI_OK) || (!(readMFRC522(Status2Reg) & 0x08)))
		status = MI_ERR;
	return status;
}

unsigned char RFID_read(unsigned char blockAddr, unsigned char *recvData)
{
	unsigned char status;
	unsigned int unLen;
	recvData[0] = PICC_READ;
	recvData[1] = blockAddr;
	calculateCRC(recvData,2, &recvData[2]);
	status = MFRC522ToCard(PCD_TRANSCEIVE, recvData, 4, recvData, &unLen); 
	if ((status != MI_OK) || (unLen != 0x90))
	status = MI_ERR;
	return status;
}

unsigned char RFID_write(unsigned char blockAddr, unsigned char *writeData)
{
	unsigned char status;
	unsigned int recvBits;
	unsigned char i;
	unsigned char buff[18];
	buff[0] = PICC_WRITE;
	buff[1] = blockAddr;
	calculateCRC(buff, 2, &buff[2]);
	status = MFRC522ToCard(PCD_TRANSCEIVE, buff, 4, buff, &recvBits);
	if ((status != MI_OK) || (recvBits != 4) || ((buff[0] & 0x0F) != 0x0A))
		status = MI_ERR;
	if (status == MI_OK){
		for (i=0; i<16; i++)
		buff[i] = *(writeData+i);
		calculateCRC(buff, 16, &buff[16]);
		status = MFRC522ToCard(PCD_TRANSCEIVE, buff, 18, buff, &recvBits);
		if ((status != MI_OK) || (recvBits != 4) || ((buff[0] & 0x0F) != 0x0A))
			status = MI_ERR;
	}
	return status;
}
unsigned char RFID_selectTag(unsigned char *serNum)
{
	unsigned char i;
	unsigned char status;
	unsigned char size;
	unsigned int recvBits;
	unsigned char buffer[9];
	buffer[0] = PICC_SElECTTAG;
	buffer[1] = 0x70;
	for (i=0; i<5; i++)
		buffer[i+2] = *(serNum+i);
	calculateCRC(buffer, 7, &buffer[7]);
	status = MFRC522ToCard(PCD_TRANSCEIVE, buffer, 9, buffer, &recvBits);
	if ((status == MI_OK) && (recvBits == 0x18))
		size = buffer[i];
	else
		size = 0;
	return size;
}

unsigned char RFID_halt()
{
	unsigned char status;
	unsigned int unLen;
	unsigned char buff[4];
	buff[0] = PICC_HALT;
	buff[1] = 0;
	calculateCRC(buff, 2, &buff[2]);
	status = MFRC522ToCard(PCD_TRANSCEIVE, buff, 4, buff,&unLen);
	return status;
}

