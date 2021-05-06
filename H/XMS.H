/* лл XMS.H ллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл */

  int xms_installed(void);
  void xms_init(void);

  unsigned int xms_getversion(void);
  unsigned int xms_getfreemem(void);

  int xms_allocate(int far *handle, unsigned int size);
  int xms_reallocate(int handle, unsigned int newsize);
  int xms_free(int far *handle);

  typedef struct
    {
      unsigned long length;
      unsigned int  sourcehandle;
      unsigned long sourceoffset;
      unsigned int  desthandle;
      unsigned long destoffset;
    } MOVEPARAMS;

  int xms_move(MOVEPARAMS far *params);

/* лллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл */

