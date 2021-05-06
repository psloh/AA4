const char XMSErrNoError             = 0x00;
const char XMSErrNotImplemented      = 0x80;
const char XMSErrVDiskDetected       = 0x81;
const char XMSErrA20Error            = 0x82;
const char XMSErrNoFreeXMS           = 0xA0;
const char XMSErrNoFreeHandles       = 0xA1;
const char XMSErrInvalidHandle       = 0xA2;
const char XMSErrInvalidSourceHandle = 0xA3;
const char XMSErrInvalidSourceOffset = 0xA4;
const char XMSErrInvalidDestHandle   = 0xA5;
const char XMSErrInvalidDestOffset   = 0xA6;
const char XMSErrInvalidLength       = 0xA7;
const char XMSErrInvalidOverlap      = 0xA8;
const char XMSErrParity              = 0xA9;
const char XMSErrBlockNotLocked      = 0xAA;
const char XMSErrHandleLocked        = 0xAB;
const char XMSErrLockCountOverflow   = 0xAC;
const char XMSErrLockFailed          = 0xAD;

struct TMoveParams;
typedef TMoveParams *PMoveParams;
struct TMoveParams
{
	long Length;
   unsigned int SourceHandle;
   long SourceOffset;
   unsigned int DestHandle;
   long DestOffset;
};

class XMSObject
{
 private:
   char * VerStr(unsigned w);
 public:
	char XMSInst(void);
   void InitXMS(void);
   unsigned GetXMSVersion(void);
   char * GetXMSVersionStr(void);
   unsigned GetXMSRevision(void);
	char * GetXMSRevisionStr(void);
   unsigned GetLargestXMSBlock(void);
   unsigned GetFreeXMS(void);
   char AllocateEMB(unsigned &Handle, unsigned Size);
   char ReallocateEMB(unsigned &Handle, unsigned NewSize);
	char MoveEMB(PMoveParams Params);
   char FreeEMB(unsigned Handle);
	char CopyToEMB(void far *SourcePtr, long Length, unsigned DestHandle, long DestOffset);
	char CopyFromEMB(unsigned SourceHandle, long SourceOffset, long Length, void far *DestPtr);
	char GetXMSError(void);
   void ClearXMSError(void);
   char * GetXMSErrorStr(char ErrorNum);
};