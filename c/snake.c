#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

// Constants for the game size
#define WIDTH 40
#define HEIGHT 20
#define DELAY 100000 // Microseconds

enum Direction { UP, DOWN, LEFT, RIGHT };

typedef struct SnakeSegment {
    int x, y;
    struct SnakeSegment *next;
} SnakeSegment;

int score = 0;

// Initialize the game 
void initGame() {
    srand(time(0));
}

// End the game and clean up
void endGame() {
    endwin(); // End ncurses mode
    printf("Game Over! Thanks for playing. Your score was %d.\n", score);
}

// Generate random food coordinates
void generateFood(int *foodX, int *foodY) {
    *foodX = rand() % WIDTH;
    *foodY = rand() % HEIGHT;
}

// Move the snake in the specified direction
void moveSnake(SnakeSegment *snake, enum Direction dir) {
    // Move each segment to the position of the one in front of it
    SnakeSegment *current = snake;
    int prevX = current->x;
    int prevY = current->y;
    int prev2X, prev2Y;

    // Move the head
    switch (dir) {
        case UP:
            current->y--;
            break;
        case DOWN:
            current->y++;
            break;
        case LEFT:
            current->x--;
            break;
        case RIGHT:
            current->x++;
            break;
    }

    current = current->next;
    while (current != NULL) {
        prev2X = current->x;
        prev2Y = current->y;
        current->x = prevX;
        current->y = prevY;
        prevX = prev2X;
        prevY = prev2Y;
        current = current->next;
    }
}

// Check collisions walls and self
int checkCollision(SnakeSegment *snake) {
    // wall
    if (snake->x < 0 || snake->x >= WIDTH || snake->y < 0 || snake->y >= HEIGHT) {
        return 1;
    }

    // self
    SnakeSegment *current = snake->next;
    while (current != NULL) {
        if (snake->x == current->x && snake->y == current->y) {
            return 1;
        }
        current = current->next;
    }

    return 0;
}

// add seggment
void growSnake(SnakeSegment *snake) {
    SnakeSegment *current = snake;
    while (current->next != NULL) {
        current = current->next;
    }

    SnakeSegment *newSegment = (SnakeSegment *)malloc(sizeof(SnakeSegment));
    newSegment->x = current->x;
    newSegment->y = current->y;
    newSegment->next = NULL;
    current->next = newSegment;
}

// Check if eat food
int checkFood(SnakeSegment *snake, int foodX, int foodY) {
    if (snake->x == foodX && snake->y == foodY) {
        return 1;
    }
    return 0;
}

int main() {
    int foodX, foodY;
    int gameOver = 0;
    enum Direction dir = RIGHT;

    // Initialize ncurses
    initscr();           // Start ncurses mode
    noecho();            // Don't echo input
    curs_set(FALSE);     // Hide cursor
    timeout(100);        // Non-blocking input
    keypad(stdscr, TRUE);// Enable arrow keys

    // Initialize game components
    SnakeSegment *snake = (SnakeSegment *)malloc(sizeof(SnakeSegment));
    snake->x = WIDTH / 2;
    snake->y = HEIGHT / 2;
    snake->next = NULL;
    
    generateFood(&foodX, &foodY);

    while (!gameOver) {
        // clear();  // Clear the screen

        // Handle user input
        int ch = getch();
        switch (ch) {
            case KEY_UP:
                if (dir != DOWN) dir = UP;
                break;
            case KEY_DOWN:
                if (dir != UP) dir = DOWN;
                break;
            case KEY_LEFT:
                if (dir != RIGHT) dir = LEFT;
                break;
            case KEY_RIGHT:
                if (dir != LEFT) dir = RIGHT;
                break;
        }

        // Move the snake
        moveSnake(snake, dir);

        // Check for collisions
        if (checkCollision(snake)) {
            gameOver = 1;
        }

        // Check if the snake ate the food
        if (checkFood(snake, foodX, foodY)) {
            score++;
            growSnake(snake);
            generateFood(&foodX, &foodY);
        }
        clear();

        // Draw the snake and food
        SnakeSegment *current = snake;
        while (current != NULL) {
            mvprintw(current->y, current->x, "O");
            current = current->next;
        }
        mvprintw(foodY, foodX, "*");

        // Refresh the screen
        refresh();

        // Delay for game speed
        usleep(DELAY);
    }

    // End the game
    endGame();
    free(snake); // Free the snake memory

    return 0;
}