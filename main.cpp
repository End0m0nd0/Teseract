#include <iostream>
#include <SFML/Graphics.hpp>
#include <cmath>
#include <memory>

class Point4D
{
    double x;
    double y;
    double z;
    double t;

public:
    sf::CircleShape point;
    Point4D(){
        point = sf::CircleShape(10,200);
    }
    void setPos(double x,double y,double z,double t){
        this->x=x;
        this->y=y;
        this->z=z;
        this->t=t;
    }
    double getX()    {
        return x;
    }
    double getY(){
        return y;
    }
    double getZ(){
        return z;
    }
    double getT(){
        return t;
    }

    void draw(sf::RenderWindow& window){
        point.setPosition(x,y);
        window.draw(point);
    }

    void rotateZW(double angle){
        angle=angle*M_PI/180;
        setPos(x* cos(angle)- y*sin(angle),
               x*sin(angle)+y*cos(angle),
               z,
               t);
    }

    void rotateYW(double angle){
        angle=angle*M_PI/180;
        setPos(x* cos(angle)-z* sin(angle),
               y,
               x*sin(angle)+z*cos(angle),
               t);
    }

    void rotateYZ(double angle){
        angle=angle*M_PI/180;
        setPos(x* cos(angle)-t*sin(angle),
               y,
               z,
               x*sin(angle)+t*cos(angle));
    }

    void rotateXW(double angle){
        angle=angle*M_PI/180;
        setPos(x,
               y*cos(angle)-z*sin(angle),
               y*sin(angle)+z*cos(angle),
               t);
    }

    void rotateXZ(double angle){
        angle=angle*M_PI/180;
        setPos(x,
               y*cos(angle)-t*sin(angle),
               z,
               y*sin(angle)+t*cos(angle));
    }

    void rotateXY(double angle){
        angle=angle*M_PI/180;
        setPos(x,
               y,
               z*cos(angle)-t*sin(angle),
               z*sin(angle)+t*cos(angle));
    }
};

class Connection{
    std::vector<int> first;
    std::vector<int> second;

public:
    void add(int x, int y){
        first.push_back(x);
        second.push_back(y);
    }

    void draw(sf::RenderWindow &window, Point4D (&points)[]){
        for(int i=0;i<first.size();i++){
            sf::VertexArray line = sf::VertexArray(sf::Lines,2);
            line[0].position=sf::Vector2f(points[first[i]].getX(),points[first[i]].getY());
            line[1].position=sf::Vector2f(points[second[i]].getX(),points[second[i]].getY());
            window.draw(line);
        }
    }
};

int main() {
    auto window = sf::RenderWindow(
            sf::VideoMode({1200, 900}), "Teseract",
            sf::Style::Default, sf::ContextSettings(0, 0, 8)
    );

    sf::Font font;
    font.loadFromFile("ARIAL.TTF");
    sf::Text rotationText[6];
    sf::Text speedText[6];

    sf::RectangleShape rotationPanel[6];
    sf::RectangleShape speedPanel[6];
    sf::RectangleShape plusButton[6];
    sf::RectangleShape minusButton[6];
    for (int i = 0; i < 6; i++) {
        rotationPanel[i].setSize(sf::Vector2f(120, 35));
        rotationPanel[i].setPosition(i * 200 - 585, 350);
        rotationPanel[i].setFillColor(sf::Color(0, 0, 0, 140));
        speedPanel[i] = rotationPanel[i];
        speedPanel[i].setPosition(speedPanel[i].getPosition().x + speedPanel[i].getSize().x + 5,
                                  speedPanel[i].getPosition().y);
        speedPanel[i].setSize(sf::Vector2f(50, speedPanel[i].getSize().y));
        plusButton[i] = rotationPanel[i];
        plusButton[i].setSize(sf::Vector2f(rotationPanel[i].getSize().x, 20));
        plusButton[i].setPosition(rotationPanel[i].getPosition().x,
                                  rotationPanel[i].getPosition().y - plusButton[i].getSize().y - 5);
        minusButton[i] = plusButton[i];
        minusButton[i].setPosition(rotationPanel[i].getPosition().x,
                                   rotationPanel[i].getPosition().y + speedPanel[i].getSize().y + 5);

        rotationText[i].setFont(font);
        rotationText[i].setPosition(rotationPanel[i].getPosition().x + 5, rotationPanel[i].getPosition().y + 5);
        rotationText[i].setCharacterSize(20);
        speedText[i].setFont(font);
        speedText[i].setString("0");
        speedText[i].setPosition(speedPanel[i].getPosition().x + 10, speedPanel[i].getPosition().y + 5);
        speedText[i].setCharacterSize(rotationText[i].getCharacterSize());
    }

    rotationText[0].setString("Rotation ZW");
    rotationText[1].setString("Rotation YW");
    rotationText[2].setString("Rotation YZ");
    rotationText[3].setString("Rotation XW");
    rotationText[4].setString("Rotation XZ");
    rotationText[5].setString("Rotation XY");


    sf::View view;
    view.setCenter(0.0f, 0.0f);
    view.setSize(window.getSize().x, window.getSize().y);
    window.setFramerateLimit(144);

    int size = 150;
    Point4D points[16];
    points[0].setPos(size, size, size, size);
    points[1].setPos(size, size, size, -size);
    points[2].setPos(size, size, -size, size);
    points[3].setPos(size, size, -size, -size);
    points[4].setPos(size, -size, size, size);
    points[5].setPos(size, -size, size, -size);
    points[6].setPos(size, -size, -size, size);
    points[7].setPos(size, -size, -size, -size);
    points[8].setPos(-size, size, size, size);
    points[9].setPos(-size, size, size, -size);
    points[10].setPos(-size, size, -size, size);
    points[11].setPos(-size, size, -size, -size);
    points[12].setPos(-size, -size, size, size);
    points[13].setPos(-size, -size, size, -size);
    points[14].setPos(-size, -size, -size, size);
    points[15].setPos(-size, -size, -size, -size);

    Connection con;

    for (int i = 0; i < 16; i++)
        for (int n = 0; n < 16; n++) {
            int difference = 0;

            if (points[i].getX() != points[n].getX()) difference++;
            if (points[i].getY() != points[n].getY()) difference++;
            if (points[i].getZ() != points[n].getZ()) difference++;
            if (points[i].getT() != points[n].getT()) difference++;
            if (difference == 1) con.add(i, n);
        }

    int speedZW = 0;
    int speedYW = 0;
    int speedYZ = 0;
    int speedXW = 0;
    int speedXZ = 0;
    int speedXY = 0;

    while (window.isOpen()) {
        window.setView(view);
        window.clear(sf::Color(100, 100, 100, 255));

        for (Point4D &point: points) {
            point.rotateZW(0.05 * speedZW);
            point.rotateYW(0.05 * speedYW);
            point.rotateYZ(0.05 * speedYZ);
            point.rotateXW(0.05 * speedXW);
            point.rotateXZ(0.05 * speedXZ);
            point.rotateXY(0.05 * speedXY);
        }

        for (int i = 0; i < 6; i++) {
            window.draw(rotationPanel[i]);
            window.draw(speedPanel[i]);
            window.draw(rotationText[i]);
            window.draw(speedText[i]);
            window.draw(plusButton[i]);
            window.draw(minusButton[i]);
        }

        con.draw(window, points);

        auto event = sf::Event();
        while (window.pollEvent(event))
            if (event.type == sf::Event::Closed)
                window.close();
            else if (event.type == sf::Event::MouseButtonPressed) {
                auto mousePos = sf::Mouse::getPosition(window);
                sf::Vector2f worldPos = window.mapPixelToCoords(mousePos);
                if (plusButton[0].getGlobalBounds().contains(worldPos)) {
                    speedZW ++;
                    speedText[0].setString(std::to_string(speedZW));
                }
                else if(plusButton[1].getGlobalBounds().contains(worldPos)){
                    speedYW++;
                    speedText[1].setString(std::to_string(speedYW));
                }
                else if(plusButton[2].getGlobalBounds().contains(worldPos)){
                    speedYZ++;
                    speedText[2].setString(std::to_string(speedYZ));
                }
                else if(plusButton[3].getGlobalBounds().contains(worldPos)){
                    speedXW++;
                    speedText[3].setString(std::to_string(speedXW));
                }
                else if(plusButton[4].getGlobalBounds().contains(worldPos)){
                    speedXZ++;
                    speedText[4].setString(std::to_string(speedXZ));
                }
                else if(plusButton[5].getGlobalBounds().contains(worldPos)){
                    speedXY++;
                    speedText[5].setString(std::to_string(speedXY));
                }
                else if (minusButton[0].getGlobalBounds().contains(worldPos)) {
                    speedZW--;
                    speedText[0].setString(std::to_string(speedZW));
                }
                else if(minusButton[1].getGlobalBounds().contains(worldPos)){
                    speedYW--;
                    speedText[1].setString(std::to_string(speedYW));
                }
                else if(minusButton[2].getGlobalBounds().contains(worldPos)){
                    speedYZ--;
                    speedText[2].setString(std::to_string(speedYZ));
                }
                else if(minusButton[3].getGlobalBounds().contains(worldPos)){
                    speedXW--;
                    speedText[3].setString(std::to_string(speedXW));
                }
                else if(minusButton[4].getGlobalBounds().contains(worldPos)){
                    speedXZ--;
                    speedText[4].setString(std::to_string(speedXZ));
                }
                else if(minusButton[5].getGlobalBounds().contains(worldPos)){
                    speedXY--;
                    speedText[5].setString(std::to_string(speedXY));
                }

            }


        window.display();
    }
}
