# Introduction to Robotics (2023 - 2024)

 Hi, in this repo I will upload my homework at the discipline Introduction to Robotics, a course I take in my junior year at the Faculty of Mathematics and Computer Science, University of Bucharest. Each homework will have the task requirements, the explained code and photos about the task.

 *kind reminder :* dont forget the safety glasses in lab cause you will do this disipline next year as well :)

 I appreciate you for reading this, ty! :)

 # Homework 2

 Task requierment :
  Use a separate potentiometer for controlling each color of the RGB LED: Red, Green, and Blue. This control must leverage digital electronics. Specifically, you need to read the potentiometer’s value with Arduino and then write a mapped value to the LED pins
  
  ![Tema2_poza1](https://github.com/Beselinho/IntroductionToRobotics/assets/116555068/7f217cf9-5b60-4031-8cd1-a6ef086889bd)
  
  ![Tema2_poza2](https://github.com/Beselinho/IntroductionToRobotics/assets/116555068/8bbebcd6-762a-4967-b5c2-3d24ea20cb12)

  
  Link to video : https://www.youtube.com/watch?v=kqCAvHMge0Y

  # Homework 3

  Task requirement:
   Design a control system that simulates a 3-floor elevator using the Arduino
platform. Here are the specific requirements:
• LED Indicators: Each of the 3 LEDs should represent one of the 3 floors. The LED corresponding to the current floor should light up. Additionally,another LED should represent the elevator’s operational state. It should blink when the elevator is moving and remain static when stationary.
• Buttons: Implement 3 buttons that represent the call buttons from the 3 floors. When pressed, the elevator should simulate movement towards the floor after a short interval (2-3 seconds).
• Buzzer: The buzzer should sound briefly during the following scenarios:
– Elevator arriving at the desired floor (something resembling a ”cling”).
– Elevator doors closing and movement (pro tip: split them into 2different sounds)
• State Change & Timers: If the elevator is already at the desired floor, pressing the button for that floor should have no effect. Otherwise, after a button press, the elevator should ”wait for the doors to close” and then ”move” to the corresponding floor. If the elevator is in movement, it should either do nothing or it should stack its decision (get to the first programmed floor, open the doors, wait, close them and then go to the next desired floor).
• Debounce: Remember to implement debounce for the buttons to avoid unintentional repeated button presses.

![Tema3_photo1](https://github.com/Beselinho/IntroductionToRobotics/assets/116555068/d8792c7a-09bb-463f-85e5-1e81549de18c)

![Tema3_photo2](https://github.com/Beselinho/IntroductionToRobotics/assets/116555068/4e5b5f6f-5f66-4e97-b8e9-ce903728b340)

Link to video : https://www.youtube.com/shorts/DgNwwjv3wQo


   # Homework 4

Task requierment :
So, I have a 7 segment-digit display and using a joystick I need to go to from each LED to one of its neighbours. I know which LED I'm standing on because it always blinks. Also I can color it kinda, that means whenever I press the joystick's button the LED should remain on after I leave it, otherwise if the button is not pressed should stay off after I leave it. Aaaand if I don't like my drawing, I can always reset it by holding down the joystick's button for at least 3 second.


 ![Homework_4_photo1](https://github.com/Beselinho/IntroductionToRobotics/assets/116555068/b80df66a-6197-4a1b-acf8-8e70cc7e7552)
 ![Homework_4_photo2](https://github.com/Beselinho/IntroductionToRobotics/assets/116555068/0c001ae1-6320-401d-9dfd-a39883abd2ec)

 Link to video : https://www.youtube.com/watch?v=bFHH3CS5jDM
 


