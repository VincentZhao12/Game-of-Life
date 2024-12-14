#include <iostream>
#include <SFML/Graphics.hpp>
#include <ctime>


using namespace std;

class Game {
    public:
        Game(int n, int m) {
            map = new int*[n];

            for (int i = 0; i < n; i++) {
                map[i] = new int[m];
            }

            this->n = n;
            this->m = m;
        }
        
        void addCell(int i, int j) {
            map[i][j] = 1;
        }

        void step() {
            int** copy = new int*[n];

            for (int i = 0; i < n; i++) {
                copy[i] = new int[m];

                for (int j = 0; j < m; j++) {
                    copy[i][j] = map[i][j];
                }
            }

            for (int i = 0; i < n; i++) {
                for (int j = 0; j < m; j++) {
                    int neighbors = 
                        this->get(copy, i - 1, j - 1) +
                        this->get(copy, i - 1, j) +
                        this->get(copy, i - 1, j + 1) +
                        this->get(copy, i + 1, j - 1) +
                        this->get(copy, i + 1, j) +
                        this->get(copy, i + 1, j + 1) +
                        this->get(copy, i, j - 1) + 
                        this->get(copy, i, j + 1);

                    if (neighbors <= 1) {
                        map[i][j] = 0;
                    } else if (neighbors == 3) {
                        map[i][j] = 1;
                    } else if (neighbors == 2) {
                        map[i][j] = copy[i][j];
                    } if (neighbors >= 4) {
                        map[i][j] = 0;
                    }
                }
            }

            for (int i = 0; i < n; i++) {
                delete[] copy[i];
            }
            delete[] copy;
        }

        int getCell(int i, int j) {
            return map[i % n][j % m];
        }

        void printGame() {
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < m; j++) {
                    cout << map[i][j] << " ";
                }
                cout << "\n\n";
            }
        }

        ~Game() {
            for (int i = 0; i < n; i++) {
                delete[] map[i];
            }

            delete[] map;
        }

        void addKRandomCells(int k) {
            while (k > 0) {
                int i = rand() % n;
                int j = rand() % m;

                if (map[i][j] == 0) {
                    k--;
                    map[i][j] = 1;
                }
            }
        }

    private:
        int** map;
        int n, m;

        int get(int** arr, int i, int j) {
            return arr[(i + n) % n][(j + m) % m];
        }
};

int main() {
    srand(time(NULL));
    int window_width = 1000;
    int window_height = 800;
    float cell_size = 20.0f;
    int n = window_height / cell_size;
    int m = window_width / cell_size;

    sf::RenderWindow window(sf::VideoMode(window_width, window_height), "Conways Game of Life");

    Game game(n, m);

    game.addKRandomCells(200);


    sf::Clock clock;
    sf::Time updateInterval = sf::seconds(0.075f);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        if (clock.getElapsedTime() >= updateInterval) {
            clock.restart();
            game.step();
        }

        window.clear(sf::Color::Black);

        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                if (game.getCell(i, j) == 0) {
                    continue;
                }
                sf::RectangleShape rectangle(sf::Vector2f(cell_size, cell_size));
                rectangle.setPosition(j * cell_size, i * cell_size);
                rectangle.setFillColor(sf::Color::White);

                window.draw(rectangle);
            }
        }

        window.display();
    }

    return 0;
}