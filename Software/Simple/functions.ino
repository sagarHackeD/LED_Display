




// Bouncing ball variables
int ballX = 0;
int ballY = 0;
int ballDirX = 1;
int ballDirY = 1;
int displayWidth = 32; // 4 matrices * 8 pixels
int displayHeight = 8;

void drawBouncingBall() {
  matrix.displayClear();

  // Draw the ball (pixel) using the underlying MAX72xx object
  matrix.getGraphicObject()->setPoint(ballY, ballX, true);
  matrix.getGraphicObject()->update();

  // Update ball position
  ballX += ballDirX;
  ballY += ballDirY;

  // Bounce off walls
  if (ballX >= displayWidth - 1 || ballX <= 0) {
    ballDirX *= -1;
  }
  if (ballY >= displayHeight - 1 || ballY <= 0) {
    ballDirY *= -1;
  }
}