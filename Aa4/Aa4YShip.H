#ifndef __AA4YSHIP_H
#define __AA4YSHIP_H

#include "Aa4Class.H"
#include "Aa4Game.H"


class YShip
{
	private:
   	int x, y;          // y = row
      int xmove, ymove;        // velocities
      int velocity, acc;       // max velocity
      int shields, power;

      int next_move;
      int next_shoot[3];  // 0-basic, 1-advanced, 2-special
      int next_acc;
      int next_wswitch;   // next weapon switch
      int next_power_up, next_shield_up;  // next recharging of power/shields

      long target_num;  // enemy target counter

      void *img;
      int isize;

   public:
   	YShip(void);
      ~YShip(void);
      void Animate(void);
      void InitYShip(Pilot &P);
      void DrawDash(int primary, int secondary, long money);

      friend void CheckE(EShips &E, YShip &Y, Goodies &Gd);
      friend void CheckEP(EProjs &EP, YShip &Y);
      friend void CheckGd(Goodies &Gd, YShip &Y, Pilot &P);
		friend void CheckKybd(YShip &Y, YProjs &YP, EShips &E, Pilot &P);
      friend void Animate(EShips &E, EProjs &EP, YShip &Y);
};

#endif
