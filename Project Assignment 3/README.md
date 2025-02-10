OVERVIEW

Drone ALPHA is an autonomous drone navigation system designed for the Turkish Provincial Logistics Authority (TPLA).
The drone’s mission is to explore and deliver supplies across Türkiye, visiting as many provinces as possible without exceeding fuel limits.
The project leverages Stack and Queue data structures to efficiently manage exploration and backtracking,
ensuring that the drone maximizes the number of provinces it can visit while avoiding dead-ends and revisits.


KEY FEATURES

Stack and Queue Implementation:
  Stack: Used for backtracking when the drone reaches a dead-end, retracing its steps efficiently.
  Queue: Manages neighboring provinces for exploration in a First In, First Out (FIFO) manner.
  
Route Exploration:
  The drone starts from Ankara and explores neighboring provinces within a specified fuel range (250 km).
  The drone avoids revisiting provinces, ensuring a unique route.
  
Distance Constraints:
  The drone must refuel every 250 km, and only reachable provinces within this distance are considered for the next step.
  
Priority Provinces:
  Specific provinces marked as high-priority are given precedence during the journey.

Weather Restrictions:
  Weather-restricted provinces are avoided to ensure a safe and efficient journey.

Route Summary:
  Outputs the total number of provinces visited, distance covered, and the sequence of visited provinces.
