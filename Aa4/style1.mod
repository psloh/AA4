;STYLE 1 MODULE FILE

;projectile description block
NUMBER OF PROJECTILES=5
 ;for each projectile   <1>    "plasma bolt"
 XDIM=5
 YDIM=8
 SPEED=7
 EXPLODE=0
 EXPLOSION RADIUS=0
 HITPOINTS=3

 ;for each projectile  <2>   "fire ball"
 XDIM=7
 YDIM=7
 SPEED=10
 EXPLODE=0
 EXPLOSION RADIUS=0
 HITPOINTS=6

 ;for each projectile  <3>    "mega missile"
 XDIM=5
 YDIM=8
 SPEED=5
 EXPLODE=0
 EXPLOSION RADIUS=0
 HITPOINTS=15

 ;for each projectile  <4>    "plasma ball"
 XDIM=7
 YDIM=7
 SPEED=9
 EXPLODE=0
 EXPLOSION RADIUS=0
 HITPOINTS=8

 ;for each projectile  <5>    "laser ball"
 XDIM=7
 YDIM=7
 SPEED=8
 EXPLODE=0
 EXPLOSION RADIUS=0
 HITPOINTS=10

;path description block
NUMBER OF PATHS=4
 ;for each path
 NUMBER OF POINTS=4
  ;for each point
  COORDINATES=0 100
  COORDINATES=30 100
  COORDINATES=0 200
  COORDINATES=0 300
 ;for each path
 NUMBER OF POINTS=1
  ;for each point
  COORDINATES=0 300
 ;for each path
 NUMBER OF POINTS=1
  ;for each point
  COORDINATES=0 100
 ;for each path
 NUMBER OF POINTS=5
  ;for each point
  COORDINATES=0 50
  COORDINATES=10 70
  COORDINATES=30 80
  COORDINATES=50 100
  COORDINATES=400 120

;enemy description block
NUMBER OF ENEMIES=13
 ;for each enemy
 ;ENEMY <1>                     "fighter A"
 XDIM=16
 YDIM=15
 NUMBER OF PROJECTILES FIRED=1
  ;for each projectile fired
  TYPE=0
  X OFF=0
  Y OFF=0
  THETA=10000
 REFIRE=7
 SOUND NUM=3
 HITPOINTS=5
 SPEED=2
 MONEY=100

 ;for each enemy
 ;ENEMY <2>            "bomber A"
 XDIM=16
 YDIM=15
 NUMBER OF PROJECTILES FIRED=2
  ;for each projectile fired
  TYPE=1
  X OFF=-5
  Y OFF=0
  THETA=10000
  ;for each projectile fired
  TYPE=1
  X OFF=5
  Y OFF=0
  THETA=10000
 REFIRE=10
 SOUND NUM=4
 HITPOINTS=10
 SPEED=2
 MONEY=200

 ;for each enemy
 ;ENEMY <3>            "Ground turret A"
 XDIM=16
 YDIM=15
 NUMBER OF PROJECTILES FIRED=1
  ;for each projectile fired
  TYPE=1
  X OFF=0
  Y OFF=0
  THETA=10000
 REFIRE=14
 SOUND NUM=4
 HITPOINTS=20
 SPEED=-1
 MONEY=400

 ;for each enemy
 ;ENEMY <4>            "battleship A"
 XDIM=32
 YDIM=30
 NUMBER OF PROJECTILES FIRED=8
  ;for each projectile fired
  TYPE=1
  X OFF=0
  Y OFF=0
  THETA=-45
  ;for each projectile fired
  TYPE=1
  X OFF=0
  Y OFF=0
  THETA=225
  ;for each projectile fired
  TYPE=1
  X OFF=-10
  Y OFF=-10
  THETA=270
  ;for each projectile fired
  TYPE=1
  X OFF=10
  Y OFF=-10
  THETA=270
  ;for each projectile fired
  TYPE=1
  X OFF=10
  Y OFF=10
  THETA=0
  ;for each projectile fired
  TYPE=1
  X OFF=-10
  Y OFF=10
  THETA=180
  ;for each projectile fired
  TYPE=0
  X OFF=-9
  Y OFF=0
  THETA=270
  ;for each projectile fired
  TYPE=0
  X OFF=9
  Y OFF=0
  THETA=270
 REFIRE=14
 SOUND NUM=3
 HITPOINTS=200
 SPEED=4
 MONEY=1000


 ;for each enemy
 ;ENEMY <5>            "missile turret A"
 XDIM=16
 YDIM=15
 NUMBER OF PROJECTILES FIRED=1
  ;for each projectile fired
  TYPE=2
  X OFF=0
  Y OFF=0
  THETA=270
 REFIRE=14
 SOUND NUM=3
 HITPOINTS=20
 SPEED=-1
 MONEY=500

 ;for each enemy
 ;ENEMY <6>            "bomber B"
 XDIM=16
 YDIM=15
 NUMBER OF PROJECTILES FIRED=1
  ;for each projectile fired
  TYPE=2
  X OFF=0
  Y OFF=0
  THETA=270
 REFIRE=14
 SOUND NUM=3
 HITPOINTS=18
 SPEED=3
 MONEY=600

 ;for each enemy
 ;ENEMY <7>            "government building A"
 XDIM=32
 YDIM=30
 NUMBER OF PROJECTILES FIRED=0
 REFIRE=100
 SOUND NUM=3
 HITPOINTS=15
 SPEED=-1
 MONEY=1000

 ;for each enemy
 ;ENEMY <8>            "fighter B"
 XDIM=16
 YDIM=15
 NUMBER OF PROJECTILES FIRED=1
  ;for each projectile fired
  TYPE=3
  X OFF=0
  Y OFF=0
  THETA=10000
 REFIRE=14
 SOUND NUM=3
 HITPOINTS=10
 SPEED=6
 MONEY=800

 ;for each enemy
 ;ENEMY <9>            "asteroid A"
 XDIM=16
 YDIM=15
 NUMBER OF PROJECTILES FIRED=0
 REFIRE=100
 SOUND NUM=3
 HITPOINTS=20
 SPEED=2
 MONEY=100

 ;for each enemy
 ;ENEMY <10>            "fighter C"
 XDIM=16
 YDIM=15
 NUMBER OF PROJECTILES FIRED=2
  ;for each projectile fired
  TYPE=0
  X OFF=0
  Y OFF=0
  THETA=270
  ;for each projectile fired
  TYPE=3
  X OFF=0
  Y OFF=0
  THETA=10000
 REFIRE=14
 SOUND NUM=3
 HITPOINTS=10
 SPEED=5
 MONEY=900

 ;for each enemy
 ;ENEMY <11>            "ground turret B"
 XDIM=16
 YDIM=15
 NUMBER OF PROJECTILES FIRED=1
  ;for each projectile fired
  TYPE=3                      
  X OFF=0
  Y OFF=0
  THETA=10000
 REFIRE=14
 SOUND NUM=3
 HITPOINTS=25
 SPEED=-1
 MONEY=800

 ;for each enemy
 ;ENEMY <12>            "Warship A"
 XDIM=16
 YDIM=15
 NUMBER OF PROJECTILES FIRED=2
  ;for each projectile fired
  TYPE=0
  X OFF=-2
  Y OFF=0
  THETA=10000
  ;for each projectile fired
  TYPE=1
  X OFF=3
  Y OFF=0
  THETA=10000
 REFIRE=20
 SOUND NUM=4
 HITPOINTS=30
 SPEED=-2
 MONEY=1000

 ;for each enemy
 ;ENEMY <13>            "submarine A"
 XDIM=32
 YDIM=15
 NUMBER OF PROJECTILES FIRED=1
  ;for each projectile fired
  TYPE=4
  X OFF=-10
  Y OFF=0
  THETA=10000
 REFIRE=20
 SOUND NUM=3
 HITPOINTS=30
 SPEED=-2
 MONEY=1100

EOF.
