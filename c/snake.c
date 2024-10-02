#include <stdio.h>
// #include <conio.h>  // for getch()
#include <stdlib.h> // for system()
#include <time.h>   // for time()
#include <stdbool.h> // for boolean type

#define WIDTH 20
#define HEIGHT 20

// Directions for the snake's movement
enum Direction {UP, DOWN, LEFT, RIGHT};

// Snake structure
typedef struct {
    int x[100]; // X coordinates of snake body
    int y[100]; // Y coordinates of snake body
    int length; // Length of the snake
    enum Direction dir; // Current direction of the snake
} Snake;

// Food structure
typedef struct {
    int x; // X coordinate of food
    int y; // Y coordinate of food
} Food;

// Function prototypes
void initGame(Snake *snake, Food *food);
void drawGame(Snake *snake, Food *food);
void input(Snake *snake);
void logic(Snake *snake, Food *food, int *score);
bool collision(Snake *snake);

int main() {
    Snake snake;
    Food food;
    int score = 0;
    bool gameOver = false;

    initGame(&snake, &food);

    while (!gameOver) {
        drawGame(&snake, &food);
        input(&snake);
        logic(&snake, &food, &score);
        gameOver = collision(&snake);
    }

    printf("Game Over! Your score is: %d\n", score);
    return 0;
}

// Initialize the game
void initGame(Snake *snake, Food *food) {
    snake->length = 1; // Snake starts with length 1
    snake->x[0] = WIDTH / 2; // Start in the middle
    snake->y[0] = HEIGHT / 2; // Start in the middle
    snake->dir = RIGHT; // Initial direction

    // Place food randomly
    srand(time(NULL)); // Seed for random number generation
    food->x = rand() % WIDTH;
    food->y = rand() % HEIGHT;
}

// Draw the game state
void drawGame(Snake *snake, Food *food) {
    system("cls"); // Clear the console (Windows)
    for (int i = 0; i < WIDTH + 2; i++)
        printf("#"); // Draw top wall
    printf("\n");

    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            if (x == 0)
                printf("#"); // Draw left wall

            // Draw the snake
            bool isSnakePart = false;
            for (int j = 0; j < snake->length; j++) {
                if (snake->x[j] == x && snake->y[j] == y) {
                    printf("o"); // Snake part
                    isSnakePart = true;
                    break;
                }
            }

            // Draw food
            if (food->x == x && food->y == y) {
                printf("*"); // Food
            } else if (!isSnakePart) {
                printf(" "); // Empty space
            }

            if (x == WIDTH - 1)
                printf("#"); // Draw right wall
        }
        printf("\n");
    }

    for (int i = 0; i < WIDTH + 2; i++)
        printf("#"); // Draw bottom wall
    printf("\n");
}

// Handle input
void input(Snake *snake) {
    if (_kbhit()) { // Check for keyboard hit
        switch (getchar()) { // Get the key pressed
            case 'w':
                snake->dir = UP;
                break;
            case 's':
                snake->dir = DOWN;
                break;
            case 'a':
                snake->dir = LEFT;
                break;
            case 'd':
                snake->dir = RIGHT;
                break;
        }
    }
}

// Game logic
void logic(Snake *snake, Food *food, int *score) {
    // Move the snake
    int prevX = snake->x[0];
    int prevY = snake->y[0];
    int prev2X, prev2Y;
    snake->x[0] += (snake->dir == RIGHT) - (snake->dir == LEFT);
    snake->y[0] += (snake->dir == DOWN) - (snake->dir == UP);

    for (int i = 1; i < snake->length; i++) {
        prev2X = snake->x[i];
        prev2Y = snake->y[i];
        snake->x[i] = prevX;
        snake->y[i] = prevY;
        prevX = prev2X;
        prevY = prev2Y;
    }

    // Check if the snake has eaten the food
    if (snake->x[0] == food->x && snake->y[0] == food->y) {
        (*score)++;
        snake->length++; // Increase length
        food->x = rand() % WIDTH; // Place new food
        food->y = rand() % HEIGHT;
    }
}

// Check for collisions
bool collision(Snake *snake) {
    // Check wall collisions
    if (snake->x[0] >= WIDTH || snake->x[0] < 0 || 
        snake->y[0] >= HEIGHT || snake->y[0] < 0) {
        return true; // Game over due to wall
    }

    // Check self-collision
    for (int i = 1; i < snake->length; i++) {
        if (snake->x[0] == snake->x[i] && snake->y[0] == snake->y[i]) {
            return true; // Game over due to self-collision
        }
    }

    return false; // No collision
}
