## Program with steering, pathfinding and decision making states.
### Steering:
#### The player controls the blue car, while the red car moves left and right across the screen.
Seek - car moves towards the point where the mouse clicks.<br>
Flee - car moves away from the red car to a certain distance.<br>
Arrive - car slows down to a stop as it approaches the point where the mouse clicks.<br>
Wander - car moves in a random direction.<br>
Obstacle avoidance - car tries to avoid hitting the red car.<br>
Pursuit - steers towards the red car.<br>
### Pathfinding:
The car moves around the track using A* pathfinding. Nodes can be toggled to be visible using the button on the UI.
### Decision making:
The two cars pathfind around the track, but the blue car can overtake the red car if it is behind.

If the two cars collide, they will 'crash' and the blue car's speed will be massively reduced for a few seconds.
