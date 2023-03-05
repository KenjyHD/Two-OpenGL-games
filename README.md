# Duck Hunt and Car Racing OpenGL

## Duck Hunt
![](imgs\duck_game.png)
### Description
---
The duck spawns behind the green bush at a random positin with a random orientation. We need to shoot as much dukcs as we can before we run out of lives.
Lives are lost when the duck escapes, which happens every time we miss 3 shots or the duck fly around for about 6 seconds. If we loose all our 3 lives a game over screen appears. Still we can continue our game by pressing space

![](imgs\game_over.png)

There is a score bar that fills up every we shoot a duck. The score we get depends on the duck speed(game difficulty). Once the score bar fills up completely the current game session ends congratulating the player. 

![](imgs\score.png)

We can the continue our game with the score required to fill the bar slightly increased.

![](imgs\congrats.png)

Every time we shoot 5 ducks their speed increseases making it more difficult to shoot them. Also, the duck can take a random direction without hitting the edge of the screen at any moment of time with a cooldown of 5 seconds, confusing the hunter

The number of lives and number of shots left are shown in the top left and right corners respectively. The score bar is displayed below the number of shots left

## Car Racing

![](imgs\car_game.png)

### Description

In this game we drive a car and compete with other cars that move along a predefined path. Upon collision with other cars the main car will stop and won't be able to move until the obstacle leaves the collision radius. 

![](imgs\road.png)

The same happens if the main car will get out of the road; we can continue driving by moving towards the road area(turning and moving forward or just moving backward).

![](imgs\road_collision.png)

To create a illusion of curvature we can use the '-' and '=' keys to increase or decrease the coefficient of curvature

* Negative curvature coefficient view

![](imgs\curvature_negative.png)
* Positive curvature coefficient view

![](imgs\curvature_positive.png)

The keys to move the main car are WASD.

# Switch Games
To switch between the games we need to change in main.cpp between m1:Tema1() for Duck Game and m1:Tema2() for Car Racing

![](imgs\game_switch.png)

# Copyright
The rights to the framework used to develop these two simple games belong to the EGC team, UBP, ACS faculty