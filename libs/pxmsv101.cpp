#include <dos.h>
#include <stdlib.h>
#include <string.h>

#include "PXMSV101.H"

static void far *XMSDriver = NULL;
char XMSError;
TMoveParams TempParams;

char XMSObject::XMSInst(void)
{
	asm {
   	mov  ax, 0x4300
      int  0x2F
   }
   if (_AL == 0x80)
   	return 1;
   else
   	return 0;
}

void XMSObject::InitXMS(void)
{
	asm {
   	mov  ax, 0x4310
      int  0x2F
      mov word ptr [XMSDriver], bx
      mov word ptr [XMSDriver+2], es
	}
}

char * XMSObject::VerStr(unsigned w)
{
	unsigned Num1, Num2, Num3;
   char *Str1, *Str2, *Str3;

   Num1 = (w & 0x0F00) >> 8;
   Str1 = new char [Num1/10+2];
   itoa(Num1, Str1, 10);
   Num2 = (w & 0x00F0) >> 4;
   Str2 = new char [Num2/10+2];
   itoa(Num2, Str2, 10);
   Num3 = w & 0x000F;
   Str3 = new char [Num3/10+2];
   itoa(Num3, Str3, 10);
   char *RetStr = strcat(strcat(strcat(Str1, "."), Str2), Str3);
   return RetStr;
}

unsigned XMSObject::GetXMSVersion(void)
{
	asm {
   	mov ah, 0x00
      call [XMSDriver]
   }
   return _AX;
}

char * XMSObject::GetXMSVersionStr(void)
{
	return VerStr(GetXMSVersion());
}

unsigned XMSObject::GetXMSRevision(void)
{
	asm {
   	mov ah, 0x00
      call [XMSDriver]
   }
   return _BX;
}

char * XMSObject::GetXMSRevisionStr(void)
{
	return VerStr(GetXMSRevision());
}

unsigned XMSObject::GetLargestXMSBlock(void)
{
	asm {
   	mov ah, 0x08
      call [XMSDriver]
   }
   return _AX;
}

unsigned XMSObject::GetFreeXMS(void)
{
	asm {
   	mov ah, 0x08
      call [XMSDriver]
   }
   return _DX;
}

char XMSObject::AllocateEMB(unsigned &Handle, unsigned Size)
{
	asm {
   	mov ah, 0x09
      mov dx, Size
      call [XMSDriver]
      les di, Handle
      mov [es:di], dx
      mov XMSError, bl
   }
   return _AL;
}

char XMSObject::ReallocateEMB(unsigned &Handle, unsigned NewSize)
{
	asm {
   	mov ah, 0x0F
      mov bx, NewSize
      les di, Handle
      mov dx, [es:di]
      mov XMSError, bl
   }
   return _AL;
}

char XMSObject::FreeEMB(unsigned Handle)
{
	asm {
   	mov ah, 0x0A
      mov dx, Handle
      call [XMSDriver]
      mov XMSError, bl
   }
   return _AL;
}

char XMSObject::MoveEMB(PMoveParams Params)
{
	asm {
   	push ds
      push es
      mov  ax, ds
      mov  es, ax
      mov  ah, 0x0B
      lds  si, [Params]
      call es:[XMSDriver]        // necessary to work in huge model
      mov XMSError, bl
      pop  es
      pop  ds
   }
   return _AL;
}

char XMSObject::CopyToEMB(void far *SourcePtr, long Length, unsigned DestHandle, long DestOffset)
{
	TempParams.Length       = Length;
   TempParams.SourceHandle = 0;
   TempParams.SourceOffset = long(SourcePtr);
   TempParams.DestHandle   = DestHandle;
   TempParams.DestOffset   = DestOffset;
   return MoveEMB(&TempParams);
}

char XMSObject::CopyFromEMB(unsigned SourceHandle, long SourceOffset, long Length, void far *DestPtr)
{
	TempParams.Length       = Length;
   TempParams.SourceHandle = SourceHandle;
   TempParams.SourceOffset = SourceOffset;
   TempParams.DestHandle   = 0;
   TempParams.DestOffset   = long(DestPtr);
   return MoveEMB(&TempParams);
}

inline char XMSObject::GetXMSError(void)
{
	return XMSError;
}

inline void XMSObject::ClearXMSError(void)
{
	XMSError = 0;
}

char * XMSObject::GetXMSErrorStr(char ErrorNum)
{
	switch (ErrorNum)
   {
   	case 0x00 : return "No error";
      case 0x80 : return "Function not implemented";
      case 0x81 : return "VDISK detected";
      case 0x82 : return "A20 Error";
      case 0xA0 : return "No free XMS";
      case 0xA1 : return "No free handles";
      case 0xA2 : return "Invalid handle";
      case 0xA3 : return "Invalid source handle";
      case 0xA4 : return "Invalid source offset";
      case 0xA5 : return "Invalid destination handle";
      case 0xA6 : return "Invalid destination offset";
      case 0xA7 : return "Invalid move length";
      case 0xA8 : return "Invalid move overlap";
      case 0xA9 : return "Parity error";
      case 0xAA : return "Extended memory block not locked";
      case 0xAB : return "Extended memory block locked";
      case 0xAC : return "Lock count overflow";
      case 0xAD : return "Lock failed";
   	default   : return "Unknown error";
	}
}